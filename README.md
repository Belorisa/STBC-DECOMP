# Star Trek: Bridge Commander — matching decompilation

C/C++ source for **Star Trek: Bridge Commander** (`stbc.exe`, patch 1.1, 2002) that, built with the original
compiler and flags, reproduces the retail executable's code byte for byte, function by function. The aim is source
a reader can understand; matching bytes are the proof that it is faithful.

This repository contains no part of the game. You need your own copy to build or verify anything, and the
project is not affiliated with the game's authors or rights holders: read [NOTICE.md](NOTICE.md) first.
What the contributors wrote is in the public domain under CC0 ([LICENSE.md](LICENSE.md)).

## Status

About 84% of the executable's code is written and verified; the table under "Progress" has the numbers this tree
builds. The statically linked Microsoft runtime (about 2% of the code) is matched too, but its source is
Microsoft's and is kept out of this repository, so those rows read 0 here. What is left is mostly large
functions that are written but differ from retail in register choices, frame layout or instruction order.

## What you need

- **The game**, patch 1.1. The reference is the GOG release's `stbc.exe` (hash below), copied to `orig/stbc.exe`.
- **Microsoft Visual C++ 6.0 with Service Pack 5**, as a portable copy under `tools/vc6/` ("Toolchain status"
  below says exactly which files). Windows only.
- **Python 3.10 or later** and `pip install -r requirements.txt` for the tools.
- **Ghidra** with the GhidraMCP plugin, optional, for analysis: analyse your own `stbc.exe`, then run
  `tools/ghidra/ApplyStbcAll.java` to load this tree's names and types into it.

The compiler folder holds more than one VC6 build, because retail's objects come from several: SP5 for almost
everything, and under `tools/vc6/_sp4`, `_pp` (the Processor Pack back end, four NetImmerse sound units) and
`_rtm_backup` the others. `docs/toolchain.md`, "Compilers and flags", says which files each needs.

Optional evidence that some report tools read, each from a path you can set (`docs/evidence.md` describes them):
the game's demo, the Bridge Commander SDK 1.1 and its tools, the 1.0 retail executable. None is needed to build,
compare or match.

## Build and verify

    python tools/decomp.py build      compile every unit, compare every annotated function with retail
    python tools/decomp.py gate       the proof: every matching function spliced into a copy of retail
                                      must give back the retail hash
    python tools/decomp.py diff <function or address>     side by side with retail

Nothing is linked into a playable executable: each unit is compiled on its own and each function compared.

## Contributing

`AGENTS.md` is the project guide: the commands, the statuses, what counts as a match and what does not.
`docs/rulings.md` holds the decisions made so far, `docs/levers/` what was learned about the compiler, and
`cards/` each open function's history. `python tools/openwork.py triage` lists what is open. The project is run
with Claude and the skills in `.claude/skills/`, but nothing depends on that: a function counts when the gate
says its bytes are retail's and the rule checker passes it, however it was written.

Please do not submit game files, the compiler, Microsoft's runtime source or any engine SDK file.

## Game files

- Installed game (GOG release): `C:\Recomp\Star Trek Bridge Commander`
- Target binary: `orig/stbc.exe` (copied from the install; not committed)
  - SHA-1 `f3f9932a5b0f9de353ba9a87e430818a4f61e59d`, 6,164,543 bytes
  - PE timestamp 2002-04-09 (patched 1.1 build; see `readme_patch.txt` in the install)
- Game logic is mostly Python 1.5.2 bytecode in the install's `scripts/` (`*.pyc`). That is not part of
  this decompilation; it only serves as evidence (it calls the engine through the `App` module).

## What the binary tells us

- **PE32 x86**, image base `0x400000`, `.text` about 4.7 MB. Linker 6.0. PDB name `stbc.pdb` (not shipped).
- **Compiler (Rich header):** most C/C++ objects come from MSVC 6.0 `cl` **12.00.8966** (446 C++ and 80 C
  objects). That build is normally VC6 SP5 with the Processor Pack; this still has to be confirmed. Other
  objects come from builds 8799, 8047 and 8447, probably prebuilt third-party and CRT libraries.
- **Statically linked libraries** (these are separate modules for the project plan):
  - **NetImmerse 3.1** (NDL), the 3D engine (`NetImmerse Version 3.1`)
  - **Python 1.5.2**, embedded interpreter. Its public source is a strong reference.
  - SWIG-generated `App` wrapper layer (bindings between the scripts and the engine)
  - Miles Sound System (`Mss32.dll`) and Bink (`binkw32.dll`), both imported DLLs, not linked in
  - MSVC6 CRT and Dinkumware STL (`std::basic_*` RTTI)
- **RTTI:** only the std/CRT classes have it, so the game itself was built without `/GR`. Class identity has
  to come from vtables, constructors and strings.

## Approximate module map

Based on which code refers to each module's identifying strings. The boundaries are rough, to 64 KB at best,
and still need to be confirmed at the function level.

