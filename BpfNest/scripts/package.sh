#!/usr/bin/env bash
# Build and package only. No installation, deployment or restart commands.
set -euo pipefail
source "$(dirname -- "${BASH_SOURCE[0]}")/common.sh"
[[ $# == 0 ]] || { printf 'Usage: %s\n' "$0" >&2; exit 2; }
need python3
mkdir -p "$BPFNEST_ROOT/build/logs" "$BPFNEST_ROOT/dist"
exec > >(tee "$BPFNEST_ROOT/build/logs/package.log") 2>&1
printf 'PACKAGE UTC %s\n' "$(date -u +%FT%TZ)"
"$BPFNEST_ROOT/scripts/verify.sh"
python3 "$BPFNEST_ROOT/scripts/tooling_tests.py"
python3 - "$BPFNEST_ROOT" <<'PY'
import hashlib
from pathlib import Path
import sys
import zipfile
root=Path(sys.argv[1])
sys.path.insert(0,str(root/'scripts'))
from module_check import check
files={p.relative_to(root/'module').as_posix():p.read_bytes() for p in (root/'module').rglob('*') if p.is_file()}
files['libbpfnest.so']=(root/'build/arm64/libbpfnest.so').read_bytes()
files['SHA256SUMS']=''.join(f'{hashlib.sha256(files[n]).hexdigest()}  {n}\n' for n in sorted(files)).encode()
check(files)
output=root/'dist/BpfNest-v0.1.0-arm64.zip'
with zipfile.ZipFile(output,'w',compression=zipfile.ZIP_DEFLATED,compresslevel=9) as archive:
    for name in sorted(files):
        info=zipfile.ZipInfo(name,(2026,1,1,0,0,0))
        info.compress_type=zipfile.ZIP_DEFLATED
        info.create_system=3
        mode=0o755 if name.endswith('update-binary') else 0o644
        info.external_attr=(0o100000|mode)<<16
        archive.writestr(info,files[name])
digest=hashlib.sha256(output.read_bytes()).hexdigest()
(root/'dist'/f'{output.name}.sha256').write_text(f'{digest}  {output.name}\n')
binary=root/'dist/libbpfnest.so'
binary.write_bytes(files['libbpfnest.so'])
(root/'dist/libbpfnest.so.sha256').write_text(f'{hashlib.sha256(files["libbpfnest.so"]).hexdigest()}  libbpfnest.so\n')
print(f'ZIP {output}\nSHA256 {digest}\nSIZE {output.stat().st_size}')
PY
python3 "$BPFNEST_ROOT/scripts/module_check.py" "$BPFNEST_ROOT/dist/BpfNest-v0.1.0-arm64.zip"
printf 'PACKAGE PASS: rebuilt module/API4 check, exact ZIP manifest and payload hashes\n'
