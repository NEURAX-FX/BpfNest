#!/usr/bin/env python3
"""Offline ELF evidence. Never executes or modifies an inspected file."""
import argparse
import hashlib
import lzma
from pathlib import Path
import shutil
import struct
import subprocess
import sys


class Elf:
    def __init__(self, data):
        self.data = data
        if len(data) < 64 or data[:6] != b'\x7fELF\x02\x01':
            raise ValueError('requires ELF64 little-endian input')
        self.kind, self.machine = struct.unpack_from('<HH', data, 16)
        if self.machine != 183 or self.kind not in (2, 3):
            raise ValueError(f'requires AArch64 EXEC/DYN (type={self.kind}, machine={self.machine})')
        self.shoff = struct.unpack_from('<Q', data, 40)[0]
        entsize, count, strings = struct.unpack_from('<HHH', data, 58)
        if entsize != 64 or count == 0 or strings >= count:
            raise ValueError('missing/unsupported section table')
        self.sections = []
        for i in range(count):
            row = struct.unpack_from('<IIQQQQIIQQ', data, self.shoff + i * entsize)
            self.sections.append(dict(zip(('name_index','type','flags','address','offset','size','link','info','align','entry'), row)))
        names = self.bytes(self.sections[strings])
        for section in self.sections:
            section['name'] = self.string(names, section['name_index'])

    @staticmethod
    def string(data, offset):
        end = data.find(b'\0', offset)
        if offset >= len(data) or end < 0:
            raise ValueError('invalid ELF string')
        return data[offset:end].decode('utf-8', errors='replace')

    def bytes(self, section):
        offset, size = section['offset'], section['size']
        if offset > len(self.data) or size > len(self.data)-offset:
            raise ValueError('out-of-bounds ELF section')
        return self.data[offset:offset+size]

    def symbols(self, section):
        if section['entry'] != 24 or section['link'] >= len(self.sections):
            raise ValueError('unsupported symbol table')
        names = self.bytes(self.sections[section['link']])
        result = []
        for name, info, other, index, value, size in struct.iter_unpack('<IBBHQQ', self.bytes(section)):
            result.append({'name': self.string(names, name), 'info': info, 'other': other, 'index': index, 'value': value, 'size': size})
        return result

    def virtual(self, address, size):
        for section in self.sections:
            if section['type'] != 8 and section['address'] <= address and address+size <= section['address']+section['size']:
                offset = section['offset'] + address-section['address']
                return self.data[offset:offset+size]
        raise ValueError('virtual address lacks file bytes')


def inspect(path, require_module):
    data = path.read_bytes()
    elf = Elf(data)
    print(f'FILE {path}\nSIZE {len(data)}\nSHA256 {hashlib.sha256(data).hexdigest()}')
    print(f'ELF64 little-endian AArch64 type={elf.kind} (3=DYN, 2=EXEC)')
    exports = []
    for section in elf.sections:
        if section['type'] == 7:
            notes = elf.bytes(section)
            offset = 0
            while offset + 12 <= len(notes):
                namesize, descsize, kind = struct.unpack_from('<III', notes, offset)
                offset += 12
                name = notes[offset:offset+namesize].rstrip(b'\0')
                offset += (namesize+3)&~3
                desc = notes[offset:offset+descsize]
                offset += (descsize+3)&~3
                if name == b'GNU' and kind == 3:
                    print(f'BUILD_ID {desc.hex()}')
                elif name == b'Android' and len(desc) >= 4:
                    print(f'ANDROID_NOTE_API {struct.unpack_from("<I",desc)[0]}')
        if section['type'] in (2,11):
            symbols = elf.symbols(section)
            print(f'{section["name"]}: {len(symbols)} symbols')
            for symbol in symbols:
                name = symbol['name']
                if name == 'syscall' or 'attachProgramToCgroup' in name or name == 'zn_module':
                    print(f'  SYMBOL {name} value=0x{symbol["value"]:x} size=0x{symbol["size"]:x} shndx={symbol["index"]}')
                if section['type']==11 and name=='zn_module' and symbol['index'] and (symbol['info']>>4)==1 and (symbol['other']&3)==0:
                    exports.append(symbol)
        if section['name'] == '.gnu_debugdata':
            compressed = elf.bytes(section)
            mini_bytes = lzma.decompress(compressed, memlimit=64*1024*1024)
            mini = Elf(mini_bytes)
            print(f'.gnu_debugdata: compressed={len(compressed)} decompressed={len(mini_bytes)} SHA256={hashlib.sha256(mini_bytes).hexdigest()}')
            for table in mini.sections:
                if table['type'] == 2:
                    for symbol in mini.symbols(table):
                        if 'attachProgramToCgroup' in symbol['name'] or 'BpfHandler4init' in symbol['name']:
                            print(f'  MINI_SYMBOL {symbol["name"]} value=0x{symbol["value"]:x} size=0x{symbol["size"]:x}')
        if section['type'] == 4 and section['entry']==24:
            symbols = elf.symbols(elf.sections[section['link']])
            for address, info, addend in struct.iter_unpack('<QQq', elf.bytes(section)):
                if info & 0xffffffff == 1026:
                    symbol = symbols[info>>32]
                    if symbol['name'] == 'syscall':
                        print(f'JUMP_SLOT syscall GOT=0x{address:x} addend={addend}')
    if require_module:
        if elf.kind != 3 or len(exports)!=1 or (exports[0]['info']&15)!=1 or exports[0]['size']!=16:
            raise ValueError('compiled module must export exactly one GLOBAL DEFAULT OBJECT zn_module of size16')
        value = exports[0]['value']
        api = struct.unpack('<I', elf.virtual(value,4))[0]
        if api != 4:
            raise ValueError(f'zn_module target_api_version is {api}, expected4')
        print('MODULE_CHECK PASS: exported zn_module, target_api_version=4, ARM64 DYN')
    tool = shutil.which('llvm-readelf') or shutil.which('readelf')
    if not tool:
        raise ValueError('readelf or llvm-readelf required')
    print(f'READELF {tool}', flush=True)
    subprocess.run([tool,'-h','-l','-d',str(path)],check=True)
    disassembler = shutil.which('llvm-objdump') or shutil.which('aarch64-linux-gnu-objdump')
    if disassembler:
        result = subprocess.run([disassembler,'-d',str(path)],text=True,capture_output=True)
        if result.returncode == 0:
            lines=result.stdout.splitlines()
            references=[line.strip() for line in lines if 'syscall' in line or '\tsvc\t' in line or '\tsvc ' in line]
            print(f'DISASSEMBLER {disassembler}; syscall/svc matching lines={len(references)}')
            for line in references:
                print('  '+line)
        else:
            print('DISASSEMBLY unavailable: '+result.stderr.strip())
    else:
        print('DISASSEMBLY unavailable: no AArch64-capable objdump')
    print('LIMIT: relocations/symbols do not prove runtime reachability, loader timing, RELRO hook success or seccomp/SELinux access. Direct svc, LTO/inlining, stripped symbols or another image may bypass this PLT path.')


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--module',action='store_true',help='require compiled BpfNest zn_module/API4 export')
    parser.add_argument('files',nargs='+',type=Path)
    args=parser.parse_args()
    status=0
    for path in args.files:
        try:
            inspect(path,args.module)
        except (OSError,ValueError,struct.error,lzma.LZMAError,subprocess.CalledProcessError) as error:
            print(f'INCOMPATIBLE {path}: {error}',file=sys.stderr)
            status=1
    return status

if __name__=='__main__':
    sys.exit(main())
