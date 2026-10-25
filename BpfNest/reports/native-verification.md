# Authoritative final verification

Captured 2026-10-25T16:34:35.682815+00:00; final reviewed source.

- `BpfNest/scripts/package.sh`: exit **0**; includes full host suite, Android build/API4 ELF checks, actual production trampoline/dispatcher QEMU smoke, tooling checks and ZIP verification.
- No Android installation, device tests, network service restart or detach performed.
- `BpfNest/dist/libbpfnest.so`: 1823920 bytes; SHA256 `6ab15a86946487dd0e36d13238fc2ec3fda5fb6151c910c5165770b7c7e9e676`.
- `BpfNest/dist/BpfNest-v0.1.0-arm64.zip`: 451519 bytes; SHA256 `0e326083c20f7d7956e284b2c502475c2e1ca2fcb2c47075b7db50971b11890c`.

```text
PACKAGE UTC 2026-10-25T16:34:33Z
VERIFY UTC 2026-10-25T16:34:33Z
-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /home/neurax/NetdMultiAttach/BpfNest/build/host
ninja: no work to do.
Test project /home/neurax/NetdMultiAttach/BpfNest/build/host
    Start 1: core
1/7 Test #1: core .............................   Passed    0.00 sec
    Start 2: install
2/7 Test #2: install ..........................   Passed    0.00 sec
    Start 3: elf
3/7 Test #3: elf ..............................   Passed    0.01 sec
    Start 4: fd
4/7 Test #4: fd ...............................   Passed    0.00 sec
    Start 5: host_smoke
5/7 Test #5: host_smoke .......................   Passed    0.00 sec
    Start 6: reentrancy
6/7 Test #6: reentrancy .......................   Passed    0.00 sec
    Start 7: policy_safety
7/7 Test #7: policy_safety ....................   Passed    0.00 sec

100% tests passed, 0 tests failed out of 7

Total Test time (real) =   0.02 sec
NDK=/home/neurax/Android/Sdk/ndk/29.0.14206865
ABI=arm64-v8a
API=26
-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /home/neurax/NetdMultiAttach/BpfNest/build/arm64
ninja: no work to do.
FILE /home/neurax/NetdMultiAttach/BpfNest/build/arm64/libbpfnest.so
SIZE 1823920
SHA256 6ab15a86946487dd0e36d13238fc2ec3fda5fb6151c910c5165770b7c7e9e676
ELF64 little-endian AArch64 type=3 (3=DYN, 2=EXEC)
ANDROID_NOTE_API 26
BUILD_ID 2f1fda52a9c3df4aa289e065fc320fa440295450
.dynsym: 62 symbols
  SYMBOL syscall value=0x0 size=0x0 shndx=0
  SYMBOL zn_module value=0x3f5c8 size=0x10 shndx=23
JUMP_SLOT syscall GOT=0x3b5c0 addend=0
.symtab: 1949 symbols
  SYMBOL zn_module value=0x3f5c8 size=0x10 shndx=23
  SYMBOL syscall value=0x0 size=0x0 shndx=0
MODULE_CHECK PASS: exported zn_module, target_api_version=4, ARM64 DYN
READELF /usr/bin/llvm-readelf
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              DYN (Shared object file)
  Machine:                           AArch64
  Version:                           0x1
  Entry point address:               0x0
  Start of program headers:          64 (bytes into file)
  Start of section headers:          1821616 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         9
  Size of section headers:           64 (bytes)
  Number of section headers:         36
  Section header string table index: 34

Elf file type is DYN (Shared object file)
Entry point 0x0
There are 9 program headers, starting at offset 64

Program Headers:
  Type           Offset   VirtAddr           PhysAddr           FileSiz  MemSiz   Flg Align
  PHDR           0x000040 0x0000000000000040 0x0000000000000040 0x0001f8 0x0001f8 R   0x8
  LOAD           0x000000 0x0000000000000000 0x0000000000000000 0x034270 0x034270 R E 0x4000
  LOAD           0x034270 0x0000000000038270 0x0000000000038270 0x003358 0x003d90 RW  0x4000
  LOAD           0x0375c8 0x000000000003f5c8 0x000000000003f5c8 0x0000c0 0x000950 RW  0x4000
  DYNAMIC        0x0371e0 0x000000000003b1e0 0x000000000003b1e0 0x0001c0 0x0001c0 RW  0x8
  GNU_RELRO      0x034270 0x0000000000038270 0x0000000000038270 0x003358 0x003d90 R   0x1
  GNU_EH_FRAME   0x00c880 0x000000000000c880 0x000000000000c880 0x000eac 0x000eac R   0x4
  GNU_STACK      0x000000 0x0000000000000000 0x0000000000000000 0x000000 0x000000 RW  0x0
  NOTE           0x000238 0x0000000000000238 0x0000000000000238 0x0000bc 0x0000bc R   0x4

 Section to Segment mapping:
  Segment Sections...
   00     
   01     .note.android.ident .note.gnu.build-id .dynsym .gnu.version .gnu.version_r .gnu.hash .dynstr .rela.dyn .rela.plt .rodata .gcc_except_table .eh_frame_hdr .eh_frame .text .plt 
   02     .data.rel.ro .fini_array .init_array .dynamic .got .got.plt .relro_padding 
   03     .data .bss 
   04     .dynamic 
   05     .data.rel.ro .fini_array .init_array .dynamic .got .got.plt .relro_padding 
   06     .eh_frame_hdr 
   07     
   08     .note.android.ident .note.gnu.build-id 
   None   .comment .debug_loc .debug_abbrev .debug_info .debug_ranges .debug_str .debug_line .debug_aranges .symtab .shstrtab .strtab 
Dynamic section at offset 0x371e0 contains 28 entries:
  Tag                Type           Name/Value
  0x0000000000000001 (NEEDED)       Shared library: [liblog.so]
  0x0000000000000001 (NEEDED)       Shared library: [libdl.so]
  0x0000000000000001 (NEEDED)       Shared library: [libm.so]
  0x0000000000000001 (NEEDED)       Shared library: [libc.so]
  0x000000000000000e (SONAME)       Library soname: [libbpfnest.so]
  0x000000000000001e (FLAGS)        BIND_NOW 
  0x000000006ffffffb (FLAGS_1)      NOW 
  0x0000000000000007 (RELA)         0xc90
  0x0000000000000008 (RELASZ)       33000 (bytes)
  0x0000000000000009 (RELAENT)      24 (bytes)
  0x000000006ffffff9 (RELACOUNT)    1374
  0x0000000000000017 (JMPREL)       0x8d78
  0x0000000000000002 (PLTRELSZ)     1416 (bytes)
  0x0000000000000003 (PLTGOT)       0x3b3d8
  0x0000000000000014 (PLTREL)       RELA
  0x0000000000000006 (SYMTAB)       0x2f8
  0x000000000000000b (SYMENT)       24 (bytes)
  0x0000000000000005 (STRTAB)       0x9a8
  0x000000000000000a (STRSZ)        737 (bytes)
  0x000000006ffffef5 (GNU_HASH)     0x988
  0x0000000000000019 (INIT_ARRAY)   0x3b1d0
  0x000000000000001b (INIT_ARRAYSZ) 16 (bytes)
  0x000000000000001a (FINI_ARRAY)   0x3b1c0
  0x000000000000001c (FINI_ARRAYSZ) 16 (bytes)
  0x000000006ffffff0 (VERSYM)       0x8c8
  0x000000006ffffffe (VERNEED)      0x944
  0x000000006fffffff (VERNEEDNUM)   2
  0x0000000000000000 (NULL)         0x0
DISASSEMBLER /usr/bin/llvm-objdump; syscall/svc matching lines=86
  0000000000013644 <bpfnest_syscall_entry>:
  16158: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16334: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16a34: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16bd8: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16c58: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16cc8: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16d38: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16db8: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16e28: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16f24: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16f94: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  17014: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  17094: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  17150: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  171c0: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  17240: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1735c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  175ec: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  17694: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  17704: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  17784: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  17864: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  178e4: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  17964: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  18824: 90000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  19360: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  19410: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  194c0: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  19570: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  19620: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  198b8: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1996c: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  19a10: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  19ac0: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  19b6c: f00000ea     	adrp	x10, 0x38000 <syscall@plt+0x3da0>
  19c1c: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  19cac: f00000f8     	adrp	x24, 0x38000 <syscall@plt+0x3da0>
  1a208: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1a384: d00000e9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  1a424: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1a738: d00000e9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  1ae8c: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1afac: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1b1d4: b00000ea     	adrp	x10, 0x38000 <syscall@plt+0x3da0>
  1bac8: b00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1bc48: b00000e9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  1c258: 900000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1c6a8: 900000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1cad0: 900000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1d328: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1d3e8: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1d4b0: f00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  1d588: f00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  1d668: f00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  1d728: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1d7ec: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1d8b4: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1e424: d00000ca     	adrp	x10, 0x38000 <syscall@plt+0x3da0>
  1eb28: d00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1f12c: b00000da     	adrp	x26, 0x38000 <syscall@plt+0x3da0>
  1f54c: b00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1f754: b00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  1fed4: b00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  204ac: 900000c8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  2256c: d00000a9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  22768: d00000a9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  228e4: d00000a9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  24758: 900000a9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  28398: 90000088     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  29724: f0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2a820: d0000068     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  2b018: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b0c4: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b170: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b21c: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b2c8: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b374: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b420: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b618: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b6c4: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b770: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b81c: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2bab4: b0000068     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  31154: 94000c43     	bl	0x34260 <syscall@plt>
  0000000000034260 <syscall@plt>:
LIMIT: relocations/symbols do not prove runtime reachability, loader timing, RELRO hook success or seccomp/SELinux access. Direct svc, LTO/inlining, stripped symbols or another image may bypass this PLT path.
ARM64 QEMU PASS: production trampoline + production dispatcher; all seven argument words, exact LR, copy isolation, observe/no query, empty patch, nonempty refusal, return/input+output errno
TOOLING_TESTS PASS: release defaults/scope/hash tampering and malformed/API3 ELF rejected
VERIFY PASS: configured host suite, Android build/static checks and available QEMU completed
TOOLING_TESTS PASS: release defaults/scope/hash tampering and malformed/API3 ELF rejected
PACKAGE_CHECK PASS: exact manifest, observe default, netd-only scope, official installer, payload hashes
ZIP /home/neurax/NetdMultiAttach/BpfNest/dist/BpfNest-v0.1.0-arm64.zip
SHA256 0e326083c20f7d7956e284b2c502475c2e1ca2fcb2c47075b7db50971b11890c
SIZE 451519
PACKAGE_CHECK PASS: exact manifest, observe default, netd-only scope, official installer, payload hashes
bcf4b1d9913f3af17755569c853e0b5a75b8005f6a18eb3f86dadcc0e968c29d  META-INF/com/google/android/update-binary (612 bytes)
d2b8203193a083e43ad076fde01b9c7821e0aa521e1f8077b8628fb1cdb70c60  META-INF/com/google/android/updater-script (8 bytes)
f5f9b7b7c98ee32c67f013ff03eba0428abf1d8c2a950825e6f1f5b473d0157f  NOTICE.txt (1332 bytes)
da79f4509228d70ca4d35e65218e33d5e2f187bb57c9e63fb1d0553b661e0b43  SHA256SUMS (691 bytes)
525ab5d7a93c8bc4194e14d87cd26138a033b995a4cd794524ed5bfe5b3dfa5b  bpfnest.conf (69 bytes)
5d6d3b4b0816f6ef84cdce38dfad9b8cf8b5f636229ea3e198a96d229df4447d  customize.sh (1554 bytes)
6ab15a86946487dd0e36d13238fc2ec3fda5fb6151c910c5165770b7c7e9e676  libbpfnest.so (1823920 bytes)
2b82fd99e423f35381761952d2db682483a5931bc5ceab045fe64b2b98109b9d  module.prop (190 bytes)
3ade0c3d73b478141b13af03a66b0784a7fdeff87db7e5859e39924f434e0663  zn_modules.txt (36 bytes)
PACKAGE PASS: rebuilt module/API4 check, exact ZIP manifest and payload hashes
```

