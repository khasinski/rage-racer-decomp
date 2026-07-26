# Rage Racer (SCES_006.50 / PAL main) — Names & Structures Reference

Consolidated reverse-engineering reference for the PAL `main` binary. Sources:
`scratch/permuter-seeds/*.wip.c` header comments, the descriptive comments atop
matched `src/main/PAL/main/*.c`, and `include/game/*.h` + `include/psyq/*.h`.

Status legend:
- **[INCLUDE_ASM]** — not yet decompiled; still an assembly stub in `src/`. A
  reverse-engineered WIP seed usually exists in `scratch/permuter-seeds/`.
- **[proteza]** — byte-matched but kept as assembly / asm-in-C on purpose
  (`HANDWRITTEN_ASM`, excluded from progress; see `docs/ASM_AND_GTE_POLICY.md`).
  These are the SDK / GTE-engine functions that are not compiler-generated C.

Addresses are the function symbol (`func_AAAAAAAA` = `0xAAAAAAAA`). Word counts
are `.word` instruction counts from `asm/nonmatchings/PAL/main/*.s` (bytes = words×4).

---

## 1. Remaining functions (what each IS)

### Car physics / collision
| Func | Addr | Words | Status | Purpose |
|---|---|---:|---|---|
| func_8002D398 | 0x8002D398 | 729 | [INCLUDE_ASM] | Car-vs-car collision detection over `GameCarRuntime[11]` (`D_801F1854`, stride 0x19C). Builds rotated, bilinearly-subdivided collision quads (func_80069D18/func_80069678), point-in-quad tests (func_8002D2E8), applies response (func_80038CE8) + audio (func_8005D6EC). |
| func_80039980 | 0x80039980 | 498 | [INCLUDE_ASM] | Car-vs-car collision resolution over the car array (`D_801F19F0`); projects unit quads through a rotation matrix, subdivides self into 4 sub-quads, tests the other car's 4 corners + 5 midpoints, pushes the pair apart and damps `field_A8`. |
| func_8003B0D4 | 0x8003B0D4 | 671 | [INCLUDE_ASM] | Per-frame car-array driver: ~10 sequential passes (reset scratch, AI target-speed physics, sin/cos shake + matrix build, motion/launch state machine) over `GameCarRuntime[11]`. |
| func_8003BB50 | 0x8003BB50 | 622 | [INCLUDE_ASM] | Per-car physics/shift driver: 9 loops over `GameCarRuntime[11]` (matrix build+transpose+rotate, shift-state machine with 72/216/97/100 magic divides). Sibling of matched func_8002DEFC. |
| func_8003F9C4 | 0x8003F9C4 | 859 | [INCLUDE_ASM] | Dual-channel record/marker state machine over the `D_801E42DC`/`D_801E42E8` tables (stride 20): index/marker/divisor updates then a dx/dy/dz angle-quality clamp. |
| func_80030030 | 0x80030030 | 505 | [INCLUDE_ASM] | Car engine-sound update: derives effect-voice index/phase/volume from car speed & heading (func_8002A788), drives `GameSetIndexedEffectVoice` (func_8005C104). |

### Sound & music
| Func | Addr | Words | Status | Purpose |
|---|---|---:|---|---|
| func_8005C914 | 0x8005C914 | 295 | [INCLUDE_ASM] | Sound-cue state machine over `EffectVoice D_801E6D30[4]` + cue table `D_80012730[3][6]`; clamps mode 0..2 / volume 0..0x7F and starts/stops paired voices. Signature `(s32 mode, s32 pitchArg, s32 volume)`. |
| func_8005C31C | 0x8005C31C | 233 | [INCLUDE_ASM] | Sound-channel/state reset over the `MusicChannel D_801E6D00` block (left=right=-1, mode=1, vols=0) and related sound scalars. (WIP "hard" batch.) |
| func_8003E590 | 0x8003E590 | 345 | [INCLUDE_ASM] | Car proximity/engine audio-cue driver; calls `func_8005C914(1, tone, proximity)`. (WIP "hard" batch, DIFFS≈33.) |
| func_8007010C | 0x8007010C | 360 | [INCLUDE_ASM] | libsnd sequence tick/step over `SeqStruct` (`D_801E79CC`, stride 0xAC). |

### Menu / HUD / overlay renderers
| Func | Addr | Words | Status | Purpose |
|---|---|---:|---|---|
| func_800418D4 | 0x800418D4 | 1211 | [INCLUDE_ASM] | HUD/billboard sprite-and-quad primitive builder: transforms points, packs 0x28-byte prims at scratchpad `0x1F800000`, links into the OT via func_80064DDC/EB8/F30. Largest remaining TU. (cc=2.7.2) |
| func_8004A248 | 0x8004A248 | 1435 | [INCLUDE_ASM] | Team-logo canvas renderer, called every frame by the "TEAM LOGO" screen (`GameUpdateTeamLogoScreen`, func_80057748) and its sample picker (func_800580C8) as `func_8004A248(dir, mode)`. Largest non-SDK function. (WIP "hard" batch, cc=2.7.2) |
| func_8004D384 | 0x8004D384 | 1017 | [INCLUDE_ASM] | Ranking/records overlay renderer: draws 5 rows from the `S22` record tables (`D_801E7744` ranking / `D_8019CB78` time), number+name+bg sprites, two jump-table switches on `record.vC`. |
| func_8004C0D8 | 0x8004C0D8 | 894 | [INCLUDE_ASM] | 4bpp texture / RGB-palette editor debug tool (dec@0x1000 / inc@0x4000 palette switches, draw/erase plot loops). (cc=2.7.2) |
| func_8005290C | 0x8005290C | 849 | [INCLUDE_ASM] | `GameDrawCourseSelectScreen` — slot 1 of the menu overlay table `D_80082EF0`, i.e. the fade/transition overlay of the **COURSE SELECT** screen: scroll accumulator `D_8009B2C0`, wave/color offsets, sprite/number draws (func_80046A2C/func_80047BD4). (cc=2.7.2) |
| func_8005568C | 0x8005568C | 783 | [INCLUDE_ASM] | `GameUpdateCarSelectScreen` — slot 4 of the menu state-machine table `D_80082EB8`, i.e. the **CAR SELECT** screen (rows: race start / customize / car shop / engineer shop / course select). Jump-table switch on `GameMenuBusy` picks the exit: race, or screens 5 / 11 / 12 / 1. |
| func_800496F0 | 0x800496F0 | 675 | [INCLUDE_ASM] | Debug palette/gradient UI renderer: approaches color counters `D_8009B270[0..2]`, mode counter, scroll `D_8007FB08`; draws header + scrollbar + 4-entry palette. (cc=2.7.2) |
| func_8004E724 | 0x8004E724 | 585 | [INCLUDE_ASM] | HUD/standings renderer driven by frame counter `D_8007FB28`; `arg0`=counter delta, `arg1`=highlighted row. (cc=2.7.2) |
| func_8003479C | 0x8003479C | 396 | [INCLUDE_ASM] | Title-screen "RAGE RACER GE" sprite/frame drawer (`GameDrawTitleScreen`, see screens.h). |
| func_8005131C | 0x8005131C | 396 | [INCLUDE_ASM] | HUD matrix helper: builds two matrices from car index (`D_8009B374`), `D_8019C7C8[]` byte table and pointer `D_8009E6F8`. |
| func_80052158 | 0x80052158 | 376 | [INCLUDE_ASM] | Menu/HUD overlay renderer (WIP near-match; ~2 register-pair swaps outstanding). |
| func_8003425C | 0x8003425C | 336 | [INCLUDE_ASM] | Debug color-grid/overlay renderer (`ColCode` table `D_8007DF1C`, `D_8007DDC0[5][16]`). |
| func_800458CC | 0x800458CC | 258 | [INCLUDE_ASM] | Walks an `Entry` record ring (stride 0x30, `D_801E42F4`/`D_801E40E8`) by modular index (`D_801AC774`), writing packed words into the `D_801E3FBx` framebuffer-head block. |

