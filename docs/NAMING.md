# Naming

## Functions

**Game functions carry no prefix.** `UpdateCamera`, `DrawRankingPanel`,
`IsCarNearWaypoint`.

They used to be `GameUpdateCamera` and so on. The stated reason was that game
code and the PSY-Q SDK are linked into one binary and had to be tellable apart
-- but that argument does not survive contact with the symbol table: **the SDK
prefixes itself**. `SsUtKeyOnV`, `SpuSetReverb`, `CdControl`, `GpuDrawSync`,
`GteRotTrans` all announce which library they came from. Anything without one
of those is game code, and a second marker on 617 functions added nothing.

Measured before removing it: of the 618 `Game*` functions, stripping the prefix
collided with exactly one non-game symbol.

| kept | why |
| --- | --- |
| `GameInitPad` | libapi already exports `InitPad` (`func_800631F0`), and this is the game's wrapper around it. A real clash, so the prefix earns its keep here. |

That is the whole exception list. If a future name collides with an SDK symbol,
extend the table rather than reintroducing the prefix wholesale.

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

Globals keep `g_` (`g_CourseIndex`), because at a use site there is nothing else
to distinguish a global from a local.

## Files

**The directory is the subsystem; the file name says what the unit is about,
in lower snake_case.**

    src/main/PAL/main/track/update_route_scenery.c
    src/main/PAL/main/race/draw_ranking_panel.c
    src/main/PAL/main/save/memcard.c

Everything under `src/main/PAL/main/` is game code and everything under
`src/main/PAL/lib/` is library code, so the path carries what a prefix would.

Library and SDK files keep their Sony names as file names (`SpuInit.c`,
`CD_sync.c`, `_spu_writeByIO.c`) -- there the file name *is* the recovered
symbol.

Two further rules:

- **A file named `func_XXXXXXXX.c` means the function is genuinely
  unidentified.** If the function inside has a name, the file is stale and
  should be renamed (this was true of `func_80078F4C.c`, which had declared
  `_spu_writeByIO` for some time).
- **A merged translation unit is named for its subsystem, not for whichever
  function happens to sit first in it.** `save/memcard.c` holds eight former
  files; naming it `poll_memory_card_status.c` would describe an eighth of it.

## Mechanics

Renaming a function is free: the `asm("func_XXXXXXXX")` alias fixes the symbol,
so the C identifier is decoration and the emitted code cannot change. Verify
anyway.

Renaming a file means renaming its `configs/PAL/main.yaml` segment and any
`INCLUDE_ASM("asm/.../<old name>", ...)` path, then `make split && make check`.
`asm/` and `linkers/` are generated and need no edits.

## Auditing the file names

Two criteria find real mistakes; both need a human pass afterwards, because
each has a large false-positive class.

1. **The name promises a function the file does not define.** This is the
   reliable one. It caught `PopMatrix.c` (holds `ApplyMatrix`), and
   `Gpu_ArmTimeout.c` (holds `Gpu_CheckTimeout`).
2. **The name describes one behaviour of a larger subsystem.** `init_menu_mode.c`
   also flipped the course card and counted owned cars; `draw_rear_view_mirror.c`
   also reset the mirror and bracketed its pass. Both are now named after the
   subsystem.

**A thematic name matching no single function is correct, not a finding.**
`draw_prims.c` holds ten drawing primitives and `cd_audio_control.c` holds the
whole start/pause/resume/fade set. Renaming either after its first function
would describe a tenth of the file and contradict the rule above. Any audit
counting "functions whose name contains the file's words" will rank these low
purely because the code uses synonyms: `title_screen.c` scores 2 of 9 and is
exactly right.

Two traps cost a wrong answer here on 2026-08-04:

- A definition regex that misses one-liners (`void StSetRing(long b, long s)
  { ... }`) or definitions with a return type reports **zero** findings, which
  reads identically to a clean tree. Assert a plausible function count inside
  the script, not just a file count, before believing any zero.
- Confirm a rename by grepping the file for its own name first. Four files
  looked misnamed and each defined its namesake on a single line.

## Raw func_ call sites

Most `func_XXXXXXXX(...)` calls are **not** unnamed functions. On 2026-08-04 the
2310 raw call sites split as:

- ~900 simply had not been switched over, and now use the name the function
  already carried. The safe transform is narrow: rewrite the prototype the file
  already has, keeping its exact parameter types, and rename the uses. Do not
  insert or delete declarations, and skip a file whose call *precedes* its
  prototype: that call relied on an implicit declaration of the raw symbol, and
  renaming it emits a symbol nothing defines.
- **1105 in 153 functions are blocked by a signature disagreement.** The name
  exists, but the header and the call sites do not agree. `DrawSprite` is
  declared `u32 flags` in `game/render.h` while callers declare `s32 flags`;
  `UpdateCarLaunch` takes one parameter in `game/car.h` and is called with two.
  The raw name hid this. Renaming exposes it as a compile error, so these need
  the signature settled first, one function at a time.
- 272 in 80 functions are genuinely unnamed and need real evidence.

Substituting a header's types for a caller's own can also crash cc1 2.6.3
outright (segfault or bus error, not a diagnostic) - `SetDrawMode`,
`SetShadeTex` and `SetGteObjectMatrix` all did. Treat a compiler crash as a
signal that the declaration changed, not as a toolchain glitch.