---

# Historical execution record

Earlier failures and superseded runs below are retained, not final passing evidence.

# Native verification

Local only; no Android deployment.

## `cmake --build BpfNest/build/host && ctest --test-dir BpfNest/build/host --output-on-failure`

Exit **1**

```text
[1/9] Building CXX object CMakeFiles/bpfnest_core.dir/src/core.cpp.o
[2/9] Building CXX object CMakeFiles/bpfnest_install_test.dir/tests/install_test.cpp.o
FAILED: [code=1] CMakeFiles/bpfnest_install_test.dir/tests/install_test.cpp.o 
/usr/bin/c++  -I/home/neurax/NetdMultiAttach/BpfNest/include -I/home/neurax/NetdMultiAttach/BpfNest/third_party -g -std=c++17 -Wall -Wextra -Werror -MD -MT CMakeFiles/bpfnest_install_test.dir/tests/install_test.cpp.o -MF CMakeFiles/bpfnest_install_test.dir/tests/install_test.cpp.o.d -o CMakeFiles/bpfnest_install_test.dir/tests/install_test.cpp.o -c /home/neurax/NetdMultiAttach/BpfNest/tests/install_test.cpp
/home/neurax/NetdMultiAttach/BpfNest/tests/install_test.cpp: In lambda function:
/home/neurax/NetdMultiAttach/BpfNest/tests/install_test.cpp:45:108: error: ‘errno’在此作用域中尚未声明
   45 |             [](void* p,const std::uint64_t* a) { CHECK(std::memcmp(a,static_cast<Frame*>(p)->args,56)==0); errno=EINVAL; return -1L; },
      |                                                                                                            ^~~~~
/home/neurax/NetdMultiAttach/BpfNest/tests/install_test.cpp:5:1: note: ‘errno’ is defined in header ‘<cerrno>’; this is probably fixable by adding ‘#include <cerrno>’
    4 | #include <cstring>
  +++ |+#include <cerrno>
    5 | 
/home/neurax/NetdMultiAttach/BpfNest/tests/install_test.cpp:45:114: error: ‘EINVAL’在此作用域中尚未声明
   45 |             [](void* p,const std::uint64_t* a) { CHECK(std::memcmp(a,static_cast<Frame*>(p)->args,56)==0); errno=EINVAL; return -1L; },
      |                                                                                                                  ^~~~~~
/home/neurax/NetdMultiAttach/BpfNest/tests/install_test.cpp: In member function ‘void {anonymous}::Fixture::check_passthrough()’:
/home/neurax/NetdMultiAttach/BpfNest/tests/install_test.cpp:47:9: error: ‘errno’在此作用域中尚未声明
   47 |         errno=EINTR; CHECK(dispatch(frame,active_state(state),ops)==-1); CHECK(errno==EINVAL);
      |         ^~~~~
/home/neurax/NetdMultiAttach/BpfNest/tests/install_test.cpp:47:9: note: ‘errno’ is defined in header ‘<cerrno>’; this is probably fixable by adding ‘#include <cerrno>’
/home/neurax/NetdMultiAttach/BpfNest/tests/install_test.cpp:47:15: error: ‘EINTR’在此作用域中尚未声明
   47 |         errno=EINTR; CHECK(dispatch(frame,active_state(state),ops)==-1); CHECK(errno==EINVAL);
      |               ^~~~~
/home/neurax/NetdMultiAttach/BpfNest/tests/install_test.cpp:47:87: error: ‘EINVAL’在此作用域中尚未声明
   47 |         errno=EINTR; CHECK(dispatch(frame,active_state(state),ops)==-1); CHECK(errno==EINVAL);
      |                                                                                       ^~~~~~
/home/neurax/NetdMultiAttach/BpfNest/tests/install_test.cpp:7:29: note: in definition of macro ‘CHECK’
    7 | #define CHECK(x) do { if (!(x)) { std::fprintf(stderr,"install:%d: %s\n",__LINE__,#x); std::exit(1); } } while(0)
      |                             ^
[3/9] Linking CXX static library libbpfnest_core.a
[4/9] Building CXX object CMakeFiles/bpfnest_core_test.dir/tests/core_test.cpp.o
ninja: build stopped: subcommand failed.
```

## `BpfNest/scripts/smoke-arm64.sh`

Exit **1**

```text
ld.lld: error: undefined symbol: __emutls_get_address
>>> referenced by core.cpp:81 (BpfNest/src/core.cpp:81)
>>>               /tmp/core-b15bbb.o:(bpfnest::dispatch(bpfnest::Frame const&, bpfnest::State const&, bpfnest::Operations const&))
clang++: error: linker command failed with exit code 1 (use -v to see invocation)
```

## `BpfNest/scripts/package.sh`

Exit **1**

```text
PACKAGE UTC 2026-10-25T16:23:51Z
VERIFY UTC 2026-10-25T16:23:51Z
-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /home/neurax/NetdMultiAttach/BpfNest/build/host
[1/6] Linking CXX executable bpfnest_core_test
[2/6] Linking CXX executable bpfnest_elf_test
[3/6] Building CXX object CMakeFiles/bpfnest_install_test.dir/tests/install_test.cpp.o
FAILED: [code=1] CMakeFiles/bpfnest_install_test.dir/tests/install_test.cpp.o 
/usr/bin/c++  -I/home/neurax/NetdMultiAttach/BpfNest/include -I/home/neurax/NetdMultiAttach/BpfNest/third_party -g -std=c++17 -Wall -Wextra -Werror -MD -MT CMakeFiles/bpfnest_install_test.dir/tests/install_test.cpp.o -MF CMakeFiles/bpfnest_install_test.dir/tests/install_test.cpp.o.d -o CMakeFiles/bpfnest_install_test.dir/tests/install_test.cpp.o -c /home/neurax/NetdMultiAttach/BpfNest/tests/install_test.cpp
/home/neurax/NetdMultiAttach/BpfNest/tests/install_test.cpp: In lambda function:
/home/neurax/NetdMultiAttach/BpfNest/tests/install_test.cpp:45:108: error: ‘errno’在此作用域中尚未声明
   45 |             [](void* p,const std::uint64_t* a) { CHECK(std::memcmp(a,static_cast<Frame*>(p)->args,56)==0); errno=EINVAL; return -1L; },
      |                                                                                                            ^~~~~
/home/neurax/NetdMultiAttach/BpfNest/tests/install_test.cpp:5:1: note: ‘errno’ is defined in header ‘<cerrno>’; this is probably fixable by adding ‘#include <cerrno>’
    4 | #include <cstring>
  +++ |+#include <cerrno>
    5 | 
/home/neurax/NetdMultiAttach/BpfNest/tests/install_test.cpp:45:114: error: ‘EINVAL’在此作用域中尚未声明
   45 |             [](void* p,const std::uint64_t* a) { CHECK(std::memcmp(a,static_cast<Frame*>(p)->args,56)==0); errno=EINVAL; return -1L; },
      |                                                                                                                  ^~~~~~
/home/neurax/NetdMultiAttach/BpfNest/tests/install_test.cpp: In member function ‘void {anonymous}::Fixture::check_passthrough()’:
/home/neurax/NetdMultiAttach/BpfNest/tests/install_test.cpp:47:9: error: ‘errno’在此作用域中尚未声明
   47 |         errno=EINTR; CHECK(dispatch(frame,active_state(state),ops)==-1); CHECK(errno==EINVAL);
      |         ^~~~~
/home/neurax/NetdMultiAttach/BpfNest/tests/install_test.cpp:47:9: note: ‘errno’ is defined in header ‘<cerrno>’; this is probably fixable by adding ‘#include <cerrno>’
/home/neurax/NetdMultiAttach/BpfNest/tests/install_test.cpp:47:15: error: ‘EINTR’在此作用域中尚未声明
   47 |         errno=EINTR; CHECK(dispatch(frame,active_state(state),ops)==-1); CHECK(errno==EINVAL);
      |               ^~~~~
/home/neurax/NetdMultiAttach/BpfNest/tests/install_test.cpp:47:87: error: ‘EINVAL’在此作用域中尚未声明
   47 |         errno=EINTR; CHECK(dispatch(frame,active_state(state),ops)==-1); CHECK(errno==EINVAL);
      |                                                                                       ^~~~~~
/home/neurax/NetdMultiAttach/BpfNest/tests/install_test.cpp:7:29: note: in definition of macro ‘CHECK’
    7 | #define CHECK(x) do { if (!(x)) { std::fprintf(stderr,"install:%d: %s\n",__LINE__,#x); std::exit(1); } } while(0)
      |                             ^
[4/6] Linking CXX executable bpfnest_fd_test
ninja: build stopped: subcommand failed.
```


# Final integrated rerun

## `BpfNest/scripts/verify.sh`

Exit **0**

