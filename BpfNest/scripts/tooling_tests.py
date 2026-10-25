#!/usr/bin/env python3
"""Local contract regressions; no device or network access."""
import contextlib
import hashlib
import io
from pathlib import Path
import struct
import tempfile
import subprocess
import sys
import module_check

root = Path(__file__).resolve().parent.parent
files = {p.relative_to(root/'module').as_posix(): p.read_bytes() for p in (root/'module').rglob('*') if p.is_file()}
files['libbpfnest.so'] = (root/'build/arm64/libbpfnest.so').read_bytes()
files['SHA256SUMS'] = ''.join(f'{hashlib.sha256(files[n]).hexdigest()}  {n}\n' for n in sorted(files)).encode()
with contextlib.redirect_stdout(io.StringIO()):
    module_check.check(files)
for name, value in [('zn_modules.txt', b'name=netd libbpfnest.so\n'), ('bpfnest.conf', b'mode=patch\n'), ('libbpfnest.so', b'corrupt')]:
    bad = dict(files); bad[name] = value
    try:
        module_check.check(bad)
    except ValueError:
        pass
    else:
        raise AssertionError(f'accepted altered {name}')
# Reject truncated/malformed ELF and an otherwise valid module advertising API3.
with tempfile.TemporaryDirectory(prefix='bpfnest-tooling-') as tmp:
    good = bytearray(files['libbpfnest.so'])
    from elf_check import Elf
    elf = Elf(good)
    symbol = next(s for section in elf.sections if section['type']==11 for s in elf.symbols(section) if s['name']=='zn_module')
    section = next(s for s in elf.sections if s['address']<=symbol['value']<s['address']+s['size'] and s['type']!=8)
    struct.pack_into('<I', good, section['offset']+symbol['value']-section['address'], 3)
    for i, payload in enumerate((b'not elf', good)):
        path = Path(tmp)/f'bad-{i}.so'; path.write_bytes(payload)
        result = subprocess.run([sys.executable,str(root/'scripts/elf_check.py'),'--module',str(path)],capture_output=True)
        if result.returncode == 0:
            raise AssertionError('accepted malformed/API3 module')
print('TOOLING_TESTS PASS: release defaults/scope/hash tampering and malformed/API3 ELF rejected')