### Track & rendering
| Func | Addr | Words | Status | Purpose |
|---|---|---:|---|---|
| func_80032280 | 0x80032280 | 596 | [INCLUDE_ASM] | Track/route marker or sprite builder; sibling of matched func_80031298. Uses scratchpad struct `0x1F80011C`, atan2 (func_8001A6AC), rsin/rcos (func_80068634/func_80068568). |
| func_8002C478 | 0x8002C478 | 548 | [INCLUDE_ASM] | Track-geometry sample builder: fills a large per-track work aggregate from `GameTrackPoint` samples (`D_8009E688`) using SVec/Vec4 rotation (func_80069678). |
| func_8001DFC0 | 0x8001DFC0 | 445 | [INCLUDE_ASM] | Render-object transform: applies the camera-row horizon (`CamRow` at `D_8019C9A8`) and builds view matrices for a `GameRenderObject`. |

### CD / streaming
| Func | Addr | Words | Status | Purpose |
|---|---|---:|---|---|
| func_8006D1D0 | 0x8006D1D0 | 604 | [INCLUDE_ASM] | CD-streaming "data ready" state machine (func_8006CDA0→): advances `D_80099418` through states 1..0xA, DMAs sector headers/data, drives the `StStrHeader` ring (`D_8009DF1C` / `D_801E8AAC` / `D_801E6C74`/`D_801E6C84`). (cc=2.7.2) |

### SDK library
| Func | Addr | Words | Status | Purpose |
|---|---|---:|---|---|
| func_800632F0 | 0x800632F0 | 535 | [INCLUDE_ASM] | libc `vsprintf`/formatted-string core (memchr func_80063B4C, memmove func_80063B9C, strlen func_80063C08). Ordinary compiler C, not yet matched. |
| **GTE geometry/command engine** | 0x80027FF4–0x8002A2CC | ~2.6k | [proteza] | Hand-written scratchpad-`0x1F800000` GTE dispatch engine: custom calling convention (state in t0/t6-t9/a2), multiple mid-routine entry points, heavy COP2. 25 funcs incl. func_80027FF4 (75), func_80028120 (469), func_80028874 (248), func_800298B0 (360), func_80029FD8 (144), func_8002A2CC (249). |
| **libgte matrix routines** | 0x80069110–0x800696C8 | — | [proteza] | Hand-written libgte matrix multiply/transpose/apply (narrow unsigned-multiply fixed-point idioms): func_80069110 (73), func_80069458 (68), func_80069568 (68), func_80069728 (76), func_80069CC8 (3×3 s16 matrix transpose). Siblings of matched func_80069D18 (RotMatrix). |
| func_800689A8 | 0x800689A8 | 33 | [proteza] | GTE-LZC fixed-point square-root helper. |
| func_80064588 | 0x80064588 | 217 | [proteza] | Hand-written MDEC/DCT bitstream-decoder inner loop (COP0 status read/modify/write). |
| func_80063220 | 0x80063220 | 4 | [proteza] | PSY-Q kernel `ExitCriticalSection` syscall trampoline. |

> The full `HANDWRITTEN_ASM` set is 48 functions (see PROGRESS.md / ASM_AND_GTE_POLICY.md);
> the GTE engine and libgte matrix families above are their bulk. All are byte-matched
> and excluded from the C-progress totals.

---

## 2. Struct inventory

### `include/game/*.h`
| Struct | Header | Size | Purpose |
|---|---|---:|---|
| GameCarEntry | car.h | 8 | Per-car static config: model variant, shape/texture index, enabled flag. |
| GameCarRuntime | car.h | 0x19C | Per-car runtime physics/AI state. 11-entry array at `D_801F1854`. |
| GameCarRuntimeProgressWindow | car.h | 0x19C | Alternate 0x19C-stride window (field_6C / activeFlag) onto the car array. |
| GameCarDrive | car.h | 0xA8 | Drivetrain / input block at `car->field_BC`: pedals, gear, shift and launch state. |
| GameCarAiBlock | car.h | 0xE0 | A second, halfword-wide view of the same block used by the traffic-avoidance / grid-seed code (see 3b). |
| GameCarSpec | car.h | >=0x15C | The loaded car's spec block behind `g_CarSpec`: gear tables, rev limits, tachometer geometry. |
| GameCarSpecShiftPoint | car.h | 4 | One automatic-gearbox shift point, `spec->shiftPoints[gear - 1]`. |
| GameCarTrackAngleWindow | car.h | 0x19C | Alternate 0x19C-stride window (trackPointIndex / headingAngle). |
| GameRenderAxisMatrix | render.h | 0x12 | `s16 m[3][3]` orientation matrix. |
| GameScratchpadRenderState | render.h | ~0x7C | Scratchpad (0x1F800000) render state: prim ptr, depth, Matrix, ordering/mode, clip rect. |
| GameRenderView | render.h | 8 | Camera view angles (0/2/4) + horizon (6). |
| GameRenderObject | render.h | 0xE8 | World render object: position, angle set, flags (0x48 bit 0x1000), color/attribute fields. |
| GameRenderSourcePoint | render.h | ~0xB0 | Sparse source-vertex record (u16 windows at 0/4/8/20/24/28…). |
| GameRenderSinglePoint | render.h | 0x1C | Single transformed point (u16 x8 + s32/u32 + u16). |
| GameRenderPairPoint | render.h | 0x30 | Point pair (first_* / second_* halfwords + two u32). |
| MusicChannel | sound.h | 0x18 | Music/mode channel (left/right/mode/volLeft/volRight). Array at `D_801E6D00[2]`. |
| EffectVoice | sound.h | 0x14 | Effect voice (note/tone/state/pitch/volume). Array at `D_801E6D30[4]` (HW voices 10..13). |
| GameScoreRecord | menu.h | 4 | Menu score record: value + count. |
| GameSaveHeaderRow | memcard.h | 0x80 | Memory-card save-file header row: name, save counter, checksum. |
| GameSaveBlock | memcard.h | 0x1000 | The whole memory-card payload (documentation only, see 3b). |
| GameImageBlock | asset.h | var | Self-describing VRAM upload record: size + rect + pixels. |
| GameSpriteDesc | render.h | 0x14 | Pre-baked SPRT description at `D_8007DAE0`. |
| GameShuttleScenery | track.h | 0x34 | Shuttle scenery prop state, `D_801E4FB8[2]`. |
| GameTrackPoint | track.h | 0x18 | Track centerline point: x/z/y, angle, segmentLength. Array at `D_8009E688`. |
| TrackWaypointSeed | waypoint.h | 0xC | Waypoint seed: origin + step. |
| TrackWaypointRuntime | waypoint.h | 0x38 | Waypoint runtime: position/velocity/scale/magnitude. |
| GameRaceProgress | race.h | 0x14 | Race state/lap/progression/elapsedTime. |
| GameRaceRanking | race.h | var | Ranking: count + values[]. |
| GameAssetTripleHeader | asset.h | 0xC | Three sub-asset offsets. |
| GameCdLoadEntry | asset.h | 8 | CD load descriptor: position + size. |
| GameCarModelAsset | asset.h | 0x28 | Car model asset: model + image data offsets. |
| GameSceneAssetHeader | asset.h | 0x2C | Scene asset header: 11 sub-offsets. |