```text
VERIFY UTC 2026-10-25T16:25:24Z
-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /home/neurax/NetdMultiAttach/BpfNest/build/host
[1/3] Linking CXX executable bpfnest_host_smoke
[2/3] Building CXX object CMakeFiles/bpfnest_install_test.dir/tests/install_test.cpp.o
[3/3] Linking CXX executable bpfnest_install_test
Test project /home/neurax/NetdMultiAttach/BpfNest/build/host
    Start 1: core
1/5 Test #1: core .............................   Passed    0.00 sec
    Start 2: install
2/5 Test #2: install ..........................   Passed    0.00 sec
    Start 3: elf
3/5 Test #3: elf ..............................   Passed    0.01 sec
    Start 4: fd
4/5 Test #4: fd ...............................   Passed    0.00 sec
    Start 5: host_smoke
5/5 Test #5: host_smoke .......................   Passed    0.00 sec

100% tests passed, 0 tests failed out of 5

Total Test time (real) =   0.01 sec
NDK=/home/neurax/Android/Sdk/ndk/29.0.14206865
ABI=arm64-v8a
API=26
-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /home/neurax/NetdMultiAttach/BpfNest/build/arm64
[1/5] Building CXX object CMakeFiles/bpfnest_core.dir/src/install.cpp.o
[2/5] Building CXX object CMakeFiles/bpfnest_core.dir/src/core.cpp.o
[3/5] Linking CXX static library libbpfnest_core.a
[4/5] Building CXX object CMakeFiles/bpfnest.dir/src/runtime.cpp.o
[5/5] Linking CXX shared library libbpfnest.so
FILE /home/neurax/NetdMultiAttach/BpfNest/build/arm64/libbpfnest.so
SIZE 1823888
SHA256 5cb94a4564d215ab20e926637f0352723574e81425a074c547188c9e60d75479
ELF64 little-endian AArch64 type=3 (3=DYN, 2=EXEC)
ANDROID_NOTE_API 26
BUILD_ID 28194b8055b94e9566711b74922b3885b5cbc575
.dynsym: 62 symbols
  SYMBOL syscall value=0x0 size=0x0 shndx=0
  SYMBOL zn_module value=0x3f5d8 size=0x10 shndx=23
JUMP_SLOT syscall GOT=0x3b5d0 addend=0
.symtab: 1949 symbols
  SYMBOL zn_module value=0x3f5d8 size=0x10 shndx=23
  SYMBOL syscall value=0x0 size=0x0 shndx=0
MODULE_CHECK PASS: exported zn_module, target_api_version=4, ARM64 DYN
READELF /usr/bin/llvm-readelf
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              DYN (Shared object file)
  Machine:                           AArch64
  Version:                           0x1
  Entry point address:               0x0
  Start of program headers:          64 (bytes into file)
  Start of section headers:          1821584 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         9
  Size of section headers:           64 (bytes)
  Number of section headers:         36
  Section header string table index: 34

Elf file type is DYN (Shared object file)
Entry point 0x0
There are 9 program headers, starting at offset 64

Program Headers:
  Type           Offset   VirtAddr           PhysAddr           FileSiz  MemSiz   Flg Align
  PHDR           0x000040 0x0000000000000040 0x0000000000000040 0x0001f8 0x0001f8 R   0x8
  LOAD           0x000000 0x0000000000000000 0x0000000000000000 0x034280 0x034280 R E 0x4000
  LOAD           0x034280 0x0000000000038280 0x0000000000038280 0x003358 0x003d80 RW  0x4000
  LOAD           0x0375d8 0x000000000003f5d8 0x000000000003f5d8 0x0000c0 0x000950 RW  0x4000
  DYNAMIC        0x0371f0 0x000000000003b1f0 0x000000000003b1f0 0x0001c0 0x0001c0 RW  0x8
  GNU_RELRO      0x034280 0x0000000000038280 0x0000000000038280 0x003358 0x003d80 R   0x1
  GNU_EH_FRAME   0x00c880 0x000000000000c880 0x000000000000c880 0x000eac 0x000eac R   0x4
  GNU_STACK      0x000000 0x0000000000000000 0x0000000000000000 0x000000 0x000000 RW  0x0
  NOTE           0x000238 0x0000000000000238 0x0000000000000238 0x0000bc 0x0000bc R   0x4

 Section to Segment mapping:
  Segment Sections...
   00     
   01     .note.android.ident .note.gnu.build-id .dynsym .gnu.version .gnu.version_r .gnu.hash .dynstr .rela.dyn .rela.plt .rodata .gcc_except_table .eh_frame_hdr .eh_frame .text .plt 
   02     .data.rel.ro .fini_array .init_array .dynamic .got .got.plt .relro_padding 
   03     .data .bss 
   04     .dynamic 
   05     .data.rel.ro .fini_array .init_array .dynamic .got .got.plt .relro_padding 
   06     .eh_frame_hdr 
   07     
   08     .note.android.ident .note.gnu.build-id 
   None   .comment .debug_loc .debug_abbrev .debug_info .debug_ranges .debug_str .debug_line .debug_aranges .symtab .shstrtab .strtab 
Dynamic section at offset 0x371f0 contains 28 entries:
  Tag                Type           Name/Value
  0x0000000000000001 (NEEDED)       Shared library: [liblog.so]
  0x0000000000000001 (NEEDED)       Shared library: [libdl.so]
  0x0000000000000001 (NEEDED)       Shared library: [libm.so]
  0x0000000000000001 (NEEDED)       Shared library: [libc.so]
  0x000000000000000e (SONAME)       Library soname: [libbpfnest.so]
  0x000000000000001e (FLAGS)        BIND_NOW 
  0x000000006ffffffb (FLAGS_1)      NOW 
  0x0000000000000007 (RELA)         0xc90
  0x0000000000000008 (RELASZ)       33000 (bytes)
  0x0000000000000009 (RELAENT)      24 (bytes)
  0x000000006ffffff9 (RELACOUNT)    1374
  0x0000000000000017 (JMPREL)       0x8d78
  0x0000000000000002 (PLTRELSZ)     1416 (bytes)
  0x0000000000000003 (PLTGOT)       0x3b3e8
  0x0000000000000014 (PLTREL)       RELA
  0x0000000000000006 (SYMTAB)       0x2f8
  0x000000000000000b (SYMENT)       24 (bytes)
  0x0000000000000005 (STRTAB)       0x9a8
  0x000000000000000a (STRSZ)        737 (bytes)
  0x000000006ffffef5 (GNU_HASH)     0x988
  0x0000000000000019 (INIT_ARRAY)   0x3b1e0
  0x000000000000001b (INIT_ARRAYSZ) 16 (bytes)
  0x000000000000001a (FINI_ARRAY)   0x3b1d0
  0x000000000000001c (FINI_ARRAYSZ) 16 (bytes)
  0x000000006ffffff0 (VERSYM)       0x8c8
  0x000000006ffffffe (VERNEED)      0x944
  0x000000006fffffff (VERNEEDNUM)   2
  0x0000000000000000 (NULL)         0x0
DISASSEMBLER /usr/bin/llvm-objdump; syscall/svc matching lines=85
  0000000000013644 <bpfnest_syscall_entry>:
  16160: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1633c: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16a3c: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16be0: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16c60: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16cd0: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16d40: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16dc0: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16e30: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16f2c: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16f9c: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1701c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1709c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  17158: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  171c8: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  17248: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  17364: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  175f4: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1769c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1770c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1778c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1786c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  178ec: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1796c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1882c: 90000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19368: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19418: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  194c8: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19578: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19628: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  198c0: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19974: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19a18: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19ac8: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19b74: f00000ea     	adrp	x10, 0x38000 <syscall@plt+0x3d90>
  19c24: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19cb4: f00000f8     	adrp	x24, 0x38000 <syscall@plt+0x3d90>
  1a210: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1a38c: d00000e9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1a42c: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1a740: d00000e9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1ae94: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1afb4: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1b1dc: b00000ea     	adrp	x10, 0x38000 <syscall@plt+0x3d90>
  1bad0: b00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1bc50: b00000e9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1c260: 900000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1c6b0: 900000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1cad8: 900000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1d330: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1d3f0: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1d4b8: f00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1d590: f00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1d670: f00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1d730: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1d7f4: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1d8bc: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1e42c: d00000ca     	adrp	x10, 0x38000 <syscall@plt+0x3d90>
  1eb30: d00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1f134: b00000da     	adrp	x26, 0x38000 <syscall@plt+0x3d90>
  1f554: b00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1f75c: b00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1fedc: b00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  204b4: 900000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  22770: d00000a9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  228ec: d00000a9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  24760: 900000a9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  283a0: 90000088     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  2972c: f0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2a828: d0000068     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  2b020: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b0cc: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b178: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b224: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b2d0: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b37c: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b428: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b620: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b6cc: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b778: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b824: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2babc: b0000068     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  31164: 94000c43     	bl	0x34270 <syscall@plt>
  0000000000034270 <syscall@plt>:
LIMIT: relocations/symbols do not prove runtime reachability, loader timing, RELRO hook success or seccomp/SELinux access. Direct svc, LTO/inlining, stripped symbols or another image may bypass this PLT path.
ARM64 QEMU PASS: production trampoline + production dispatcher; all seven argument words, exact LR, copy isolation, observe/no query, empty patch, nonempty refusal, return/input+output errno
TOOLING_TESTS PASS: release defaults/scope/hash tampering and malformed/API3 ELF rejected
VERIFY PASS: configured host suite, Android build/static checks and available QEMU completed
```

## `BpfNest/scripts/package.sh`

Exit **0**

