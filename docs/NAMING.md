# Naming

## Functions and globals

Game code carries a `Game` prefix (`GameUpdateCamera`, `g_CourseIndex`); library
code carries its own. That prefix is not decoration -- game code and the PSY-Q
SDK are linked into one binary and sit next to each other in the same
directories, so `SsUtKeyOnV`, `CdControl` and `GameDrawRankingPanel` have to be
tellable apart at a glance. It also matches `docs/names.md`, which records for
every name whether it was recovered from evidence or invented.

Library names are evidence and must not be tidied:

| shape | what it is |
| --- | --- |
| `SpuInit`, `CdControl`, `SsUtKeyOnV` | Sony's public API, confirmable in LibRef47 |
| `_spu_init`, `_SpuDataCallback` | libspu internals; see the caveat below |
| `Gpu_*`, `Gte_*`, `Spu_*` | ours, for an internal whose Sony name cannot be pinned |
| `func_8006AB5C` | not yet identified |

Caveat worth settling before anyone unifies the two libspu internal styles:
`docs/names.md` calls the `_spu_*` / `_Spu*` families invented, but both match
the real libspu internal naming closely enough that they may be recovered
symbols. If they are, they are evidence and normalising them would destroy
information.

## Files

**The directory is the subsystem; the file name says what the unit is about,
in lower snake_case, with no `Game` prefix.**

    src/main/PAL/main/track/update_route_scenery.c
    src/main/PAL/main/race/draw_ranking_panel.c
    src/main/PAL/main/save/memcard.c

The prefix is dropped because the path already says it: everything under
`src/main/PAL/main/` is game code, and everything under `src/main/PAL/lib/` is
library code. Repeating `Game` on 151 file names added nothing that the
directory did not already carry. Functions keep it, because they are read far
from their file.

Library and SDK files keep their Sony names as file names (`SpuInit.c`,
`CD_sync.c`, `_spu_writeByIO.c`) -- there the name *is* the recovered symbol.

Two further rules:

- **A file whose name is still `func_XXXXXXXX.c` means the function is genuinely
  unidentified.** If the function inside has a name, the file is stale and
  should be renamed (this was true of `func_80078F4C.c`, which had declared
  `_spu_writeByIO` for some time).
- **A merged translation unit is named for its subsystem, not for whichever
  function happens to sit first in it.** `save/memcard.c` holds eight former
  files; naming it `poll_memory_card_status.c` would describe an eighth of it.

Renaming a file means renaming its `configs/PAL/<basename>.yaml` segment and any
`INCLUDE_ASM("asm/.../<old name>", ...)` path, then `make split && make check`.
`asm/` and `linkers/` are generated and need no edits.
