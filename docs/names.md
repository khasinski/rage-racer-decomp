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
| 0x801F1854 | `D_801F1854` | `GameCarRuntime[11]`, stride 0x19C. `car[0]`=0x801F1854, `car[1]`=`D_801F19F0`, … (see car.h). |
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
| 0x8009B340 | `g_MenuHandlerIndex` | menu.h | Index into the overlay handler table `D_80082EF0`, -1 = none. |
| 0x8009B318 | `g_MenuOverlayPattern` | menu.h | Element mask passed to `GameDrawBitPatternOverlay`. |
| 0x801E6CA4 | `g_EffectVolumeScale` | audio.h | Master SE volume scale 0..0x80 (`.scale` of the `SoundScale` record). |

A handful of translation units declare one of these with a different type
(`u32` vs `s32`, `volatile`, or a locally-defined struct view). Those files
carry their own `extern <their type> g_Name asm("D_XXXXXXXX");` instead of
including the header, so the name is identical everywhere while the generated
load stays exactly as it was. Files that both include the header and need a
different type keep the raw `D_` symbol.

---

## 3a. The menu-mode screen table (identified by emulation)

Everything the front end draws while `g_MainState == 3` (i.e. after GRAND PRIX or
TIME ATTACK is chosen) is one of fourteen screens. `func_8005ACA0` dispatches
them through **two parallel tables indexed by the same screen id in
`D_8019C9F8`**:

```
D_80082EB8[D_8019C9F8]()              /* per-frame state machine  -> GameUpdate...Screen */
D_80082EF0[g_MenuHandlerIndex](0x14)  /* fade/transition overlay  -> GameDraw...Screen   */
```

Each `GameDraw...Screen` owns one accumulator in `0x8009B2C4..0x8009B2EC`,
clamped to `[0, 0x1FC]`; `GameInitMenuMode` (func_80050C18) resets all fourteen
by calling them with 0. Slots 0/3/13 point at the no-op `func_8005AC98`.

Identified by booting the retail PAL disc on an instrumented psx-ruby, sampling
`D_8019C9F8` once per vblank and screenshotting every transition, so each row is
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

The game's internal pad bit layout (`D_801E4368` held / `D_801E436A` held /
`g_PadEdge2` = `D_801E436E` edge) is *not* the SIO0 order; measured by holding
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