```text
PACKAGE UTC 2026-10-25T16:25:26Z
VERIFY UTC 2026-10-25T16:25:26Z
-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /home/neurax/NetdMultiAttach/BpfNest/build/host
ninja: no work to do.
Test project /home/neurax/NetdMultiAttach/BpfNest/build/host
    Start 1: core
1/5 Test #1: core .............................   Passed    0.00 sec
    Start 2: install
2/5 Test #2: install ..........................   Passed    0.00 sec
    Start 3: elf
3/5 Test #3: elf ..............................   Passed    0.01 sec
    Start 4: fd
4/5 Test #4: fd ...............................   Passed    0.00 sec
    Start 5: host_smoke
5/5 Test #5: host_smoke .......................   Passed    0.00 sec

100% tests passed, 0 tests failed out of 5

Total Test time (real) =   0.02 sec
NDK=/home/neurax/Android/Sdk/ndk/29.0.14206865
ABI=arm64-v8a
API=26
-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /home/neurax/NetdMultiAttach/BpfNest/build/arm64
ninja: no work to do.
FILE /home/neurax/NetdMultiAttach/BpfNest/build/arm64/libbpfnest.so
SIZE 1823888
SHA256 5cb94a4564d215ab20e926637f0352723574e81425a074c547188c9e60d75479
ELF64 little-endian AArch64 type=3 (3=DYN, 2=EXEC)
ANDROID_NOTE_API 26
BUILD_ID 28194b8055b94e9566711b74922b3885b5cbc575
.dynsym: 62 symbols
  SYMBOL syscall value=0x0 size=0x0 shndx=0
  SYMBOL zn_module value=0x3f5d8 size=0x10 shndx=23
JUMP_SLOT syscall GOT=0x3b5d0 addend=0
.symtab: 1949 symbols
  SYMBOL zn_module value=0x3f5d8 size=0x10 shndx=23
  SYMBOL syscall value=0x0 size=0x0 shndx=0
MODULE_CHECK PASS: exported zn_module, target_api_version=4, ARM64 DYN
READELF /usr/bin/llvm-readelf
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              DYN (Shared object file)
  Machine:                           AArch64
  Version:                           0x1
  Entry point address:               0x0
  Start of program headers:          64 (bytes into file)
  Start of section headers:          1821584 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         9
  Size of section headers:           64 (bytes)
  Number of section headers:         36
  Section header string table index: 34

Elf file type is DYN (Shared object file)
Entry point 0x0
There are 9 program headers, starting at offset 64

Program Headers:
  Type           Offset   VirtAddr           PhysAddr           FileSiz  MemSiz   Flg Align
  PHDR           0x000040 0x0000000000000040 0x0000000000000040 0x0001f8 0x0001f8 R   0x8
  LOAD           0x000000 0x0000000000000000 0x0000000000000000 0x034280 0x034280 R E 0x4000
  LOAD           0x034280 0x0000000000038280 0x0000000000038280 0x003358 0x003d80 RW  0x4000
  LOAD           0x0375d8 0x000000000003f5d8 0x000000000003f5d8 0x0000c0 0x000950 RW  0x4000
  DYNAMIC        0x0371f0 0x000000000003b1f0 0x000000000003b1f0 0x0001c0 0x0001c0 RW  0x8
  GNU_RELRO      0x034280 0x0000000000038280 0x0000000000038280 0x003358 0x003d80 R   0x1
  GNU_EH_FRAME   0x00c880 0x000000000000c880 0x000000000000c880 0x000eac 0x000eac R   0x4
  GNU_STACK      0x000000 0x0000000000000000 0x0000000000000000 0x000000 0x000000 RW  0x0
  NOTE           0x000238 0x0000000000000238 0x0000000000000238 0x0000bc 0x0000bc R   0x4

 Section to Segment mapping:
  Segment Sections...
   00     
   01     .note.android.ident .note.gnu.build-id .dynsym .gnu.version .gnu.version_r .gnu.hash .dynstr .rela.dyn .rela.plt .rodata .gcc_except_table .eh_frame_hdr .eh_frame .text .plt 
   02     .data.rel.ro .fini_array .init_array .dynamic .got .got.plt .relro_padding 
   03     .data .bss 
   04     .dynamic 
   05     .data.rel.ro .fini_array .init_array .dynamic .got .got.plt .relro_padding 
   06     .eh_frame_hdr 
   07     
   08     .note.android.ident .note.gnu.build-id 
   None   .comment .debug_loc .debug_abbrev .debug_info .debug_ranges .debug_str .debug_line .debug_aranges .symtab .shstrtab .strtab 
Dynamic section at offset 0x371f0 contains 28 entries:
  Tag                Type           Name/Value
  0x0000000000000001 (NEEDED)       Shared library: [liblog.so]
  0x0000000000000001 (NEEDED)       Shared library: [libdl.so]
  0x0000000000000001 (NEEDED)       Shared library: [libm.so]
  0x0000000000000001 (NEEDED)       Shared library: [libc.so]
  0x000000000000000e (SONAME)       Library soname: [libbpfnest.so]
  0x000000000000001e (FLAGS)        BIND_NOW 
  0x000000006ffffffb (FLAGS_1)      NOW 
  0x0000000000000007 (RELA)         0xc90
  0x0000000000000008 (RELASZ)       33000 (bytes)
  0x0000000000000009 (RELAENT)      24 (bytes)
  0x000000006ffffff9 (RELACOUNT)    1374
  0x0000000000000017 (JMPREL)       0x8d78
  0x0000000000000002 (PLTRELSZ)     1416 (bytes)
  0x0000000000000003 (PLTGOT)       0x3b3e8
  0x0000000000000014 (PLTREL)       RELA
  0x0000000000000006 (SYMTAB)       0x2f8
  0x000000000000000b (SYMENT)       24 (bytes)
  0x0000000000000005 (STRTAB)       0x9a8
  0x000000000000000a (STRSZ)        737 (bytes)
  0x000000006ffffef5 (GNU_HASH)     0x988
  0x0000000000000019 (INIT_ARRAY)   0x3b1e0
  0x000000000000001b (INIT_ARRAYSZ) 16 (bytes)
  0x000000000000001a (FINI_ARRAY)   0x3b1d0
  0x000000000000001c (FINI_ARRAYSZ) 16 (bytes)
  0x000000006ffffff0 (VERSYM)       0x8c8
  0x000000006ffffffe (VERNEED)      0x944
  0x000000006fffffff (VERNEEDNUM)   2
  0x0000000000000000 (NULL)         0x0
DISASSEMBLER /usr/bin/llvm-objdump; syscall/svc matching lines=85
  0000000000013644 <bpfnest_syscall_entry>:
  16160: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1633c: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16a3c: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16be0: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16c60: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16cd0: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16d40: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16dc0: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16e30: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16f2c: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16f9c: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1701c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1709c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  17158: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  171c8: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  17248: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  17364: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  175f4: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1769c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1770c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1778c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1786c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  178ec: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1796c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1882c: 90000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19368: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19418: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  194c8: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19578: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19628: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  198c0: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19974: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19a18: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19ac8: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19b74: f00000ea     	adrp	x10, 0x38000 <syscall@plt+0x3d90>
  19c24: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19cb4: f00000f8     	adrp	x24, 0x38000 <syscall@plt+0x3d90>
  1a210: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1a38c: d00000e9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1a42c: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1a740: d00000e9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1ae94: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1afb4: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1b1dc: b00000ea     	adrp	x10, 0x38000 <syscall@plt+0x3d90>
  1bad0: b00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1bc50: b00000e9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1c260: 900000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1c6b0: 900000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1cad8: 900000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1d330: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1d3f0: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1d4b8: f00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1d590: f00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1d670: f00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1d730: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1d7f4: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1d8bc: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1e42c: d00000ca     	adrp	x10, 0x38000 <syscall@plt+0x3d90>
  1eb30: d00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1f134: b00000da     	adrp	x26, 0x38000 <syscall@plt+0x3d90>
  1f554: b00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1f75c: b00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1fedc: b00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  204b4: 900000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  22770: d00000a9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  228ec: d00000a9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  24760: 900000a9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  283a0: 90000088     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  2972c: f0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2a828: d0000068     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  2b020: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b0cc: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b178: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b224: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b2d0: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b37c: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b428: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b620: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b6cc: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b778: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b824: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2babc: b0000068     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  31164: 94000c43     	bl	0x34270 <syscall@plt>
  0000000000034270 <syscall@plt>:
LIMIT: relocations/symbols do not prove runtime reachability, loader timing, RELRO hook success or seccomp/SELinux access. Direct svc, LTO/inlining, stripped symbols or another image may bypass this PLT path.
ARM64 QEMU PASS: production trampoline + production dispatcher; all seven argument words, exact LR, copy isolation, observe/no query, empty patch, nonempty refusal, return/input+output errno
TOOLING_TESTS PASS: release defaults/scope/hash tampering and malformed/API3 ELF rejected
VERIFY PASS: configured host suite, Android build/static checks and available QEMU completed
TOOLING_TESTS PASS: release defaults/scope/hash tampering and malformed/API3 ELF rejected
PACKAGE_CHECK PASS: exact manifest, observe default, netd-only scope, official installer, payload hashes
ZIP /home/neurax/NetdMultiAttach/BpfNest/dist/BpfNest-v0.1.0-arm64.zip
SHA256 c3f69af237bf1ccb3beb4f1fe55275aa2a6bf4f9c8f3500f1b4f5c0a00234891
SIZE 451474
PACKAGE_CHECK PASS: exact manifest, observe default, netd-only scope, official installer, payload hashes
bcf4b1d9913f3af17755569c853e0b5a75b8005f6a18eb3f86dadcc0e968c29d  META-INF/com/google/android/update-binary (612 bytes)
d2b8203193a083e43ad076fde01b9c7821e0aa521e1f8077b8628fb1cdb70c60  META-INF/com/google/android/updater-script (8 bytes)
f5f9b7b7c98ee32c67f013ff03eba0428abf1d8c2a950825e6f1f5b473d0157f  NOTICE.txt (1332 bytes)
8d7ff5c9f8213bfb5744b39f2dbca214feea94bffe6610fea89df6852784266d  SHA256SUMS (691 bytes)
525ab5d7a93c8bc4194e14d87cd26138a033b995a4cd794524ed5bfe5b3dfa5b  bpfnest.conf (69 bytes)
5d6d3b4b0816f6ef84cdce38dfad9b8cf8b5f636229ea3e198a96d229df4447d  customize.sh (1554 bytes)
5cb94a4564d215ab20e926637f0352723574e81425a074c547188c9e60d75479  libbpfnest.so (1823888 bytes)
2b82fd99e423f35381761952d2db682483a5931bc5ceab045fe64b2b98109b9d  module.prop (190 bytes)
3ade0c3d73b478141b13af03a66b0784a7fdeff87db7e5859e39924f434e0663  zn_modules.txt (36 bytes)
PACKAGE PASS: rebuilt module/API4 check, exact ZIP manifest and payload hashes
```


# Final coherent implementation (authoritative)

Earlier failures above are historical red/build integration attempts, not final passing evidence. Active API is core.h/install.h plus elf.hpp; obsolete core.hpp/module.hpp/module.cpp were removed.

## `BpfNest/scripts/verify.sh`

Exit **0**