### `include/psyq/*.h`
| Struct | Header | Size | Purpose |
|---|---|---:|---|
| Matrix | gte.h | 0x1A | GTE matrix: `s16 m[3][3]` + `s32 t[3]`. |
| CdlLOC | cd.h | 4 | Disc location (min/sec/sector/track, BCD). |
| CdlATV | cd.h | 4 | CD audio attenuation (4 volume bytes). |
| CdRegisterMap | cd.h | ~0x1BC | CD/SPU hardware register window (volumes, audio/status control). |
| StHEADER_RR | cd.h | 0x20 | Stream sector header (id, nSectors). |
| StRingEntry | cd.h | 0x20 | Stream ring entry (volatile state, nSectors, value, CdlLOC loc). |
| StRingEventRecord | cd.h | 0x20 | Stream ring event record (volatile u16 state). |
| StRingClearRecord | cd.h | 0x20 | Stream ring clear record (s32 value). |
| CdSearchDirEntry | cd.h | ~0x2C | Directory search entry (type + name[0x24]). |
| Rect | gpu.h | 8 | x/y/w/h. |
| P_TAG | gpu.h | 8 | Primitive tag + packed rgb/code word shared by every packet below. |
| POLY_F3 / POLY_F4 | gpu.h | 0x14 / 0x18 | Flat triangle / quad. |
| POLY_FT4 | gpu.h | 0x28 | Textured quad. |
| SPRT | gpu.h | 0x14 | Textured sprite. |
| TILE | gpu.h | 0x10 | Solid rectangle. |
| LINE_F2 / LINE_F3 / LINE_G2 | gpu.h | 0x10 / 0x18 / 0x14 | Flat line / 3-point polyline / gradient line. |
| GpuRectPacked | gpu.h | 8 | Packed xy + w/h. |
| GpuTexWindow | gpu.h | 8 | Texture window x/y/w/h. |
| DispEnv | gpu.h | ~0x14 | Display environment (disp/screen rects, interlace/rgb24). |
| DrawEnv | gpu.h | ~0x1C | Draw environment (clip, offset, texture window, tpage, dither/draw-to-front). |
| DrawPacket | gpu.h | 0xC | GPU draw primitive (code + two packed coords). |
| GpuCallbacks | gpu.h | ~0x40 | GPU driver function-pointer table (send/submit/putDispEnv/clearImage/resetGraph…). |
| KernelCallback | kernel.h | 4 | `void(*)(void)` callback typedef. |
| SeqStruct / SequenceState | snd.h | 0xAC | libsnd per-sequence channel state (read/loop pos, panpot/programs/vol[16], tempo, flags). Table at `D_801E79CC`. |
| VabHdr | snd.h | 0x20 | VAB header (form/ver/id/fsize, program/tone/vag counts, master vol/pan). |
| ProgAtr | snd.h | 0x10 | VAB program attributes. |
| VagAtr | snd.h | 0x20 | VAB tone/VAG attributes (adsr, pan, pitch-bend range…). |
| SpuMallocEntry | spu.h | 8 | SPU heap block (addr + size). |
| SpuVolume | spu.h | 4 | Left/right s16 volume pair. |
| SpuCommonAttr | spu.h | ~0x2C | SPU common attributes (mvol / cd / ext volume+reverb+mix). |
| SpuRxx | spu.h | ~0x1AC | SPU reverb-volume register window. |
| SpuReverbRegAttr | spu.h | ~0x44 | Full SPU reverb register set (delays/volumes/comb/APF taps). |
| SpuReverbAttr | spu.h | ~0x18 | SPU reverb attribute request (mask/mode/depth/delay/feedback). |
| SpuRevAttrState | spu.h | 0x14 | Cached reverb attribute state. |

### Seed-only structs (were not in headers before this pass)
| Struct | Size | Layout | Where used |
|---|---:|---|---|
| S22 | 0x10 | `s8 name[8]` (a.k.a. `pad[8]`); `s32 v8`; `s16 vC`; `s16 vE` | Ranking / time high-score record row. Tables `D_801E7744[][4][5]` (ranking) and `D_8019CB78[][4][5]` (time). Referenced by matched func_80021DB8 and seed func_8004D384. |
| CamRow | 0x14 | `u8 pad0[0xC]`; `s16 axis0`@0xC; `u16 axis1`@0xE; `u16 axis2`@0x10; `s16 horizon`@0x12 | Camera/horizon row. Base `D_8019C9A8`, indexed `+8*screen`. Referenced by seed func_8001DFC0 (`horizon` adjusts render-object y). |
| SoundScale | 0x0C | `s32 scale`; `s16 values[3]` | Volume-scale table at `D_801E6CA4`. Referenced by matched func_8005D414 and seed func_8005D050 (which aliases it via `asm("D_801E6CA4")` for a `.values` CSE). |
| StStrHeader | 0x20 | `u16 state`; `u16 mode`; `u16 frame`; `u16 nSectors`; `u16 nFrames`; `u8 pad0A[0x12]`; `CdlLOC loc`@0x1C | CD stream ring header. Ring pointer `D_8009DF1C`, ring base `D_801E8AAC`. Referenced by seed func_8006D1D0 (`.state` read `lhu`). |

Landing status into headers is recorded in the change note at the end of this file.

---

## 3. Global memory map (key regions)

| Address | Symbol | What it is |
|---|---|---|
| 0x801F1854 | `g_Cars` (`D_801F1854`) | `GameCarRuntime[11]`, stride 0x19C. `car[0]`=0x801F1854, `car[1]`=`D_801F19F0`, … (see car.h). Individual *fields* also have split symbols (`D_801F18BC` = `g_Cars[0].field_68`, `D_801F18C4` = `.trackProgress`, `D_801F198C/8E` = `.field_138/13A`), which is why some walkers still use them. |
| 0x801E40BC | `g_RankedCars` (`D_801E40BC`) | `GameCarRuntime *[4]`, cars sorted by race progress, leader first (car.h). |
| 0x801E4FB8 | `g_ShuttleScenery` (`D_801E4FB8`) | `GameShuttleScenery[2]`; instance 1's fields also split as `D_801E4FEC..D_801E5014` (track.h). |
| 0x801E4FAC | `g_RaceProgress` (`D_801E4FAC`) | `GameRaceProgress *` into one of three save slots (`&D_801E4094`, `&D_801E6E7C`, `&D_8019C980`); see race.h. |
| 0x801E6D00 | `D_801E6D00` | Sound work area: `MusicChannel[2]`@6D00, `EffectVoice[4]`@6D30, scalar control block (reverb depth / vol scale / flags)@6D80. Contiguous through 0x801E6DA8 (see sound.h). |
| 0x801E6CA4 | `D_801E6CA4` | `SoundScale` volume-scale table (`.scale` + `.values[3]`). |
| 0x801E79CC | `D_801E79CC` | libsnd `SeqStruct[]` sequence-state table, stride 0xAC (see snd.h / sequence.h). |
| 0x801E7744 | `D_801E7744` | `S22[][4][5]` ranking record table. |
| 0x8019CB78 | `D_8019CB78` | `S22[][4][5]` time record table. |
| 0x8009B200 | (block) | Menu / UI state block (~0x550 B), accessed field-by-field: cursor `D_8009B2F4`, busy `D_8009B308`, cursor-anim `D_8009B380`, load phase `D_8009B740`, memcard sub-state `D_8009B71C…`, scroll counters `D_8009B270[4]` / `D_8009B2C0`, car index `D_8009B374` (see menu.h). |
| 0x8009DF1C | `D_8009DF1C` | Active `StStrHeader *` for CD streaming; points into the ring based at `D_801E8AAC` (`= (D_801E6C74<<5)+D_801E8AAC`). Ring indices `D_801E6C74`/`D_801E6C84`. |
| 0x8009DF20 | (block) | Sound voice work buffer: voice*0x10 block at base + voice*0x34 block at 0x8009E0B8; per-voice status bytes `g_SndVoiceFlags` @ 0x8009E0A0 (see audio.h). |
| 0x8019C9A8 | `D_8019C9A8` | `CamRow` camera/horizon rows, indexed `+8*screen`. |
| 0x1F800000 | (scratchpad) | Render primitive scratch: `*(void**)0x1F800004` = OT/prim base; prims packed at 0x1F800000 (cursor `s2 += 0x28`). GTE engine state at 0x1F800068 / 0x1F80011C etc. |
| 0x8007C704 | `g_MainState` (`D_8007C704`) | Top-level scene/state machine selector (state.h). |
| 0x8019CB14 | `g_GameMode` (`D_8019CB14`) | Current game mode; indexes `g_GameModeHandlers` (`D_8007D67C`). |
| 0x8007BED8 | `g_AssetLoadState` (`D_8007BED8`) | Asset-load state-machine phase (asset.h). |

### Named scalars and tables (aliased in headers)

All of these keep their original emitted symbol via
`extern <type> g_Name asm("D_XXXXXXXX");`, so naming them changed no bytes.

