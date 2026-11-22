# Notice

This repository is a matching decompilation of **Star Trek: Bridge Commander** (`stbc.exe`, patch 1.1, 2002): source
text reconstructed so that the original compiler emits the retail executable's code byte for byte. It exists for
preservation, study and interoperability. It is not affiliated with or endorsed by Totally Games, Activision,
Paramount, Numerical Design Limited or any later holder of their rights.

## What is not here

- **No part of the game.** No executable, data, art, sound, script or installer is included, and none can be built
  from this repository alone: the comparison needs your own copy of the game (the GOG release is the reference; its
  hash is in `README.md`). Do not add game files to this repository or to a fork of it.
- **No compiler.** Building needs Microsoft Visual C++ 6.0 with Service Pack 5, which you must supply.
- **No Microsoft runtime source.** The game links Microsoft's C and C++ runtime statically. That code is Microsoft's;
  its source ships with Visual C++ 6.0 and is not reproduced here. `config/symbols.txt` records only the names and
  addresses of the runtime's functions, so that calls into them resolve.
- **No NetImmerse or Gamebryo SDK.** The engine code under `src/netimmerse` is reconstructed from the executable.
  Later releases of the engine and the game's own SDK tools served as evidence for names and structure;
  `docs/evidence.md` says which and how each was used. No SDK file is included.

## Rights

The reconstructed source under `src/game`, `src/netimmerse` and `src/gamespy` describes code whose rights belong to
its original authors and their successors. Nothing here grants any right in that code, in the game, or in the
Star Trek name and setting. If you hold rights in the original and have a concern, open an issue or contact the
maintainer.

What the contributors of this project wrote themselves (the tools under `tools/`, the documentation under `docs/`,
`AGENTS.md`, the configuration and ledgers under `config/` and `cards/`, and whatever original expression the
reconstruction contains) is dedicated to the public domain under CC0 1.0: see `LICENSE.md`.

## Third-party text included under its own license

| path | what | license |
|---|---|---|
| `upstream/python-1.5.2/`, `src/python/` | Python 1.5.2, the interpreter the game embeds; `src/python` is that source with the game's changes reconstructed | `upstream/python-1.5.2/COPYRIGHT` |
| `upstream/swig-1.3.6-r1429/` | SWIG 1.3.6 runtime templates, the generator of the game's `App` wrapper layer | `upstream/swig-1.3.6-r1429/LICENSE` |
| `upstream/gamespy-ign-2011/`, `src/gamespy/` | two files of the GameSpy SDK as IGN published it in 2011 (BSD 3-clause); `src/gamespy` is the game's 2001 version of that SDK, written from it and fitted to the executable | `upstream/gamespy-ign-2011/license.txt` |
| `.claude/skills/` | the matching-decompilation skills this project is run with, by lemiur, from https://github.com/lemiur/Game-Decompilation-Skill-For-Claude; modified here (this project's rulings, tools and worker rules were added to them) | CC BY 4.0, https://creativecommons.org/licenses/by/4.0/ |

## Acknowledgements

Other projects on the same game were read as leads for facts (addresses, vtables, behaviour); no code or text was
taken from them:

- STBC-Reverse-Engineering (SandboxServers): https://github.com/SandboxServers/STBC-Reverse-Engineering
- the `stbc_reference` clean-room specification (CC BY-SA)
- BC Dauntless, a behavioural reimplementation of the game's engine (GPL-3)

Most of the reconstruction and tooling was written with Claude (Anthropic) under the maintainer's direction and
rulings (`docs/rulings.md`); every function counted as matched is verified against the retail bytes by
`tools/decomp.py gate`, whoever or whatever wrote it.