```text
VERIFY UTC 2026-10-25T16:27:43Z
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Success
-- Found Threads: TRUE
-- Configuring done (0.1s)
-- Generating done (0.0s)
-- Build files have been written to: /home/neurax/NetdMultiAttach/BpfNest/build/host
[1/4] Building CXX object CMakeFiles/bpfnest_policy_safety_test.dir/tests/policy_safety_test.cpp.o
[2/4] Linking CXX executable bpfnest_policy_safety_test
[3/4] Building CXX object CMakeFiles/bpfnest_reentrancy_test.dir/tests/reentrancy_test.cpp.o
[4/4] Linking CXX executable bpfnest_reentrancy_test
Test project /home/neurax/NetdMultiAttach/BpfNest/build/host
    Start 1: core
1/7 Test #1: core .............................   Passed    0.00 sec
    Start 2: install
2/7 Test #2: install ..........................   Passed    0.00 sec
    Start 3: elf
3/7 Test #3: elf ..............................   Passed    0.01 sec
    Start 4: fd
4/7 Test #4: fd ...............................   Passed    0.00 sec
    Start 5: host_smoke
5/7 Test #5: host_smoke .......................   Passed    0.00 sec
    Start 6: reentrancy
6/7 Test #6: reentrancy .......................   Passed    0.00 sec
    Start 7: policy_safety
7/7 Test #7: policy_safety ....................   Passed    0.00 sec

100% tests passed, 0 tests failed out of 7

Total Test time (real) =   0.02 sec
NDK=/home/neurax/Android/Sdk/ndk/29.0.14206865
ABI=arm64-v8a
API=26
-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /home/neurax/NetdMultiAttach/BpfNest/build/arm64
ninja: no work to do.
FILE /home/neurax/NetdMultiAttach/BpfNest/build/arm64/libbpfnest.so
SIZE 1823888
SHA256 5cb94a4564d215ab20e926637f0352723574e81425a074c547188c9e60d75479
ELF64 little-endian AArch64 type=3 (3=DYN, 2=EXEC)
ANDROID_NOTE_API 26
BUILD_ID 28194b8055b94e9566711b74922b3885b5cbc575
.dynsym: 62 symbols
  SYMBOL syscall value=0x0 size=0x0 shndx=0
  SYMBOL zn_module value=0x3f5d8 size=0x10 shndx=23
JUMP_SLOT syscall GOT=0x3b5d0 addend=0
.symtab: 1949 symbols
  SYMBOL zn_module value=0x3f5d8 size=0x10 shndx=23
  SYMBOL syscall value=0x0 size=0x0 shndx=0
MODULE_CHECK PASS: exported zn_module, target_api_version=4, ARM64 DYN
READELF /usr/bin/llvm-readelf
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              DYN (Shared object file)
  Machine:                           AArch64
  Version:                           0x1
  Entry point address:               0x0
  Start of program headers:          64 (bytes into file)
  Start of section headers:          1821584 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         9
  Size of section headers:           64 (bytes)
  Number of section headers:         36
  Section header string table index: 34

Elf file type is DYN (Shared object file)
Entry point 0x0
There are 9 program headers, starting at offset 64

Program Headers:
  Type           Offset   VirtAddr           PhysAddr           FileSiz  MemSiz   Flg Align
  PHDR           0x000040 0x0000000000000040 0x0000000000000040 0x0001f8 0x0001f8 R   0x8
  LOAD           0x000000 0x0000000000000000 0x0000000000000000 0x034280 0x034280 R E 0x4000
  LOAD           0x034280 0x0000000000038280 0x0000000000038280 0x003358 0x003d80 RW  0x4000
  LOAD           0x0375d8 0x000000000003f5d8 0x000000000003f5d8 0x0000c0 0x000950 RW  0x4000
  DYNAMIC        0x0371f0 0x000000000003b1f0 0x000000000003b1f0 0x0001c0 0x0001c0 RW  0x8
  GNU_RELRO      0x034280 0x0000000000038280 0x0000000000038280 0x003358 0x003d80 R   0x1
  GNU_EH_FRAME   0x00c880 0x000000000000c880 0x000000000000c880 0x000eac 0x000eac R   0x4
  GNU_STACK      0x000000 0x0000000000000000 0x0000000000000000 0x000000 0x000000 RW  0x0
  NOTE           0x000238 0x0000000000000238 0x0000000000000238 0x0000bc 0x0000bc R   0x4

 Section to Segment mapping:
  Segment Sections...
   00     
   01     .note.android.ident .note.gnu.build-id .dynsym .gnu.version .gnu.version_r .gnu.hash .dynstr .rela.dyn .rela.plt .rodata .gcc_except_table .eh_frame_hdr .eh_frame .text .plt 
   02     .data.rel.ro .fini_array .init_array .dynamic .got .got.plt .relro_padding 
   03     .data .bss 
   04     .dynamic 
   05     .data.rel.ro .fini_array .init_array .dynamic .got .got.plt .relro_padding 
   06     .eh_frame_hdr 
   07     
   08     .note.android.ident .note.gnu.build-id 
   None   .comment .debug_loc .debug_abbrev .debug_info .debug_ranges .debug_str .debug_line .debug_aranges .symtab .shstrtab .strtab 
Dynamic section at offset 0x371f0 contains 28 entries:
  Tag                Type           Name/Value
  0x0000000000000001 (NEEDED)       Shared library: [liblog.so]
  0x0000000000000001 (NEEDED)       Shared library: [libdl.so]
  0x0000000000000001 (NEEDED)       Shared library: [libm.so]
  0x0000000000000001 (NEEDED)       Shared library: [libc.so]
  0x000000000000000e (SONAME)       Library soname: [libbpfnest.so]
  0x000000000000001e (FLAGS)        BIND_NOW 
  0x000000006ffffffb (FLAGS_1)      NOW 
  0x0000000000000007 (RELA)         0xc90
  0x0000000000000008 (RELASZ)       33000 (bytes)
  0x0000000000000009 (RELAENT)      24 (bytes)
  0x000000006ffffff9 (RELACOUNT)    1374
  0x0000000000000017 (JMPREL)       0x8d78
  0x0000000000000002 (PLTRELSZ)     1416 (bytes)
  0x0000000000000003 (PLTGOT)       0x3b3e8
  0x0000000000000014 (PLTREL)       RELA
  0x0000000000000006 (SYMTAB)       0x2f8
  0x000000000000000b (SYMENT)       24 (bytes)
  0x0000000000000005 (STRTAB)       0x9a8
  0x000000000000000a (STRSZ)        737 (bytes)
  0x000000006ffffef5 (GNU_HASH)     0x988
  0x0000000000000019 (INIT_ARRAY)   0x3b1e0
  0x000000000000001b (INIT_ARRAYSZ) 16 (bytes)
  0x000000000000001a (FINI_ARRAY)   0x3b1d0
  0x000000000000001c (FINI_ARRAYSZ) 16 (bytes)
  0x000000006ffffff0 (VERSYM)       0x8c8
  0x000000006ffffffe (VERNEED)      0x944
  0x000000006fffffff (VERNEEDNUM)   2
  0x0000000000000000 (NULL)         0x0
DISASSEMBLER /usr/bin/llvm-objdump; syscall/svc matching lines=85
  0000000000013644 <bpfnest_syscall_entry>:
  16160: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1633c: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16a3c: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16be0: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16c60: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16cd0: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16d40: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16dc0: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16e30: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16f2c: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16f9c: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1701c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1709c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  17158: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  171c8: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  17248: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  17364: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  175f4: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1769c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1770c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1778c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1786c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  178ec: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1796c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1882c: 90000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19368: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19418: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  194c8: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19578: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19628: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  198c0: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19974: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19a18: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19ac8: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19b74: f00000ea     	adrp	x10, 0x38000 <syscall@plt+0x3d90>
  19c24: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19cb4: f00000f8     	adrp	x24, 0x38000 <syscall@plt+0x3d90>
  1a210: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1a38c: d00000e9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1a42c: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1a740: d00000e9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1ae94: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1afb4: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1b1dc: b00000ea     	adrp	x10, 0x38000 <syscall@plt+0x3d90>
  1bad0: b00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1bc50: b00000e9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1c260: 900000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1c6b0: 900000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1cad8: 900000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1d330: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1d3f0: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1d4b8: f00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1d590: f00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1d670: f00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1d730: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1d7f4: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1d8bc: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1e42c: d00000ca     	adrp	x10, 0x38000 <syscall@plt+0x3d90>
  1eb30: d00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1f134: b00000da     	adrp	x26, 0x38000 <syscall@plt+0x3d90>
  1f554: b00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1f75c: b00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1fedc: b00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  204b4: 900000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  22770: d00000a9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  228ec: d00000a9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  24760: 900000a9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  283a0: 90000088     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  2972c: f0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2a828: d0000068     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  2b020: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b0cc: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b178: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b224: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b2d0: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b37c: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b428: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b620: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b6cc: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b778: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b824: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2babc: b0000068     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  31164: 94000c43     	bl	0x34270 <syscall@plt>
  0000000000034270 <syscall@plt>:
LIMIT: relocations/symbols do not prove runtime reachability, loader timing, RELRO hook success or seccomp/SELinux access. Direct svc, LTO/inlining, stripped symbols or another image may bypass this PLT path.
ARM64 QEMU PASS: production trampoline + production dispatcher; all seven argument words, exact LR, copy isolation, observe/no query, empty patch, nonempty refusal, return/input+output errno
TOOLING_TESTS PASS: release defaults/scope/hash tampering and malformed/API3 ELF rejected
VERIFY PASS: configured host suite, Android build/static checks and available QEMU completed
```

## `BpfNest/scripts/package.sh`

Exit **0**