| Address | Name | Header | What it is |
|---|---|---|---|
| 0x8009E6A4 | `g_GrandPrixClass` | race.h | 0-based Grand Prix class; `CLASS%d` = +1. Also the track tier: asset index = `0x57 + (course << 1) + (class << 3)`. OVAL is gated to class >= 2 (i.e. CLASS 3). |
| 0x801E428C | `g_CourseIndex` | race.h | Low 2 bits = course (0 BIG, 1 MID, 2 HI, 3 OVAL). Bits 2+ transiently carry `g_GrandPrixSeries` between func_80050C18 and func_80053730. |
| 0x8019CABC | `g_GrandPrixSeries` | race.h | 0 = first series (CALME..DIABLE, 6 classes), non-zero = advanced series (AISANCE..RAGE, 5 classes). Outer index of the record tables. |
| 0x8007D3D8 | `g_GrandPrixNames` | race.h | `[0..5]` first-series class names, `[6..10]` advanced-series names, `[11..13]` course names (MYTHICAL COAST / OVER PASS CITY / LAKESIDE GATE). |
| 0x8009EC90 | `g_GrandPrixRound` | race.h | Round within the class; `"R O U N D %d"` / `"ROUND%d IN"`. |
| 0x8009E834 | `g_RacePosition` | race.h | Player position, 1 = leading; at the finish it indexes `g_PrizeMoney`. |
| 0x8007BEEC | `g_PrizeMoney` | race.h | `[course][class][place]` prize money; `"1ST/%7dv"` .. `"3RD/%7dv"`. |
| 0x8007C48C | `g_AssetPaths` | asset.h | `char *[135]` CD paths; see the layout comment in asset.h. |
| 0x801F17A8 | `g_AssetBlockPtr` | asset.h | Asset sub-block currently being installed. |
| 0x8009E688 | `g_TrackPoints` | track.h | Track centreline points of the loaded course. |
| 0x8009E6A8 | `g_TrackPointCount` | track.h | Number of valid `g_TrackPoints` entries; every walker wraps modulo it. |
| 0x8019C7C8 | `g_CarTable` | car.h | Active `GameCarEntry` table; repointed by func_8001B5DC. |
| 0x801E42E4 | `g_SceneId` | state.h | Identity of the running scene; queried, never dispatched. |
| 0x801E40B8 | `g_SceneTimer` | state.h | Per-scene frame counter, reset with every `g_SceneId` write. |
| 0x8009E694 | `g_AnimTimer` | state.h | Free-running animation phase counter (sine/blink/modulo effects). |
| 0x8019C900 | `g_DrawBuffer` | render.h | Base of the frame's draw work area; the OT lives at +0xCC. |
| 0x801E42E0 | `g_FadeLevel` | render.h | Full-screen fade level 0..0x100. |
| 0x801E42A0 | `g_FadeStep` | render.h | Per-frame delta added to `g_FadeLevel`. |
| 0x8019C9F0 | `g_UiScriptProgress` | render.h | `GameRunTimedDrawScript` progress counter (layer 1). |
| 0x8009B2F8 | `g_UiScriptProgress2` | render.h | Second, independent script progress counter (layer 2). |
| 0x8009B340 | `g_MenuHandlerIndex` | menu.h | Index into the overlay handler table `g_MenuScreenDraw`, -1 = none. |
| 0x8009B318 | `g_MenuOverlayPattern` | menu.h | Element mask passed to `GameDrawBitPatternOverlay`. |
| 0x801E6CA4 | `g_EffectVolumeScale` | audio.h | Master SE volume scale 0..0x80 (`.scale` of the `SoundScale` record). |

Second naming pass (race / car / menu / CD state, all still byte-neutral):

| Address | Name | Header | What it is |
|---|---|---|---|
| 0x801E4DAC | `g_GrandPrixMode` | race.h | 1 = Grand Prix (championship), 0 = Time Attack. Set from `g_TitleMenuSelection` by func_8001B884; picks the pre-race panel (ROUND + prize money vs best-lap/best-total), the innermost index of the record tables, and the in-race option count (`2 - mode`). |
| 0x801E408C | `g_RaceSeries` | race.h | In-race copy of `g_GrandPrixSeries`, latched by func_80038844 when the grid is built. Outer index of the per-series tables **and** the reverse-lap-direction flag, because the advanced series runs the courses backwards (mirrored progress `g_TrackLength - pos`, look-ahead instead of look-behind). Corrects the older "lap-direction flag" comments. |
| 0x801E6E74 | `g_RacePhase` | race.h | Race phase: 0 pre-start (physics frozen), 1 countdown, 2 racing, 4/5 finished, 7 goal/retire, 8 aborted. |
| 0x801E4034 | `g_SeriesSelection` | race.h | Series/save-file the title menu picked (0 first, 1 advanced); `GameInitMenuMode` copies it into `g_GrandPrixSeries`. |
| 0x8019CAC0 | `g_AdvancedSeriesUnlocked` | race.h | Set by func_800206B8 after the last class of the first series; gates title-menu entry 1 and widens func_80053688's course limit from 2-3 to 6-7. Saved at save+0x4E. |
| 0x801E772C | `g_MaxClassReached[2]` | race.h | Highest class reached per series/save file (`[1]` is the old `D_801E7730`). Unlocks courses and bounds the attract-demo class roll. Saved at save+0x50. |
| 0x8019CACC | `g_MirrorMode` | race.h | Mirror mode, armed by the held `0x80C` pad combination as the race scene starts. Swaps the two steering masks and negates body roll (func_8002CD4C), swaps the stereo pan arguments (func_80040ADC / func_8004087C) and selects the mirrored sound cue (func_8002DEFC). |
| 0x801E40D8 | `g_TrackLength` | track.h | Total lap distance: func_8002A6B0 sums every `segmentLength` into it. |
| 0x801E4150 | `g_TrackEventData` | track.h | Base of the course's event/marker block, installed by func_80034E04 (which logs `"event ok"`). `*(s32 *)` is the walk start index; func_80038FF0 reads 8-entry 0x40-byte marker rows at `+ g_RaceSeries * 576 + 0x474`. |
| 0x801E40D4 | `g_PlayerCarIndex` | car.h | Index into `g_CarTable` of the car the player drives; selects the model/texture pack to install. |
| 0x801E4B88 | `g_CarListCursor` | car.h | Cursor of the car list being browsed in the shop; buying sets `enabled` then copies it into `g_PlayerCarIndex`. |
| 0x801E42D8 | `g_CarSpec` | car.h | The loaded car's spec block (`func_80034DF4` is just `g_CarSpec = arg0;`), now typed as `GameCarSpec *`: rev limit / top gear / redline / steer response, the per-gear ratio and shift-point tables, and the tachometer geometry. |
| 0x8019C9F8 | `g_MenuScreen` | menu.h | Menu-mode screen id; see section 3a for the id -> screen map. |
| 0x80082EB8 | `g_MenuScreenUpdate[]` | menu.h | The per-screen state-machine table `func_8005ACA0` dispatches with `g_MenuScreen`. |
| 0x80082EF0 | `g_MenuScreenDraw[]` | menu.h | The parallel fade/overlay table, dispatched with `g_MenuHandlerIndex` / `g_MenuHandlerIndex2`. |
| 0x8009B344 | `g_MenuHandlerIndex2` | menu.h | Second slot into `g_MenuScreenDraw`, run with `-10`; result kept in `D_8009B348`. |
| 0x801E4184 | `g_TitleMenuSelection` | menu.h | Title-menu cursor 0..4 (two Grand Prix files, Time Attack, attract demo, options); entry 1 is skipped until `g_AdvancedSeriesUnlocked`. |
| 0x801E436A | `g_PadHeld` | menu.h | Buttons held this frame — `+0x02` of the pad block at 0x801E4368, computed as `~(raw[0] << 8 \| raw[1])`, with `+0x04` the previous frame and `g_PadEdge2` = `held & ~previous`. |
| 0x8009B34C / 0x8009B350 | `g_MenuViewAngle` / `…Target` | menu.h | Eased current/target rotation angle of the 3D menu view, in 1/1000 units; the carousel wraps at 500000 per entry. |
| 0x8009B358 / 0x8009B35C | `g_MenuViewOffset` / `…Target` | menu.h | The second eased current/target pair, a translation component of the same view. |
| 0x8007F45C / 0x8007F460 | `g_TeamNameLength` / `g_TeamNameChars[]` | menu.h | Renamed from `GameMenuStackDepth` / `GameMenuStack`: the team-name entry buffer written by screen 9, not a screen stack. |
| 0x8007C700 | `g_CdLoadPhase` | asset.h | `GameLoadAsset`'s own CD state machine, 0..6 (seek / read / wait / `"read:%d"` / `"File read error:%s"`). |
| 0x801E6834 | `g_AssetCdEntries[]` | asset.h | Disc location + size of each of the 135 assets, read from the `\RAGE.BIN;1` index by func_80017E8C. |
| 0x8007C6A8 | `g_StreamCdEntries[]` | asset.h | Same for the 11 `\RAGE.STR;1` streams; func_80019B3C picks `1 + class` / `5 + class`. |
| 0x8009F0B8 | `g_LoadBuffer[]` | asset.h | Boot CD scratch buffer: the RAGE.BIN index first, then asset 0. |
| 0x801E6CA8 | `g_VabIds[]` | audio.h | libsnd VAB ids of the loaded banks; every key-on passes `g_VabIds[slot]`. |

