# Reference sources and trust

Read this before naming a class, member or function, claiming an identity, or adopting code from a lineage source.
`AGENTS.md` is the project guide; this file holds the evidence sources and how far each is trusted.

- **Python 1.5.2** (`src/python`, from the upstream tarball): surviving original source, presumed authentic. A
  difference from upstream is a Totally Games modification and needs evidence (a `// TG: <evidence>` comment).
- **VC6 CRT** source in `tools/vc6/VC98/CRT/SRC`: surviving source for the CRT module. The game overrides
  `malloc`/`free` with its own allocator (`0x718D60`/`0x718DC0`). Retail links the service-pack CRT: where its code
  follows the SP heap (`__active_heap`, `__try` around the small-block heap), the SP4/SP5 source is the original
  (`C:\Recomp\installers\sp5\x\cabs\vc98\crt\src`, identical to `ref/dl/vs6sp4/.../vc98/crt/src` for
  `realloc.c`), not the RTM text in `upstream/vc6-crt` (realloc's unused `origSize` is the SP source's own).
- **SWIG `App` wrappers**: each string `Class_Method` (for example `ShipClass_GetAlertLevel`) names the C++
  class and method that the wrapper calls. This is the main naming evidence for game classes.
- **SWIG 1.3.6's `Lib/python/embed15.i`** (user's ruling, 2026-11-15): the same distribution as the runtime templates
  in `upstream/swig-1.3.6-r1429/`, mirrored there unchanged and adopted the same way. It is the Python inittab code
  in AppWrap: `swig_add_module`, `swig_add_builtin`, the replacement `main` that sets `PyImport_Inittab`
  (0x5BC190-0x5BC220), `swig_inittab` (config.c's table included again, 0x8E6388) and `_SwigImport_Inittab`
  (0x98F098).
- **Game scripts** (`C:\Recomp\Star Trek Bridge Commander\scripts\*.pyc`): show how the engine API is used. The
  BC SDK 1.1 (`C:\Recomp\bcsdk-1.1`, April 2002) ships most of them as `.py` source with comments, and four TG
  tools built from the same code base (AnimOnly, DamageTool, ModelPropertyEditor, STBCExporter).
- **The demo's `stbc.exe`** (`C:\Recomp\bcdemo\stbc.exe`, built 2002-02-09 from `C:\UTOPIA\BRANCHES\DEMO\CODE`, no
  PDB shipped): a second build of the same code base two months before retail, with the same toolchain (identical
  Rich header except 24 more C objects). Its `nt` module still has `getcwd`, `putenv`, `popen`, `spawnv`,
  `execve` and the unconfined file functions, and it lacks the ban list, `\scripts\Custom` and dedicated-server
  strings: the 1.1 patch's custom-mission sandbox and multiplayer changes are what the retail build adds. Evidence
  for `// TG:` notes and for which retail code is new in 1.1; the project's internal name was "Utopia".
  As a second compile it settles a helper only where the two builds differ: every MATCH-PENDING function pairs
  byte-identical (relocations masked), its only demo-only functions are three `_$E` stubs, and call sets differ in
  three open functions (`sibling.py callsets --open`). So it can't show an inline helper both builds expand the same
  way; use it for call-set and `// TG:` questions, not for a spelling both builds share.