```text
PACKAGE UTC 2026-10-25T16:27:45Z
VERIFY UTC 2026-10-25T16:27:45Z
-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /home/neurax/NetdMultiAttach/BpfNest/build/host
ninja: no work to do.
Test project /home/neurax/NetdMultiAttach/BpfNest/build/host
    Start 1: core
1/7 Test #1: core .............................   Passed    0.00 sec
    Start 2: install
2/7 Test #2: install ..........................   Passed    0.00 sec
    Start 3: elf
3/7 Test #3: elf ..............................   Passed    0.01 sec
    Start 4: fd
4/7 Test #4: fd ...............................   Passed    0.00 sec
    Start 5: host_smoke
5/7 Test #5: host_smoke .......................   Passed    0.00 sec
    Start 6: reentrancy
6/7 Test #6: reentrancy .......................   Passed    0.00 sec
    Start 7: policy_safety
7/7 Test #7: policy_safety ....................   Passed    0.00 sec

100% tests passed, 0 tests failed out of 7

Total Test time (real) =   0.02 sec
NDK=/home/neurax/Android/Sdk/ndk/29.0.14206865
ABI=arm64-v8a
API=26
-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /home/neurax/NetdMultiAttach/BpfNest/build/arm64
ninja: no work to do.
FILE /home/neurax/NetdMultiAttach/BpfNest/build/arm64/libbpfnest.so
SIZE 1823888
SHA256 5cb94a4564d215ab20e926637f0352723574e81425a074c547188c9e60d75479
ELF64 little-endian AArch64 type=3 (3=DYN, 2=EXEC)
ANDROID_NOTE_API 26
BUILD_ID 28194b8055b94e9566711b74922b3885b5cbc575
.dynsym: 62 symbols
  SYMBOL syscall value=0x0 size=0x0 shndx=0
  SYMBOL zn_module value=0x3f5d8 size=0x10 shndx=23
JUMP_SLOT syscall GOT=0x3b5d0 addend=0
.symtab: 1949 symbols
  SYMBOL zn_module value=0x3f5d8 size=0x10 shndx=23
  SYMBOL syscall value=0x0 size=0x0 shndx=0
MODULE_CHECK PASS: exported zn_module, target_api_version=4, ARM64 DYN
READELF /usr/bin/llvm-readelf
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              DYN (Shared object file)
  Machine:                           AArch64
  Version:                           0x1
  Entry point address:               0x0
  Start of program headers:          64 (bytes into file)
  Start of section headers:          1821584 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         9
  Size of section headers:           64 (bytes)
  Number of section headers:         36
  Section header string table index: 34

Elf file type is DYN (Shared object file)
Entry point 0x0
There are 9 program headers, starting at offset 64

Program Headers:
  Type           Offset   VirtAddr           PhysAddr           FileSiz  MemSiz   Flg Align
  PHDR           0x000040 0x0000000000000040 0x0000000000000040 0x0001f8 0x0001f8 R   0x8
  LOAD           0x000000 0x0000000000000000 0x0000000000000000 0x034280 0x034280 R E 0x4000
  LOAD           0x034280 0x0000000000038280 0x0000000000038280 0x003358 0x003d80 RW  0x4000
  LOAD           0x0375d8 0x000000000003f5d8 0x000000000003f5d8 0x0000c0 0x000950 RW  0x4000
  DYNAMIC        0x0371f0 0x000000000003b1f0 0x000000000003b1f0 0x0001c0 0x0001c0 RW  0x8
  GNU_RELRO      0x034280 0x0000000000038280 0x0000000000038280 0x003358 0x003d80 R   0x1
  GNU_EH_FRAME   0x00c880 0x000000000000c880 0x000000000000c880 0x000eac 0x000eac R   0x4
  GNU_STACK      0x000000 0x0000000000000000 0x0000000000000000 0x000000 0x000000 RW  0x0
  NOTE           0x000238 0x0000000000000238 0x0000000000000238 0x0000bc 0x0000bc R   0x4

 Section to Segment mapping:
  Segment Sections...
   00     
   01     .note.android.ident .note.gnu.build-id .dynsym .gnu.version .gnu.version_r .gnu.hash .dynstr .rela.dyn .rela.plt .rodata .gcc_except_table .eh_frame_hdr .eh_frame .text .plt 
   02     .data.rel.ro .fini_array .init_array .dynamic .got .got.plt .relro_padding 
   03     .data .bss 
   04     .dynamic 
   05     .data.rel.ro .fini_array .init_array .dynamic .got .got.plt .relro_padding 
   06     .eh_frame_hdr 
   07     
   08     .note.android.ident .note.gnu.build-id 
   None   .comment .debug_loc .debug_abbrev .debug_info .debug_ranges .debug_str .debug_line .debug_aranges .symtab .shstrtab .strtab 
Dynamic section at offset 0x371f0 contains 28 entries:
  Tag                Type           Name/Value
  0x0000000000000001 (NEEDED)       Shared library: [liblog.so]
  0x0000000000000001 (NEEDED)       Shared library: [libdl.so]
  0x0000000000000001 (NEEDED)       Shared library: [libm.so]
  0x0000000000000001 (NEEDED)       Shared library: [libc.so]
  0x000000000000000e (SONAME)       Library soname: [libbpfnest.so]
  0x000000000000001e (FLAGS)        BIND_NOW 
  0x000000006ffffffb (FLAGS_1)      NOW 
  0x0000000000000007 (RELA)         0xc90
  0x0000000000000008 (RELASZ)       33000 (bytes)
  0x0000000000000009 (RELAENT)      24 (bytes)
  0x000000006ffffff9 (RELACOUNT)    1374
  0x0000000000000017 (JMPREL)       0x8d78
  0x0000000000000002 (PLTRELSZ)     1416 (bytes)
  0x0000000000000003 (PLTGOT)       0x3b3e8
  0x0000000000000014 (PLTREL)       RELA
  0x0000000000000006 (SYMTAB)       0x2f8
  0x000000000000000b (SYMENT)       24 (bytes)
  0x0000000000000005 (STRTAB)       0x9a8
  0x000000000000000a (STRSZ)        737 (bytes)
  0x000000006ffffef5 (GNU_HASH)     0x988
  0x0000000000000019 (INIT_ARRAY)   0x3b1e0
  0x000000000000001b (INIT_ARRAYSZ) 16 (bytes)
  0x000000000000001a (FINI_ARRAY)   0x3b1d0
  0x000000000000001c (FINI_ARRAYSZ) 16 (bytes)
  0x000000006ffffff0 (VERSYM)       0x8c8
  0x000000006ffffffe (VERNEED)      0x944
  0x000000006fffffff (VERNEEDNUM)   2
  0x0000000000000000 (NULL)         0x0
DISASSEMBLER /usr/bin/llvm-objdump; syscall/svc matching lines=85
  0000000000013644 <bpfnest_syscall_entry>:
  16160: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1633c: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16a3c: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16be0: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16c60: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16cd0: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16d40: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16dc0: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16e30: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16f2c: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  16f9c: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1701c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1709c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  17158: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  171c8: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  17248: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  17364: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  175f4: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1769c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1770c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1778c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1786c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  178ec: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1796c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1882c: 90000108     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19368: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19418: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  194c8: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19578: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19628: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  198c0: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19974: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19a18: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19ac8: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19b74: f00000ea     	adrp	x10, 0x38000 <syscall@plt+0x3d90>
  19c24: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  19cb4: f00000f8     	adrp	x24, 0x38000 <syscall@plt+0x3d90>
  1a210: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1a38c: d00000e9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1a42c: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1a740: d00000e9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1ae94: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1afb4: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1b1dc: b00000ea     	adrp	x10, 0x38000 <syscall@plt+0x3d90>
  1bad0: b00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1bc50: b00000e9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1c260: 900000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1c6b0: 900000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1cad8: 900000e8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1d330: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1d3f0: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1d4b8: f00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1d590: f00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1d670: f00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1d730: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1d7f4: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1d8bc: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1e42c: d00000ca     	adrp	x10, 0x38000 <syscall@plt+0x3d90>
  1eb30: d00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1f134: b00000da     	adrp	x26, 0x38000 <syscall@plt+0x3d90>
  1f554: b00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  1f75c: b00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  1fedc: b00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  204b4: 900000c8     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  22770: d00000a9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  228ec: d00000a9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  24760: 900000a9     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  283a0: 90000088     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  2972c: f0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2a828: d0000068     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  2b020: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b0cc: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b178: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b224: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b2d0: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b37c: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b428: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b620: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b6cc: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b778: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2b824: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3d90>
  2babc: b0000068     	adrp	x8, 0x38000 <syscall@plt+0x3d90>
  31164: 94000c43     	bl	0x34270 <syscall@plt>
  0000000000034270 <syscall@plt>:
LIMIT: relocations/symbols do not prove runtime reachability, loader timing, RELRO hook success or seccomp/SELinux access. Direct svc, LTO/inlining, stripped symbols or another image may bypass this PLT path.
ARM64 QEMU PASS: production trampoline + production dispatcher; all seven argument words, exact LR, copy isolation, observe/no query, empty patch, nonempty refusal, return/input+output errno
TOOLING_TESTS PASS: release defaults/scope/hash tampering and malformed/API3 ELF rejected
VERIFY PASS: configured host suite, Android build/static checks and available QEMU completed
TOOLING_TESTS PASS: release defaults/scope/hash tampering and malformed/API3 ELF rejected
PACKAGE_CHECK PASS: exact manifest, observe default, netd-only scope, official installer, payload hashes
ZIP /home/neurax/NetdMultiAttach/BpfNest/dist/BpfNest-v0.1.0-arm64.zip
SHA256 c3f69af237bf1ccb3beb4f1fe55275aa2a6bf4f9c8f3500f1b4f5c0a00234891
SIZE 451474
PACKAGE_CHECK PASS: exact manifest, observe default, netd-only scope, official installer, payload hashes
bcf4b1d9913f3af17755569c853e0b5a75b8005f6a18eb3f86dadcc0e968c29d  META-INF/com/google/android/update-binary (612 bytes)
d2b8203193a083e43ad076fde01b9c7821e0aa521e1f8077b8628fb1cdb70c60  META-INF/com/google/android/updater-script (8 bytes)
f5f9b7b7c98ee32c67f013ff03eba0428abf1d8c2a950825e6f1f5b473d0157f  NOTICE.txt (1332 bytes)
8d7ff5c9f8213bfb5744b39f2dbca214feea94bffe6610fea89df6852784266d  SHA256SUMS (691 bytes)
525ab5d7a93c8bc4194e14d87cd26138a033b995a4cd794524ed5bfe5b3dfa5b  bpfnest.conf (69 bytes)
5d6d3b4b0816f6ef84cdce38dfad9b8cf8b5f636229ea3e198a96d229df4447d  customize.sh (1554 bytes)
5cb94a4564d215ab20e926637f0352723574e81425a074c547188c9e60d75479  libbpfnest.so (1823888 bytes)
2b82fd99e423f35381761952d2db682483a5931bc5ceab045fe64b2b98109b9d  module.prop (190 bytes)
3ade0c3d73b478141b13af03a66b0784a7fdeff87db7e5859e39924f434e0663  zn_modules.txt (36 bytes)
PACKAGE PASS: rebuilt module/API4 check, exact ZIP manifest and payload hashes
```


# Unrelated-call TLS fast path (final)

Regression: `scripts/smoke-arm64.sh` exited1 before the fix because the freestanding emutls access counter increased for disabled/non-BPF/non-attach/untrusted-LR dispatches. Fixed by returning directly to original before TLS access or attr-stack work. The counter covers module TLS access only, not a whole-runtime allocation/performance guarantee.

## `BpfNest/scripts/verify.sh`

Exit **0**

```text
VERIFY UTC 2026-10-25T16:33:27Z
-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /home/neurax/NetdMultiAttach/BpfNest/build/host
ninja: no work to do.
Test project /home/neurax/NetdMultiAttach/BpfNest/build/host
    Start 1: core
1/7 Test #1: core .............................   Passed    0.00 sec
    Start 2: install
2/7 Test #2: install ..........................   Passed    0.00 sec
    Start 3: elf
3/7 Test #3: elf ..............................   Passed    0.01 sec
    Start 4: fd
4/7 Test #4: fd ...............................   Passed    0.00 sec
    Start 5: host_smoke
5/7 Test #5: host_smoke .......................   Passed    0.00 sec
    Start 6: reentrancy
6/7 Test #6: reentrancy .......................   Passed    0.00 sec
    Start 7: policy_safety
7/7 Test #7: policy_safety ....................   Passed    0.00 sec

100% tests passed, 0 tests failed out of 7

Total Test time (real) =   0.02 sec
NDK=/home/neurax/Android/Sdk/ndk/29.0.14206865
ABI=arm64-v8a
API=26
-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /home/neurax/NetdMultiAttach/BpfNest/build/arm64
ninja: no work to do.
FILE /home/neurax/NetdMultiAttach/BpfNest/build/arm64/libbpfnest.so
SIZE 1823920
SHA256 6ab15a86946487dd0e36d13238fc2ec3fda5fb6151c910c5165770b7c7e9e676
ELF64 little-endian AArch64 type=3 (3=DYN, 2=EXEC)
ANDROID_NOTE_API 26
BUILD_ID 2f1fda52a9c3df4aa289e065fc320fa440295450
.dynsym: 62 symbols
  SYMBOL syscall value=0x0 size=0x0 shndx=0
  SYMBOL zn_module value=0x3f5c8 size=0x10 shndx=23
JUMP_SLOT syscall GOT=0x3b5c0 addend=0
.symtab: 1949 symbols
  SYMBOL zn_module value=0x3f5c8 size=0x10 shndx=23
  SYMBOL syscall value=0x0 size=0x0 shndx=0
MODULE_CHECK PASS: exported zn_module, target_api_version=4, ARM64 DYN
READELF /usr/bin/llvm-readelf
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              DYN (Shared object file)
  Machine:                           AArch64
  Version:                           0x1
  Entry point address:               0x0
  Start of program headers:          64 (bytes into file)
  Start of section headers:          1821616 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         9
  Size of section headers:           64 (bytes)
  Number of section headers:         36
  Section header string table index: 34

Elf file type is DYN (Shared object file)
Entry point 0x0
There are 9 program headers, starting at offset 64

Program Headers:
  Type           Offset   VirtAddr           PhysAddr           FileSiz  MemSiz   Flg Align
  PHDR           0x000040 0x0000000000000040 0x0000000000000040 0x0001f8 0x0001f8 R   0x8
  LOAD           0x000000 0x0000000000000000 0x0000000000000000 0x034270 0x034270 R E 0x4000
  LOAD           0x034270 0x0000000000038270 0x0000000000038270 0x003358 0x003d90 RW  0x4000
  LOAD           0x0375c8 0x000000000003f5c8 0x000000000003f5c8 0x0000c0 0x000950 RW  0x4000
  DYNAMIC        0x0371e0 0x000000000003b1e0 0x000000000003b1e0 0x0001c0 0x0001c0 RW  0x8
  GNU_RELRO      0x034270 0x0000000000038270 0x0000000000038270 0x003358 0x003d90 R   0x1
  GNU_EH_FRAME   0x00c880 0x000000000000c880 0x000000000000c880 0x000eac 0x000eac R   0x4
  GNU_STACK      0x000000 0x0000000000000000 0x0000000000000000 0x000000 0x000000 RW  0x0
  NOTE           0x000238 0x0000000000000238 0x0000000000000238 0x0000bc 0x0000bc R   0x4

 Section to Segment mapping:
  Segment Sections...
   00     
   01     .note.android.ident .note.gnu.build-id .dynsym .gnu.version .gnu.version_r .gnu.hash .dynstr .rela.dyn .rela.plt .rodata .gcc_except_table .eh_frame_hdr .eh_frame .text .plt 
   02     .data.rel.ro .fini_array .init_array .dynamic .got .got.plt .relro_padding 
   03     .data .bss 
   04     .dynamic 
   05     .data.rel.ro .fini_array .init_array .dynamic .got .got.plt .relro_padding 
   06     .eh_frame_hdr 
   07     
   08     .note.android.ident .note.gnu.build-id 
   None   .comment .debug_loc .debug_abbrev .debug_info .debug_ranges .debug_str .debug_line .debug_aranges .symtab .shstrtab .strtab 
Dynamic section at offset 0x371e0 contains 28 entries:
  Tag                Type           Name/Value
  0x0000000000000001 (NEEDED)       Shared library: [liblog.so]
  0x0000000000000001 (NEEDED)       Shared library: [libdl.so]
  0x0000000000000001 (NEEDED)       Shared library: [libm.so]
  0x0000000000000001 (NEEDED)       Shared library: [libc.so]
  0x000000000000000e (SONAME)       Library soname: [libbpfnest.so]
  0x000000000000001e (FLAGS)        BIND_NOW 
  0x000000006ffffffb (FLAGS_1)      NOW 
  0x0000000000000007 (RELA)         0xc90
  0x0000000000000008 (RELASZ)       33000 (bytes)
  0x0000000000000009 (RELAENT)      24 (bytes)
  0x000000006ffffff9 (RELACOUNT)    1374
  0x0000000000000017 (JMPREL)       0x8d78
  0x0000000000000002 (PLTRELSZ)     1416 (bytes)
  0x0000000000000003 (PLTGOT)       0x3b3d8
  0x0000000000000014 (PLTREL)       RELA
  0x0000000000000006 (SYMTAB)       0x2f8
  0x000000000000000b (SYMENT)       24 (bytes)
  0x0000000000000005 (STRTAB)       0x9a8
  0x000000000000000a (STRSZ)        737 (bytes)
  0x000000006ffffef5 (GNU_HASH)     0x988
  0x0000000000000019 (INIT_ARRAY)   0x3b1d0
  0x000000000000001b (INIT_ARRAYSZ) 16 (bytes)
  0x000000000000001a (FINI_ARRAY)   0x3b1c0
  0x000000000000001c (FINI_ARRAYSZ) 16 (bytes)
  0x000000006ffffff0 (VERSYM)       0x8c8
  0x000000006ffffffe (VERNEED)      0x944
  0x000000006fffffff (VERNEEDNUM)   2
  0x0000000000000000 (NULL)         0x0
DISASSEMBLER /usr/bin/llvm-objdump; syscall/svc matching lines=86
  0000000000013644 <bpfnest_syscall_entry>:
  16158: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16334: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16a34: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16bd8: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16c58: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16cc8: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16d38: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16db8: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16e28: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16f24: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16f94: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  17014: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  17094: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  17150: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  171c0: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  17240: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1735c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  175ec: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  17694: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  17704: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  17784: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  17864: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  178e4: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  17964: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  18824: 90000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  19360: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  19410: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  194c0: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  19570: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  19620: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  198b8: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1996c: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  19a10: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  19ac0: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  19b6c: f00000ea     	adrp	x10, 0x38000 <syscall@plt+0x3da0>
  19c1c: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  19cac: f00000f8     	adrp	x24, 0x38000 <syscall@plt+0x3da0>
  1a208: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1a384: d00000e9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  1a424: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1a738: d00000e9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  1ae8c: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1afac: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1b1d4: b00000ea     	adrp	x10, 0x38000 <syscall@plt+0x3da0>
  1bac8: b00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1bc48: b00000e9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  1c258: 900000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1c6a8: 900000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1cad0: 900000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1d328: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1d3e8: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1d4b0: f00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  1d588: f00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  1d668: f00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  1d728: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1d7ec: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1d8b4: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1e424: d00000ca     	adrp	x10, 0x38000 <syscall@plt+0x3da0>
  1eb28: d00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1f12c: b00000da     	adrp	x26, 0x38000 <syscall@plt+0x3da0>
  1f54c: b00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1f754: b00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  1fed4: b00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  204ac: 900000c8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  2256c: d00000a9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  22768: d00000a9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  228e4: d00000a9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  24758: 900000a9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  28398: 90000088     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  29724: f0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2a820: d0000068     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  2b018: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b0c4: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b170: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b21c: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b2c8: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b374: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b420: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b618: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b6c4: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b770: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b81c: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2bab4: b0000068     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  31154: 94000c43     	bl	0x34260 <syscall@plt>
  0000000000034260 <syscall@plt>:
LIMIT: relocations/symbols do not prove runtime reachability, loader timing, RELRO hook success or seccomp/SELinux access. Direct svc, LTO/inlining, stripped symbols or another image may bypass this PLT path.
ARM64 QEMU PASS: production trampoline + production dispatcher; all seven argument words, exact LR, copy isolation, observe/no query, empty patch, nonempty refusal, return/input+output errno
TOOLING_TESTS PASS: release defaults/scope/hash tampering and malformed/API3 ELF rejected
VERIFY PASS: configured host suite, Android build/static checks and available QEMU completed
```