A handful of translation units declare one of these with a different type
(`u32` vs `s32`, `volatile`, or a locally-defined struct view). Two remedies keep
the name identical everywhere without moving a byte:

- if the difference is only signedness or a pointer type, the file keeps the
  canonical declaration and gets an explicit cast at the few places where the
  difference is observable — `(u32)g_SceneTimer < 61` still assembles to `sltiu`,
  `(u8 *)g_TrackPoints` and `(s32)g_DrawBuffer` are free;
- if the file needs a genuinely different object view (a `volatile` load, a
  narrower width, or a locally-defined struct), it carries its own
  `extern <its type> g_Name asm("D_XXXXXXXX");` and does not include the header.

Between them these retired the eight translation units that the first pass had to
revert (`g_TrackPoints`, `g_SceneTimer`, `g_AnimTimer`, `g_FadeStep`,
`g_DrawBuffer`, `g_PadEdge2`, `g_AssetBlockPtr`, `g_RacePosition`), so no file is
left on a raw `D_` symbol for a name that exists.

#### Deliberately still unnamed

High-reach globals whose *meaning* is not settled, with what is actually known —
naming these would be a guess, and a wrong name costs more than none:

- **`D_8019CB0C` (21 files) / `D_8009B338`** — a menu layout selector. When set,
  x coordinates shift left (0xA8→0x69, 0xC0→0x92, `xBase -= 0x2C`), a sprite pair
  widens (`halfWidth` 0x58 instead of 1), `func_80051D6C`'s offset bias becomes 64
  instead of 40, one panel (func_8004F650) stops drawing entirely, and the timed
  draw script skips element types 9/19/29/39 while keeping 0/10/20/30. Each screen
  either zeroes it (ids 2, 7, 8) or copies `D_8009B338` into it. **`D_8009B338` is
  written `0` and nothing else** — verified by scanning every `lui`+lo pair in the
  retail image, not just the decompiled C — so the alternate layout is dead code in
  the shipped build and there is no way to observe which layout it is.
- **`D_801E4030` (12 files)** — `func_800455EC` sets it to `sceneMode == 4`, where
  `sceneMode` (`D_801E4026`) also indexes a 48-byte-stride colour table. It selects
  object flag bit 2 instead of bit 1 in `func_8004123C` and writes `0x10000`
  instead of `0` to the scratchpad render word `0x1F800084`. Consistent with a
  night/alternate-lighting variant but not proven.
- **`D_8019C768` (12 files)** — written `0x80` or `0x180` on entry to almost every
  scene; the only reader in the whole image is
  `GameAdvanceSaveHeaderCounter`, which advances the play-time counter by 1 when it
  is `0x80` and by 2 otherwise. Not enough to say what the number *is*.
- **`D_801E6F2C` (13 files)** — a 0x200-word buffer that nine `func_8004Bxxx`
  helpers scroll by 8 words at a time and that `StoreImage`/`func_80065B24` move
  to and from the VRAM rect `D_8007BEE4`; also written to the memory card. Either a
  scrolling text/log buffer or a saved VRAM tile, undecided.
- **`D_8009E6D4` (10 files)** — a small transform struct (`func_8005194C` writes
  `D_8009E6D4 = 23 - pos` and `D_8009E6DC = -20`) that is also passed by address to
  the car/physics/render calls. Declared three incompatible ways.
- **`D_8009E67C` (9 files)** — per-save-file course-progress record, repointed
  like `g_CarTable` (`&D_801E42EC` / `&D_8009E874`); bytes [0..3] are per-course
  flags, +4 and +6 are s16 fields whose meaning is unclear.
- **SDK data** — `D_800941E0`/`E4`/`EA`, `D_800942BC` (libgpu), `D_801E79CC`
  (libsnd `SeqStruct[]`), `D_8009AB7C`, `D_801E416C`, `D_801E42F8`, and everything
  at or above 0x80063200 need matching against Sony sources, not invented names.
  `D_8009E674` is referenced *only* from 0x80074xxx–0x80078xxx, i.e. purely from
  library code, so it belongs to that set too.

---

## 3a. The menu-mode screen table (identified by emulation)

Everything the front end draws while `g_MainState == 3` (i.e. after GRAND PRIX or
TIME ATTACK is chosen) is one of fourteen screens. `func_8005ACA0` dispatches
them through **two parallel tables indexed by the same screen id in
`g_MenuScreen`**:

```
g_MenuScreenUpdate[g_MenuScreen]()          /* per-frame state machine -> GameUpdate...Screen */
g_MenuScreenDraw[g_MenuHandlerIndex](0x14)  /* fade/transition overlay -> GameDraw...Screen   */
```

Each `GameDraw...Screen` owns one accumulator in `0x8009B2C4..0x8009B2EC`,
clamped to `[0, 0x1FC]`; `GameInitMenuMode` (func_80050C18) resets all fourteen
by calling them with 0. Slots 0/3/13 point at the no-op `func_8005AC98`.

Identified by booting the retail PAL disc on an instrumented psx-ruby, sampling
`g_MenuScreen` once per vblank and screenshotting every transition, so each row is
backed by a picture of the screen's own on-screen title.

| id | `GameUpdate…Screen` | `GameDraw…Screen` | accumulator | on-screen title / rows |
|---:|---|---|---|---|
| 0 | func_80052778 | – | – | menu-mode bootstrap; falls straight into id 1 |
| 1 | func_80053730 | func_8005290C | (`D_8009B2F0`, shared) | **COURSE SELECT** (TIME ATTACK header in TA mode) |
| 2 | func_80054D10 | func_80054C84 | `D_8009B2C4` | **RANKING** — total time / lap time / exit |
| 3 | func_80055618 | – | – | one-frame bridge into id 4 |
| 4 | func_8005568C | func_800551BC | `D_8009B2CC` | **CAR SELECT** — race start / customize / car shop / engineer shop / course select |
| 5 | func_800563A0 | func_800562C8 | `D_8009B2D0` | **CUSTOMIZE** — tire / transmission / exit |
| 6 | func_80057198 | func_80056E64 | `D_8009B2D4` | **DESIGN MODE** — logo / name / color / exit |
| 7 | func_80057748 | func_800576BC | `D_8009B2D8` | **TEAM LOGO** — sample / paint / exit |
| 8 | func_800580C8 | func_8005803C | `D_8009B2DC` | **TEAM LOGO** (sample picker) — character / background / exit |
| 9 | func_8005873C | func_800586B0 | `D_8009B2E0` | **TEAM NAME** — 4x11 character grid, 0x2A = BS, 0x2B = ED |
| 10 | func_80058C14 | func_80058B88 | `D_8009B2E4` | **PAINT COLOR** — body color 1 / body color 2 / exit |
| 11 | func_80059558 | func_80059248 | `D_8009B2E8` | **SHOP** (car shop) — buy / exit |
| 12 | func_8005A3A4 | func_8005A2CC | `D_8009B2EC` | **SHOP** (engineer shop) — tune-up / exit |
| 13 | (NULL) | – | – | unused |

Ids 6..12 are the GRAND PRIX-only design/shop subtree; TIME ATTACK only reaches
0..5. **SAVE&LOAD and OPTION are not in this table** — they are separate
`g_MainState` scenes (2 and 7, entered from func_800182D0 / func_80018B98).

The game's internal pad bit layout (`g_PadHeld` = `D_801E436A`, `g_PadEdge2` =
`D_801E436E`) is *not* the SIO0 order; measured by holding
each button in turn:

| bit | button | | bit | button |
|---|---|---|---|---|
| 0x0001 | L2 | | 0x0100 | Select |
| 0x0002 | R2 | | 0x0800 | Start |
| 0x0004 | L1 | | 0x1000 | Up |
| 0x0008 | R1 | | 0x2000 | Right |
| 0x0010 | Triangle | | 0x4000 | Down |
| 0x0020 | Circle | | 0x8000 | Left |
| 0x0040 | Cross | | | |
| 0x0080 | Square | | | |

Hence the recurring menu masks: `0x860` = Start|Cross|Circle (confirm), `0x90` =
Square|Triangle (cancel), `0x1000`/`0x4000` = up/down, `0x8000`/`0x2000` =
left/right.