- **GameSpy SDK** (the 2001-era C SDK: `goaceng` server list, query and reporting, `darray`, `hashtable`,
  `gutil` crypt; retail 0x6A9930-0x6AD5B0, about 90 functions): lineage source like Python and the CRT (user's
  ruling, 2026-11-03), compiled as its own C module; a difference from the chosen lineage version needs evidence
  and a `// TG:` comment. Copies checked on 2026-11-15:
  - IGN's 2011 open-source GameSpy SDK (`github.com/nitrocaster/GameSpy`, BSD-3; common code 2.06.00, 2007) is the
    closest found. Its `darray`, `hashtable`, `nonport` and `md5` changed little and are candidates for adoption,
    while its server list and reporting code is the later generation (`serverbrowsing`, `qr2`): names and structure for
    retail's `goaceng`/`qr`, fitted to retail and proved by the gate.
  - `github.com/0xdefea7ed/nightfirespy` (GPL-2) reimplements a 2002 master server with the `enctype`/`gsmsalg`
    algorithms: leads only, never copied.
  - Croteam's official Serious Engine repository has no GameSpy code (it replaced it with its own GameAgent).
  - **The 2001 `goaceng` source, as remembered** (no copy on disk) (user's ruling, 2026-11-16): lineage for the names
    and structure of retail's server-list helpers, which IGN's 2011 SDK no longer has. Never pasted, every body fitted
    to retail and proved by the gate; a name not recalled with confidence stays marked invented.
- **Matt Pietrek's `MSJExceptionHandler`** (Microsoft Systems Journal, "Under the Hood", 1997-1998): lineage
  source for `game/MSJExceptionHandler` (retail 0x716A10-0x7177C0), the crash reporter that writes `<exe>.RPT`. Its
  banner and format strings, the `.RPT` extension and the static members agree with retail; no string in the binary
  names the class, so its names come from the article (user's ruling, 2026-11-15). Totally Games' additions
  (`TGSymbolEngine`, loading DBGHELP.DLL at run time) are named from the binary's strings.
- **Alexander Pukall's PC1 cipher**: lineage source for `TGEncrypt`, identified by its multipliers 0x15A and 0x4E35
  and its key schedule; member names follow the PC1 reference code (user's ruling, 2026-11-15).
- **The later Visual Studio CRT source** (VS .NET and on; no copy on disk, read from memory) (user's ruling,
  2026-11-15): lineage evidence for the parts VC6 ships no source for, the C++ EH runtime and the floating-point
  library (cv.h, cvt, intrncvt, strgtold, x10fout and the rest). Names, structure and layout only, as with the Gamebryo
  1.2.2 source: never pasted, every body fitted to retail and proved by the gate. The names in VC6's own LIBCMT objects'
  symbol tables come first.
- **The 1.0 retail CD's `stbc.exe`** (`C:\Recomp\bc-1.0-cd\Setup\stbc.exe`, extracted from the user's CD image;
  SHA-1 `46b75f50eba7538fb9dfadbc549102b14278f8e0`, 6,172,746 bytes, built 2002-01-29 from
  `C:\Utopia\current\code\Utopia___Win32_CDChecking`, no PDB shipped; `sibling.py --build v10`): the release build
  ten weeks before the 1.1 patch, not copy-protected. 90,679 of its functions equal retail's with addresses masked;
  35 annotated functions differ, which is what the 1.1 patch changed. It holds the three functions retail has
  binary-patched as the compiler emitted them, so their original bytes are known, not reconstructed from the dead
  body: FindDiscWithContents returns `0x115` for a NULL argument, CheckDisc's result is `neg eax; sbb al, al;
  and eax, 3`, and CDCheckWindow::RunCheck has `setne cl`. Evidence only, like the demo: never run, copied or
  committed.
- **The BC SDK's debug NetImmerse** (`C:/Recomp/bcsdk-1.1/Tools/AnimOnly/AnimOnly.exe`, VC6 linker, April 2001;
  `tools/sdkdebug.py`): the SDK's animation viewer, linked against a DEBUG build of "NetImmerse Version 3.1" from
  Totally Games' shared tree (`C:\TGSharedExternal\NDL3\Src\{NiMain,NiAnimation,NiCollision,NiSurface,NiDirect3D,...}`).
  Freely distributed under the SDK's EULA: evidence only, read like the demo executable, never run, copied or pasted.
  Its 1231 asserts give the original file of the function they sit in, the line, and member names in the original's
  Hungarian (`m_pBase`; written in the project's style, `base`); its link order gives each original file's functions
  in definition order. Its code is /Od (NiDirect3D and NiSurface with /GZ), which VC6 SP5 reproduces byte for byte
  from the same source, so it settles spellings that compile to the same release bytes: statement and declaration
  order, the frame order of named locals (the lineage's local names reproduce it, ours may not), every inline call,
  and so an inline's cost. A difference from it needs evidence from retail's bytes (BC's engine is the same version,
  built a year later), and the source cites it at the code (`AnimOnly.exe 0x426171 (NiCamera.cpp)`).
  Not covered: TG's own NetImmerse additions (NiTriIntersect, the OBB classes, the Miles sound units) are not in it.
- **The SDK's `/GR` tools** (`DamageTool.exe`, `ModelPropertyEditor_s.exe`; `sdkdebug.py --exe ... rtti`): their RTTI
  names classes and direct bases. ModelPropertyEditor's is MFC and the C++ library apart from `TGStreamException`;
  DamageTool's names TG's damage-tool classes (`MetaVolumePolygonizer`, `BinaryVoxelizer`, `OBBForest`, ...) and
  template instances (`NiTMap<__int64, unsigned short>`): leads for our placeholders where retail's code is the same.
  DamageTool.exe (April 2001) also keeps its COFF symbol table (18,456 symbols: `PointerToSymbolTable` in the PE
  header, values are RVAs): the decorated name of every function, static member and function-local static of the
  damage, OBB, voxelizer and overlay code (`?ProcessTri@MetaSurfacePolygonizer@@...`). It is an earlier build of
  the same code: a retail function whose instructions match a DamageTool function's (masked addresses), or whose
  vtable slot, parameter list and role match one, takes its name; layouts and signatures that differ stay fitted
  to retail. The source cites the DamageTool address (`DamageTool.exe 0x41A5A0`). `sibling.py --build damagetool`
  pairs its functions with retail's (its names against our symbols, RTTI vtables, masked bytes, calls), and
  `sibling.py syms` prints a pair's decorated name against ours with what differs.
  The same table is grouped by original object: each `@comp.id` record starts one object's static symbols (365
  objects, 143 with code), and an object's `.text` records give the addresses of its code sections, so 2,704 of the
  3,833 named functions fall in a known object. That gives, for the code DamageTool shares with retail, which
  functions one original file held (OBBNode, OBBLeaf and OBBRoot are one object), each object's `_$E` numbers as
  the original compiler counted them, and the names and signatures of file-local `static` functions, which no other
  source has (the TGA reader's `GetPixel24( NiFile &, unsigned char *, int )`, the SGI reader's `GetBytePixel`,
  `GetWordPixel`, `ProcessNormalRow` and `ProcessRLERow`, the texture manager's `Find*TextureFormatCallback`,
  `D3DDeviceEnumCallback`, `GetNextMipmapLevel`, `EnumZBufferCallback`). No tool reads the grouping yet. It has no
  line numbers and no `.file` records.
- **The SDK tools as sibling builds** (`sibling.py --build mpe|damagetool|exporter`, `sibling.py open`): release
  compiles of the TG framework and NetImmerse under other projects' declarations. Where retail's and ours' call sets
  disagree, a sibling that calls what retail calls (its masked bytes equal to retail's, or not) shows retail's
  decision made in another build too: evidence that the call is the source's, not an inline budget or a
  declaration count. A sibling's revisions (DamageTool is a year older, the exporter's NetImmerse is another
  version) make a disagreement with retail evidence of nothing by itself.
- **NetImmerse 3.1**: class names come from strings and later public Gamebryo material; trust is decided case by
  case.
- **Gamebryo 1.1.2 SDK** (`C:\Recomp\gamebryo-1.1.2`, April 2004, the closest surviving engine release): headers,
  `.inl` files and static libraries with CodeView debug information, and in `AppFrameworks`, `Samples` and `Tools`
  the source of NiApplication, NiAppWindow, NiFrameRate, NiCursor, NiFont, NiInput, the samples and the tools (the
  evaluation's timelock lives in NiSystem's `timelock`/`NiSystemTimelock` objects); six NiMain variants are built with the VC6
  Processor Pack back end (`@comp.id` 49.9044). It is lineage evidence two years newer than the game's engine:
  names, signatures, member order, vtable order and inline helpers are strong evidence once retail's bytes agree,
  and a source comment cites them (`// Gamebryo 1.1.2: NiNode::AttachChild`). Layouts and vtables are always
  fitted to retail, never taken as they are. The SDK itself (headers, libraries) is never committed and never
  pasted into `src/`; an `.inl` body may be the base of a helper whose behaviour retail confirms, and it then goes
  through the gate like any reconstruction (user's ruling, 2026-11-03).
- **Gamebryo 1.2.2 source** (`C:\Recomp\gamebryo-1.2.2`, `NiVersion.h` 1.2.2.6, 2005): the full engine source
  (`CoreLibs\NiMain`, `NiCollision`, `NiAnimation`, `NiOldParticle`, `NiDX8Renderer`, ...) under the same policy as
  the 1.1.2 SDK: lineage evidence three years newer than the game's engine, read for names, control structure,
  helpers and member order, never pasted into `src/` and never committed. A reconstruction is written in the
  project's style, fitted to retail and proved by the gate, and cites its source (`// Gamebryo 1.2.2:
  NiOBBNode::DivideTriangles`). The headers differ little from 1.1.2's (user's ruling, 2026-11-03).
- **NifTools `nif.xml`** (`C:\Recomp\nifxml`, from `https://github.com/niftools/nifxml`, GPLv3): the NIF file
  format by version, and it names Bridge Commander for versions 3.0 and 3.1 (the game's `.nif` files are 3.0 and
  3.1). It gives each class's inheritance and the order, types and names of its streamed fields: evidence for the
  `LoadBinary`/`SaveBinary` bodies and for member order and names, cited in a comment
  (`// nif.xml: NiAVObject.Translation`). Under the same policy as the Gamebryo SDK: never committed, and no code is
  generated from it.