## `BpfNest/scripts/package.sh`

Exit **0**

```text
PACKAGE UTC 2026-10-25T16:33:28Z
VERIFY UTC 2026-10-25T16:33:28Z
-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /home/neurax/NetdMultiAttach/BpfNest/build/host
ninja: no work to do.
Test project /home/neurax/NetdMultiAttach/BpfNest/build/host
    Start 1: core
1/7 Test #1: core .............................   Passed    0.00 sec
    Start 2: install
2/7 Test #2: install ..........................   Passed    0.00 sec
    Start 3: elf
3/7 Test #3: elf ..............................   Passed    0.01 sec
    Start 4: fd
4/7 Test #4: fd ...............................   Passed    0.00 sec
    Start 5: host_smoke
5/7 Test #5: host_smoke .......................   Passed    0.00 sec
    Start 6: reentrancy
6/7 Test #6: reentrancy .......................   Passed    0.00 sec
    Start 7: policy_safety
7/7 Test #7: policy_safety ....................   Passed    0.00 sec

100% tests passed, 0 tests failed out of 7

Total Test time (real) =   0.02 sec
NDK=/home/neurax/Android/Sdk/ndk/29.0.14206865
ABI=arm64-v8a
API=26
-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /home/neurax/NetdMultiAttach/BpfNest/build/arm64
ninja: no work to do.
FILE /home/neurax/NetdMultiAttach/BpfNest/build/arm64/libbpfnest.so
SIZE 1823920
SHA256 6ab15a86946487dd0e36d13238fc2ec3fda5fb6151c910c5165770b7c7e9e676
ELF64 little-endian AArch64 type=3 (3=DYN, 2=EXEC)
ANDROID_NOTE_API 26
BUILD_ID 2f1fda52a9c3df4aa289e065fc320fa440295450
.dynsym: 62 symbols
  SYMBOL syscall value=0x0 size=0x0 shndx=0
  SYMBOL zn_module value=0x3f5c8 size=0x10 shndx=23
JUMP_SLOT syscall GOT=0x3b5c0 addend=0
.symtab: 1949 symbols
  SYMBOL zn_module value=0x3f5c8 size=0x10 shndx=23
  SYMBOL syscall value=0x0 size=0x0 shndx=0
MODULE_CHECK PASS: exported zn_module, target_api_version=4, ARM64 DYN
READELF /usr/bin/llvm-readelf
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              DYN (Shared object file)
  Machine:                           AArch64
  Version:                           0x1
  Entry point address:               0x0
  Start of program headers:          64 (bytes into file)
  Start of section headers:          1821616 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         9
  Size of section headers:           64 (bytes)
  Number of section headers:         36
  Section header string table index: 34

Elf file type is DYN (Shared object file)
Entry point 0x0
There are 9 program headers, starting at offset 64

Program Headers:
  Type           Offset   VirtAddr           PhysAddr           FileSiz  MemSiz   Flg Align
  PHDR           0x000040 0x0000000000000040 0x0000000000000040 0x0001f8 0x0001f8 R   0x8
  LOAD           0x000000 0x0000000000000000 0x0000000000000000 0x034270 0x034270 R E 0x4000
  LOAD           0x034270 0x0000000000038270 0x0000000000038270 0x003358 0x003d90 RW  0x4000
  LOAD           0x0375c8 0x000000000003f5c8 0x000000000003f5c8 0x0000c0 0x000950 RW  0x4000
  DYNAMIC        0x0371e0 0x000000000003b1e0 0x000000000003b1e0 0x0001c0 0x0001c0 RW  0x8
  GNU_RELRO      0x034270 0x0000000000038270 0x0000000000038270 0x003358 0x003d90 R   0x1
  GNU_EH_FRAME   0x00c880 0x000000000000c880 0x000000000000c880 0x000eac 0x000eac R   0x4
  GNU_STACK      0x000000 0x0000000000000000 0x0000000000000000 0x000000 0x000000 RW  0x0
  NOTE           0x000238 0x0000000000000238 0x0000000000000238 0x0000bc 0x0000bc R   0x4

 Section to Segment mapping:
  Segment Sections...
   00     
   01     .note.android.ident .note.gnu.build-id .dynsym .gnu.version .gnu.version_r .gnu.hash .dynstr .rela.dyn .rela.plt .rodata .gcc_except_table .eh_frame_hdr .eh_frame .text .plt 
   02     .data.rel.ro .fini_array .init_array .dynamic .got .got.plt .relro_padding 
   03     .data .bss 
   04     .dynamic 
   05     .data.rel.ro .fini_array .init_array .dynamic .got .got.plt .relro_padding 
   06     .eh_frame_hdr 
   07     
   08     .note.android.ident .note.gnu.build-id 
   None   .comment .debug_loc .debug_abbrev .debug_info .debug_ranges .debug_str .debug_line .debug_aranges .symtab .shstrtab .strtab 
Dynamic section at offset 0x371e0 contains 28 entries:
  Tag                Type           Name/Value
  0x0000000000000001 (NEEDED)       Shared library: [liblog.so]
  0x0000000000000001 (NEEDED)       Shared library: [libdl.so]
  0x0000000000000001 (NEEDED)       Shared library: [libm.so]
  0x0000000000000001 (NEEDED)       Shared library: [libc.so]
  0x000000000000000e (SONAME)       Library soname: [libbpfnest.so]
  0x000000000000001e (FLAGS)        BIND_NOW 
  0x000000006ffffffb (FLAGS_1)      NOW 
  0x0000000000000007 (RELA)         0xc90
  0x0000000000000008 (RELASZ)       33000 (bytes)
  0x0000000000000009 (RELAENT)      24 (bytes)
  0x000000006ffffff9 (RELACOUNT)    1374
  0x0000000000000017 (JMPREL)       0x8d78
  0x0000000000000002 (PLTRELSZ)     1416 (bytes)
  0x0000000000000003 (PLTGOT)       0x3b3d8
  0x0000000000000014 (PLTREL)       RELA
  0x0000000000000006 (SYMTAB)       0x2f8
  0x000000000000000b (SYMENT)       24 (bytes)
  0x0000000000000005 (STRTAB)       0x9a8
  0x000000000000000a (STRSZ)        737 (bytes)
  0x000000006ffffef5 (GNU_HASH)     0x988
  0x0000000000000019 (INIT_ARRAY)   0x3b1d0
  0x000000000000001b (INIT_ARRAYSZ) 16 (bytes)
  0x000000000000001a (FINI_ARRAY)   0x3b1c0
  0x000000000000001c (FINI_ARRAYSZ) 16 (bytes)
  0x000000006ffffff0 (VERSYM)       0x8c8
  0x000000006ffffffe (VERNEED)      0x944
  0x000000006fffffff (VERNEEDNUM)   2
  0x0000000000000000 (NULL)         0x0
DISASSEMBLER /usr/bin/llvm-objdump; syscall/svc matching lines=86
  0000000000013644 <bpfnest_syscall_entry>:
  16158: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16334: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16a34: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16bd8: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16c58: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16cc8: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16d38: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16db8: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16e28: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16f24: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  16f94: d0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  17014: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  17094: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  17150: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  171c0: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  17240: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1735c: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  175ec: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  17694: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  17704: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  17784: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  17864: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  178e4: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  17964: b0000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  18824: 90000108     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  19360: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  19410: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  194c0: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  19570: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  19620: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  198b8: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1996c: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  19a10: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  19ac0: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  19b6c: f00000ea     	adrp	x10, 0x38000 <syscall@plt+0x3da0>
  19c1c: f00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  19cac: f00000f8     	adrp	x24, 0x38000 <syscall@plt+0x3da0>
  1a208: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1a384: d00000e9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  1a424: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1a738: d00000e9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  1ae8c: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1afac: d00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1b1d4: b00000ea     	adrp	x10, 0x38000 <syscall@plt+0x3da0>
  1bac8: b00000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1bc48: b00000e9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  1c258: 900000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1c6a8: 900000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1cad0: 900000e8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1d328: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1d3e8: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1d4b0: f00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  1d588: f00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  1d668: f00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  1d728: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1d7ec: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1d8b4: f00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1e424: d00000ca     	adrp	x10, 0x38000 <syscall@plt+0x3da0>
  1eb28: d00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1f12c: b00000da     	adrp	x26, 0x38000 <syscall@plt+0x3da0>
  1f54c: b00000c8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  1f754: b00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  1fed4: b00000c9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  204ac: 900000c8     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  2256c: d00000a9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  22768: d00000a9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  228e4: d00000a9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  24758: 900000a9     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  28398: 90000088     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  29724: f0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2a820: d0000068     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  2b018: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b0c4: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b170: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b21c: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b2c8: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b374: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b420: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b618: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b6c4: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b770: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2b81c: b0000069     	adrp	x9, 0x38000 <syscall@plt+0x3da0>
  2bab4: b0000068     	adrp	x8, 0x38000 <syscall@plt+0x3da0>
  31154: 94000c43     	bl	0x34260 <syscall@plt>
  0000000000034260 <syscall@plt>:
LIMIT: relocations/symbols do not prove runtime reachability, loader timing, RELRO hook success or seccomp/SELinux access. Direct svc, LTO/inlining, stripped symbols or another image may bypass this PLT path.
ARM64 QEMU PASS: production trampoline + production dispatcher; all seven argument words, exact LR, copy isolation, observe/no query, empty patch, nonempty refusal, return/input+output errno
TOOLING_TESTS PASS: release defaults/scope/hash tampering and malformed/API3 ELF rejected
VERIFY PASS: configured host suite, Android build/static checks and available QEMU completed
TOOLING_TESTS PASS: release defaults/scope/hash tampering and malformed/API3 ELF rejected
PACKAGE_CHECK PASS: exact manifest, observe default, netd-only scope, official installer, payload hashes
ZIP /home/neurax/NetdMultiAttach/BpfNest/dist/BpfNest-v0.1.0-arm64.zip
SHA256 0e326083c20f7d7956e284b2c502475c2e1ca2fcb2c47075b7db50971b11890c
SIZE 451519
PACKAGE_CHECK PASS: exact manifest, observe default, netd-only scope, official installer, payload hashes
bcf4b1d9913f3af17755569c853e0b5a75b8005f6a18eb3f86dadcc0e968c29d  META-INF/com/google/android/update-binary (612 bytes)
d2b8203193a083e43ad076fde01b9c7821e0aa521e1f8077b8628fb1cdb70c60  META-INF/com/google/android/updater-script (8 bytes)
f5f9b7b7c98ee32c67f013ff03eba0428abf1d8c2a950825e6f1f5b473d0157f  NOTICE.txt (1332 bytes)
da79f4509228d70ca4d35e65218e33d5e2f187bb57c9e63fb1d0553b661e0b43  SHA256SUMS (691 bytes)
525ab5d7a93c8bc4194e14d87cd26138a033b995a4cd794524ed5bfe5b3dfa5b  bpfnest.conf (69 bytes)
5d6d3b4b0816f6ef84cdce38dfad9b8cf8b5f636229ea3e198a96d229df4447d  customize.sh (1554 bytes)
6ab15a86946487dd0e36d13238fc2ec3fda5fb6151c910c5165770b7c7e9e676  libbpfnest.so (1823920 bytes)
2b82fd99e423f35381761952d2db682483a5931bc5ceab045fe64b2b98109b9d  module.prop (190 bytes)
3ade0c3d73b478141b13af03a66b0784a7fdeff87db7e5859e39924f434e0663  zn_modules.txt (36 bytes)
PACKAGE PASS: rebuilt module/API4 check, exact ZIP manifest and payload hashes
```