| range (VA) | module | evidence |
|---|---|---|
| `0x401000`–`~0x73xxxx` | game + Totally Games `TG*` framework, SWIG `App` wrappers in roughly `0x55xxxx`–`0x6Exxxx` | `TG*` strings, `App.` strings, multiplayer `banlist`/`masterserver` near `0x6Axxxx`–`0x6Bxxxx` |
| `~0x740000`–`~0x786000` | Python 1.5.2 | `Py_Initialize` at `0x74A6xx`, list/tuple/string objects at `0x748`–`0x751xxx`, ceval/import/marshal at `0x758`–`0x777xxx` |
| `0x78CB70`–`~0x855760`, plus `0x86E3F0`–`0x86F720` after the CRT | NetImmerse 3.1 | its own RTTI registration (153 classes); application framework (command line, display-device dialog) at `0x7B6300`–`0x7BFCB0`; DirectX 7 renderer (`NiD3DRender`) at `0x7BFCB0`–`~0x7D84C0`; `NiObject` from `0x7D84C0`, `NiNode` at `0x7E3xxx`, `NiTriShape` at `0x7F1`–`0x80Cxxx` |
| `~0x850000`–`0x887FFF` | MSVC6 CRT + Dinkumware STL | entry point `0x85AC73`, STL exception strings at `0x857`–`0x858xxx` |

Ghidra (auto-analysis and the project's analysis passes): 28,255 functions, 331 imports. The count includes
thunks and fragments, so it overstates what the compiler emitted; bytes are the firmer measure.

## Progress

Counted by `python tools/decomp.py gate` (see `AGENTS.md`).

| module | matched under the rules | bytes matched | MATCH-PENDING |
|---|---|---|---|
| Python 1.5.2 | 1490 | 1490 | 0 |
| SWIG `App` wrappers (`game/AppWrap`) | 5575 | 5575 | 0 |
| Totally Games framework (`src/game`, other units) | 10701 | 10724 | 23 |
| MSVC6 C runtime (LIBCMT and LIBCPMT C members; not in this tree) | 0 | 0 | 0 |
| MSVC6 C++ runtime (LIBCPMT and LIBCMT C++ members; not in this tree) | 0 | 0 | 0 |
| NetImmerse 3.1 | 3588 | 3588 | 0 |
| GameSpy SDK (`src/gamespy`) | 134 | 134 | 0 |
| **total** | **21488** | **21511** | **23** |

`.text` built from source: 3,909,419 of 4,745,714 bytes (82.378%).
Compiler-generated static initialisers (`_$E`), counted apart: 59,992 of 59,996 matched, 4 MATCH-PENDING.
MATCH-PENDING functions (exact bytes, held back by a [match] rule) are listed in `config/exceptions.md`.
ORIGINAL-ASM (listed apart, not counted): the 53 MASM objects of the CRT.

## Toolchain status

- [x] MSVC 6.0 SP5, portable install in `tools/vc6/` (not committed). Run `call tools\vc6env.bat` to set it up.
      - Base: Visual C++ 6.0 Enterprise disc 1 (X04-68511, image MD5 `711f996dc7ebc83455e997048a8c71e3`).
        Only `VC98\` and the `COMMON\MSDEV98\BIN` DLLs were extracted; the setup program was never run.
      - SP5: `vs6sp5.exe` (MD5 `971e5b1791ee90e6f397ec7fae520ba9`, valid Microsoft signature). Its
        `vc98\bin` was copied over the base, plus `msvcep.dll` installed as `C2.DLL` (12.00.8966, the Enterprise
        optimizer, as `sp598ent.inf` installs it). The RTM binaries are kept in `tools/vc6/_rtm_backup/`.
      - The C++ standard headers that the disc stores under 8.3 names were also copied to their real names in
        `VC98\INCLUDE`, as Setup would have done: `STREAMBF`→`streambuf`, `ALGRITHM`→`algorithm`, `FCTIONAL`→
        `functional`, `STDXCEPT`→`stdexcept`, `STRSTREM`→`strstream`, `XCEPTION`→`exception`.
      - Verified: `cl` 12.00.8804. Objects are stamped `@comp.id` prod 10 (C) and prod 11 (C++), build 8966,
        the same as the game's Rich header.
      - Not covered yet: 4 objects in the Rich header are prod 49, build 9044. `sp598ent.inf` lists a `vcpp.dll`
        13.0.9044 build of `c2.dll`, which is likely the Processor Pack. Only needed if those 4 units are targeted.
- [x] MSVC 6.0 SP4 compiler in `tools/vc6/_sp4/BIN` (not committed), for the Rich header's build-8799 objects.
      - Source: the Visual Studio 6.0 Service Pack 4 disc (X05-86594), read from a raw CD image
        (2448-byte sectors) converted to ISO.
      - From its `enu` tree, as `sp498ent.inf` installs them: `vc98\bin\cl.exe` (12.00.8804), `c1.dll` and
        `c1xx.dll` (12.0.8867), and `os\system\msvcep.dll` as `C2.DLL` (12.0.8799, the Enterprise optimizer).
        Verified: C++ objects are stamped `@comp.id` prod 11, build 8799.
- [x] Ghidra with the GhidraMCP plugin. The loaded program is the install's `stbc.exe`, which is identical to
      `orig/stbc.exe` (SHA-256 `f2e2a7da…3a383fbc`).
- [x] Compare, byte gate and rule checker: `tools/decomp.py` (see `AGENTS.md`)