Four functions that `GameInitMenuMode` also resets are **not** per-screen and
must not be named as screens — they are shared menu drawing helpers with no slot
in `D_80082EF0`: `func_800496F0` (called unconditionally by func_8005ACA0 every
frame), `func_8004CF30` (brightness overlay used by ids 1/3/4/5), `func_800509C4`
(counter in `D_8007FB4C`) and `func_80052158` (primitive shared by the id 5/11/12
draw halves).

---

## 3b. Typing pass: raw offsets replaced by real structs

A dedicated pass replaced `*(s32 *)(base + 0xNN)` byte arithmetic with real
struct types across the MAIN tree. Every step was validated by rebuilding: a
wrong offset, width or signedness changes the emitted load, so `make check
VERSION=PAL` printing `main.exe: OK` (sha1
`2913e15648eddef40821c5f666460abc04155ee6`) is a hard test of the layout.

Raw-offset dereferences in `src/main/PAL/{main,lib}`:

| metric | before | after |
|---|---:|---:|
| all `*(T *)(...)` casts | 961 | 647 |
| the `*(T *)(base +/- 0xNN)` form specifically | 554 | 293 |

### Structs completed or newly defined

| Struct | Header | Size | What it is |
|---|---|---:|---|
| `g_Cars` | car.h | `GameCarRuntime[11]` | `D_801F1854` finally has one canonical declaration and a name. It had been declared four incompatible ways (`Arr412`, `GameCarRuntime`, `s32`, `u8`), which is exactly why the earlier naming pass could not name it. `Arr412` (0x19C, `f120`) was folded into `GameCarRuntime.field_78`. |
| `g_RankedCars` | car.h | `GameCarRuntime *[4]` | `D_801E40BC`: the four contenders sorted by race progress (`field_68 + field_6C`), rewritten every frame by func_8003A728 and read by func_8003A974 to rubber-band the AI. Slots 1..3 also carry their own split symbols `D_801E40C0/C4/C8`. |
| `GameCarSpec` | car.h | >= 0x15C | The car spec block behind `g_CarSpec` (`D_801E42D8`), previously a bare `u8 *` read by raw offset in 10 files: `gearLoad[6]`@0xCC, `gearRatio[7]`@0xE4 (both indexed by 1-based gear), `revLimit`@0x100, `unk102`, `topGear`@0x104, `redline`@0x106, `steerResponse`@0x10A, `unk10C/10E/110/112`, `shiftPoints[6]`@0x120 (`{downshiftSpeed, upshiftSpeed}` per gear), `tachoNeedleX/Y`@0x138/13A, `tachoFaceDX/DY`@0x13C/13E, `tachoDigitsX/Y`@0x140/142, `needleQuad[4]`@0x14C, `needleAngleMin/Max`@0x150/152, `needleColor[4]`@0x154, `needleColorAlt[4]`@0x158. |
| `GameCarDrive` | car.h | 0xA8 | The drivetrain/input block at `car->field_BC`, promoted out of func_8002DEFC's local `CarDrive` and extended (`unk42/unk44`, `unk6C..unk72`, `unk84/unk88`, `unk9C/unk9E`) so func_80030814 and func_8002F690 could adopt it too. |
| `GameCarAiBlock` | car.h | 0xE0 | A **second, irreconcilable** view of that same block: func_8003A280 and func_800385FC load 0x104..0x134 as halfwords where `GameCarDrive` declares words. Both views reproduce their own translation unit, so both are kept and cross-referenced. |
| `GameShuttleScenery` | track.h | 0x34 | `D_801E4FB8[2]`, previously three mutually incompatible local typedefs (`Unk3F0F8State`, `Unk3F2A4`, plain `u8[]`): `dwellCounter`, `travelStep`, `startEndpoint`, `pathIndex`, `x/y/z`@0x10, `angleX/Y/Z`@0x20. Confirms the layout the header comment had only described in prose. |
| `g_RaceProgress` | race.h | `GameRaceProgress *` | `D_801E4FAC` named, and the three incompatible declarations (`GameRaceProgress *`, `s32 *`, `u8 *`, plus a local `RaceProg`) unified. The struct's field names were corrected from the code that reads them: `state`→`course`, `pad4`→`carIndex`, `lap`→`classIndex`, `progression`→`maxClassReached`, `elapsedTime`→`unk10` (the last is mode-overloaded: elapsed time in Grand Prix, `g_GrandPrixSeries` in Time Attack). |
| `GameSaveBlock` | memcard.h | 0x1000 | The complete memory-card payload, reconstructed from func_8005F88C and GameLoadSaveStateBlock and verified to add up to exactly 0x1000 with the checksum at +0xFFC. Documentation only — see the revert list below. |
| `GameImageBlock` | asset.h | var | The self-describing VRAM upload record func_8001A3C0 chains and func_8001A2E0 uploads: `size`, `x/y/w/h`, `pixels[]`. |
| `GameSpriteDesc` | render.h | 0x14 | The ready-made SPRT description at `D_8007DAE0` that func_80032BD0 fills from the car spec's tacho fields and func_80032FF0 expands into a real SPRT. |
| `P_TAG`, `POLY_F3`, `POLY_F4`, `POLY_FT4`, `SPRT`, `TILE`, `LINE_F2`, `LINE_F3`, `LINE_G2` | psyq/gpu.h | 0x10..0x28 | The libgpu primitive packets in their PSY-Q layout. `func_800468FC.c` (the game's whole 2D drawing layer: `GameDrawSprite`, `GameDrawFlatTriangle`, `GameDrawFlatQuad`, `GameDrawTexturedQuad`, `GameDrawSolidRect`, `GameDrawLine`, `GameDrawPolyLine3`, `GameDrawGradientLine`) went from 47 raw offsets to zero, and every `prim += 0x14` became `prim++`. |

Structs that already existed and were merely applied for the first time:
`GameSceneAssetHeader` (func_80018FC4's 21 sub-block lookups, via an
`ASSET_SUB(base, k)` macro), `GameCarModelAsset` (func_80018A70 /
func_80018530), `GameTrackPoint` (func_8002BF68), `MusicChannel` /
`EffectVoice` (GameResetSoundState, GameInitEffectVoiceRuntime,
GameForceBasicEffectVoicesEnabled, func_8005C6C0), `DrawPacket`
(SetTexWindow.c's four packet builders), `GameCarRuntime` (func_80037808's
func_800383A8 / func_800385FC, 70 offsets).

### The gcc 2.6.3 rule that decides whether a struct can be applied

gcc 2.6.3's `true_dependence` treats a `MEM` tagged `MEM_IN_STRUCT_P` and a
plain scalar `MEM` as **guaranteed not to alias**. A struct-member access and a
plain global therefore stop constraining each other's order the moment the raw
cast becomes `p->field`, and the compiler starts hoisting global loads out of
loops or ahead of stores — costing extra callee-saved registers and breaking the
match. This is the single reason every revert below happened, and the reason
some conversions had to keep one or two accesses raw.

The corollary is a cheap trick that *does* work: addressing a field through
another symbol at a fixed offset (`&D_801E6D00[0].mode + offset` instead of
`D_801E6D08 + offset`) changes only the relocation's displacement, and the
linker resolves it to the identical instruction word as long as the two symbols
share a `%hi`. The unlinked `.o` diff shows `sw v,8(at)` where retail has
`sw v,0(at)`, and `make check` still passes.

### Reverted, and why

- **`GameSaveBlock` applied to func_8005F88C and GameLoadSaveStateBlock.** Both
  functions are nothing but a flat copy between the block and ~30 plain globals.
  As struct members, gcc hoists every global load to the top of the function
  (+3 saved registers in the writer, +2 in the reader). The struct is kept in
  `memcard.h` as the verified layout and both functions' comments point at it,
  but the bodies stay on raw offsets. Secondary reason on the reader side: its
  body needs a `u8 *` parameter and the header's prototype is `void *`, which
  gcc 2.6.3 reports as `conflicting types` (and still leaves a usable `.o`).
- **`GameCarSpec` applied to func_80032BD0.** Same aliasing rule: the function
  interleaves spec loads with stores to the `D_8019C7Dx` globals, and member
  reads let gcc reorder the two groups. Reverted to `u8 *data = (u8 *)g_CarSpec`
  with the offsets named in a comment.
- **`GameCarAiBlock` in func_8003A280's inner loop.** Three accesses
  (`field_104`, `field_10C`) had to stay raw casts behind file-local
  `AVOID_BLOCKED` / `AVOID_NEARBY` macros, because as struct members they let
  gcc hoist the `D_8009E778` load out of the loop. The other ten accesses in the
  function convert fine.
- **`TrackWaypointRuntime` in func_80037860.** Retail keeps a second induction
  variable biased to `&waypoint->velocityMagnitude` and addresses the velocity
  block at negative displacements off it. Written as `waypoint->field` the
  second iv disappears and every store re-bases; 24 accesses stay raw with an
  offset->field map in a comment.
- **`GameCarSpec`'s colour fields in func_8003351C, and `GameCarRuntime` in
  func_8003A728 / func_8003A280 / func_80038FF0.** Same class of problem from
  the other direction: retail addresses these off a *biased* base register
  (`spec + 0x138`, `&g_Cars[0].field_A4`) or through a field's own split symbol
  (`D_801F18BC`, `D_801F198C`), so the displacement the struct member would
  produce is not the one retail emits. In each case the base expression was
  rewritten to name the struct field it points at (`(u8 *)&g_Cars[0].field_A4`,
  `(u8 *)&p->tachoNeedleX`) and the offsets from there are commented, which is
  as far as this can go without changing bytes.
- **`func_8003591C` left entirely raw.** `route + i*4 + 0xAC / 0xC0 / 0xC4` are
  per-lap arrays inside the drive block whose element counts and relationship
  are not established (0xC0 and 0xC4 overlap when indexed the way the code
  indexes them), and every access is deliberately written through an explicit
  offset temporary to force the match. Typing it would mean inventing a layout.
- **libsnd (`SsSetTableSize`, `SsUtPitchBend`, `SpuVmSetSeqVol`) and
  func_8003E464 / func_8003EBCC / func_8003F700 left raw.** These walk `SeqStruct`
  and hardware voice work areas through doubly-indirected pinned registers
  (`*(s32 *)(offset + (s32)*slot + 0x90)`); the data's meaning is not settled
  enough to name fields honestly.

### Still on raw offsets, by size

`func_8003591C` (46), `GameLoadSaveStateBlock` (42), `func_80037808` (39, the
waypoint tail plus the handful of width casts), `func_8005F88C` (33),
`func_80048078` (27), `func_80038FF0` (24), `func_8003F700` (20),
`func_80074D1C` (18), `func_8003351C` (18), `func_80046600` (16),
`SsUtPitchBend` / `SsSetTableSize` (16 each), `func_8003F0F8` (12, static
position tables), `func_8003E464` (12), `SpuVmSetSeqVol` (12).

Down from a "before" list where seven files were over 30 and `func_80037808`
alone held 101.

---

### Detail moved out of the headers

The headers were carrying long investigation write-ups; those were compressed to
1-3 lines per declaration plus a per-field one-liner inside each struct. What is
not already recorded elsewhere in this file:

**Boot / frame spine (was in `state.h`).** The crt0 entry stub at `0x800630B4`
calls `GameMainLoop` (`func_80016510`), which runs the one-shot init chain
(`GameInitSubsystems`, `func_80018038`, …) and then never returns: an endless
per-frame loop that ticks CD audio, the sequencer and the pending asset loads,
dispatches the current screen through `g_GameModeHandlers` (indexed by
`D_801E42E4`), waits on VSync, swaps the display env and finally calls
`GameUpdatePadState`. The frontend (`func_8001BB58`) and the 3D scene
(`func_80026AE0`) are two entries of that table, which is why they are mutually
exclusive per frame.

**Pad button mapping (was in `state.h`).** `GameLoadPadButtonMapping` copies one
8-entry row of button bitmasks out of `D_8007C0A8` and one out of `D_8007C128`
into `D_801E4B60` / `D_801E4B70`; `GameApplyPadButtonMapping` re-applies it from
the two saved selections. The controller-config and NeGcon calibration screens
are `g_GameModeHandlers` entries 7..11, identified by the strings they draw:
`"INSERT CONTROLLER"`, `"CONTROLLER ERROR"`, `"Hold the \"NeGcon\" in an
untwisted position and press start button."`, `"Steer play."`, `"Maximum twist."`.

**Frontend sub-state machine (was in `screens.h`).** `GameUpdateFrontend`
(`func_8001BB58`) is `g_GameModeHandlers` slot 4 and runs one of four handlers
each frame through the 4-entry jump table at `D_8007C748`, indexed by the
frontend sub-state `D_8009F098`:

| `D_8009F098` | handler | hand-off |
|---:|---|---|
| 0 | `GameUpdateTitleScreen` | waits for Start (`g_PadEdge2 & 0x800`) |
| 1 | `GameUpdateMainMenuOpen` | 48-frame row wipe-in (`D_801E6F1C` to 0x30) |
| 2 | `GameUpdateMainMenuInput` | cursor `% 5` skipping the locked entry 1; move plays cue 1, confirm cue 2, reseeds the opponent order (`func_8001B488`) |
| 3 | `GameUpdateMainMenuExit` | fades out over 0x81 frames (`func_80033AA0(2*t, 0x59)`), then requests scene 6 / 0x1F (race), 0x19 (SAVE&LOAD) or 0x16 (OPTION) |

All four end by calling `GameDrawMainMenuRows` or `GameDrawPressStartPrompt`, so
the visible frontend is always drawn by whichever state is active. The prompt is
a 112x16 cell (tpage code `0x7E84`, uv `0x70,0xA0`) at `(0x68, 0xC8)` whose
brightness comes from `func_80068568(g_AnimTimer * 96)`; the rows are the same
page at `x = 0x68`, `y = 0x64 + 0x18*row`, uv `(0, 0xA0 + 0x10*entry)`, with each
row's wipe height `D_801E6F1C - 8*row` clamped to `0..0x10`.

**`g_AssetPaths[0..9]` (was in `asset.h`).** `LOGO.TMS`, `TITLE.TMS`, `RG3.VH`,
`RG3.VB`, `RES.DAT`, `CAR.TMS`, `SAVE.TMS`, `SELBGM.BIN`, `SELECT.BIN`,
`OPTION.BIN`. `[10..73]` are `CAR_xx.1ST` / `CAR_xx.2ND` (32 car models in two
halves), `[74..85]` `GP0..GP11.TMS`, `[86]` `VOICE.BIN`, `[87..134]` the track
packs. `func_80017BD4` logs each load as `"Now Loading [%s]->[0x%08x] ..."`.

**Sound work-area provenance (was in `sound.h`).** The 0x801E6D00 layout was
reconstructed from the retail disassembly (base loaded as `lui rX,0x801e` plus a
decimal offset from 27904) and cross-checked against every C file that casts
these symbols. Retired symbol equivalences: `D_801E6D08 ==
D_801E6D00[0].mode`, `D_801E6D18 == D_801E6D00[1].left`, `D_801E6D38/3C/40 ==
D_801E6D30[0].state/pitch/volume`, `D_801E6D44/58/6C == D_801E6D30[1..3].note`.

**`g_SceneTimer` signedness (was in `state.h`).** Four translation units need an
unsigned load and carry their own `extern u32 g_SceneTimer asm("D_801E40B8");`,
so the name is identical everywhere while the load stays `sltiu`.

---

## Change note (this consolidation pass)

The four seed-only structs above were all landed into headers; `make check
VERSION=PAL` stayed byte-identical (`main.exe: OK`) after each:

- **CamRow** → `include/game/render.h` (new typedef; no compiled file defined it).
- **StStrHeader** → `include/psyq/cd.h` (new typedef; uses the existing `CdlLOC`).
- **S22** → `include/game/menu.h`; local `typedef` removed from the matched
  `src/main/PAL/main/func_80021DB8.c`, which now `#include "game/menu.h"`.
- **SoundScale** → `include/game/sound.h`; local `typedef` removed from the
  matched `src/main/PAL/main/func_8005D414.c`, which now `#include "game/sound.h"`.

---

## 4. Translation units

Matched functions are being grouped into multi-function source files, so a
config `c` segment spans an address range and its `.c` defines every function
in that range (in ascending address order). The emitted symbols are unchanged,
so the ROM stays byte-identical; `asm/` and `linkers/PAL/main.ld` are generated
by `make split` and never hand-edited.

### What the evidence can and cannot prove

Merging only changes the output when there is something to over-dedup (shared
strings, rodata, statics). That gives an asymmetry worth stating plainly:

- a merge that **fails** `make check` is real evidence of **separateness** —
  the two sides were different translation units in the original build;
- a merge that **succeeds** proves **nothing** about togetherness. Two
  genuinely separate original files that shared no constants merge without
  changing a byte.

Measured over the first 42 functions: only **4** byte-proven boundaries. So the
ROM constrains grouping very weakly, and unbounded merging would produce
implausibly large files. Groupings below are therefore **consistent with
byte-exactness**, not recovered originals. Cohesion signals used, in order of
weight: direct call edges; a **shared external callee** (a family of thin
wrappers each calling the same helper with different constants — no edges among
themselves, but clearly one family); shared `D_` global blocks; same structs.
Units are capped (~12 functions / ~800 lines) so grouping cannot run away.

### Car physics / AI (`func_80034DCC`–`func_800396FC`)

| Unit leader | Funcs | Lines | Closed by |
|---|---:|---:|---|
| func_80034DCC | 6 | 92 | seam (thin wrappers sharing helper `func_8001674C`) |
| func_80034F74 | 5 | 394 | byte-proven boundary |
| func_800357BC | 1 | 70 | byte-proven boundary (singleton) |
| func_8003591C | 3 | 990 | cap |
| func_80037200 | 2 | 275 | byte-proven boundary |
| func_80037808 | 10 | 876 | cap |
| func_80038844 | 6 | 284 | seam |
| func_80038CE8 | 2 | 252 | byte-proven boundary |
| func_80038FF0 | 7 | 599 | end of range |

`func_8001674C` is declared **un-prototyped** in the `func_8003591C` unit: the
frame-matching hack there calls it with seven arguments against one declared
parameter, which only compiles without a prototype. That is evidence the
original translation unit also declared it K&R-style.

### Toolchain note: cc1 crashes on merged units

cc1 segfaults *while formatting a diagnostic* when a call passes a pointer whose
type disagrees with a callee definition the merge has just made visible (you see
a truncated `warning:` then the crash). Fix by casting the argument at the call
site to the definition's parameter type. This is **not** a translation-unit size
limit: 256 functions / 43k lines in one file compiles fine.

## 5. The title screen / main menu, and the animated course scenery

Two clusters named in one pass. The frontend was settled from the jump tables
plus emulator screenshots; the scenery cluster from its data tables, its
per-frame runtime trace and the course-name table.

### 5a. Frontend state machine (`func_8001Bxxx`)

`GameUpdateFrontend` (`func_8001BB58`, frame-mode table slot 4) runs one of four
handlers per frame through the **4-entry jump table at `D_8007C748`**, indexed by
the frontend sub-state `D_8009F098`. Dumping that table settles the whole
cluster: its entries are exactly `0x8001B260, 0x8001B440, 0x8001B5DC,
0x8001B884`.

| vaddr | name | role |
|---|---|---|
| `0x8001B014` | `GameEnterTitleScreen` | frame-mode slot 3 — the scene the 3D scene requests when the attract race or a race ends. `func_8001AF70` is its slot-2 twin, entered from the menu/story screens |
| `0x8001B260` | `GameUpdateTitleScreen` | state 0: wait for Start (`g_PadEdge2 & 0x800`), then state = 1 |
| `0x8001B440` | `GameUpdateMainMenuOpen` | state 1: the row wipe-in. Advances `D_801E6F1C` and hands over the frame it hits `0x30`, so it runs for exactly 48 frames |
| `0x8001B5DC` | `GameUpdateMainMenuInput` | state 2: Up `0x1000` / Down `0x4000` on the cursor `D_801E4184` (`% 5`), confirm `0x860` |
| `0x8001B884` | `GameUpdateMainMenuExit` | state 3: fade over `0x81` frames, then request the scene for the chosen row |
| `0x8001B170` | `GameDrawPressStartPrompt` | the pulsing 112x16 "PRESS START" sprite at `(0x68, 0xC8)`, brightness driven by `func_80068568(g_AnimTimer * 96)` |
| `0x8001B2D4` | `GameDrawMainMenuRows` | all five rows: 112x16 cells at `x = 0x68`, `y = 0x64 + 0x18*row`, `uv (0, 0xA0 + 0x10*entry)` |

`GameDrawPressStartPrompt` and `GameDrawMainMenuRows` share one texture page —
the prompt is the `u = 0x70` column (code `0x7E84`), the rows are the `u = 0`
column (`0x7E85`, or `0x7E86` under the cursor). Entry 1 is skipped while
`D_8019CAC0 == 0`, which is why the screen shows four rows (GRAND PRIX, TIME
ATTACK, SAVE&LOAD, OPTION) out of five. The exit handler's targets confirm
`D_801E4DAC`: it is set to 1 for the two Grand Prix rows and 0 for TIME ATTACK.

`GameCanSelectNextCourse` (`func_80053688`) is the course-select gate:
`g_CourseIndex < (class < 2 ? 2 : 3)`, or `6 : 7` for the advanced series whose
number is packed into bits 2+ of `g_CourseIndex`. It is the OVAL unlock, i.e.
the predicate form of the `func_80018C88` re-roll.

### 5b. Animated course scenery (`func_8003Dxxx` / `func_8003Fxxx`)

Not an ending sequence — the recurring `g_GrandPrixClass == 5` test is just the
last class suppressing prop animation. `GameDrawCourseScenery2` runs **once per
frame for the whole attract scene** (164/164 frames in the trace, from
`func_80026920`), and `GameDrawCourseScenery` is the race-path copy.

All four courses share one coordinate space — they are different routes through
the same landscape, with THE EXTREME OVAL shifted `+0x5000` in z — so the prop
positions are a single static table at `0x8007E2C0..0x8007E3E8` and each prop
culls itself against `D_801E6828`, the 32x32 bitmask of visible 2048-unit
terrain cells that `func_800414F0` rebuilds every frame from the camera. Each
leaf then builds a rotation matrix, calls `GameSetGteObjectMatrix` and submits a
model by index into the course object bank, clamping against the bank size
`D_801E40E4` with a fallback to model 1.

| vaddr | name | evidence |
|---|---|---|
| `0x8003E1A4` | `GameDrawCourseScenery` | dispatcher on the course index; race path (scene `0xC`) |
| `0x8003E2E8` | `GameDrawCourseScenery2` | same switch; replay (`0x11`) / attract (`0x1E`) path, separate animation state |
| `0x8003D6F0` | `GameDrawAnimatedScenery` | 16-phase model-swap animation + a 3-variant companion part on a 4-step palette cycle; Grand Prix only |
| `0x8003DA90` | `GameDrawAnimatedScenery2` | the replay/attract copy of it, argument-driven |
| `0x8003DDAC` | `GameDrawSpinningScenery` | spins about Z at a rate re-randomised every 512 frames; 1 on MYTHICAL COAST, 3 in a row on OVER PASS CITY from class 2 up |
| `0x8003DF68` | `GameDrawStaticScenery` | the one landmark drawn on all four courses |
| `0x8003E0D0` | `GameDrawHighClassScenery` | MYTHICAL COAST only, class 4+; the only prop with no cull |
| `0x8003F2A4` | `GameUpdateShuttleScenery` | lerps a prop between two endpoints, dwells, then reverses |
| `0x8003F4BC` | `GameDrawShuttleScenery` | draws it (model `0x3F`, or `0x3C` from course 2 up) |
| `0x8003F0F8` | `GameInitShuttleScenery` | seeds both instances at course load |

Shuttle paths, from `D_8007E360` / `D_8007E3D8` / `D_8007E3E0`: OVER PASS CITY
gets one instance that descends 2938 units over 628 frames and waits 300;
LAKESIDE GATE gets two on paths 1 and 2, the same near-level 3400-unit run
traversed in opposite directions so the pair passes mid-way (512-frame
traversal, 128-frame wait).

The course index maps to the names in `g_GrandPrixNames[11..13]` (pointer table
at `0x8007D404`) and the track packs: 0 = MYTHICAL COAST (`BIG`),
1 = OVER PASS CITY (`MID`), 2 = LAKESIDE GATE (`HI`), 3 = THE EXTREME OVAL
(`OVAL`).

**Deliberately generic.** The models live in the course object bank on the disc,
which this repo does not have, so no prop's *subject* is proven and the names
describe motion only. The spinners are very likely windmills or wind turbines
(vertical-plane rotation at a wind-like drifting rate, three in a row, and the
game's own class names are French wind strengths — CALME, BRISE, RAFALE,
MISTRAL, TEMPETE), and the LAKESIDE GATE shuttles are very likely boats or a
cable car, but neither is asserted in a name.