## Independent native worker execution

These commands were actually executed against the integrated core.h/install.h tree; no device deployment:

```sh
cmake -S BpfNest -B BpfNest/build/host -G Ninja -DCMAKE_BUILD_TYPE=Release && cmake --build BpfNest/build/host -j 2 && ctest --test-dir BpfNest/build/host --output-on-failure
```
Exit 0: 18 build steps; core/install/elf/fd/host_smoke/reentrancy/policy_safety all passed; `100% tests passed, 0 tests failed out of 7`.

```sh
cmake -S BpfNest -B BpfNest/build/arm64 -G Ninja -DCMAKE_TOOLCHAIN_FILE=/home/neurax/Android/Sdk/ndk/29.0.14206865/build/cmake/android.toolchain.cmake -DANDROID_ABI=arm64-v8a -DANDROID_PLATFORM=android-26 -DANDROID_STL=c++_static -DCMAKE_BUILD_TYPE=Release && cmake --build BpfNest/build/arm64 -j 2
```
Exit 0; CMake generated, `ninja: no work to do` (already-built integrated artifact, not a claim of a fresh compilation).

```sh
clang --target=aarch64-linux-gnu -fuse-ld=lld -nostdlib -static -Wl,-e,_start BpfNest/tests/trampoline_smoke.S -o /tmp/bpfnest-trampoline-red
```
Exit 1: undefined `bpfnest_syscall_entry`. Adding actual entry:
```sh
clang --target=aarch64-linux-gnu -fuse-ld=lld -nostdlib -static -Wl,-e,_start BpfNest/tests/trampoline_smoke.S BpfNest/src/trampoline.S -o /tmp/bpfnest-trampoline && qemu-aarch64-static /tmp/bpfnest-trampoline
```
Exit 0: `aarch64-smoke: x0..x6/LR/callee-saved and real mmap sixth argument/getpid/ENOSYS passed`.
This independent Linux AArch64 raw-syscall harness executes actual entry; checks sixth-argument unaligned mmap offset EINVAL, aligned mmap/read/write/munmap, getpid and ENOSYS. It is NOT Android execution.

```sh
bash BpfNest/scripts/smoke-arm64.sh
```
Exit 0: production trampoline/dispatcher QEMU smoke passed exact LR, registers, copy isolation, observe, empty patch, nonempty refusal, input/output errno.

```sh
c++ -std=c++17 -Wall -Wextra -Werror -I BpfNest/include BpfNest/tests/host_smoke.cpp BpfNest/src/core.cpp -o /tmp/bpfnest-host-smoke && /tmp/bpfnest-host-smoke
```
Exit 0: real cgroup2 dev/inode/fs identity + descriptor pins + safe self-read + changed dispatch + original-buffer/errno semantics; real libc getpid/ENOSYS. Only privileged BPF query/attach interface doubled.

```sh
c++ -std=c++17 -Wall -Wextra -Werror -I BpfNest/include BpfNest/tests/elf_tests.cpp BpfNest/src/elf.cpp -o /tmp/bpfnest-elf-final && /tmp/bpfnest-elf-final libnetd_updatable.so
```
Exit 0: real supplied ELF accepted; malformed identity/layout/callsite rejected.

```sh
bash BpfNest/scripts/inspect-elf.sh BpfNest/build/arm64/libbpfnest.so
llvm-objdump -d --disassemble-symbols=bpfnest_syscall_entry BpfNest/build/arm64/libbpfnest.so
llvm-nm -D --defined-only BpfNest/build/arm64/libbpfnest.so
cmp analysis/zygisk_next_api.main.h BpfNest/third_party/zygisk_next_api.h && sha256sum BpfNest/third_party/zygisk_next_api.h
```
All exit 0. Android note API26; ELF64 LE AArch64 DYN; RELRO/NOW; nonexecuting GNU_STACK; NEEDED liblog/libdl/libm/libc; only defined dynamic symbol `zn_module`. Actual entry stores x0..x6 and LR before C dispatch. Header byte-identical, SHA256 `957922aeec1bb31091cc37dca29267b7fa41e4e109f679af1526ee4c5e4325c8`.

### TDD qualification

- Initial core.hpp-based tests: absent parse_config/dispatch link RED, then seven behavior groups GREEN. That interface/test file was removed during director integration; **not evidence for final core.h coverage**.
- Real ELF test: absent validate_target link RED, then real ELF + mutations GREEN; retained final test reran above.
- Reentrancy test passed first run after director had implemented guard; characterization only, not claimed RED→GREEN.
- Original policy_safety test expecting existing direct MULTI to be patched failed (`f.patched&&f.queries==1`). Director subsequently changed expectation to refusal; that is a changed acceptance, **not the same test turning GREEN**. Integrated implementation currently refuses ALL nonempty direct attachments (including MULTI); this limitation must remain explicit, not silently described as support for existing MULTI.

### Expanded installation concurrency coverage

```sh
c++ -std=c++17 -Wall -Wextra -Werror -pthread -I BpfNest/include -I BpfNest/third_party BpfNest/tests/install_concurrency_test.cpp BpfNest/src/install.cpp -o /tmp/bpfnest-install-concurrency && /tmp/bpfnest-install-concurrency
cmake -S BpfNest -B BpfNest/build/host -G Ninja -DCMAKE_BUILD_TYPE=Release && cmake --build BpfNest/build/host -j 2 && ctest --test-dir BpfNest/build/host --output-on-failure
```
Both exit0. Manual test: `install-concurrency: pre-return publication, duplicate race, required API pointers and fresh process passed`. Full suite builds 20 steps and reports `100% tests passed, 0 tests failed out of 8` (core/install/elf/fd/host_smoke/reentrancy/policy_safety/install_concurrency). Test blocks actual concurrent installation inside the provider, verifies passthrough until hook success and duplicate suppression, all five required API pointer failures, and independent fresh-process state. This is added characterization coverage, not claimed RED-first.

## 最终独立收尾验证（取代上文历史计数）

2026-10-25T16:41:11Z执行 `BpfNest/scripts/package.sh`，退出0。仅在CMake注册已有install_concurrency_test.cpp，链接bpfnest_core与Threads::Threads；无生产源码修改。

host CTest **8/8**。Android API26/arm64与API4静态检查、生产trampoline/dispatcher QEMU、工具回归、ZIP CRC/manifest/payload哈希全部通过。完整输出见build/logs/package.log与verify.log。

最终so：1823920字节，SHA256 `6ab15a86946487dd0e36d13238fc2ec3fda5fb6151c910c5165770b7c7e9e676`。最终ZIP：451519字节，SHA256 `0e326083c20f7d7956e284b2c502475c2e1ca2fcb2c47075b7db50971b11890c`。没有设备验证或部署，既有MULTI拒绝及重启风险不变。
