# Code organization audit

This audit covers every C translation unit listed in `configs/PAL/main.yaml`.
At the time of the audit the manifest contains 345 C entries, and there is a
one-to-one correspondence between those entries and the C files under
`src/main/PAL`: no source is unconfigured and no configured source is missing.
The YAML manifest is therefore the exhaustive per-file inventory and the
authoritative record of link order.

## Evidence and decision rules

The evidence was applied in this order:

1. Executable addresses and the order of C entries in the splat manifest.
2. Symbol, data and call relationships in the PAL image.
3. Function families and module boundaries in PsyQ/PsyZ.
4. Responsibility of the code and its callers.
5. Existing file names, which were treated only as hints.

A source was split only when an address boundary and a clear change of
responsibility both supported the split. Consecutive functions were not
assumed to come from separate original object files merely because they could
be separated semantically. Conversely, a PsyZ file was not treated as proof of
one original object: its decompilation sources can collect several adjacent
functions.

All implemented moves and splits are **confirmed** by an unchanged PAL image.
Responsibility-based names are **high-confidence** descriptions, but do not by
themselves prove an original PsyQ object name. Files mentioned under
"Boundaries retained" require more object/archive evidence before a further
split would be justified.

## Target directory taxonomy

Every C entry in the manifest is classified by its directory:

| Directory | Classification | Responsibility |
| --- | --- | --- |
| `PAL/main/asset` | game | Loading, installing and selecting game assets |
| `PAL/main/audio` | game | Game-level music, voice and sound policy |
| `PAL/main/boot` | game/platform entry | Startup and main scene loop |
| `PAL/main/car` | game | Vehicle simulation, AI and track contact |
| `PAL/main/cd` | game | Game-level CD audio and request policy |
| `PAL/main/fmv` | game | Game-level FMV scenes and stream playback |
| `PAL/main/menu` | game | Front-end scenes and menu rendering |
| `PAL/main/pad` | game/platform integration | Controller setup and game configuration |
| `PAL/main/race` | game | Race flow, results, records and replay |
| `PAL/main/random` | game/runtime glue | Game PRNG entry points |
| `PAL/main/render` | game | Game renderers and packet submission |
| `PAL/main/save` | game | Save model and memory-card UI/workflow |
| `PAL/main/track` | game | Course geometry, scenery, cameras and scripts |
| `PAL/lib/libapi` | PsyQ SDK | BIOS/kernel, event, interrupt and counter APIs |
| `PAL/lib/libcard` | PsyQ SDK | Memory-card driver API |
| `PAL/lib/libcd` | PsyQ SDK | CD-ROM driver and ISO9660 implementation |
| `PAL/lib/libds` | PsyQ SDK | Disc search helpers |
| `PAL/lib/libetc` | PsyQ SDK | Display synchronization helpers |
| `PAL/lib/libgpu` | PsyQ SDK | GPU packets, queues, environments and transfer |
| `PAL/lib/libgte` | PsyQ SDK | GTE matrices, geometry and register helpers |
| `PAL/lib/libpress` | PsyQ SDK | Streaming and MDEC support |
| `PAL/lib/libsnd` | PsyQ SDK | Sequence and voice-level sound library |
| `PAL/lib/libspu` | PsyQ SDK | Low-level SPU driver |
| `PAL/lib/libc` | C runtime | Standard C library routines linked into the image |

There are no longer parallel `main/sdk`, `main/gte` or `lib/kernel` buckets.
Paths inside `INCLUDE_ASM` still use their historical extraction locations;
they name asm inputs, not the current C module classification.

## Confirmed mixed-source splits

The following old aggregate sources crossed clear subsystem or responsibility
boundaries. Their manifest entries remain at the original addresses and in the
same order:

| Old aggregate | Resulting modules and start addresses |
| --- | --- |
| `CdReadDataReadyCallback.c` | game `race/prologue_step` at `0x179EC`; SDK `libcd/read_callbacks` at `0x17A38` |
| `CdRead.c` | SDK `libcd/read` at `0x17E88`; game `render/sprite_string` at `0x18074` |
| `_new_card.c` | `libapi/new_card_stub` at `0x54634`; `libpress/mdec_api` at `0x54644` |
| `DeliverEvent.c` | `libapi/deliver_event_stub` at `0x5ABD8`; `libcd/drive_state` at `0x5ABE8` |
| `ExitCriticalSection.c` | `libapi/exit_critical_section` at `0x53A20`; `libapi/bios_file_stubs` at `0x53A30` |
| `update_menu_mode.c` | menu dispatcher retained; game `audio/audio_runtime_control` starts at `0x4B778` |
| `get_track_surface_height.c` | game `car/track_contact`; game `render/tachometer_needle` starts at `0x233D0` |
| `fmv_playback.c` | game `fmv/stream_playback`; game `race/replay_recording` starts at `0xF8E0` |
| `race_init.c` | `asset/race_data_installation` at `0x255CC`; `audio/sound_initialization` at `0x25688`; `race/time_conversion` at `0x25718` |
| `prize_and_progress.c` | `menu/prize_money_screen` at `0x115DC`; `audio/apply_audio_settings` at `0x11A24`; `save/save_defaults` at `0x11A88`; `race/class_grade` at `0x11CB8` |

## Confirmed relocations and module naming

The GPU packet/environment sources formerly grouped below `main/render` and
all sources formerly below `main/sdk` were assigned to their actual PsyQ
libraries. GTE helpers were moved to `libgte`; CD driver code to `libcd`; MDEC
and streaming code to `libpress`; BIOS and kernel services to `libapi`; sound
code to `libsnd`/`libspu`; standard functions to `libc`. `SetRCnt` is in
`libapi`, matching PsyZ's `libapi/counter.c`, while `VSync` remains in
`libetc`.

Collective SDK sources now use responsibility names such as
`primitive_packets`, `draw_environment_state`, `command_queue_execution`,
`initialization_and_toc`, `sequence_tone_modulation` and
`interrupt_initialization`, instead of an address or an arbitrary member
function. Official single-entry SDK sources may retain the public entry-point
name where it accurately identifies the entire object.

Game sources were similarly moved when their responsibility was in the wrong
subsystem: menu car resources are in `asset/menu_car_assets`, terrain packet
submission is in `render/terrain_submission`, controller setup is in `pad`,
and race countdown/course-event control is in `race/course_event_runtime`.

## Boundaries retained pending stronger evidence

Some broad modules contain several related entry points but no defensible
binary boundary. They remain one TU to avoid inventing object boundaries:

- `libgpu/primitive_packets` and `libgte/register_access` are cohesive SDK
  helper families despite their high function counts.
- `asset/model_banks` is a cohesive table-driven asset-bank module.
- `track/dynamic_track_views` interleaves camera and scenery state belonging to
  the same dynamic track-view mechanism.
- `menu/menu_visual_effects`, `race/race_runtime`, `render/draw_prims` and
  `render/text` are broad game helper modules whose internal functions share
  state and contiguous placement.
- Remaining public-entry-point filenames in `libsnd` and `libspu` describe
  small cohesive objects. Renaming every singleton to a paraphrase would lose
  useful SDK identity without adding TU evidence.

Further division of these files needs an original PsyQ linker map, archive
member list, debug symbols, or independent object-boundary evidence. Function
count or aesthetics alone are not sufficient.

## Verification contract

Every logical batch must preserve manifest order and pass `make check`. Final
acceptance additionally requires a clean rebuild and identical SHA-1 values
for `assets/PAL/main.exe` and `build/PAL/main.exe`.
