#!/usr/bin/env python3
"""Validate the exact, non-deploying BpfNest module payload or release archive."""
import hashlib
from pathlib import Path
import sys
import zipfile

REQUIRED = {'module.prop', 'customize.sh', 'bpfnest.conf', 'zn_modules.txt',
            'libbpfnest.so', 'META-INF/com/google/android/update-binary',
            'META-INF/com/google/android/updater-script', 'NOTICE.txt', 'SHA256SUMS'}

def check(files):
    if set(files) != REQUIRED:
        raise ValueError(f'manifest mismatch: missing={REQUIRED-set(files)} extra={set(files)-REQUIRED}')
    if files['zn_modules.txt'] != b'path=/system/bin/netd libbpfnest.so\n':
        raise ValueError('incorrect netd scope/library path')
    if files['bpfnest.conf'].splitlines()[-1:] != [b'mode=observe']:
        raise ValueError('release must ship mode=observe')
    prop = dict(line.split('=', 1) for line in files['module.prop'].decode().splitlines() if line)
    if prop.get('id') != 'bpfnest' or not all(prop.get(k) for k in ('name','version','versionCode','author','description')):
        raise ValueError('invalid module.prop')
    if files['META-INF/com/google/android/updater-script'] != b'#MAGISK\n':
        raise ValueError('missing manager marker')
    installer = files['META-INF/com/google/android/update-binary']
    if b'. /data/adb/magisk/util_functions.sh' not in installer or b'\ninstall_module\n' not in installer:
        raise ValueError('missing genuine Magisk installer entry')
    sums = files['SHA256SUMS'].decode().splitlines()
    expected = [f'{hashlib.sha256(files[n]).hexdigest()}  {n}' for n in sorted(REQUIRED-{'SHA256SUMS'})]
    if sums != expected:
        raise ValueError('payload hash manifest mismatch')
    print('PACKAGE_CHECK PASS: exact manifest, observe default, netd-only scope, official installer, payload hashes')

def main():
    path = Path(sys.argv[1])
    if path.is_dir():
        files = {p.relative_to(path).as_posix(): p.read_bytes() for p in path.rglob('*') if p.is_file()}
    else:
        with zipfile.ZipFile(path) as z:
            names = z.namelist()
            if len(names) != len(set(names)) or any(n.endswith('/') for n in names):
                raise ValueError('duplicate or unexpected directory entries')
            if z.testzip() is not None:
                raise ValueError('ZIP CRC failure')
            files = {n: z.read(n) for n in names}
    check(files)
    for name in sorted(files):
        print(f'{hashlib.sha256(files[name]).hexdigest()}  {name} ({len(files[name])} bytes)')

if __name__ == '__main__':
    try:
        main()
    except (OSError, ValueError, KeyError, zipfile.BadZipFile) as e:
        sys.exit(f'PACKAGE_CHECK FAIL: {e}')
