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

Every non-handwritten `INCLUDE_ASM` stub now carries a real name; the segment
name, the source file name and the `INCLUDE_ASM` path argument were renamed
together and `make check VERSION=PAL` stayed byte-identical. `func_8007010C` is
now `SsSeqApplyDataEntry` (section 17d); the only symbol in the whole tree still
left on a `func_` name as a deliberate decision is `func_8006AB5C` (section 17i).

Several rows below correct earlier descriptions in this file; where a name
supersedes a wrong one the old claim is called out, because it is also repeated
in the headers and in commit messages.

### Controller-configuration screen (`0x80014A60`–`0x80015440`)

`GameDrawControllerConfigScreen` (func_80015444) keeps **two** independent 0..7
configuration selections: `D_8019CE08` for the standard pad, whose 8 rows of
button bitmasks live at `D_8007C028`, and `D_8019CB08` for the NeGcon, whose rows
live at `D_8007C0A8`. `GameLoadPadButtonMapping` installs one of each into
`D_801E4B60` / `D_801E4B70`. The screen draws whichever one matches the pad type
byte `D_801E4369` (`== 0x23` is the NeGcon), which is what identifies the two
diagram functions. Rows 4 and 5 of the two mask tables are what differ: the pad
table shifts on R1|R2 / L1|L2, the NeGcon table on Down / Up.

| Name | Addr | Words | Purpose / how it is known |
|---|---|---:|---|
| `GameDrawLeftArrow` | 0x80014A60 | 68 | 16x32 arrow sprite (tpage 0x7F82, v = 0xB8, **u = 0x48**) plus an optional additive glow whose brightness is `rcos(D_8007C13C)`. The config screen draws it at (0x28, 0xE0) with the glow flag `selection != 0`. |
| `GameDrawRightArrow` | 0x80014B70 | 68 | Byte-identical except **u = 0x58**; drawn at (0x108, 0xE0) with the flag `(selection ^ 7) != 0`. The `!= 0` / `!= 7` pair against the 0..7 selection range is what proves which arrow is which. Both are reused by the NeGcon "steer play" and "maximum twist" screens. |
| `GameDrawPadConfigSelector` | 0x80014C80 | 139 | The framed number panel: a 0x30x0xC caption sprite, three 8x16 glyphs whose middle one is `u = 0x50 + 8 * index`, and a white/black double frame. Called once per screen with the active selection. |
| `GameDrawPadConfigLabels` | 0x80014EAC | 193 | The five action labels of one diagram, positions taken from a 5-byte row. |
| `GameDrawPadConfigCallouts` | 0x800151B0 | 87 | Five lines (its only callee is `GameQueueLine`) from the label anchors `D_8007C168[6]` to the button positions `D_8007C180[16]` — entries 0..7 are the standard pad's buttons, 8..0xF the NeGcon's, which is why the two row tables have disjoint value ranges. |
| `GameDrawPadConfigDiagram` | 0x8001530C | 30 | Standard-pad diagram: `GameDrawPadConfigLabels` then `GameDrawPadConfigCallouts` with `D_8007C1C0 + 5 * D_8019CE08` and `D_8007C1E8 + 5 * D_8019CE08`. |
| `GameDrawNegconConfigDiagram` | 0x80015384 | 30 | Same shape with `D_8007C210` / `D_8007C238` and `D_8019CB08`; this is the branch taken when `D_801E4369 == 0x23`. |
| `GameBeginControllerConfig` | 0x800153FC | 18 | Entry hook: copies both selections into the backup slots `D_8019C7A8` / `D_8019C76C` (which `GameUpdateControllerConfigScreen` restores on cancel) and clears `D_801E8AA4` / `D_801E8A9C` / `D_801E7A4C` / `D_801E6C7C`. Its caller sets `g_GameMode = 7` in the next instruction. |

### Car physics / collision
| Name | Addr | Words | Purpose |
|---|---|---:|---|
| `GameCollidePlayerWithCars` | 0x8002D398 | 729 | Player-vs-field collision, **detection and response together**. Called only from the player physics driver func_8002DEFC (`func_8002DEFC(&D_8009E6D4)`), and returns 0 or the struck sub-quad 1..4. Early-returns unless `g_GrandPrixMode`. Builds rotated bilinearly-subdivided quads (func_80069D18 / func_80069678), point-in-quad tests (func_8002D2E8), then shake `D_801E4BA0`, grip/damage decay, func_80038CE8 impulses on **both** cars, and a `GamePlaySoundCue` crash cue 0xA..0xD chosen by proximity band and by `(hit & 1) != g_MirrorMode`. |
| `GameCollideRivalCars` | 0x80039980 | 498 | One row of the AI pairwise sweep: tests `car[index]` against `car[index + 1 .. 10]` only (base `D_801F19F0` + 0x19C * index; callers pass index 0..9, i.e. the upper triangle). Same quad tests and push-apart, but **no sound, no damage globals and no mode gate** — that is what separates it from `GameCollidePlayerWithCars`. |
| `GameUpdateRaceCars` | 0x8003B0D4 | 671 | Race variant of the rival-car driver: ~10 sequential passes over `GameCarRuntime[11]`. Called from the race scene only when `g_RacePhase >= 2 && g_GrandPrixMode != 0`. Owns the three race-only passes func_8003A728 / func_8003A974 / func_8003A6A4 and time-slices cars 4..10 on `(i & 1) == (g_AnimTimer & 1)`. |
| `GameUpdateAttractCars` | 0x8003BB50 | 622 | Attract/replay variant of the same driver, called by the three non-interactive 3D scenes (func_80025C58, func_80026570 — which writes `g_SceneId = 0x1E` — and func_80026AE0). No player to budget for, so every car runs every frame; additionally wraps `car->field_68` modulo `g_TrackLength`. |
| `GameUpdateCarLaunch` | 0x80030030 | 505 | Car motion-state handler for `state98 == 1`, the one-frame takeoff of a jump. func_8002A810 dispatches `car + 0xBC + 0x98` over func_8002F690 (state 0, which also arms state 1), this, func_80030814 (state 2, airborne) and func_80030BC4. Turns the launch spin seeded in route+0x50 into clamped yaw, recomputes revs / tacho / world velocity, then sets route+0x38 = 0x14 and route+0x98 = 2. **Was described here as an "engine-sound update"** — the func_8005C104 call is 12 of 505 words and is a house idiom shared with the sibling handlers. |
| `GameInitPlayerCar` | 0x8002C478 | 548 | Race-entry init for the player object: `g_RacePhase = 2`, `g_RaceSeries`, clear the runtime block, seat the car at the start pose from `g_TrackEventData + series * 0x90 + 0x354`, and build the speed/gear lookup tables `D_801E8884` (0x40 per gear), `D_801E4114`, `D_801E4154` from `g_CarSpec`. Its own GameDebugPrintf labels are `init_car`, `h_tbl`, `init0`, `init1`, `init1b`, `init2`, `init4`..`init6`, `init_ok`. **Was described here as a "track-geometry sample builder"**; `g_TrackPoints` is touched once, to seat the car. |
| `GameResetCarTrackState` | 0x80032280 | 596 | The non-clamping twin of matched func_80031298: recomputes a car's track-relative placement (segment interpolation, progress modulo `g_TrackLength`, road heading and grade) from `car->trackPointIndex`, with none of func_80031298's boundary clamp or collision push, and writes the reference orientation triple at +0x50/0x54/0x58 rather than the live +0x20. All four call sites are the init/reset paths in func_80034F74. **Was described here as a "marker or sprite builder"**; it builds no primitives. |

### Animated course scenery with sound (see also 5b)
| Name | Addr | Words | Purpose |
|---|---|---:|---|
| `GameUpdateFlybyScenery` | 0x8003E590 | 345 | The course's one scripted **airborne** prop, updated (not drawn) once a frame from the race scene and twice from func_8003F608. Armed when `D_8009E83C == D_801E4308 && D_8009E74C` matches the per-series entry in the course event block, then lives 0x1C3 = 451 frames. Linearly lerps three Euler angles over each keyframe's `duration` (-1 wraps), builds `rsinY(0x800 - rotY) * rsinX(rotX) * rsinZ(rotZ)` and **integrates** position by rotating the forward vector `(0, 0, -radius * 4)` through it, so the keyframes steer a heading rather than list waypoints. Sound: distance to the listener `D_8009E6D4/D8/DC` as `dx²/8 + dy²/16 + dz²/8` through func_8006888C, mapped to volume `0x74 - dist`, at fixed pitch 0x1900 — the halved vertical term is what says the object is above the track. `g_CourseIndex & 3` picks cue 1 (MYTHICAL COAST), cue 1 forced silent (OVER PASS CITY) or cue 2 (LAKESIDE GATE, THE EXTREME OVAL). **Was filed under "Sound & music" as a "car proximity/engine audio-cue driver".** |
| `GameUpdatePathScenery` | 0x8003F9C4 | 859 | The course's **permanently looping** prop, seeded by func_8003F700 and drawn by func_80040730. Two keyframe tracks: A at `D_801E42DC` stride 0x14 `{s32 x,y,z; s16 loopIndex; s16 duration; s16 easeFlag}` drives position `D_801E4DB8/BC/C0`, B at `D_801E42E8` stride 0x0C `{s16 rx,ry,rz; …}` drives rotation `D_801E4DC8/CA/CC`. Motion is a **sinusoidal ease** between waypoints (`P[i+1] - half - half*rcos(t*0x800/dur)>>12` then `P[i] + half + half*rsin(…-0x400)>>12`), not a lerp. Sound is cue 0, culled outside a ±0x1000 box, `vol = 0x64 - (sqrt(dx²/4 + dy²/8 + dz²/4) >> 10)`, slew-limited ±0x14 against `D_801E4DF0`, with pitch `((delta/2) + 0x3C) << 7` — an approximated Doppler shift. **Was described here as a "dual-channel record/marker state machine … stride 20"**, wrong on the framing and on both strides. |

### Sound & music
| Name | Addr | Words | Purpose |
|---|---|---:|---|
| `GameSetPitchedSoundCue` | 0x8005C914 | 295 | Mono positional cue setter over `EffectVoice D_801E6D30[4]`. `arg0` is a **cue index**, not a mode: each cue owns a fixed pair of voices (cue 0 → voices 0..1 / hw 10,11; cues 1 and 2 → voices 2..3 / hw 12,13, hence mutually exclusive), and the call keys the pair on, updates it in place if it already holds the cue's programs, or keys it off at volume 0. `pitch` is a 7.7 note (0x1900 = note 50); volume 0..0x7F scaled by the record's volScale. Its two callers are `GameUpdateFlybyScenery` and `GameUpdatePathScenery`. |
| `GameSetStereoSoundCue` | 0x8005C31C | 233 | The stereo twin, over `D_801E6D00[2]`, taking independent left/right volumes when `D_80082F40` (written by `GameSetStereoOutput` / `GameSetMonoOutput`) says stereo and the average in both when it says mono; func_80040ADC picks the argument order from `g_MirrorMode`. Cues {0,1} and {2,3} form two groups and a stop only lands within the requested group. **Was described here as a "sound-channel/state reset"** — it only looks like one because func_8003591C's teardown calls it four times in a row as (2,0,0) (3,0,0) (0,0,0) (1,0,0). |

Both share one 7-record table of stride 0x18 running contiguously from
`D_800126D0` to `D_80012748`, shaped
`{ s32 voiceCount; s32 volScale; struct { s32 prog; s32 tone; } v[]; }` —
records 0..3 are the stereo cues, 4..6 (`D_80012730` onwards) the mono ones.
Note that `EffectVoice`/`MusicChannel` +0x00/+0x04 are the VAB **program** and
**tone** numbers, not the field names currently in `sound.h`.

### Menu / HUD / overlay renderers
| Name | Addr | Words | Purpose |
|---|---|---:|---|
| `GameDrawSkyBackground` | 0x800418D4 | 1211 | The sky/horizon backdrop, drawn by every scene that has a horizon. A 4 × 8 sweep emits the visible half of a 16-segment panorama cylinder as POLY_FT4s, uv rows from `D_8007F510 + 8 * D_8007F470[…]` indexed by `(yaw >> 7) + j`, linked at OT + 0xAFC; the gradient bands underneath are shaded between successive colour slots with the same `g_CourseIndex == 2 ? slots 5,6 : slots 7,8` split func_80045CD4 uses. Yaw and roll are negated when `g_MirrorMode` disagrees with the scratchpad flag at 0x1F800068. **Was described here as a "HUD/billboard primitive builder".** (cc=2.7.2) |
| `GameDrawTeamLogoCanvas` | 0x8004A248 | 1435 | Draw half of the logo painter (largest non-SDK function). `(0, 0)` resets both panel accumulators `D_8007FB0C` / `D_8007FB10`; otherwise they ramp and gate the outer panel (12 slide steps) and the paint sub-panel. Uploads the 64×64 4bpp canvas `D_801E6F2C`, its raw CLUT `D_801E444C`, and a copy scaled by the fade level `D_8009B298` whose entry 0 is three phase-shifted sines of `D_8009B288` (the colour-cycling cursor), then emits the frame, the zoomed canvas, the 1:1 preview, the swatch boxes and the crosshair. (cc=2.7.2) |
| `GameUpdateTeamLogoCanvas` | 0x8004C0D8 | 894 | Input half of the same widget, called from the PAINT branch of `GameUpdateTeamLogoScreen`. Plots with Circle held — replaces the nibble at `(u16 *)D_801E6F2C + (y << 4) + (x >> 2)` over a `D_8007F94C`-sized brush — and maps the d-pad through the auto-repeat timer `D_8007FB14` onto the scroll/flip/rotate helpers func_8004B9B8..func_8004BF48. Holding all four shoulder buttons and pressing Select toggles `D_8007F930`, a hidden palette editor over the 5-bit channels of the selected CLUT entry. **Was described here as a "4bpp texture / palette editor debug tool"** — it is the shipped feature. (cc=2.7.2) |
| `GameDrawRankingTable` | 0x8004D384 | 1017 | The five record rows, called three times from `GameUpdateRankingScreen` as `(accumulator, step, table)`. Reads the `S22` records from `D_801E7744` (ranking) or `D_8019CB78` (time) and draws the place number, its suffix from the `"ST"/"ND"/"RD"/"TH"` table at `D_80011920`, the holder's name and the row background. Sole caller of `GameFormatLapTime` in the image. |
| `GameDrawCourseSelectScreen` | 0x8005290C | 849 | Slot 1 of the overlay table `g_MenuScreenDraw`, i.e. the fade/transition overlay of the **COURSE SELECT** screen: scroll accumulator `D_8009B2C0`, wave/colour offsets, sprite/number draws. (cc=2.7.2) |
| `GameUpdateCarSelectScreen` | 0x8005568C | 783 | Slot 4 of `g_MenuScreenUpdate`, the **CAR SELECT** hub (race start / customize / car shop / engineer shop / course select); a jump-table switch on `GameMenuBusy` picks the exit — a race, or screens 5 / 11 / 12 / 1. |
| `GameDrawCarSpecGraph` | 0x800496F0 | 675 | The car performance bar chart, drawn obliquely: 45°-recession floor lines plus four bars at `x = 0x66 + 12i`, each a front face with a lightened top (+0x40) and darkened right (−0x40) face and a semi-transparent drop shadow, over the four violet colours at `D_80011870`. Bars 0..2 ease towards the car asset's spec bytes +0x0B/0x0C/0x0D, bar 3 towards one of 10/30/50/70/90 selected by the tire grade. func_8005ACA0 calls it every menu frame, but its `step` argument `D_8009B324` only leaves 0 on entry to and exit from CUSTOMIZE, so it is only visible there. **Was described here as a "debug palette/gradient UI renderer".** (cc=2.7.2) |
| `GameDrawTeamNameEntry` | 0x8004E724 | 585 | The whole **TEAM NAME** widget: the 4×11 grid of 8px glyph cells at `x = 0x56 + 12*col` wiping in as `D_8007FB28` climbs to 25, the cursor cell redrawn with flags 0x5B, the pulsing highlight box (green channel from `rcos(D_8009B28C += 96)`), the 12×24 caret while `g_TeamNameLength < 6`, and the typed `g_TeamNameChars`. Grid cell 10 is a gap, hence the `index >= 11 ? index - 1` glyph fixup. **Was described here as a "HUD/standings renderer".** (cc=2.7.2) |
| `GameDrawRaceOptionMenu` | 0x8003479C | 396 | The in-race option/pause overlay, drawn by both in-race scene handlers after they clamp the cursor `D_801E414C` against `2 - g_GrandPrixMode` and decrement `g_SceneTimer` to freeze the scene; `cursorRow` steps the 64×11 highlight outline 10 pixels per row. `"  RAGE RACER GE"` is not a title — it is the first 0x14-byte half of one entry in the 4 × 0x28 marquee table at `D_8007DF34` (`"  RAGE RACER GETS YOU GOING!  "` twice, `"   KICK BACK AND CHILL OUT!   "`, `"    SLASH THOSE RECORDS!     "`), selected by `g_SceneTimer & 3`, scrolled by the two accumulators `D_8007DF30`/`D_8007DF32` and clipped to (114, 138, 92, 12). **Was named `GameDrawTitleScreen` in screens.h; that alias is retired.** |
| `GameDrawMenuCarView` | 0x8005131C | 396 | The 3D car view behind screens 3, 4, 5, 6, 10, 11, 12. Installs the menu view matrix from `D_80082D6C`, eases `g_MenuViewOffset` / `g_MenuViewAngle` (angle wraps modulo 600000), and on arrival with no load pending flips the two-slot model double buffer `D_8009E87C` and commits `D_8009B378` into `D_8009B374`. Then submits **two** models: the car through the render object at `D_8009E6D4`, and fixed model 14 (the showroom floor) with the OT cursor bumped 0x78. L2/R2 nudge the tilt `D_8009E718` within ±6144; L1/R1 nudge `D_801E8268` within ±64. |
| `GameDrawCarEngineSpec` | 0x80052158 | 376 | The two engine spec lines, `MAX POWER <n> ps / <n> rpm` at `y = 0xCC - yOffset` and `MAX TORQUE <n>.<n> kgm / <n> rpm` at `0xDA - yOffset`, each number through `sprintf("%d")` with `x` advanced 6 per digit; values are the car asset header fields +0x10/0x12 and +0x14/0x15/0x16. Its third argument is loaded and discarded. Shared by the id 5 / 11 / 12 draw halves. |
| `GameDrawStartCountdown` | 0x8003425C | 336 | The race-start signal gantry, live for `105 <= g_SceneTimer < 300` behind a `g_RacePhase < 4` guard. `phase = (t - 90) / 30` selects one of the 32×16 dot-matrix bitmaps `D_8007DDC0[1..4]` — they read **"3", "2", "1", "GO"** — stamped bit by bit onto 512 TILE prims of stride 0x10, each cell taking one of the four `code \| rgb` words at `D_8007DF1C` (code 0x60 = TILE; red on/off then blue on/off), with the band `7 - t' < row < t' + 8` inverted so each digit wipes open. Phase 0 flashes every cell. Underneath: two 96×24 backings and a 3×2 array of 32×24 start lamps whose column `phase - 1` brightens over 16 frames; `D_8007DF18` then slides the board off at −16 a frame. **Was described here as a "debug colour-grid renderer".** |
| `GameSeekEnvironmentScript` | 0x800458CC | 258 | Jumps the environment colour timeline to `time`: wraps modulo `D_8019C774`, walks the 0x30-stride cue list at `D_801E42F4` for the record containing it, backs up two records (wrapping to the tail), publishes that record's nine RGB words into the colour slots, sets the lerp numerator/denominator, applies one frame, enables the script unless `g_GrandPrixClass >= 5`, and programs `SetFarColor(slot 0)` + `SetFogNear(D_8009B24C, 320)`. Two of its callers pass the current position minus 1800 or 3000 frames, i.e. a rewind. **Was described here as writing "packed words into the `D_801E3FBx` framebuffer-head block"** — see the colour-slot layout below. |

The block the last two share is nine 12-byte colour slots at
`D_801E3FB6 + 0x0C * k`, each `{ u8 cur[3]; u8 pad; u8 from[3]; u8 pad; u8
to[3]; u8 pad }`. Slot 0 is the GTE far/fog colour, slots 1..8 the sky gradient.
func_80045CD4 is the per-frame lerp (fraction `(D_801E4022 << 12) /
D_801E4024`), which also cross-fades a 16-colour CLUT and `StoreImage`s it to
VRAM (0xE0, 0x1E6, 16, 1). `D_801E3FB4` is the fog-active flag, `D_8009B24C`
the fog-near distance and `D_801E4026` the environment id (2 = the heavy-fog
variant).

### Track & rendering
| Name | Addr | Words | Purpose |
|---|---|---:|---|
| `GameDrawCar` | 0x8001DFC0 | 445 | Draws one car, from the func_800389F0 loop over the 11 runtime entries (`activeFlag != -1 && field_BC == 1`). Culls on `out[2] >= 0`, then picks a LOD by Manhattan camera distance: < 3328 gives the full body plus three extra prims and a two-pass mirrored sub-part (the pass negates matrix columns 0 and 2 and the +0xC offset — the left/right wheels), < 9472 a single low-detail prim, beyond that nothing. Every submission is `*(s32 *)0x1F800084 = colour; func_80028DEC(0x1F800000, primId)` with primId clamped against the object-bank size `D_801E4168`. **Was described here as a "render-object transform"**; correspondingly, §2's `CamRow` entry is wrong — `D_8019C9A8` is a pointer, the index is the per-object model selector `D_8007D3AC[g_CourseIndex][obj->field_AE]` rather than a screen number, +0xC/0xE/0x10 is a mirrored sub-part offset vector and +0x12 a Y bias restored on exit. |

### CD / streaming
| Name | Addr | Words | Purpose |
|---|---|---:|---|
| `StCdInterrupt` | 0x8006D1D0 | 604 | The libds streaming state machine: advances `D_80099418` through states 1..0xA, reads the CD result (error bit 0x04 → state 3), recomputes the ring cursor as `D_8009DF1C = (D_801E6C74 << 5) + D_801E8AAC`, DMAs sector header then body, compares the chunk against `StStrHeader.nFrames` and calls `StFreeRing`. Named as the public entry point rather than an internal because it takes no arguments, because the `CdReadyCallback` the library installs (func_8006CDA0) is a six-instruction stub whose whole body calls it, and because the game calls it directly — func_8001EBC8 does `if (D_8019CA00) { StCdInterrupt(); D_8019CA00 = 0; }`, the documented `StSetMask` use. Its TU already supplies `StClearRing`, `StGetBackloc` and `StSetStream`. (cc=2.7.2) |

### SDK library
| Name | Addr | Words | Status | Purpose |
|---|---|---:|---|---|
| `LibcSprintf` | 0x800632F0 | 535 | [INCLUDE_ASM] | PSY-Q libc `sprintf`, the whole formatter with no `vsprintf` split — all ~30 call sites pass varargs directly and nothing wraps it. Digit tables `"0123456789ABCDEF"` at `D_800131E4` and `"0123456789abcdef"` at `D_800131F8`; callees are the matched `LibcMemchr` / `LibcMemmove` / `LibcStrlen`. |
| `TransposeMatrix` | 0x80069CC8 | 46 | matched C | libgte `TransposeMatrix(m0, m1)`: transposes only the 3×3 rotation part and returns `m1`. Sits next to `RotMatrix` (func_80069D18) and all nine callers are inside func_80043BCC. Body is decompiled with register pinning; the file also carries func_80069D18 as raw asm. |
| `SsSeqApplyDataEntry` | 0x8007010C | 360 | [INCLUDE_ASM] | Named in section 17d. libsnd internal, so the name is descriptive rather than a recovered Sony symbol. Behaviourally it is the MIDI **Control Change #6 (Data Entry MSB)** handler: func_8006F1E0 routes status 0xB0 to func_8006F5F4, whose `case 6:` is this. It applies the pending RPN/NRPN to the channel's VAB program by rewriting the `VagAtr` of every tone — `SsUtGetProgAtr` for the tone count, then per tone `SsUtGetVagAtr` → mutate → `SsUtSetVagAtr` — with the field chosen by `SeqStruct + 0x13`: 0 → +0x0C/+0x0D (`pbmin`/`pbmax`, i.e. RPN 0 pitch-bend sensitivity), 1 → +0x05 (`shift`), 2 → +0x04 (`center`), all gated on `play_mode == 0`. **Was described here as the "libsnd sequence tick/step"** — that is `SsSeqCalledTbyT` at 0x800731CC. |
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
| S22 | 0x10 | `s8 name[8]` (a.k.a. `pad[8]`); `s32 v8`; `s16 vC`; `s16 vE` | Ranking / time high-score record row. Tables `D_801E7744[][4][5]` (ranking) and `D_8019CB78[][4][5]` (time). Referenced by matched func_80021DB8 and by `GameDrawRankingTable`. |
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
| 1 | func_80053730 | `GameDrawCourseSelectScreen` | (`D_8009B2F0`, shared) | **COURSE SELECT** (TIME ATTACK header in TA mode) |
| 2 | func_80054D10 | func_80054C84 | `D_8009B2C4` | **RANKING** — total time / lap time / exit |
| 3 | func_80055618 | – | – | one-frame bridge into id 4 |
| 4 | `GameUpdateCarSelectScreen` | func_800551BC | `D_8009B2CC` | **CAR SELECT** — race start / customize / car shop / engineer shop / course select |
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
in `D_80082EF0`: `GameDrawCarSpecGraph` (called unconditionally by func_8005ACA0
every frame, but only visible on CUSTOMIZE — see section 1), `func_8004CF30`
(brightness overlay used by ids 1/3/4/5), `func_800509C4` (counter in
`D_8007FB4C`) and `GameDrawCarEngineSpec` (the engine spec lines shared by the
id 5/11/12 draw halves).

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

### Was 2.7.2 ever used?

No. The `Makefile` used to pin eight objects to `cc1-psx-272`; all eight now
match under gcc 2.6.3 and the pins are gone, so the second toolchain is dead
weight and the game was almost certainly built entirely with 2.6.3. Every pin
turned out to be a property of the *decompiled* source rather than of the
original build:

| Unit | Why it was pinned | What fixed it under 2.6.3 |
|---|---|---|
| `track/GameLoadEnvironmentCue` | cc1 2.6.3 **crashes** on `__attribute__((packed))` applied to a struct | move the attribute onto the field; identical `.text` |
| `race/GameExitBgmSelect` | a `delta = g_FadeStep` temporary flipped the `addu` operand order | drop the temporary, test and add `g_FadeStep` directly |
| `menu/GameComposeSampleTeamLogo` | `src0++` sat between the third and fourth nibble test, so 2.6.3 filled a delay slot with it instead of duplicating the branch target | move `src0++` to the end of the loop body |
| `lib/libsnd/SsSeqOpen` | the loop flag was `s32` with a `(u8)` cast at the test, which 2.6.3 folds away | declare the flag `u8` (PsyQ's `unsigned char flag`); the `andi …,0xff` comes back |
| `lib/libsnd/SpuVmSeKeyOff` | `(u16)` cast on an `s32` fed by an `lhu`; 2.6.3 knows the high bits are clear and drops the mask | declare the voice index `u16` so the truncation is the variable's type |
| `lib/libspu/_SpuSetAnyVoice` | a pinned `hi` temporary put the `or` result in the wrong register, blocking the cross-jump that merges the two `sh` stores | fold `hi` into the `*reg_hi |= …` / `&= ~…` expressions |
| `sdk/func_80077A88`, `sdk/func_8006DB74` | nothing — both already matched byte-for-byte under 2.6.3 | no change |

Two further pins (`func_8006DD30`, `func_8006E390`) named `.c` files that no
longer exist after the subsystem rename and had been inert for some time.

The pattern is consistent: 2.7.2 is the *less* aggressive compiler on narrowing
casts (it keeps `andi …,0xff` / `andi …,0xffff` that 2.6.3 proves redundant), so
a decomp written with a redundant cast will look like it "needs" 2.7.2. It does
not; the cast belongs in the variable's declared type instead. Nothing here
distinguishes Sony library code from Namco game code — the libsnd/libspu units
fell to the same class of fix as the game ones.

The `2.7.2` branch in `tools/scripts/cc.sh` is now unreachable from the build
and can be deleted whenever convenient. Note that `(cc=2.7.2)` annotations in
the function table above predate this and should be read as "was compared
against 2.7.2 output at the time", not as a requirement.

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

### What closes a run, in practice

Sweeping every `PAL/main` `c` segment for merge candidates (cap 8 functions /
700 lines) left **131 boundaries** that are not the cap. They fall into four
classes, and only the first two are evidence about the original build:

| Class | Count | What it means |
|---|---:|---|
| conflicting declaration of a shared symbol | 50 | the two sides spell one `D_` global with different types (`u16`/`s16`, `volatile`/plain) or one callee with a different signature. Both spellings are load-bearing, so the two functions were **not** one unit. |
| the merged unit does not compile | 27 | the definition's own parameter types disagree with the prototype in a header one side includes — e.g. `GameDrawLeftArrow(…, s32 x, s32 y, …)` against `game/state.h`'s `s16`. The definition side never saw that header. |
| per-file compiler | 27 | the `Makefile` pins the object with an explicit `RAGE_CC1_VERSION_OBJ` rule; a unit compiles as one object, so such a file cannot join a neighbour under a different pin. (Historically some of these pins were to gcc 2.7.2; none are left — see "Was 2.7.2 ever used?".) |
| named by a `.rodata` subsegment | 27 | the config places an asm rodata block through `[…, .rodata, PAL/main/<unit>]`, and splat emits `<unit>.c.o(.rodata)` for it. Such a file may **lead** a unit but may not be merged away, or the link loses the object. |

Two mechanical constraints are easy to get wrong and both produce a build that
fails rather than a wrong ROM:

- **Adjacency is over *all* `c` segments, not just `PAL/main` ones.** libsnd and
  libspu units are interleaved into the same address order; two `PAL/main`
  files that look consecutive in a filtered list can have a libsnd unit between
  them, and merging them relocates that unit.
- **`asm/` and the objects that `.include` it have no make dependency.** After
  `make split` regenerates a stub, the `.c.o` that includes it must be deleted
  by hand or make will happily relink a stale object.

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

## 12. Globals naming, round 3

Round 3 named 53 more `D_XXXXXXXX` globals, all byte-neutral (`make check
VERSION=PAL` = `build/PAL/main.exe: OK`, sha1 `2913e15648eddef40821c5f666460abc04155ee6`,
with `build/PAL/src/main/PAL/main/*.o` wiped before every check). Targets were
picked by measured reach (`grep -rl` over `src/`) working top-down; of ~1400
still-raw symbols only ~20 reach 10+ files and ~90 reach 5-9, and 812 appear in
exactly one file, so the single-file tail was left alone.

### 12a. The environment / time-of-day chain, and why the sky changes run to run

This is the correlation that confused readers, and it is one variable, not two.

    g_GrandPrixClass  (a dice roll in Time Attack)
      -> course asset index = 0x57 + (course << 1) + (class << 3)
      -> that VARIANT's data carries an environment mode at +0x2C
      -> g_EnvironmentMode
      -> which 48-byte (16 x RGB) sky palette record is the lerp target
      -> plus fog near/far, plus g_IsEnvironmentMode4's prop and model choices

`func_80018A70` re-rolls `g_GrandPrixClass` on every Time Attack race start,
bounded by `g_MaxClassReached[g_GrandPrixSeries]`, with a re-roll to `>= 2` on
THE EXTREME OVAL (which corroborates the earlier finding that OVAL variants 1
and 2 were never authored). Because the same value is the track-variant index,
one roll simultaneously decides which sky loads and whether the spinning props
appear at all (the dispatcher gates them on class `>= 2`). There is no separate
weather variable. `g_GrandPrixClass` keeps its name: it really is the class, and
Time Attack simply picks one per race.

The colour timeline itself (`func_800455EC` loads a keyframe, `func_80045CD4`
lerps one frame of it) is nine 12-byte `{ cur, from, to }` RGB slots at
`D_801E3FB6`; slot 0 is the GTE far/fog colour, slots 1..8 the sky gradient.
`g_EnvironmentMode` indexes `g_EnvPaletteTable` for the 16-colour CLUT that gets
uploaded through the 16x1 VRAM rect at (0xE0, 0x1E6), and `g_EnvironmentModePrev`
is the lerp's source palette. Mode 2 alone ramps `SetFogNear` up to 0x7FFF
(clear); every other mode ramps it down to 0x1770 (hazy).

**What mode 4 is remains unknown.** `g_IsEnvironmentMode4` is provably
`g_EnvironmentMode == 4` and provably selects `GameDrawStaticScenery`'s model
`0x3B` over `0x3A` and the `flags & 2` prop set over `flags & 1`, and every car
and track renderer forwards it to scratchpad `0x1F800084`. Whether mode 4 is
night, dusk or rain cannot be settled from the executable: `g_EnvPaletteTable`
is a runtime pointer installed by `func_8004553C` from the loaded variant asset,
so the palette contents are on the disc, which this repo does not have. The name
is therefore deliberately mechanical rather than descriptive.

**`D_801E4028` left unnamed.** It is `((*(u16 *)(src + 0x2E) >> 15) ^ 1)`, and it
gates lerping `D_801E3FB9` - the fourth, spare byte of colour slot 0 - toward
that halfword's low bits. No decompiled function anywhere reads `D_801E3FB9`, so
what the flag is *for* is unrecoverable and any name would be a guess.

### 12b. Named this round

| address | name | type | files | justification |
|---|---|---|---|---|
| `D_801E4030` | `g_IsEnvironmentMode4` | `s32` | 12 | `g_EnvironmentMode == 4`; picks static-landmark model `0x3B` and the `flags & 2` props |
| `D_801E4026` | `g_EnvironmentMode` | `s16` | 2 | variant data `+0x2C`; indexes the 48-byte sky palette records |
| `D_801E4FB0` | `g_EnvironmentModePrev` | `s32` | 2 | the previous mode, i.e. the CLUT lerp's source palette |
| `D_801E4140` | `g_EnvPaletteTable` | `u8 *` | 2 | base of those records; installed by `func_8004553C` |
| `D_801E6F2C` | `g_TeamLogoCanvas` | per-file | 13 | 2048 bytes = 64x64 4bpp; VRAM rect `D_8007BEE4` {0x290,0x30,16,64}; the 4-bit nibble-carry scroll helpers only make sense at 4bpp |
| `D_8009E6D4` | `g_PlayerCar` | per-file | 10 | 0x19C bytes, the `g_Cars[]` stride; `func_80034F74` calls the same helper on it and on `g_Cars[0]` at the same field offsets |
| `D_80082460` | `g_UiChromeScript` | `u8` | 9 | always `GameRunTimedDrawScript`'s table arg paired with `g_UiScriptProgress`; 15 records + terminator, limit 25 |
| `D_801E40E4` | `g_CourseModelCount` | `s32` | 9 | written from `bank[0]` by `func_80017848`; every scenery drawer clamps a model id against it and falls back to 1 |
| `D_8019C70C` | `g_BestTotalTimes` | per-file | 8 | `[series][course][mode]` ms, lower-is-better; the 3x/6x default ratio vs `g_BestLapTimes` proves total-vs-lap |
| `D_801E6828` | `g_VisibleCellMask` | per-file | 8 | `func_800414F0` rebuilds 32 words as `mask[sy] |= 1 << sx` from `camera / 2048`; consumers cull on the cell bit |
| `D_801E444C` | `g_TeamLogoClut` | per-file | 8 | 16 `u16`, uploaded through the 16x1 rect `D_8007BEDC`, always back-to-back with the canvas |
| `D_8009E698` | `g_CarModelAsset` | per-file | 8 | sole writer `func_80017BAC` sets it from the loaded-asset registry; readers use `GameCarModelAsset` `+0x18/+0x20/+0x24` |
| `D_8009E67C` | `g_CourseProgress` | per-file | 8 | repointed to `D_801E42EC` / `D_8009E874` with the file's car table; bytes 0..3 are best place per course |
| `D_8009E83C` | `g_PlayerLap` | per-file | 8 | `g_PlayerCar + 0x168`; incremented when progress passes lap x `g_TrackLength` |
| `D_801E4B30` | `g_ImageBlockBuffer` | per-file | 8 | `g_AssetBlockPtr + 0x40000`; every read hands it to the image-block installer `func_8001A3C0` |
| `D_801E4168` | `g_ModelBankCount` | `s32` | 7 | same clamp idiom as `g_CourseModelCount` but for the bank `func_80017A10` last bound |
| `D_801E4364` | `g_LapCount` | `s32` | 7 | `= (g_CourseIndex == 3) ? 6 : 3` at race init; bound of the per-lap time arrays |
| `D_8009E744` | `g_PlayerTrackProgress` | per-file | 7 | `g_PlayerCar + 0x70`, matched against `obj->field_70` at a shared callee |
| `D_8009E74C` | `g_PlayerTrackSection` | `s16` | 7 | `g_PlayerCar + 0x78`; drives the tunnel tpage swap and track-event triggers |
| `D_8007F604` | `g_CdCommandPending` | `s32` | 7 | `-1` idle, 1 play / 2 pause / 3 resume; dispatched by `func_80043974` |
| `D_8007F60C` | `g_CdCommandStep` | `s32` | 7 | 0..6 sub-state of the running CD-DA command; reset by every setter |
| `D_801E4369` | `g_PadType` | `u8` | 6 | only ever compared against `0x41` (digital pad) and `0x23` (NeGcon); never written from C |
| `D_80082790` | `g_UiChromeScript2` | `u8` | 6 | the `g_UiScriptProgress2` sibling table, 8 records, limit 16 |
| `D_8009B300` | `g_MenuConfirmTimer` | `s32` | 6 | set to `0x23` the frame a confirm is accepted, decremented to 0, then the screen advances |
| `D_8009B320` | `g_MenuPlateCarIndex` | `s32` | 6 | `func_8004FCE8`'s car index; latched from `g_CarListCursor` / `g_PlayerCarIndex` |
| `D_8009B194` | `g_CdVolume` | `u8` | 6 | divided by 127 into the four CD mix words; `func_80043134` rescales the 0..15 option into it |
| `D_8009E870` | `g_CameraViewMode` | per-file | 6 | `func_80043BCC`'s mode arg; 0 chase (only mode with a mirror), 1 in-car, 2 replay |
| `D_8007BEDC` | `g_TeamLogoClutRect` | per-file | 6 | literal `RECT{16,480,16,1}` in `.data`, used only with `g_TeamLogoClut` |
| `D_8019CB40` | `g_ClassRecords` | per-file | 6 | 0x2C bytes = 11 x `{s16 grade, s16 clears}`, index `series * 6 + class`; `-1` locked |
| `D_801E4388` | `g_CarTable3` | per-file | 6 | save block `+0x128`, the third 13 x `GameCarEntry`; `g_CarTable` is repointed at it |
| `D_8019CAFC` | `g_AssetLoadCursor` | per-file | 6 | advanced by each `GameLoadAsset`'s returned size; its settled value becomes `g_AssetBase` |
| `D_801E6D90` | `g_SeqHandle` | `s16` | 6 | the `SsSeqOpen` return value, passed as `seq` to `SsSeqPlay/Stop/SetVol` (its old header comment said "sequence volume" and was wrong) |
| `D_8007F600` | `g_CdTrackPending` | `s32` | 5 | indexes the per-track `CdlLOC` table `D_8009AFD4`; `-1` = none |
| `D_8007F608` | `g_CdTrackStep` | `s32` | 5 | 0..7 sub-state of the track-change sequence in `func_800432A8` |
| `D_8009B2F0` | `g_MenuSubCursor` | `u8` | 5 | the open modal's cursor; deliberately generic, its range differs per screen |
| `D_80011BA0` | `g_MenuBlankCaption` | `u8` | 5 | the `.asciz " "` literal, always `GameDrawMenuButton`'s caption with flag `0x10` clear |
| `D_801E40A8` | `g_BgmTrackCount` | `s32` | 5 | `9`, or `10` once five class records are grade 1; bound of the `D_801E7734` shuffle bag |
| `D_8019C8EC` | `g_SeriesCleared` | `s32` | 5 | set only when the series' final class is completed; wipes the save and starts the ending |
| `D_8019C760` | `g_StreamReturnScene` | per-file | 5 | `g_SceneId` is restored from it when the stream ends; non-zero also skips title re-init |
| `D_8009E87C` | `g_CarModelSlot` | per-file | 5 | `= (self < 1)`, a pure 0/1 flip selecting one of two 0x20000 model buffers |
| `D_8009E6AC` | `g_SceneLightMatrix` | `Matrix` | 5 | assigned wholesale from a per-scene constant matrix, then loaded into the GTE |
| `D_8019C904` | `g_AssetBase` | per-file | 5 | base of the resident asset block; all sub-block pointers are `base + base[n]` |
| `D_801E8AB0` | `g_AssetSubBlockPtr` | per-file | 5 | always `base + header->offsets[n + 1]`, the companion of `g_AssetBlockPtr` |
| `D_80082F28` | `g_SoundSlotTone` | `s16[]` | 5 | `s16[6][2]`, SPU program per sound slot; slot n drives hardware voice n + 14 |
| `D_801E6C9C` | `g_AudioSlotMask` | `s32` | 5 | `|= 1 << slot` on load, `^= bit` on close; returned by `GameGetActiveAudioSlots` |
| `D_801E6CE4` | `g_PanVoiceVolumeL` | `s32` | 5 | left of an L/R pair clamped 0..0x80, `-1` idle, applied to voice `0x15` |
| `D_801E8A50` | `g_SfxVolumeSetting` | `s32` | 5 | OPTIONS row 1, 0..15, feeds `g_EffectVolumeScale = (n << 7) / 15` |
| `D_801E6C70` | `g_MonoOutput` | `s32` | 5 | OPTIONS row 2, 0/1; `0 -> GameSetStereoOutput`, non-zero `-> GameSetMonoOutput` |
| `D_8019C704` | `g_BgmVolumeSetting` | `s32` | 5 | OPTIONS row 0, 0..15, feeds `GameSetSequenceVolumeSetting` |
| `D_801E4408` | `g_BestLapTimes` | per-file | 5 | updated from the best-lap accumulator `D_801E4BCC`; printed one line under `g_BestTotalTimes` |
| `D_801E41E8` | `g_BestSectorTimes` | per-file | 5 | `[series][course][3]`; the three sector boundaries are `trackLength/3` and `2/3` |
| `D_8009E73C` | `g_PlayerProgressA` | `s32` | 5 | `g_PlayerCar + 0x68` |
| `D_8009E740` | `g_PlayerProgressB` | `s32` | 5 | `g_PlayerCar + 0x6C`; `A + B` is the value the position sort and lap test use |

`g_PlayerProgressA` / `B` are named for their identity, not their semantics: the
sum is provably total race progress, but which half carries laps-worth of
accumulated length and which the within-segment remainder is not settled, so the
names stay neutral.

### 12c. Declared per file, not in one header

Where a symbol is spelled with a different type in different translation units
(`u8 *` / `u32` / `s32` / a typed struct pointer for the same pointer, or
`s32[][4][2]` / `s32[]` / `u8[]` for the same table), it gets its own aliased
`extern <that file's type> g_Name asm("D_XXXXXXXX");` in each file instead of one
canonical header declaration. Same name everywhere, identical codegen, and no
`conflicting types` from a header that disagrees with a local view. The relevant
headers carry a comment block listing those names and addresses so they are
still discoverable; the uniform-typed symbols do get real header declarations
and their local `extern`s were deleted.

Two mechanical traps worth recording for the next pass:

* **Inline asm does not follow `asm()` labels.** `func_8005F88C.c` references
  `D_8019C70C`, `D_801E4408`, `D_801E41E8`, `D_8019C704`, `D_801E8A50` and
  `D_801E6C70` inside `asm volatile` strings as `%hi(...)`/`%lo(...)`. Those
  files must keep the raw `D_` spelling; the same applies to `func_8001D338.c`
  for `D_801E444C`. Renaming them there produces `undefined reference` at link.
* **Multi-declarator lines.** `extern s32 A, B, C;` with an `asm()` label
  appended attaches the label to the last declarator only. Every such line had
  to be split.

### 12d. High-reach globals deliberately still unnamed

* **`D_8019CB0C` (20 files)** - an alternate menu layout selector: it shifts
  panel x from `0xA8` to `0x69`, adds a `0x2C` wide offset, and makes
  `GameDrawScriptedSprite` skip script element types 9/19/29/39. It is only ever
  assigned from `D_8009B338`, which round 2 proved is only ever written zero, so
  the alternate layout is unreachable in retail and cannot be characterised.
  Confirms and keeps the round-2 entry.
* **`D_8019C768` (12 files)** - written `0x80` on entry to eleven scenes and
  `0x180` in three race-side inits, and read in exactly one place
  (`GameAdvanceSaveHeaderCounter`: `+= 1` if `0x80`, else `+= 2`) which nothing
  in the image calls. Write-many, read-never-reached; no recoverable meaning.
* **`D_8009B31C` (6 files)** - the fade-step argument of `func_8004FCE8`'s car
  name plate. Its only write in the whole program is `= 0` in
  `GameInitMenuMode`, and `func_8004FCE8` returns immediately when the argument
  is 0, so the plate never draws in the shipped build. Same reasoning that left
  `D_8009B338` unnamed in round 2.
* **`D_801E4BC8` (5 files)** - swapped in lockstep with `g_VisibleCellMask`
  (main view `-> &D_8009EC94`, mirror `-> &D_8009E888`) and walked 64 times while
  the bitmask is rebuilt, then DMA'd to scratchpad with count `0x40`. The role is
  clear but the 16-byte element is still four unidentified words, so no name.
* **`D_801E4028` (2 files)** - see 12a.

### 12e. SDK data: identified, deliberately not renamed

These are Sony library globals. Their meanings are now pinned - in most cases by
a public API accessor in the same file - but they should be given Sony's own
names from Sony sources rather than invented `g_` names, so they were left raw.
Recorded here so the identification is not lost.

| address | files | what it is | how it was pinned |
|---|---|---|---|
| `D_80099048` | 7 | libcd debug verbosity 0/1/2 | `CdSetDebug` is its setter |
| `D_8009905D` | 7 | last CD command byte | `CdLastCom` returns it |
| `D_80099060` | 6 | 32 x `const char *` CdlCOM name table | `CdComstr` indexes it; strings are in `.rodata` |
| `D_800990E0` | 5 | 8 x `const char *` interrupt-code names | `CdIntstr` indexes it |
| `D_8009903C` | 7 | user sync/command-complete callback | `CdSyncCallback` swaps it; called as `fn(status, D_8009BAF0)` |
| `D_80099040` | 6 | user data-ready (INT1) callback | `CdReadyCallback` swaps it |
| `D_80099318` | 7 | 3 status bytes: sync / ready / data-end | the CD ISR writes each byte per interrupt code |
| `D_80099300` | 6 | `-> 0x1F801800`, CD index/status register | literal address in `.data`; bit 5 tested as FIFO-not-empty |
| `D_8009BAF0` / `D_8009BAF8` | 5 each | the two 8-byte CD result buffers | the ISR copies 8 response bytes into one or the other per interrupt |
| `D_8009A588` | 9 | `-> 0x1F801C00`, SPU register base | literal address; `[0x1AA/2]` is SPUCNT |
| `D_8009A4C0` | 6 | `-> 0x1F801074`, I_MASK | `GetIntrMask` / `SetIntrMask` are its accessors |
| `D_8009DF20` | 10 | 24 x 16-byte SPU voice register shadow | `SsUtFlush` copies it to the SPU 16 bytes per voice; zeroed as 192 `u16` |
| `D_8009E0B8` +0x04/0x0E/0x12/0x14/0x1B | 14/6/5/5/6/7 | one 24 x 0x34 libsnd voice-state table, and its `pitch`/`seq_sep`/`program`/`tone`/`status` fields | stride 0x34 in four places; count 24 from `SsUtFlush`'s terminator `< D_8009E0B8` |
| `D_801E4BD0` +0x07/0x0C/0x16/0x1A | 6/6/5/5/11 | one 0x20-byte libsnd current-key-on record and its `program_index`/`tone`/`seq_sep`/`voice` fields | six offset-compatible local typedefs |
| `D_801F2A08` / `D_801F2A0C` | 10 each | pending SPU KEY-OFF masks, voices 0-15 / 16-23 | `SsUtFlush` writes them to SPU halfword `0xC6/0xC7` |
| `D_801E6C6C` | 5 | libsnd tick rate (50/60/120/240) | `SsSetTickMode` sets it; it is the tempo divisor |
| `D_801E40AC` | 7 | libsnd voice-manager re-entrancy guard | the same `if (x != 1) { x = 1; ...; x = 0; }` in five API bodies |
| `D_801E3FB0` | 6 | libsnd mono/stereo flag | `SsSetMono` sets 1, `SsSetStereo` sets 0 |
| `D_801E4110` / `D_801E413C` | 6 / 5 | open VAB's `ProgAtr` table / `VabHdr` | both written in `SpuVmVSetUp` from the per-VAB registries |
| `D_801E4CFC` | 6 | `u8[16]` per-VAB-id state, 0 free / 1 open / 2 allocated | `SsVabOpen` sets, `SsVabTransBody` promotes, `SsVabClose` frees |

---

## 6. Disc asset archive (RAGE.BIN)

All 135 assets live in one archive, `\RAGE.BIN;1`, indexed by a table of
contents in its **first sector**. `func_80017BD4`'s loader reads that sector
once at boot and expands it into `g_AssetCdEntries`:

```
TOC entry i (8 bytes, 135 entries):  { u32 sectorOffset, u32 sizeInBytes }
asset i:  LBA = LBA(RAGE.BIN) + sectorOffset,  length = sizeInBytes
```

`GameLoadAsset(index, dst)` then seeks that LBA and reads `(size + 0x7FF) >> 11`
sectors. `g_AssetPaths[index]` holds the matching `\..\PACK\NAME` string, which
is only ever passed to the debug printf - the path is not used to find the file.
`\RAGE.STR;1` carries the 11 streams in `g_StreamCdEntries` the same way.

Layout of the index space: 0-9 misc (LOGO/TITLE/RG3.VH/RG3.VB/RES.DAT/CAR.TMS/
SAVE.TMS/SELBGM/SELECT/OPTION), 10-73 the 32 car models as `.1ST`/`.2ND` pairs,
74-85 the GP screens, 86 VOICE.BIN, 87-134 the tracks as 6 variants x 4 courses
x 2 halves in the order BIG, MID, HI, OVAL - hence the loader's
`87 + class*8 + course*2`.

### Car data block

The first bytes of a car's `.1ST` are read directly by the menus through
`D_8009E698`:

| offset | meaning |
|---|---|
| +8 | non-zero if the car offers an automatic gearbox. When zero the CUSTOMIZE screen refuses to open the transmission row and plays the rejection cue, and the car shop flags it before purchase. |
| +9 | 4, 5 or 6 - the gear count (the ranking screen switches on it). |

Nine of the thirty-two cars are manual-only (`+8 == 0`): CAR_20, CAR_21,
CAR_60, CAR_61, CAR_80, CAR_81, CAR_90, CAR_B0, CAR_C0. Almost every six-speed
car is manual-only; `CAR_A0` and the `CAR_7x` family are the exceptions.

### The Oval variants are authored, not placeholders

Measured from the disc: all six OVAL variants are distinct data of normal size
(663-716 KB, larger than the matching BIG variants). An earlier guess that the
low-tier Oval data was never produced is **wrong**. What is true is that the
course-select gate (`GameCanSelectNextCourse`) and the attract re-roll both
require class >= 2 before Oval can be chosen, so variants 1 and 2 are not
reachable that way; the no-save first-run path in `func_8001B5DC` does set
class 0 with course 3, which selects OVAL1.

### Default car table (`D_8007BE68`)

Thirteen 8-byte entries, one per model, copied into the car table for a new
game. Only **model 3 has `enabled = 1`**, so `CAR_3x` is the car the player
starts with - not `CAR_00`, whose asset index happens to be first.

The template's transmission byte is set for models 2, 6, 8, 9, 11 and 12,
exactly the models whose asset data clears the automatic-gearbox flag. Two
independent sources agree, which settles the encoding: **0 = automatic,
1 = manual**.

Models 10-12 sit outside the gradeable range (`func_80018A70` and the design
screen both gate on `g_PlayerCarIndex < 10`), so the game shows `GRADE ?` for
them - its own handling of the four single-grade specials, not a defect.
---

## 7. High-fanout names (this pass)

Named by reach: the functions referenced from the most translation units, so
each name pays off at many call sites. "Files" is the number of `src/` files
that mentioned the `func_` symbol before the rename. Every entry below was
verified with `make check VERSION=PAL` staying at
`2913e15648eddef40821c5f666460abc04155ee6`.

### PSY-Q SDK region (>= 0x80063200) — real Sony symbols only

| Name | Addr | Files | Evidence |
|---|---|---:|---|
| `MulMatrix2` | 0x80069568 | 21 | Loads m0 into the GTE rotation control registers (`ctc2 $0..$4`), pushes each column of m1 through `MVMVA` (`cop2 0x486012`: mx=rotation, v=V0, cv=none, sf=12) and stores the three result columns **back into m1**, returning m1. That destination is what separates it from `MulMatrix`/`MulMatrix0`. |
| `SetDispMask` | 0x80065860 | 18 | Its own trace string at `D_80013520` is `"SetDispMask(%d)...\n"`. Body issues GP1(03h): `0x03000000` when enabling, `0x03000001` when disabling, and clears the 0x14-byte cached DISPENV on disable. |
| `ApplyMatrix` | 0x80069678 | 11 | Same `MVMVA` as the multiply family, but with a single vector: `lwc2 $0/$1` load an SVECTOR from arg1 and `swc2 $25/$26/$27` store MAC1..3 into the VECTOR at arg2. Callers confirm the widths (`SVec` in, `LVec` out in func_80031E98). |
| `SsUtKeyOnV` | 0x80077C7C | 10 | Eight arguments `(voice, vabId, prog, tone, note, fine, volL, volR)`; rejects `voice >= 24` and an unknown program with -1, stamps the libsnd utility sep number `0x21` into the current-voice record `D_801E4BD0`, derives volume/pan from the two volumes, copies the ProgAtr/VagAtr fields and returns the voice number. Call sites pass note 0x3C. |
| `MulMatrix` | 0x80069458 | 9 | Instruction-for-instruction identical to `MulMatrix2` except that the four result stores target `a0` and the return is `a0` — the in-place form. |
| `SquareRoot12` | 0x8006888C | 5 | Normalises with `Lzc`, runs the hyperbolic CORDIC in func_80068738 (the repeated shift-4 iteration is the giveaway) and re-scales. Simulated over the input range it returns exactly `64 * sqrt(a)` = `sqrt(a << 12)`, i.e. 12 fractional bits in and out — the `12` variant, not `SquareRoot0`. |
| `MemCopy` | 0x800681BC | 5 | Byte copy returning `dst`, NULL-guarded; shares a translation unit with the already-named `MemFill` (func_80068180). |
| `MulMatrix0` | 0x80068B98 | 4 | The three-argument form of the same MVMVA multiply: result written to m2, m2 returned. |
| `RotMatrix` | 0x80069D18 | 4 | Already identified in section 1; the callers now use the name. |

Deliberately **left generic** in this region, because the Sony symbol could not
be pinned: func_80067F38 and func_80067F04 (the libgpu timeout watchdog — they
own the `"GPU timeout:que=%d,..."` / `"func=(%08x)(%08x,%08x)"` strings and the
`VSync(-1) + 240` deadline, but the public name is unknown), func_8006AB5C (a
libcd interrupt decoder, see section 15d), and the software matrix routines func_80069110 /
func_80069728 / func_800696C8.

### Namco game code (< 0x80063200)

| Name | Addr | Files | Evidence |
|---|---|---:|---|
| `GameDebugPrintf` | 0x8001674C | 31 | The single trace/printf entry point; every surviving PSY-Q format string in the image is passed to it, from libgpu, libcd, libspu and the game alike. The file was already named for it; only the call sites were still on the raw symbol. |
| `GameBuildRotMatrixY` | 0x8001A530 | 24 | Fills `m[0][0]=c, m[0][2]=-s, m[1][1]=0x1000, m[2][0]=s, m[2][2]=c` from rsin/rcos of a 12-bit angle — the Y rotation. Siblings differ only in which row/column holds the identity. |
| `GameBuildRotMatrixZ` | 0x8001A4C0 | 13 | Same shape with `m[2][2] = 0x1000`. |
| `GameBuildRotMatrixX` | 0x8001A5A0 | 12 | Same shape with `m[0][0] = 0x1000`. |
| `GameAtan2` | 0x8001A6AC | 12 | Four-quadrant arctangent over the table `D_8007B664`, in 12-bit angle units (`0x400` = 90 degrees). **Argument order is (x, y)**, the reverse of C's `atan2`: `GameAtan2(0, +y)` is `0x400`, and callers pass `(dx, dz)` / `(dx, dy)`. |
| `GameSelectModelBank` | 0x80017A10 | 12 | Points the scratchpad bank cursor (0x1F800050/54/58) and `g_ModelBankCount` at entry `index` of the registered bank table `D_801E41A8`; `GameSubmitModel` reads exactly those slots. |
| `GameSetupDisplay240` | 0x8001BE9C | 11 | `SetGeomOffset(0xA0, 0x78)` + `SetGeomScreen(0x140)`, then the def-draw/def-disp env pair for two 320x240 buffers stacked at y=0 and y=0xF0, with (r, g, b) as the background clear colour. |
| `GameGetAngleDelta` | 0x8002A7C4 | 10 | Signed shortest delta between two 12-bit angles (already documented in-file). |
| `GameStartCdVolumeFade` | 0x80042CCC | 10 | Sets the remaining frame count of the CD-DA volume fade, clamped to +/-0xFFF; callers pass 1, 8, 30, 60, 120, 250 frames. |
| `GameUploadImageAsset` | 0x8001A3C0 | 10 | Walks the chain of `GameImageBlock` records in a loaded image asset and hands each to func_8001A2E0, which `LoadImage`s it into VRAM. |
| `GameSubmitCourseModel` | 0x800296B4 | 9 | Hand-written GTE engine entry point. Reads the **course** object bank pointer from scratchpad 0x1F800048 (the one func_80017A6C installs, stride 12 bytes, size `g_CourseModelCount`), indexes it by arg1 and interprets the model's opcode list through `jtbl_8007DA54`. Every caller clamps arg1 against `g_CourseModelCount` with a fallback of 1. |
| `GameSetCameraRotMatrix` | 0x8001A610 | 9 | Composes Y*X*Z from the scratchpad camera angles at 0x1F800018 / 0x1C / 0x20 into the scratchpad matrix 0x1F800028 and installs it with `SetRotMatrix`; `D_8019CB18` receives the same product pre-multiplied by a 180-degree Y turn. |
| `GameDrawCourseObjects` | 0x8004123C | 9 | The per-frame loop over the world object array `D_801E4B2C` (`D_801E4BBC` entries): sector-bitmask cull, Z rotation, GTE transform, shade/semi-trans mode word, then `GameSubmitCourseModel` / `...2`. |
| `GameAddTilePrim` | 0x80032F34 | 8 | `SetTile` + `AddPrim` on a caller-supplied 0x10-byte packet, returning the advanced cursor. Declared per translation unit rather than in a header: callers disagree on whether `ot`/`prim` are pointers or `s32`. |
| `GameUpdateCamera` | 0x80043BCC | 8 | The camera-mode state machine (already documented in-file). Also per-TU, for the same reason. |
| `GameUpdateEnvironment` | 0x80045CD4 | 8 | Advances the course environment command script `D_801E40E8`, cross-fades the 16-entry sky/fog CLUT between `g_EnvironmentModePrev` and `g_EnvironmentMode` into VRAM at (0xE0, 0x1E6), then updates the GTE far colour and fog distance. **It draws nothing** — the "draws the sky/background" reading is wrong; `GameDrawSkyBackground` (func_800418D4) is the drawing half. |
| `GameSubmitModel` | 0x80028DEC | 7 | The same engine entry point for the bank at scratchpad 0x1F800050, i.e. the one `GameSelectModelBank` installs; it also mirrors the GTE rotation matrix when the scratchpad mirror flag is set. |
| `GameStartCdAudio` | 0x80042BF0 | 7 | Posts CD command 1, whose handler (func_80043494) issues `CdControl(CdlPlay)`. |
| `GameRequestCdTrack` | 0x80042BC0 | 6 | Posts a pending track index for the func_800432A8 seek/play state machine. |
| `GameResetCdAudioState` | 0x80042C94 | 6 | Clears the pending track and command and resets the current track index to 2. |
| `GameSetCdVolume` | 0x80042FA0 | 6 | Scales the four `D_8007F5A8` mix values by 0..0x7F into both current and target levels and pushes them through `CdMix` (func_8006A94C, a one-line `CD_mix` wrapper returning 1). |
| `GameDrawCars` | 0x800389F0 | 6 | Selects model bank 1 and calls `GameDrawCar` for each of the 11 runtime cars with `activeFlag != -1 && field_BC == 1`. |
| `GameFindTrackSegment` | 0x80030EB4 | 6 | Spiral search for the track segment quad containing the car (already documented in-file). |
| `GameGetCarUnlockLevel` | 0x8001785C | 6 | `g_CarTable[i].modelVariant + D_8007C474[i]` — the progress level a purchase of this model's next grade requires. (Listed here as `GameGetCarModelIndex` until section 17b; the source name was always the right one.) |
| `GameGetAngleDistance` | 0x8002A788 | 6 | Unsigned companion of `GameGetAngleDelta`. Declared **unprototyped** in the header on purpose: func_8002A810 calls it with two extra arguments the original left live in a2/a3. |
| `GameSetupDisplay480` | 0x8001C088 | 5 | The 320x480 variant of `GameSetupDisplay240` (`SetGeomOffset(0xA0, 0xF0)`, both envs full height). |
| `GameSubmitCourseModel2` | 0x80029E50 | 5 | Byte-identical to `GameSubmitCourseModel` except it dispatches through `jtbl_8007DA64`. Selected by `GameDrawCourseObjects` on the per-object flag (bit 2 when `g_IsEnvironmentMode4`, else bit 1). The `2` is deliberate — what the second opcode table renders differently is not proven. |
| `GameRegisterModelBank` | 0x80017948 | 5 | Rebases a freshly loaded model bank's internal offsets to absolute addresses and stores it at `D_801E41A8[index]`. |
| `GameInitRenderState` | 0x80017884 | 5 | Seeds the scratchpad render state: draw distance 0xA, fog/far colours, 0x140x0xF0 screen, mirror flag from `g_MirrorMode`, visibility-mask pointer. |
| `GamePauseCdAudio` | 0x80042C0C | 3 | Posts CD command 2, whose handler captures `CdlGetlocP` and then issues `CdlPause`. |
| `GameResumeCdAudio` | 0x80042C28 | 2 | Undoes the pause, replaying the track when the pause crossed a track boundary. |
| `GameStepCdVolumeFade` | 0x80042D10 | 2 | One frame of the fade, then `CdMix`. |

Deliberately **left generic** here: func_80019EFC (8 files) picks the target
page of the two-state VRAM texture swap that func_8001A030 walks one row at a
time — the mechanism is clear but what the two states *are* is not; func_8004FCE8
(6) draws an 8x16 glyph out of a strip indexed by the car model index, subject
unproven; func_80018410 (6) requests main-state 3 asset loading, and what state
3 selects is not established.

### Existing names propagated to their call sites

These already had a name in a header but their call sites were still on the raw
symbol: `GamePlaySoundCue` (33 files), `GameRandom15` (13), `LoadImage` (12),
`AddPrim` (12), `VSync` (11), `CdControl` (10), `SsUtKeyOffV` (9).
`GameQueueDrawModePrim` (func_80017390), `GameSetGteObjectMatrix`
(func_80017794) and `GameDrawText8x8` (func_80016754) were **not** propagated:
their header prototypes take narrow (`u16`/`s16`) parameters that the call
sites pass as full words, so switching the call sites to the prototype would
insert truncations and change the emitted code.

### Correction found while doing this

`SetDefDrawEnv` at 0x80064B78 is misnamed: it writes a `disp`/`screen` Rect
pair plus `isinter`/`isrgb24` at +0x10/+0x11 — an 0x14-byte **DISPENV** — so it
is `SetDefDispEnv`. The real `SetDefDrawEnv` is the still-unnamed func_80064AA8,
which writes clip/ofs/tw/tpage/dtd/dfe/isbg/rgb (0x1C bytes) and derives `dfe`
from the buffer height and the DMA interrupt state. `GameSetupDisplay240` calls
them in exactly that order (draw env first, disp env at base + 0x5C). **Done in section 17b**: `SetDefDispEnv` is now func_80064B78 and
`SetDefDrawEnv` func_80064AA8.

## 13. Subsystem naming pass (asset loader, FMV, cdread, CD-DA, logo editor)

93 more `func_` symbols got real names, chosen by subsystem rather than by
fan-out. 36 unit files and their config segments were renamed with them. Every
batch was verified with `make check VERSION=PAL` (sha1 unchanged,
`2913e15648eddef40821c5f666460abc04155ee6`).

### 13a. The asset-load state machine (`0x80017BD4`–`0x80019C04`)

The decisive evidence is the path table `g_AssetPaths` (D_8007C48C, 135 entries)
read together with the asset index each loader passes to `GameLoadAsset`:

| index | file |
|---|---|
| 0..9 | LOGO.TMS, TITLE.TMS, RG3.VH, RG3.VB, RES.DAT, CAR.TMS, SAVE.TMS, SELBGM.BIN, SELECT.BIN, OPTION.BIN |
| 10..73 | `CAR_<model><grade>.1ST` / `.2ND`, i.e. `10 + 2 * GameGetCarAssetIndex(model, grade)` |
| 74..85 | `GP0..GP11.TMS`, addressed as `series * 6 + class + 0x4A` |
| 86 | VOICE.BIN |
| 87..134 | `BIG/MID/HI/OVAL <1..6>.1ST` / `.2ND`, addressed as `0x57 + (course << 1) + (class << 3)` |

`GameServiceAssetLoad` (func_80019C04) runs once per frame and dispatches
`g_MainState` 1..12; each phase has a `GameRequest*` that arms it and a
`GameLoad*Assets` step that runs it. Reading the index each step loads is what
names the phase:

| Name | Addr | Evidence |
|---|---|---|
| `GameServiceAssetLoad` | 0x80019C04 | 12-way `switch (g_MainState)` over the loaders below, gated on `g_AssetLoadState != 0` |
| `GameSetTrackCameraTable` | 0x80017BD4 | stores its argument in D_8019C9A8, the camera/horizon row base documented in `game/render.h`; fed sub-block 0 of the `.2ND` track pack |
| `GameResetAssetLoader` | 0x80017BE4 | aborts a running CdRead (`g_CdLoadPhase == 4`) and clears all three state words |
| `GameEnableCdAudioMode` | 0x80017C2C | `CdSync` then `CdControl(CdlSetmode, 0x07)` = report/autopause/CDDA; the last step of every track load |
| `GameLoadAssetBlocking` | 0x80017E48 | `while (GameLoadAsset(...) == 0) {}` |
| `GameLoadDiscArchiveIndex` | 0x80017E8C | its own strings: `Now Searching [%s] ...`, `\RAGE.BIN;1`, `read:%dsector`, `Search ok!`; builds the 135 `g_AssetCdEntries` and the 11 `g_StreamCdEntries` |
| `GameInitAssetSystem` | 0x80018038 | index load + blocking load of asset 0 (LOGO.TMS) + `GameUploadImageAsset` |
| `GameLoadBootAssets` | 0x800180CC | phase 1, assets 1..5 = TITLE.TMS, RG3.VH + RG3.VB (VAB upload), RES.DAT, CAR.TMS, ending with the team-logo `StoreImage` pair |
| `GameLoadSaveScreenAssets` | 0x80018344 | phase 2, asset 6 = SAVE.TMS |
| `GameLoadSelectBgmAssets` | 0x80018484 | phase 3, asset 7 = SELBGM.BIN, split into its three sub-blocks |
| `GameLoadCarSelectAssets` | 0x80018588 | phase 4: uploads the SELBGM bank, loads asset 8 = SELECT.BIN, then the player's `CAR_xx.1ST`. **The dynamic trace guessed "GameLoadRaceAudioBank" for this one; the asset indices say otherwise.** |
| `GameLoadCarModel` | 0x800188B8 | phase 5, `10 + 2 * GameGetCarAssetIndex(car, car->modelVariant)` into the double-buffered showroom slot D_801E4090 (+0x20000 when `g_CarModelSlot == 0`) |
| `GameLoadUpgradedCarModel` | 0x80018A70 | phase 6, the same body but with `modelVariant + 1` — the next grade's car, i.e. the upgrade preview |
| `GameLoadOptionScreenAssets` | 0x80018C0C | phase 7, asset 9 = OPTION.BIN |
| `GameLoadRoundAssets` | 0x80018DF8 | phase 8, the `GP*.TMS` round screen then asset 0x56 = VOICE.BIN; its requester also rolls a random class when `g_GrandPrixMode == 0` |
| `GameLoadRaceAssets` | 0x8001901C | phase 9, the full race load: VOICE bank upload, the player's `CAR_xx.2ND`, the course `.1ST` (`0x57 + …`) and `.2ND` (`0x58 + …`) |
| `GameInstallCourseAssets` | 0x80019730 | unpacks the resident `.1ST` pack out of `g_AssetBase` — the same seven calls `GameLoadRaceAssets` makes in its step 5 |
| `GameLoadTrackDataAssets` | 0x8001989C | phase 12, the `.2ND` pack, handing sub-blocks 0..10 to the camera table, physics, AI, model banks and scenery — the same eleven calls as `GameLoadRaceAssets` step 6 |
| `GameRelocateCarModel` | 0x80018F08 | copies the live `g_CarModelAsset` into `g_AssetBase`, un-relocates it with `GameUnrelocateModelBank`, then re-registers the bank at its new address |

The `GameRequest*` twins (0x80018078, 0x800182D0, 0x8001839C, 0x80018410,
0x80018530, 0x8001882C, 0x800189E4, 0x80018B98, 0x80018C88, 0x80018FC4,
0x80019844) are all the same shape: return 1 while `g_AssetLoadState != 0`,
otherwise latch `g_MainState` and return 0 once the phase has been consumed.
`GameRequestSelectBgmAssetsNoReset` (0x8001839C) differs from
`GameRequestSelectBgmAssets` (0x80018410) only in starting the phase at
`g_AssetLoadState = 2`, which skips the `GameCloseLoadedAudioSlots` step.
`GameLoadCarModelNow` / `GameLoadUpgradedCarModelNow` request and then pump
`GameServiceAssetLoad` until it goes idle.

Registration helpers in the same TU: `GameUnrelocateModelBank` (0x800179B4) is
the exact arithmetic inverse of `GameRegisterModelBank`;
`GameRegisterCourseModels` (0x80017A6C) fills the already-named
`g_CourseModelCount`; `GameSetCarImageSlot` / `GameUploadCarImage` (0x80017B44 /
0x80017B5C) and `GameSetCarModelSlot` / `GameSelectCarModelSlot` (0x80017B94 /
0x80017BAC) are two tiny registries — the second pair is what `game/asset.h`
already described as "the entry selected by func_80017BAC".

### 13b. FMV playback (`0x8001E6B4`–`0x8001F018`)

One of three wrappers starts a stream, and the caller context names each:
`GameBeginIntroFmv` (0x80019AF0, stream 0, return scene 3, reached from the
title/attract path), `GameBeginClassFmv` (0x80019B3C, stream `1 + class` in the
first series and `5 + class` in the advanced one, return scene 7, called when a
class is cleared) and `GameBeginEndingFmv` (0x80019BB8, stream 10, return scene
0x21, called only on the `g_SeriesCleared` branch).

`GameBeginFmv` (0x8001E6B4) stops the sound, resets CD audio, sets
`g_SceneId = 5` and stores the return scene in `g_StreamReturnScene`.
`GameUpdateFmv` (0x8001E71C) then walks `D_8009F094`: 0 →
`GameStartFmvPlayback` (0x8001E79C, display setup + `GameSetupFmvBuffers`
0x8001EB14 + `GameInitFmvContext` 0x8001EA7C + `GameOpenFmvStream` 0x8001EB5C),
1 → `GameDecodeFmvFrame` (0x8001E8A4), 2 → `GameEndFmv` (0x8001EA34).
`GameDecodeFmvFrame` is also the abort path: `g_PadEdge2 & 0x800` (Start) jumps
straight to state 2. `GameWaitFmvDecode` (0x8001EF54) is named by its own
timeout message `time out in decoding !`; `GameUploadFmvSlice` (0x8001EBC8) is
the DMA1 callback that `LoadImage`s one decoded strip; `GamePresentFmvFrame` /
`GameGetFmvFrame` (0x8001ED3C / 0x8001EDC4) dequeue a ring frame and resize the
display when the stream's frame size changes; `GameStartStreamRead`
(0x8001F018) is the `CdlSetmode 0x80` + `CdlReadS` retry loop.

### 13c. `cdread.c` is linked below 0x80063200

`0x80027238`–`0x8002785C` is not game code at all: it is Sony's `libcd`
`cdread.c`, identified by its three surviving messages — `CdRead: sector error`,
`CdRead: Shell open...` and `CdRead: retry...` — and confirmed by the shapes:

| Name | Addr | Evidence |
|---|---|---|
| `CdRead` | 0x80027688 | `(sectors, buf, mode)`; stores the mode, derives 0x200 / 0x249 / 0x246 words from `mode & 0x30`, ORs in 0x20, saves the sync/ready callbacks, `CdControlB(CdlPause)` if the drive is busy, then `CdReadRetry(0)` |
| `CdReadSync` | 0x80027790 | `(mode, result)`; polls the remaining-sector count with a 0x4B0-vblank watchdog, then `CdReady(1, result)` |
| `CdReadBreak` | 0x80027634 | zeroes the remaining count, restores both saved callbacks, `CdlPause` |
| `CdReadCallback` | 0x8002785C | stores a callback, returns the previous one |
| `CdReadDataReadyCallback` | 0x80027238 | cdread.c's static `data_ready_callback`: `(intr, result)`, drains one sector with `CdGetSector2`, prints `CdRead: sector error` on a position mismatch |
| `CdReadRetry` | 0x8002745C | cdread.c's static `read_retry`: prints `CdRead: Shell open...` / `CdRead: retry...`, re-issues `CdlSetmode` and the read |

This deliberately breaks the "everything below 0x80063200 gets a `Game*` name"
rule, because inventing a `Game*` name for a verbatim SDK module would be the
wrong name, not a conservative one. The two statics keep descriptive `CdRead*`
names rather than libcd's lowercase static names, since the project already uses
`data_ready_callback` for libds's unrelated static at 0x8006CE78.

### 13d. Further SDK identifications from the FMV path

`func_8006DF94` (currently declared `ResetCallback` in `psyq/kernel.h`) is
really `DMACallback`: it is a kernel-table thunk, and its three wrappers pass
0, 1 and 3 — MDECin, MDECout and CD-ROM. That settles several names at once:

| Name | Addr | Evidence |
|---|---|---|
| `DecDCTout` | 0x8006402C | tail-calls the already-named `MDEC_out`; called as `(buf, w * h / 2)` once per frame |
| `DecDCTinSync` | 0x8006404C | tail-calls `MDEC_in_sync` |
| `DecDCToutSync` | 0x8006406C | tail-calls `MDEC_out_sync` |
| `DecDCTinCallback` | 0x8006408C | `DMACallback(0, f)` — DMA0 is MDECin |
| `DecDCToutCallback` | 0x800640B0 | `DMACallback(1, f)`; installed with `GameUploadFmvSlice` and cleared by `GameEndFmv` |
| `CdDataCallback` | 0x8006A994 | `DMACallback(3, f)` — DMA3 is the CD-ROM; installed with libds's `data_ready_callback` |
| `CdMix` | 0x8006A94C | wraps `CD_vol` and returns 1 |
| `CdGetSector2` | 0x8006A970 | wraps `CD_getsector2`; used by `CdReadDataReadyCallback` |
| `StGetNext` | 0x8006D0EC | two out-parameters, returns 0 after marking the ring entry state 4 — the classic `StGetNext(addr, header)` |
| `StUnSetRing` | 0x8006CE20 | `EnterCriticalSection` → `CdDataCallback(0)` / `CdReadyCallback(0)` → clear both kernel callback slots → `ExitCriticalSection`, at stream teardown |

**`ResetCallback` at func_8006DF94 was not renamed in this pass** — it is an
existing name with existing call sites, and correcting it belongs in its own
change. **Done in section 17b**: func_8006DF94 is `DMACallback`, and the name
`ResetCallback` now belongs to func_8006DF34.

### 13e. CD-DA pump (`0x8004310C`–`0x80043974`)

`game/cd.h` already documented the request words D_8007F600..D_8007F60C; this
pass names the machine that drains them. `GameTickCdAudio` (0x80043974) runs
once per frame and issues at most one `CdControl`: a pending track goes to
`GameStepCdTrackRequest` (0x800432A8, `CdlSeekP 0x16`), otherwise
`g_CdCommandPending` 1/2/3 selects `GameStepCdPlayRequest` (0x80043494,
`CdlPlay`), `GameStepCdPauseRequest` (0x80043598, `CdlGetlocP` then `CdlPause`)
or `GameStepCdResumeRequest` (0x800437B8, `CdlPlay`). The play and resume steps
are byte-identical duplicates; they are told apart by which pending value each
serves — `GameStartCdAudio` posts 1 and `GameResumeCdAudio`'s
crossed-a-track-boundary branch posts 3. `GameInitCdAudio` (0x800438BC) is the
one-time setup, `GameBuildCdTrackTable` (0x800431BC) fills D_8009AFD4 from
`CdGetToc` (each audio track pushed 0x3C sectors in) plus `DsSearchFile` rows,
and `GameApplyCdVolume` / `GameSetCdVolumeSetting` / `GameSetCdMixPreset`
(0x8004310C / 0x80043134 / 0x8004318C) are the three ways the attenuator is
poked.

### 13f. TEAM LOGO canvas transforms (`0x8004B9B8`–`0x8004BF48`)

Eight whole-canvas operations, all dispatched by the already-named
`GameUpdateTeamLogoCanvas` and all operating in place on `g_TeamLogoCanvas`
(64 rows x 8 words x 8 nibbles = 64x64 at 4bpp). Column `c` lives in word
`c >> 3`, nibble `c & 7` — proved by the scrolls, where word `i`'s nibble 7
receives word `i + 1`'s nibble 0.

| Name | Addr | Evidence |
|---|---|---|
| `GameScrollTeamLogoUp` | 0x8004B9B8 | saves row 0, copies row `r + 1` down over row `r`, writes the saved row into row 63 |
| `GameScrollTeamLogoDown` | 0x8004BA50 | the mirror image of the above |
| `GameScrollTeamLogoLeft` | 0x8004BAE4 | `word >>= 4` with the next word's low nibble shifted into bit 28; the leftmost column wraps to the right |
| `GameScrollTeamLogoRight` | 0x8004BBA8 | `word <<= 4` with the previous word's high nibble |
| `GameFlipTeamLogoVertical` | 0x8004BC68 | swaps row `i` with row `63 - i` for `i` in 0..31 |
| `GameFlipTeamLogoHorizontal` | 0x8004BCE4 | reverses the nibble order inside each word and swaps word `w` with word `7 - w` |
| `GameRotateTeamLogoCcw` | 0x8004BDB4 | works out to `dst(y, x) = src(x, 63 - y)`; `func_8004BDEC` is its second entry point, past the sound cue |
| `GameRotateTeamLogoCw` | 0x8004BF48 | works out to `dst(y, x) = src(63 - x, y)` |

The four scrolls play sound cue 1, the four flips/rotations cue 8 — a second,
independent split along the same line.

### 13g. Boot defaults and audio settings

`GameInitSaveDefaults` (0x80021338, called once from `GameInitSubsystems`) fills
every memory-card-backed global with new-game values: the three car tables from
D_8007BE68, the three `GameRaceProgress` slots via `GameResetProgressSlot`
(0x80021288), both course-progress blocks via `GameResetCourseProgress`
(0x800212F0), `g_MaxClassReached`, the BGM selection and the three audio
settings, which it hands to `GameApplyAudioSettings` (0x80021224).
`GameApplyAudioSettings` is also the tail of `GameLoadSaveStateBlock`, which is
what makes "apply", not "init", the right verb. `GameSetEffectVolumeSetting`
(0x8005BDD4) is the effect-side twin of the already-named
`GameSetSequenceVolumeSetting`.

### 13h. Left generic on purpose

- `func_800271EC`, `func_80026570`, `func_80026AE0` and `func_80026920`, plus
  their handler tables D_8007D778 / D_8007D6B8 / D_8007D6D0, are attract- and
  replay-sequence drivers. The dynamic trace proposed `GameUpdateRaceScene` and
  `GameDrawRaceSceneSubsystems` for the last two, but reading them shows they
  are per-sequence step functions with hard-coded frame thresholds, not a race
  driver and not a subsystem fan-out. Naming them needs each sequence
  identified first.
- `func_80017AD0` registers a 0x800 + 0x1000 + table triple out of the `.2ND`
  pack; what the first two blocks are is not established.
- `func_8004B8B4` clamps two globals (0x40..0x100 and 0..0x100) and derives
  D_8007F948 from the second; it has one caller and nothing else reads them.
- `func_8004B5C8`, `func_8004B6CC`, `func_8004B764`, `func_8004B7F8`,
  `func_8004CBE4`, `func_8004CC14` and `func_8004CC44` draw UI widgets but are
  reached only through tables, so which panel each belongs to is unproven.
- `func_8004CED0` / `func_8004CF00` both `LoadImage` into `g_TeamLogoClutRect`,
  one from a ROM table and one from `g_TeamLogoClut`; which is "default" and
  which is "current" is a guess until their two callers are named.
- `func_80027874` draws a proportional string of 0x18-tall glyphs into the
  scratchpad primitive buffer. It is clearly a third text renderer alongside
  `GameDrawProportionalText` and `GameDrawText8x8`, but nothing yet pins which
  font it uses.
- `func_8001E684` is not a function at all: it is a nonmatching label inside
  `func_8001DFC0`'s epilogue, already listed in
  `configs/PAL/nonmatching_labels.main.txt`.

## 14. Subsystem directory pass (`src/main/PAL/main/<subsystem>/`)

Every `PAL/main` unit now lives in a subsystem directory: `boot menu race car
track render audio cd fmv asset save pad gte sdk`. Moving a unit rewrites four
things in lockstep — the `.c` path, the `configs/PAL/main.yaml` name token, the
`INCLUDE_ASM` first argument, and (where one exists) the per-object
`RAGE_CC1_VERSION_OBJ` rule in the `Makefile`, whose target is spelled as an
object path. Dropping that last one silently changes which compiler the unit
gets and can change the ROM, so it is not optional. (Two such rules survived a
rename as dead lines pointing at `func_8006DD30.c` / `func_8006E390.c`; they
have since been removed.)

### 14a. The scene-handler table `D_8007C268`

The 34-entry word table at `0x8007C268` is indexed by `g_SceneId`
(`D_801E42E4`) and holds the per-frame handler of every scene. Dumping it is
what settles most of the `0x8001F330`–`0x80027000` cluster, because those units
are scene handlers with no other distinguishing globals:

| id | handler | what it is |
|---:|---|---|
| 1 | func_800232B4 | boot logo sequence, hands over to `GameBeginIntroFmv` |
| 2 / 3 | func_8001AF70 / `GameEnterTitleScreen` | title entry (menu-side / 3D-side) |
| 4 | `GameUpdateFrontend` | title + main menu |
| 5 | `GameUpdateFmv` | FMV playback |
| 6 | `GameInitMenuMode` | enter the fourteen-screen menu mode |
| 7 | func_80022EE4 | return scene of `GameBeginClassFmv` |
| 8 | func_8005ACA0 | menu-mode per-frame dispatcher |
| 9 / 10 | func_8001C7BC / func_8001CFB4 | ROUND screen: prize money, best times, BGM selector |
| 11 / 12 | func_8003609C / func_800363D4 | the race |
| 13..16 | func_800215FC / func_80021748 / func_80021920 / func_80021964 | LOST RACE → TRY AGAIN / END RACE prompt, then the continue countdown |
| 17..20 | func_8001FD3C / func_80020C24 / func_80020DDC / func_80022748 | RESULT, prize money / promotion, OPTION, record entry |
| 21 | func_80022794 | record name entry |
| 22 | func_800235D8 | enter the attract 3D scene |
| 23 | func_80025870 | attract 3D scene |
| 24..26 | func_800613B8 / func_80061458 / func_80061520 | SAVE & LOAD |
| 27..32 | func_80025A14 … func_800271EC | attract / replay sequence steps |
| 33 | func_80022F2C | return scene of `GameBeginEndingFmv` |
| 34 | func_80022FAC | the post-ending still, fades back to the title |

`g_GameMode` (`D_8019CB14`) is a *second*, independent dispatcher over
`g_GameModeHandlers` (`D_8007D67C`, 12 entries). Reading its rows shows it is
the setup menu: 0 is the fade transition, 1 the six-row root menu, 2/3/4 its
sub-panels, 5 the sound settings (BGM / SFX / mono), 6 the screen-position
adjust, and 7..11 the already-named controller-configuration screens. That is
what files `func_80023A60`, `func_80023BB4` and `func_800250BC` under `menu/`.

### 14b. Named in this pass

Single-function units whose in-file documentation already pinned the behaviour;
each got the name, the file and the config token together, with the symbol kept
behind an `asm()` alias.

| Old | New | Dir |
|---|---|---|
| func_8002A788 | `GameGetAngleDistance` (file only; both functions were already named) | car |
| func_8002A6B0 | `GameInstallTrackPoints` | track |
| func_8002C168 | `GameAccumulateLapProgress` | car |
| func_8002F4E4 | `GameAdvanceCarPosition` | car |
| func_8002FAE8 | `GameBlendAngle` | track |
| func_8002FB60 | `GameInterpolateTrackAngle` | track |
| func_8002FC84 | `GameInterpolateTrackPoint` | track |
| func_8002FD9C | `GameSmoothTrackAngle` | track |
| func_8003A148 | `GameClampCarLateralOffset` | car |
| func_80031E98 | `GameSampleTrackSurfaceHeight` | car |
| func_8003AE2C | `GameUpdateRacePosition` | race |

### 14c. Units whose directory is a judgement call

These straddle two subsystems; each was filed by which functions dominate the
unit by word count, and the losing half is recorded here so the choice can be
revisited.

- `func_8001DAB0` → `render/`: the render-object submitter and `GameDrawCar`
  (769 words) against `GameBeginFmv` / `GameUpdateFmv` (58 words).
- `func_8002317C` → `boot/`: the boot logo scene against the attract-scene
  entry `func_800235D8`.
- `func_80020DDC` → `menu/`: the OPTION screen against `GameInitSaveDefaults`
  and its `GameReset*` helpers, which are boot-time.
- `func_80041840` → `track/`: `GameDrawSkyBackground` (1211 words) against the
  five one-line CD-DA request wrappers at the tail (67 words), which belong to
  `cd/`.
- `func_800271EC` → `sdk/`: scene 32's handler (19 words) against libcd
  `cdread.c`'s three statics (276 words).
- `func_800333DC` → `race/`: the tachometer HUD against `func_80033AA0`, the
  fade overlay every screen in the game calls.
- `func_80043B18` / `func_80043BCC` (`GameUpdateCamera`) → `track/`, on the
  strength of "track cameras" being the only camera entry in the taxonomy;
  `render/` is equally arguable.
- The libgpu internals `func_80065738`–`func_80067F38` → `sdk/`. The placed
  libgpu units are already split between `render/` (`DrawOTag`, `GetPrimAddr`,
  `SetDrawTPage`, `GetDrawEnv`) and `sdk/` (`LoadImage`, `GetDispEnv`,
  `Gpu_WriteGp1`, `DumpClut`, `Gpu_LoadTexImageAndGetTPage`), so there is no
  consistent precedent to follow. libgte went to `render/` instead, where all
  five already-placed libgte units (`SetRotMatrix`, `SetFogNear`,
  `MatrixApply*`) live.
- `func_8005ECE0` → `save/` on its own `"bu%1d%1d:"` device strings and
  `BiosFormatDevice` call, against an adjacency that says `cd/`. Note that its
  two neighbours `cd/GameCdReadStatusPair` and `cd/GameClearCdResultEvents` are
  the same family and one of them includes `game/memcard.h`; the three probably
  belong together, in `save/`.

---

## 15. Gameplay core pass (`race/`, `car/`, `track/`)

The three gameplay directories were the least-named part of the tree: 50 of
their 68 units were still called `func_XXXXXXXX.c`. All 68 now carry a name, and
about 145 functions plus 145 globals inside them were named. Everything is an
`asm()` alias, so `make check VERSION=PAL` stayed at
`build/PAL/main.exe: OK`, sha1 `2913e15648eddef40821c5f666460abc04155ee6`,
verified after every batch.

Names that leak into `render/`, `menu/`, `asset/`, `cd/`, `fmv/`, `sdk/` or
`boot/` were applied **only inside the three gameplay directories**; those files
keep the raw `D_` / `func_` spelling and still link, because the emitted symbol
never changed. The same applies to the four inline-`asm` `%hi`/`%lo` references
in `car/GameBlendPaintColor.c`, which must keep the raw `D_8019CB38` /
`D_8019CB3A` spelling (see 12c).

### 15a. The wrong-way warning — one chain, four names

The strongest single result of this pass, because it explains a global that
reaches three directories and a HUD element nobody had placed.

`GameCarRuntime + 0xB8` was called `routeRow`. It is a direction flag:

- `GameBuildStartingGrid` seeds it to `g_RaceSeries` for all 11 cars;
- for the player it is **rewritten every frame** — `sh v0, 184(s1)` at
  `0x8002DF38` inside `GameUpdatePlayerCar` stores the return of
  `GameIsCarFacingBackwards` (`func_8002CD08`), which compares the car's
  `headingAngle` against `0xC00 - trackPoint->angle` and returns whether the
  delta lands in `0x401..0x7FF`, i.e. facing backwards;
- `GameSteerCarAlongRoute` uses it as `flag << 11`, a clean 0/0x800 = 0°/180°
  yaw flip;
- the advanced series runs the courses backwards, so `g_RaceSeries` **is** the
  expected direction.

Hence `field_B8` is now `facingBackwards`, `D_8009E78C` (the player's copy) is
`g_PlayerFacingBackwards`, and the test `g_PlayerFacingBackwards != g_RaceSeries`
that guards `func_800333DC` is the wrong-way condition. `D_801E8A8C`, which
counts those frames, is `g_WrongWayTimer`: past 10 frames the banner shows and a
cue repeats, `GameUpdateRivalCueGate` mutes the rival cues, and in Time Attack 60
frames on lap 0 forces `g_RacePhase = 5` and ends the run. `func_800333DC` is
therefore `GameDrawWrongWayWarning` — three sprites over a 0x78x0x20 backing at
screen centre.

### 15b. Race timing and the split HUD (`game/race.h`)

`GameUpdateSplitTimes` (`func_800352B8`) and its drawing-only twin
`GameDrawSplitTimes` (`func_800357BC`) are a three-sector split system. Named in
`race.h`, all from `GameEnterRaceScene`'s seeding and the two consumers:

| address | name | what settles it |
|---|---|---|
| `D_801E4D64` | `g_LapTimeMs` | zeroed at race init, drawn as `m'ss"fff` |
| `D_801E4BCC` | `g_BestLapThisRace` | seeded from `g_BestLapTimes[series][course][mode]` |
| `D_8009AF8C` | `g_RefLapTime` | the same seed; the lap-line delta is measured against it |
| `D_801E4148` | `g_SectorIndex` | `-2` before the first crossing, then `0..2` modulo 3 |
| `D_801E4D98/9C/A0` | `g_SectorEndDistance[3]` | written `L/3`, `2L/3`, `L` at race init |
| `D_8009AF80/84/88` | `g_SectorTimes[3]` | filled as each boundary is crossed |
| `D_8009AF90/94/98` | `g_RefSectorTimes[3]` | loaded from `g_BestSectorTimes` and written back at the finish |
| `D_8009AF78` | `g_LastSectorTime` | `g_SectorTimes[justCompleted]` |
| `D_8009AF7C` / `D_8009AFAC` | `g_SplitDelta` / `g_SplitSign` | `|ref - now|` and `+1` ahead / `-1` behind (cue 0x3E vs 0x3F, tile 0x7810 vs 0x780F) |
| `D_8009AFA4/A8/B0` | `g_SplitSector` / `g_SplitTimer` / `g_SplitTargetTime` | the sector shown, the `0..0x3C` display window, the reference drawn |
| `D_801E4BA8` | `g_RaceTotalTime` | summed from the per-lap array, saturated at `0x927BF`, compared against `g_BestTotalTimes` |
| `D_8009AF9C` | `g_RaceTimeRemaining` | `15000` at init, `--` while `g_RacePhase >= 2 && g_GrandPrixMode`; `<= 0` ends the race |

`0x927BE` (599998 ms = 9'59"998) is the display cap everywhere; `0x927BF` is the
saturation value the HUD prints as dashes. `D_8009AFA0` (`g_LapTimeSaturated`) is
set when a lap time saturates and is **write-only in retail** — three stores, no
loads anywhere in the image.

Two arrays keep per-file spellings because their elements also carry split
symbols: `GameUpdateLapAndFinish` touches one element at a time and declares
`g_RefSectorTime0/1/2` and `g_SectorEndDistance0/1/2`, while `GameSeedReplayCars`
indexes them and declares `g_RefSectorTimes[]` / `g_SectorEndDistance[]`.

### 15c. Speed, revs and the tachometer

`D_8009E778` is `g_PlayerCar.field_A4`. Three independent uses agree that it is
speed: `GameDrawSpeedDigits` prints `g_PlayerSpeed * 160 / 1168`,
`GameInitRivalCarAi` converts a top speed the other way with `* 1168 / 160`, and
the launch thresholds `g_LaunchSpeedThresholds` are compared against `field_A4`.
So `field_A4` is speed and 1168 internal units = 160 on the readout.

`D_8019CAB4` is `g_EngineRpm`: it is slewed toward `g_PlayerCar.field_134`,
clamped to `[500, g_CarSpec->revLimit]`, handed to the engine-sound driver, and
passed to `GameDrawTachometer` (`func_8003351C`), which turns it into the needle
angle `needleAngleMin + rpm * (needleAngleMax - needleAngleMin) / 10000`.
`D_801E4170` (`g_EngineRpmJitter`) is the idle/redline wobble added to it for
both the sound and the needle. `D_8009E806` is `g_PlayerCar.field_132`, the
current gear, drawn as a single glyph by `GameDrawHudDigit` (`func_80033B7C`,
an 8x8 `SPRT_8` with `u = digit * 8`).

`GameBeginCarStandingStart` (`func_8002BE18`) is the green light: it turns the
revs held at the line into `g_StandingStartSpin` (`D_8019CA04`), which
`GameUpdateCarStandingStart` (`func_80030BC4`, drive `state98 == 3`) then burns
down. That corrects the old "crash / tumble" reading of `func_80030BC4`: the
only write of `state98 = 3` in the whole image is in `GameInitPlayerCar`, so
state 3 is the grid state.

### 15d. Named this pass, by directory

**`track/` (28 units, 18 renamed).** Environment timeline:
`GameSetEnvironmentScript`, `GameSetEnvPaletteTable`, `GameLerpEnvColor`,
`GameLoadEnvironmentCue`, with `g_EnvScriptCues` / `g_EnvScriptLength` /
`g_EnvScriptClock` / `g_EnvScriptEnabled` / `g_EnvFogEnabled` /
`g_EnvLerpFrame` / `g_EnvLerpDuration`. World: `GameSetCourseObjects`,
`g_CourseObjects` / `g_CourseObjectCount`, `GameBuildVisibleCells`,
`GameGetCellRegion`, `GameIsCellVisibleFromRegion`, `g_TerrainCellGrid`,
`g_CellVisibilityTable`, `GameDrawTerrainCells` / `…Wide`. Cameras:
`GameFindNearestTrackCamera`, `g_TrackCameras`, `g_TrackSectionCount`
(`= (g_TrackLength >> 8) + 1`, which is also what proves `car + 0x78` is
`progress >> 8`), `GameUpdateFinishCamera`, `GameSeedFinishCamera`,
`GameUpdateFreeLookCamera`. Scripted scenery: `GameDrawFlybyScenery`,
`GameSeedFlybyScenery`, `GameUpdateRouteScenery`, `GameDrawRouteScenery`,
`GameSeedRouteScenery`, `GameInitPathScenery`, `GameDrawPathScenery`,
`GameDrawScriptedScenery`, `GameDrawStartGridScenery`, plus their data and
runtime blocks. Cues: `GamePlayCountdownCues` (cues 0x1E-0x21 land one frame
after each of `GameDrawStartCountdown`'s "3"/"2"/"1"/"GO" phases at t = 120 /
150 / 180 / 210 — that timing is what identifies it), `GameTriggerRaceCues`,
`GameUpdateRivalCueGate`, `g_RaceCueFlags`, `g_RaceCueDelay`.

**`car/` (20 units, 13 renamed).** Paint: `GameBlendPaintColor` and its
thirds/quarters siblings, `GameApplyBodyColor1/2`, `GameSetBodyColor1/2`,
`g_BodyColorPrimary` / `g_BodyColorSecondary`. Drivetrain:
`GameUpdateCarDrivetrain`, `GameUpdateCarDriving` (state 0),
`GameUpdateCarAirborne` (state 2), `GameUpdateCarStandingStart` (state 3),
`GameSteerCarToTrackLine`, `g_GearTorqueCurve`, `g_ShiftTargetRpm`,
`g_AutoShiftCooldown`, `g_SteerHoldFrames`, `g_LaunchSpeedThresholds`.
Track state: `GameUpdateCarTrackState` (the clamping twin of the already-named
`GameResetCarTrackState`), `GameSeedCarLapProgress`,
`GameGetTrackSurfaceHeight`. Grid and AI: `GameBuildStartingGrid`,
`GameInitRivalCar`, `GameInitRivalCarAi`, `GameSeedCarRouteMarkers`,
`GameUpdateCarAiTargetSpeed`, `GameApplyCarRacingLineHint`,
`GameUpdateCarTrafficAvoidance`, `GameRankContenders`,
`GameUpdateRivalRubberBand`, `GameSlowRivalAhead`. Body motion:
`GameSetCarKnockback`, `GameApplyCarKnockback`, `GameStartCarBodyKick`,
`GameUpdateCarBodyKick`, `GameUpdateCarCrestHop`, `GameGetCarCrestTrigger`,
`GameClearCarMotionState`, `GameUpdateCarBodyRoll`, `GameIsPointInQuad`.

**`race/` (20 units, 19 renamed).** Replay: `GameApplyReplayFrame`,
`GameRecordReplayFrame`, `GameBeginReplay`, `GameUpdateReplayScene`,
`GameSeedReplayCars`, `GameUpdateReplayCars`, and the ring globals
`g_ReplayWriteCursor` / `g_ReplayReadCursor` / `g_ReplayFrameCount` /
`g_ReplayBufferWrapped` / `g_ReplayFramesGp` / `g_ReplayFramesTimeAttack`.
Results and progression: `GameDrawGrandPrixResultPanel`, `GameDrawRaceTimePanel`,
`GameDrawPrizeMoneyPanel`, `GameCommitClassProgress`,
`GameAdvanceGrandPrixClass`, `GameEnterPrizeScreen`, `g_PrizeAmount`,
`g_PromotionBonus`, `g_ClassPromoted`, `g_ClassCompleted`, `g_ClassResultPlace`.
Records: `GameInitRecordTables`, `GameDrawRankingPanel`,
`GameDrawTimeRecordPanel`, `GameInsertRaceRecords`, `GameUpdateRecordEntry`,
`g_NameEntryCharset` (the 42 codes `0-9`, space, `A-Z`, `. - ! ? @`),
`g_NameEntryCursor`, `g_NameEntryChar`, `g_PlaceSuffixNames`, `g_CarNames`,
`g_CarClassNames`. LOST RACE: `GameEnterLostRaceScreen`,
`GameUpdateLostRaceScreen`, `g_LostRaceChoice`, `g_ChanceDigits`. BGM select:
`GameDrawBgmSelectBar`, `GameUpdateBgmSelect`, `GameExitBgmSelect`,
`GameAdvanceBgmShuffleBag`, `g_BgmSelectTrack`, `g_BgmRandomPlay`,
`g_CdTrackEnded`. Attract and prologue: `GameEnterAttractDemo`,
`GameUpdateAttractDemoRace`, `GameEnterPrologue`, `GameDrawPrologueText`,
`g_PrologueLines` (the `{x, y, text}` table whose strings are the RAGE RACER
opening narration, in order), `g_PrologueCameraCuts`. Race spine:
`GameUpdateLapAndFinish`, `GameEnterRaceScene`, `GameUpdateRaceScene`,
`GameExitRaceScene`, `GameGetTrackZoneBlend`, `g_RacePaused`, `g_PauseDebounce`,
`g_RaceFadeTimer`, `g_RaceOptionCursor`. HUD: `GameDrawTimeValue`,
`GameDrawMinuteSecondTime`, `GameDrawSpeedDigits`, `GameDrawHudDigit`.

### 15e. Judgement calls, and the readings they beat

- **`D_8009E6A0` → `g_RivalCueEnabled`, not `g_CommentaryEnabled`.** Every
  reader is inside `GameUpdateRivalRubberBand`, where all eight cues (0x2D,
  0x2F, 0x30, 0x32-0x34, 0x36, 0x37) are individually wrapped in
  `if (D_8009E6A0 != 0)`, and those cues fire from rival-proximity events. It is
  tri-state (`GameEnterRaceScene` writes 1, `GameUpdateLapAndFinish` writes 2),
  so it is not a boolean "enabled". "Commentary" would assert that the cues are
  speech; the call sites only prove they are about rivals. Same reasoning
  renamed its companion `D_801E6C90` to `g_RaceCueDelay` rather than
  `g_CommentaryDelay` or `g_LapCueTimer`.
- **`D_801E4BA0` → `g_GripLossTimer`.** It is set to 200 by
  `GameBeginCarStandingStart` on a launch in gear >= 2 and by
  `GameCollidePlayerWithCars`, is decremented once a frame, and its only effect
  is halving `car->field_A8` while positive. "Wheelspin timer" fits the launch
  writer but not the collision writer; "speedometer suppress" fits neither
  writer's trigger. The chosen name states only what is proven.
- **`func_80033D50` → `GameDrawTimeValue`, not `GameDrawLapTime`.** It draws the
  running lap time, the last sector time, the reference time and the best total
  time; four of its six call sites are not lap times.
- **`func_8003351C` keeps `GameDrawTachometer`; `func_8002F458` is
  `GameDrawPlayerTachometer`.** The wrapper only picks the dial-lighting mode
  from `g_EnvScriptClock` and forwards `g_EngineRpm + g_EngineRpmJitter`; the
  drawing is all in `func_8003351C`.
- **`D_8007D404` → `g_CourseNames` is an alias of `&g_GrandPrixNames[11]`.** The
  same four pointers are reachable both ways; the second name is kept because
  every user of `D_8007D404` indexes it with `g_CourseIndex`, not with a class
  number.
- **`D_801E774C` → `g_RankingTimes` is `&g_RankingRecords[0][0][0].v8`**, the
  `S22` record's time field, indexed with stride 20 words. It is a split symbol
  of `D_801E7744`, in the pattern section 3 documents for `g_Cars`.

### 15f. Dead code found while naming

Verified against a full `mips-linux-gnu-objdump -d build/PAL/main.elf`, checking
both the symbol and the encoded `jal` word, and against the scene table
`D_8007C268`:

- **`race/GameUpdateWaypointRaceScene.c` and the waypoint half of
  `race/GameIsCarNearWaypoint.c` are an unreachable game mode.**
  `func_80037200` and `func_80037D90` have zero references and are not in the
  scene table, and with them go `GameSeedWaypoints`, `GameUpdateWaypoints`,
  `GameDrawWaypoints`, `GameCountActiveWaypoints`, `GameDrawLapNumber`,
  `GameApplyTrackReverbZone` and `func_80038288`. `GameUpdateFreeLookCamera`
  (`func_8003CF14`) and `g_FreeCameraAngleOffset` are reachable only from it.
  `func_80037D90` draws `"CONGRATULATIONS!!"` and counts to 257 laps, so the cut
  mode was some kind of collect-the-waypoints event.
- **`GameSeedFinishCameraAlt` (`func_8003CDF4`)** is
  instruction-for-instruction `GameSeedFinishCamera` with three constants
  changed, and has zero references.
- **`GameGetReverseTrackAngle` (`func_8002FBEC`)**, **`GameGetTrackSurfaceHeight`
  (`func_80032098`)** and the already-named **`GameSampleTrackSurfaceHeight`
  (`func_80031E98`)** have zero references.
- **`GameInitSoundSystem` (`func_80034E88`)** and **`GameInitEngineSound`
  (`func_80034ED0`)** have zero references; they are named with certainty
  anyway, because their own `GameDebugPrintf` strings are `"sound error\n"`,
  `"init_sound ok\n"` and `"init_engine ok\n"`.

The names are kept: the code is in the ROM and has to be read by someone.

### 15g. Deliberately left unnamed

- **`func_800340D8`** (called once from race init) builds two 12000-byte buffers
  of 512 chained 2x1 `TILE`s on a skewed 16x32 grid at `x = 0xCD - 3*col`,
  `y = 0x5A + 2*row`, colour 0x20, into `D_8019C90C[0..1]`. The geometry is
  certain; nothing in the decompiled tree ever reads `D_8019C90C`, so what the
  strip renders is not established.
- **`func_800218A0`** draws one 0xA0x0x18 sprite at (0x50, 0x6C), uv (0, 0x28),
  from the race-end scenes, the retire path and the post-ending still. The
  caller set does not choose between "GAME OVER", "THE END" and a logo, and the
  texture is on the disc.
- **`func_8004087C`** walks 8-byte rows in `g_TrackEventData + 0x1B7C` keyed by
  the player's track section and pans a continuous sound by the car's `+0x3C`
  value. The other use of `field_3C` treats it as an unsigned 0..0x800 width
  fraction while this one branches on its sign; until that is reconciled, any
  "roadside wall on the left/right" name would be a guess.
- **`func_8003D6E8`** has an empty body. Positionally it is the first of the
  five scenery seeders `GameEnterRaceScene` calls in a row, but there is no
  side effect to name it from.
- **`D_801E4194`, `D_801E8A4C`, `D_8019C998`, `D_801E4D84`, `D_801E4FB4`** are
  each written but never read anywhere in the image (`D_8019C998` is read but
  only ever written zero). Naming them would be inventing a feature.
- **`D_8019C768`** keeps the entry from section 3: still only `0x80` / `0x180`
  writes and one reader that cannot pin the quantity.
- **`GameCarEntry.shapeIndex` / `.textureIndex` are misnamed** — the PAINT COLOR
  screen's two rows write them and hand them straight to `GameSetBodyColor1` /
  `GameSetBodyColor2`, so they are body colour 1 and body colour 2, not
  geometry/texture selectors. The rename is not done here because
  `asset/GameRequestCarSelectAssets.c` and `menu/GameUpdateLogoSampleScreen.c`
  use the current field names and are another pass's territory.
- **Cue numbers.** Cues can be placed by their trigger conditions (0x1E-0x22
  countdown, 0x23 over-speed, 0x26 record, 0x27-0x29 laps to go, 0x2A final lap,
  0x2C wrong way, 0x3E/0x3F split faster/slower), but the audio is on the disc,
  so no cue's *content* is asserted in a name.

## 16. `sdk/`, `render/` and `menu/` naming pass

76 units in those three directories were still called `func_XXXXXXXX`. This pass
named 78 functions and 30 globals and renamed 35 unit files. Every batch was
verified with `make check VERSION=PAL` at
`2913e15648eddef40821c5f666460abc04155ee6`.

### 15a. libgpu: the driver table at `0x800941A0` decides everything

`D_800941E0` points at a 16-slot function table (dumped in
`asm/PAL/main/data/main/6BE64.data.s`). Reading which public entry point uses
which slot pins the whole module, and it corrects two `GpuCallbacks` field
names that were guesses:

| slot | function | how the slot is used |
|---|---|---|
| +0x08 | `Gpu_AddQueue` (func_800676A0) | the `send` entry: runs the worker now if the GPU is idle, else queues it |
| +0x0C | `Gpu_ClearImage` (func_80066E6C) | the worker `ClearImage` enqueues |
| +0x10 | `Gpu_WriteGp1` (already named) | `submit` |
| +0x14 | `Gpu_WriteGp0Words` (already named) | **was `putDispEnv`** — it is the raw GP0 word pusher |
| +0x18 | `Gpu_StartDmaTransfer` (already named) | **was `moveImage`**, but `DrawOTag`, `PutDrawEnv`, `DrawOTagEnv` and `MoveImage` all use it: renamed `sendList` |
| +0x1C / +0x20 | `Gpu_StoreImage` / `Gpu_LoadImage` (func_800672D8 / func_80067084) | the `StoreImage` / `LoadImage` workers |
| +0x24 | `Gpu_ExecuteQueue` (func_80067984) | the queue drain, also installed as the DMA2 callback |
| +0x28 | `Gpu_GetControlMirrorByte` | `read` |
| +0x2C | `Gpu_ClearOTagDma` (func_80066D84) | **was `clearImage`** — it is the OTC-DMA ordering-table clear `ClearOTagR` uses |
| +0x34 | `Gpu_Reset` (func_80067C80) | the body of `ResetGraph` |
| +0x38 | `_status` (already named) | **was `drawSyncStatus`** |
| +0x3C | `Gpu_DrawSync` (func_80067DBC) | the body of `DrawSync` |

The `Gpu_*` prefix is this project's existing convention for libgpu internals
whose Sony static name could not be pinned (`Gpu_WriteGp1`,
`Gpu_BuildDrawAreaTopLeftCmd`, ...), used alongside the ones that could
(`_get_mode`, `_status`, `_param`, `get_dx`). `Gpu_AddQueue` / `Gpu_ExecuteQueue`
are very probably libgpu's `_addque` / `_exeque` — `_addque(func, p, size, tag)`
has exactly this four-argument shape — but that was not provable from the image,
so the descriptive name was used.

### 15b. libgpu public entry points named from their own trace strings

| Name | Addr | String |
|---|---|---|
| `SetGraphQueue` | 0x80065738 | `D_800134F0` = `"SetGrapQue(%d)...\n"` (the SDK's own typo) |
| `ResetGraph` | 0x80065460 | `D_80013478` `"ResetGraph:jtb=%08x,env=%08x\n"` + `D_80013498` `"ResetGraph(%d)...\n"` |
| `PutDispEnv` | 0x800660AC | `D_80013614` = `"PutDispEnv(%08x)...\n"` |
| `PutDrawEnv` | 0x80065ED4 | `D_800135E0` = `"PutDrawEnv(%08x)...\n"` |
| `DrawOTagEnv` | 0x80065F98 | `D_800135F8` = `"DrawOTagEnv(%08x,&08x)...\n"` |

**Correction:** `func_80065E00` was declared `PutDispEnv` in `psyq/gpu.h`. It is
not — the string above proves `PutDispEnv` is func_800660AC. func_80065E00 is
`DrawPrim`: it does `drawSync(0)` and then pushes `prim[3]` words starting at
`prim + 4` through the +0x14 slot, i.e. draws one primitive immediately. It had
no call sites, so the correction is a pure rename.

`GPU_printf` (`D_800941E4`) was already aliased in two files; it is now used in
all six that reference it.

### 15c. libgte: the matrix stack, and Lzc

`func_80069234` / `func_800692D4` are `PushMatrix` / `PopMatrix`: they save and
restore GTE control registers `$0..$7` through a 32-byte-per-level stack at
`0x80094CAC` with the index at `0x80094CA8`, push erroring at index 0x280
(20 levels) and pop erroring at index 0. `func_80069C7C` is `Lzc` — `psyq/gte.h`
already bound the name to that address, the definition just had not taken it.

### 15d. libcd: three more self-naming internals

libcd's internals identify themselves the same way `CD_sync` / `CD_ready` /
`CD_cw` already did in this codebase:

| Name | Addr | Evidence |
|---|---|---|
| `CD_datasync` | 0x8006BF00 | stores `D_8001391C = "CD_datasync"` into the `D_8009BB10` slot its own `"%s:(%s) Sync=%s, Ready=%s\n"` timeout message prints |
| `CD_newmedia` | 0x8006C560 | owns all four `"CD_newmedia: ..."` messages; reads the PVD at sector 16, checks `"CD001"` and parses the path table |
| `CD_cachefile` | 0x8006C8E4 | owns all three `"CD_cachefile: ..."` messages; fills the 64-entry file cache from one directory |
| `cd_read` | 0x8006CB88 | `CD_newmedia`'s own error text is `"Read error in cd_read(PVD)"`; lowercase because it is a static |

**Correction to section 7:** func_8006AB5C is described there as "a libcd
error/trace helper". It is the interrupt decoder — it reads the 8-byte response
FIFO, decodes intr codes 1..5 into `D_80099318` and the two result buffers, and
is drained in a `while (...)` loop by `CD_sync`, `CD_ready`, `CD_cw` and
`CD_getsector2`. Its Sony name is still unknown, so it keeps the raw symbol.

### 15e. `render/`: the scripted camera and two HUD readouts

`GameUpdateScriptedCamera` (func_80046600) plays a table of 0x20-byte
keyframes at `g_CameraPath` (D_8007F628): six `s32` (eye xyz, then look-at xyz),
a duration at +0x18 and a Bezier control value at +0x1C. Each frame it eases the
six values with `GameBezierEase` (func_80046598 — literally
`2t(1-t)*control + t^2` over 0..10000) and installs the result with
`GameSetLookAtMatrix` (func_80046248), which builds the view matrix from an eye
and a target and hands it to `SetRotMatrix` / `SetTransMatrix`.
`GameBuildAxisRotMatrix` (func_80046188) is its single-axis rotation helper —
its `axis` argument is an **ASCII letter**, `(axis & 0xFF) - 0x58` giving 0/1/2
for `'X'/'Y'/'Z'` and 0x20/0x21/0x22 for `'x'/'y'/'z'`.

`GameDrawLapTimes` (func_80033090) and `GameDrawRacePosition` (func_80033230)
are the two race HUD readouts that live in `render/func_80032E9C.c`.

### 15f. `menu/`: the setup menu is scene 23, not the attract scene

Section 14a's scene table has two wrong rows. `func_80025870` (id 23) dispatches
`g_GameModeHandlers[g_GameMode]` and then the setup-scene overlay — it is
`GameUpdateOptionScene`, the OPTION / setup menu, not "attract 3D scene".
Correspondingly id 19 (`func_80020DDC`) is not OPTION either: it is
`GameUpdatePrizeMoneyScreen`, the nine-state machine that counts
`g_PendingPrizeMoney` and then `g_PendingClassBonus` into the save block at
`g_PrizeTickRate` / `g_BonusTickRate` per frame (x4 while confirm is held).

That settles the whole `g_GameModeHandlers` block:

| mode | handler | screen |
|---:|---|---|
| 0 | `GameUpdateOptionMenuFade` | the fade in and out of the setup menu |
| 1 | `GameUpdateOptionRootMenu` | the six-row root menu (`GameDrawOptionRootMenu`) |
| 2 / 3 | `GameUpdateClassRecordMenu` / `GameUpdateClassRecordBrowse` | the class-record list and its eleven-cell grid |
| 4 / 5 | `GameUpdateSoundOptionMenu` / `GameUpdateSoundSettingAdjust` | the SOUND panel and the left/right editor behind it |
| 6 | `GameUpdateScreenAdjustScreen` | screen position, committing to `g_ScreenOffsetX/Y` |
| 7..11 | the already-named controller-configuration screens | |

`GameDrawFullscreenFadeTile480` (func_80023A60) is the 0x140x0x1E0 twin of the
existing `GameDrawFullscreenFadeTile` (func_80033AA0, 0x140x0xF0) — the setup
scene runs in 480 lines.

Also named in `menu/`, each from its own callers or literals:
`GameEnterFrontend` (scene 2), `GameUpdateMenuMode` (scene 8),
`GameUpdateEndingStill` / `GameDrawEndingStill` (scene 34),
`GameEnterRoundScreen` / `GameUpdateRoundScreen` / `GameDrawRoundScreen` /
`GameDrawBgmSelector` (scenes 9 and 10), `GameInitTrackLighting` and its
menu-mode twin `GameInitMenuLighting`, `GameApplyZoneLighting` /
`GameRestoreColorMatrix`, `GameShuffleBgmOrder`, `GameDrawNowLoadingText`
(the literal `"NOW LOADING"` at D_80011B60), `GameEnterCourseSelectScreen`
(`g_MenuScreenUpdate[0]`), `GameCanSelectPrevCourse` (the exact mirror of the
already-named `GameCanSelectNextCourse`), `GameGetOwnedCarAssetIndex`
(`GameGetCarAssetIndex(model, owned grade)` written out longhand),
`GameSwapCarModelSlot` / `GameInstallCarModelSlot`, `GameDrawMenuCourseView`,
`GameDrawTeamNameCharModel`, `GameDrawTireCompoundSlider`,
`GameDrawBrowseArrows`, `GameDrawPaintColorPalette`, `GameDrawCarNamePlate`,
`GameComposeSampleTeamLogo`, `GameComputeClassGrade`, `GameCountOwnedCars`,
`GameDrawMenuCursorArrow`, `GameDrawOptionHintBar`, `GameDrawPadTypeHint`,
`GameDrawVolumeBar`, `GameStartOptionMenuExit`, `GameDrawTitleFadeOverlay`.

Globals named with them: `g_OptionMenuExitScene`, `g_OptionMenuCursor`,
`g_ClassRecordMenuCursor`, `g_SoundOptionCursor`, `g_ScreenOffsetX/Y`,
`g_OptionLetterboxHeight`, `g_LastValidPadType`, `g_BgmTrackNames`,
`g_RoundScreenFadeDelays`, `g_CarPriceTable`, `g_PaintColorTable`,
`g_MenuCourseModelIndex`, `g_MenuPendingCourseIndex`, `g_TeamNameCharModel`,
`g_MenuViewSpin`, `g_PrizeScreenState`, `g_PendingPrizeMoney`,
`g_PendingClassBonus`, `g_PrizeTickRate`, `g_BonusTickRate`,
`g_TeamLogoSampleData`, `g_BrowseArrowsFade`, `g_CarNamePlateFade`,
`g_CarNamePlateStep`, plus the camera-path four above and `GPU_printf`.

### 15g. Left unnamed on purpose

> **Superseded in several places by section 17**, which reopened these
> decisions with two sources this pass did not have (Sony's Run-Time Library
> Reference, and the `Ss*`/`Spu*` naming precedent already set in
> `src/main/PAL/lib/`). Read 17a before relying on any entry below.

- **Every libspu / libsnd internal in `sdk/`.** func_8006ECDC (the shared SEQ
  header parser behind `SsSeqOpen`), func_8006F5F4 (the MIDI Control Change
  dispatcher — controller numbers 0/6/7/10/11/64/65/91/98..101/121 all check
  out), func_800706AC (the NRPN parameter applicator, sibling of func_8007010C),
  func_80074348 (the noise-tone key-on, selected when `vag == 0xFF`),
  func_80074B68, func_80074D1C / func_8007521C (auto-volume / auto-pan ramp
  setup) and func_80077A88 (matching key-off) are all library-internal statics.
  Section 1 already set the precedent with func_8007010C: there is no public
  `Ss*` symbol to claim, and inventing a `Game*` one would be wrong.
  Section 12e's rule keeps their globals raw for the same reason.
- **A discrepancy worth recording, not acting on:** `psyq/snd.h` binds
  `SsUtVibrateOn` to func_800785B4 and `SsUtAutoVol` to func_80078608. Those two
  wrappers arm func_80074D1C and func_8007521C, whose tick partners
  (func_80074ECC, func_800753CC) apply the ramped value as **volume** and as
  **pan** respectively — which reads as the `SsUtAutoVol` / `SsUtAutoPan` pair,
  shifted by one. These are existing names with existing call sites, so
  correcting them belongs in its own change.
- **libgpu:** func_80067F04 / func_80067F38 (the timeout watchdog) stay generic,
  as section 7 decided; func_8006767C, the +0x04 driver slot, has no caller.
- **libcd/libds:** func_8006C53C (a 12-character filename compare),
  func_8006CD0C (the streaming-read starter — its `St*`/`Ds*` name is not
  provable), func_8006DB74 (a generic DMA-channel starter owning
  `"DMA STATUS ERROR %x\n"`, unreferenced in the image), func_8006E4E4 (a slot
  in the kernel interrupt-module vector at 0x8009A498, also unreferenced).
- **`render/`:** func_80019D24 and its family (section 7 already left
  func_80019EFC generic — what the two VRAM texture-swap states *are* is still
  unproven), func_8001D30C, func_8001A980, func_80032D5C, func_8001DAB0.
- **`menu/`:** the attract/replay step drivers func_80025940, func_80025A14,
  func_80025AC8, func_80025BD8, func_80025C20 (section 13h's reasoning still
  holds), the transition widgets func_80050400, func_800509C4, func_8004F3EC,
  func_8004E368, func_8004F650, func_8004F99C, func_8005026C — each owns a
  private accumulator and is reached only through a table, so which panel it
  belongs to is unproven — and func_800512B4 / func_800520F8, which have no C
  caller at all.

---

## 17. Finishing the unit files (`sdk/`, `menu/`, `render/`, `gte/`, `boot/`, `save/`, `asset/`, `cd/`, `audio/`)

57 unit files were still called `func_XXXXXXXX.c`. **56 now carry a name**; the
one exception is `sdk/func_8006AB5C.c`, and section 17i says why. About 110
functions and 10 globals were named with them, and **eight existing names were
found to be wrong and corrected** (17b). Everything is an `asm()` alias, so
`make check VERSION=PAL` stayed at `build/PAL/main.exe: OK`,
sha1 `2913e15648eddef40821c5f666460abc04155ee6`, verified after every batch.

Two new evidence sources made this pass possible, and both should be reused:

* **Sony's own *Run-Time Library Reference* (LibRef47).** Fetched, converted to
  text and kept at `scratchpad/psyq_libref47.txt`. It gives the exact signature,
  fixed-point format, return value and prose description of every public
  libgpu / libgte / libcd / libspu / libsnd / libapi entry point. Grepping a
  candidate name and comparing its Syntax block against the disassembly turned
  several long-standing "PROBABLE" readings into proofs and *refuted* three
  existing names outright. It documents the public API only — no `SpuVm*`, no
  `_Ss*`, no libgpu statics — so it can confirm a name but never invent one.
* **The `src/main/PAL/lib/libsnd/` and `lib/libspu/` trees.** They already carry
  invented-but-descriptive `Ss*` / `Spu*` / `_Ss*` / `_spu_*` names for library
  internals (`SsSeqApplyControlChange`, `SsSeqSetChannelParam13`,
  `SpuVmCalculateCurrentPitch`, `_SsInitTables`). Section 15g had declined to
  name the libsnd internals in `sdk/` because "there is no public `Ss*` symbol
  to claim" — but the codebase had already settled that question in the other
  direction. That precedent is what unblocks 17c.

### 17a. Reopened decisions from section 15g, and how they came out

| symbol | 15g said | now |
|---|---|---|
| `func_8006CD0C` | "the streaming-read starter — its `St*`/`Ds*` name is not provable" | **`CdRead2`**, a *public* libcd symbol. See 17e. |
| `func_8006DB74` | "unreferenced in the image" | **Wrong** — three `jal` sites, all DMA channel 3. Named `CD_dmastart`. |
| `func_8006E4E4` | "a slot in the kernel interrupt-module vector … also unreferenced" | **Wrong** — it is slot +0x10 of libapi `intr.c`'s module descriptor and is invoked by the public `StopCallback()` thunk. Named `StopKernelInterrupts`. |
| the libsnd internals | "left unnamed on purpose" | all named, see 17d |
| the `menu/` transition widgets | "reached only through a table, so which panel it belongs to is unproven" | **Wrong on the premise** — none of them is in `g_MenuScreenUpdate`, `g_MenuScreenDraw` or any other data table. Each is `jal`-ed from exactly one screen handler, and four of them are now named with certainty. See 17g. |
| `func_80067F04` / `func_80067F38` | "the public name is unknown" | still unknown; given descriptive `Gpu_*` names. |
| `func_8006AB5C` | "its Sony name is still unknown" | confirmed, and the reasoning is now stronger. Still raw. |
| the `menu/` attract/replay step drivers | left raw | untouched — no new evidence. |

### 17b. Existing names that were wrong

Eight, in rough order of how much they would have misled a reader.

1. **`SsUtVibrateOn` does not exist.** `psyq/snd.h` bound
   `SsUtVibrateOn` → func_800785B4 and `SsUtAutoVol` → func_80078608. Grepping
   LibRef47 for "Vibrate" returns **zero hits in the entire document** — there
   is no such PSY-Q symbol; the name was invented by an earlier pass. The two
   functions are `SsUtAutoVol` (func_800785B4) and `SsUtAutoPan` (func_80078608).
   Proof beyond the missing symbol: LibRef47 14-90/14-91 give both as
   `short f(short vc, short start, short end, short delta_time)` returning 0/−1,
   and both bodies are exactly `if ((u16)vc < 24) { helper(vc,a,b,c); return 0; }
   return -1;`. Which is which is settled by the tick partners — func_800785B4
   arms the `SpuVoice + 0x1C…0x26` block whose tick handler `SpuVmAutoVolTick`
   feeds the ramped value into the **volume** multiply chain, and func_80078608
   arms `+0x28…0x32` whose handler `SpuVmAutoPanTick` feeds it in as the third
   **pan** factor. Section 15g suspected a one-slot shift; the truth is that one
   of the two names was fictional. Both functions are uncalled, so this was a
   pure rename.
2. **`SsUtKeyOn` / `SsUtKeyOff` were bound to the wrong pair.** They pointed at
   func_80076B30 (6 arguments, `void`) and func_80076C1C (3 arguments). LibRef47
   14-103/14-104 give `short SsUtKeyOn(vabId, prog, tone, note, fine, volL, volR)`
   returning the allocated voice and `short SsUtKeyOff(voice, vabId, prog, tone,
   note)` returning 0/−1. Those are func_800776E4 and func_80077A88, which sit
   back to back with the already-correct `SsUtKeyOnV` (func_80077C7C) and
   `SsUtKeyOffV` (func_80078018) — LibRef's own declaration order, four in a row.
   func_80076B30 / func_80076C1C are thin `SpuVmSeKeyOn/SeKeyOff(0x21, …)`
   wrappers with no caller and no matching public signature; they are back on
   raw names, and `lib/libsnd/SsUtKeyOn.c` should be renamed with them (it was
   left alone here because it is outside this pass's directories).
3. **`SetDefDrawEnv` was the display environment.** func_80064B78 writes a
   0x14-byte DISPENV (disp Rect, screen Rect, `isinter`, `isrgb24`); the 0x1C-byte
   DRAWENV head (clip, ofs, tw, tpage, dtd, dfe, isbg, rgb, with `dfe` derived
   from the buffer height and the DMA interrupt state) is func_80064AA8, which had
   no name at all. Recorded in section 7 as "belongs in its own change"; done now.
   `GameSetupDisplay240` calls them in exactly that order, draw env first.
4. **`ResetCallback` was `DMACallback`.** func_8006DF94 is a kernel jump-table
   thunk whose five call sites pass 0, 1, 2, 3 and 4 — MDECin, MDECout, GPU,
   CD-ROM, SPU. That is `DMACallback(dma, func)`. Recorded in 13d; done now. The
   name `ResetCallback` is now free and belongs to func_8006DF34, which reaches
   slot +0x0C and is called by `ResetGraph` and `DecDCTReset(0)`.
5. **The memory-card event group was labelled "Cd".** `GameOpenCdEventGroup`,
   `GameEnableCdEventGroup`, `GameDisableCdEventGroup`, `GameCloseCdEventGroup`,
   `GameClearCdResultEvents`, `GameClearCdCompleteEvents`, `GameWaitCdResultEvent`,
   `GameWaitCdCompleteEvent`, `GamePollCdResultEventWithTimeout`,
   `GamePollCdResultEventLimit` and the two wrappers `GameStartCdEvents` /
   `GameStopCdEvents` all operate on the eight handles at `D_8009B538`, opened
   with descriptors **`0xF4000001` (HwCARD)** and **`0xF0000011` (SwCARD)** and
   specs `EvSpIOE 0x0004`, `EvSpERROR 0x8000`, `EvSpTIMOUT 0x0100`,
   `EvSpNEW 0x2000`. Nothing in them touches the CD. They are now
   `Game*MemoryCard*Events`, with the "Result"/"Complete" split renamed to the
   accurate `Hw` (`D_8009B538..544`) / `Sw` (`D_8009B548..554`).
6. **`GameSendFormattedCdCommand` formats a memory card.** func_8005EF44
   `sprintf`s `"bu%1d%1d:"` and calls `BiosFormatDevice`; its one call site is
   the branch that then shows "FORMAT DATA OK!". Now `GameFormatMemoryCard`.
7. **`GameGetCarModelIndex` in section 7 was stale.** The source has called
   func_8001785C `GameGetCarUnlockLevel` since the typing pass, and the source is
   right: three of its six call sites compare the result against class progress
   (`g_RaceProgress->maxClassReached`, `g_GrandPrixClass`), and `game/car.h`
   already documents `D_8007C474` as "per-model base of the progress level a
   purchase requires". The section 7 row is corrected.
8. **`SsSeqCalledTbyT` was the wrong function.** It was bound to func_800731CC,
   which is sixteen words of `if (guard != 1) { guard = 1; internalFlush(); guard = 0; }`
   and never touches SEQ data. LibRef47 14-32 defines `SsSeqCalledTbyT` as
   "interprets SEQ/SEP data and carries out playback" — that is func_80071018,
   the sole caller of `SsSeqAdvanceChannelTick` in the whole image. Rebound.
   func_800731CC is back on a raw name: it is very probably the public
   `SsUtFlush`, but `SsUtFlush` is currently bound to func_80075FA4 (the
   *unguarded* internal flush that func_800731CC calls), and untangling that pair
   means renaming an already-named unit file, so it is recorded rather than done.

Two further discrepancies found and **recorded, not acted on**, because they are
in files outside this pass's directories:

* `psyq/kernel.h` binds func_8006DF14 to `ChangeClearRCnt` and func_8006DF24 to
  `ChangeClearInterruptMask`. The stubs decode as **B0(5Bh) = `ChangeClearPad`**
  and **C0(0Ah) = `ChangeClearRCnt`** — one slot apart from the truth.
* `psyq/kernel.h` binds func_8006E644 to `SysEnqIntRP`, but the stub is
  **A0(72h) = `_96_remove`**, and it is called from the `ResetCallback` worker,
  which is where detaching the BIOS CD-ROM driver belongs.

### 17c. `gte/`: the hand-written GTE engine, read opcode table by opcode table

The five `gte/` units are `HANDWRITTEN_ASM` and excluded from the progress
metric, so no earlier pass had read them. They are not mysterious once the four
dispatch tables in `asm/PAL/main/data/main/6BE64.data.s` are dumped: every leaf
is a primitive emitter, and the table a leaf sits in says which primitive and
whether it is depth-cued.

Two constants in the scratchpad render state decide most of it. `GameInitRenderState`
writes `{0x80,0x80,0x80,0x2C}` at `0x1F800070` and `{0xFF,0xFF,0xFF,0x3C}` at
`0x1F800074`; those words are loaded straight into the GTE `RGBC` register
(`lwc2 $6`), so the fourth byte — the GP0 command code — travels through the
lighting/depth-cue result into the packet. **0x2C is POLY_FT4 and 0x3C is
POLY_GT4**, written as literals, so those two primitive identifications need no
inference. The others follow from the packet shape (tag length and field
layout): 5 words / 24 bytes = POLY_F4, 8 / 36 = POLY_G4, 9 / 40 = POLY_FT4,
12 / 52 = POLY_GT4.

**`jtbl_8007DA14` (8 entries × {target, stride}) is the model path**, reached
from `func_80028E9C` with `index = opcode & 0xFFFF`. Entries 0..3 are the
depth-cued set and 4..7 the plain set, each covering F4 / FT4 / G4 / GT4:

| i | function | new name | primitive | GTE |
|---:|---|---|---|---|
| 0 | func_800293F0 | `GameEmitPolyF4Fog` | POLY_F4 24 B | DPCS |
| 1 | func_80029458 | `GameEmitPolyFT4Fog` | POLY_FT4 40 B, code 0x2C from `0x1F800070` | DPCS |
| 2 | func_800294E4 | `GameEmitPolyG4Fog` | POLY_G4 36 B | NCDT + NCDS |
| 3 | func_800295BC | `GameEmitPolyGT4Fog` | POLY_GT4 52 B, code 0x3C from `0x1F800074` | NCDT + NCDS |
| 4 | func_80029064 | `GameEmitPolyF4` | POLY_F4 24 B | none |
| 5 | func_800290C8 | `GameEmitPolyFT4Raw` | POLY_FT4 40 B, code forced **0x2D** (`li t6,0x2d`) | none |
| 6 | func_80029158 | `GameEmitPolyG4` | POLY_G4 36 B | NCCT + NCCS |
| 7 | func_80029230 | `GameEmitPolyGT4` | POLY_GT4 52 B, code forced 0x3C | NCT + NCS |

`GameSubmitModel` (func_80028DEC) reaches this table through `func_80028E9C`
(`GameSubmitModelFaces` — the per-face `RTPT` / `NCLIP` / `AVSZ4` loop) with
**`addiu a1,a1,4` in the `jal` delay slot**, so it always selects entries 4..7,
the un-fogged half.

**`func_80029340` is `GameSubmitModel2`, and it is dead code.** It is
instruction-for-instruction `GameSubmitModel` with that one delay slot replaced
by `nop`, so it selects entries 0..3 — the fogged half of the same table. It has
zero references anywhere in the image and appears in no table.

**`jtbl_8007DA54` and `jtbl_8007DA64` (4 entries each) are the course path**,
and this settles the question section 7 left open about `GameSubmitCourseModel2`.
The two tables hold the *same four primitives with the same four record strides*
(0x10 / 0x1C / 0x20 / 0x20); DA64's bodies each add `RTPS` on the face's fourth
vertex followed by `DPCS`. **The second opcode table is the depth-cued one** —
verified instruction by instruction on indices 0 and 1 (`c2 0x180001` = RTPS,
`c2 0x780010` = DPCS in func_80029FD8 and func_8002A218, none at all in
func_8002970C).

| i | DA54 (plain) | DA64 (fogged) | primitive |
|---:|---|---|---|
| 0 | func_8002970C `GameEmitCoursePolyF4` | func_80029FD8 `GameEmitCoursePolyF4Fog` | POLY_F4 24 B |
| 1 | func_80029788 `GameEmitCoursePolyFT4` | 0x8002A074 → func_8002A218 `GameEmitCoursePolyFT4Fog` | POLY_FT4 40 B |
| 2 | func_80029830 `GameEmitCourseSubdividedFT4` | func_8002A2CC `GameSubmitCourseSubdividedFaces` → func_800298B0 `GameEmitCourseSubdividedFT4Fog` | subdivided POLY_FT4 + GP0 0xE2 texture window |
| 3 | 0x8002A550 | 0x8002A5F8 | as 2, plus `g_AnimTimer & 0x7F` added to the CLUT word and to all four UV pairs — a scrolling texture |

`func_80029EA8` is `GameTransformCourseModel`: both course entry points call it
in their `jal` delay slot with the course bank from `0x1F800048`. It applies the
mirror-flag negation to the GTE rotation matrix and TRX, then `RTPT`s the whole
vertex list three at a time into a screen-XY array at `0x1F80011C` (4 bytes per
vertex) and a Z array at `0x1F8002FC` (2 bytes per vertex). That is why the
course face loops contain no projection of their own.

**`jtbl_8007D9F4` (4 entries × {target, stride}) is the terrain path.**
`func_80027FF4` is `GameSubmitTerrainCells` — its three call sites all read
`GameSubmitTerrainCells(0x1F800000, g_VisibleCellList, 0x40)`, so the name is
certain. It walks 64 sixteen-byte `{x, y, z, cellIndex}` records, loads TRX/TRY/TRZ
and dispatches per cell to `GameSubmitTerrainCellFaces` (func_80028120) or, past
`TRZ >= 0xA000`, to `GameSubmitTerrainCellFacesFar` (func_80028D84), which skips
face records until one carries flag bit 2 and then branches into the same loop —
a distance LOD. All four `jtbl_8007D9F4` modes emit POLY_FT4; two of them wrap
it in a pair of 12-byte GP0 0xE2 texture-window packets, and two skip the depth
cue and instead add 1 to the CLUT row. When a face's subdivision counters survive
the distance reduction, the mode calls `GameEmitSubdividedTerrainQuad`
(func_80028874), which lerps screen XY and UVs with `INTPL` through
`GameInterpolateSubdivRow` (func_80028C54) and emits one POLY_FT4 per cell.

Naming style: `GameSubmit*` for the entry points that walk a model's opcode list
(matching the existing `GameSubmitModel` / `GameSubmitCourseModel`), and
`GameEmit*` for the jump-table leaves that write one packet. `GameEmit` is a new
verb in this codebase; it is used deliberately, to mark "hand-written engine leaf
reached only through a jump table" as distinct from the C-level `GameQueue*` /
`GameAddTilePrim` helpers.

Not settled, and therefore not in any name:

* What the alternate CLUT row selected by `func_80028120`'s modes 1 and 3
  actually is. The selector is `g_IsEnvironmentMode4` and the effect is
  `clut += 1`; what the second palette row contains is on the disc.
* `GameEmitSubdividedTerrainQuad` also emits two `LINE_F3` packets per
  subdivided quad, guarded on a sign bit, 64 OT entries further back. The
  primitive type and the guard are certain; whether they are seam filler, a road
  edge stripe or debug output is not.
* `func_80029FD8` and `func_8002A2CC` are each simultaneously a jump-table entry
  point, an emitter body and the host of a shared face loop. Their names describe
  the entry-point role, which is how they are reached; the file comments say so.

### 17d. `sdk/`: the libsnd sequencer, and the libspu voice manager

The MIDI chain reads end to end now. `SsSeqOpen` (func_8006F004) calls
**`SsSeqParseHeader`** (func_8006ECDC), which zeroes the `SeqStruct`, seeds
`programs[i]=i` / `panpot=0x40` / `vol=0x7F` for 16 channels, accepts `"SEQp"`
in either byte order, reads the big-endian resolution and the 3-byte
microseconds-per-quarter-note tempo and converts it with the literal 60000000
(`lui a2,0x393 / ori a2,a2,0x8700`). Its two error messages are crossed in
Sony's original: the *version* check prints `"This is not SEQ Data.\n"` and the
*magic* check prints `"This is an old SEQ Data Format.\n"` and returns 0, which
the caller treats as success.

`SsSeqAdvanceChannelDelta` then calls **`SsSeqDispatchMidiEvent`**
(func_8006F1E0), the status-byte router with running-status latching, whose five
targets are Note On, **`SsSeqDispatchControlChange`** (func_8006F5F4), Program
Change, Pitch Bend and Meta/SysEx. The Control Change dispatcher handles
controllers 0, 6, 7, 10, 11, 64, 65, 91, 98, 99, 100, 101 and 121, every one
with its standard MIDI meaning — which is what makes the two leaves nameable:

* **`SsSeqSetPortamento`** (func_8006FB7C) is `case 65:` (Portamento On/Off). It
  walks every tone of the channel's program and writes `VagAtr.mode = 2` for
  values under 0x40 and `0` at or above, through `SsUtGetVagAtr` /
  `SsUtSetVagAtr`. Writing the whole byte also clears the reverb bit — a real
  side effect, not a decompilation artefact.
* **`SsSeqApplyDataEntry`** (func_8007010C) is `case 6:` (Data Entry MSB), and
  section 1's row for it described only half of what it does. The RPN half
  (gated on `unk29 == 2`) writes `pbmin`/`pbmax` for RPN 0; its RPN 1 and RPN 2
  branches compute a value and then **load and store `VagAtr.shift` / `.center`
  unchanged** — they are no-ops in the shipped library. The NRPN half (gated on
  `unk2a == 2`) is the functional one: CC 99 carries the tone number (0x10 meaning
  "all tones"), CC 98 the parameter number, and both go to
  **`SsSeqApplyNrpn`** (func_800706AC).
* **`SsSeqApplyNrpn`** implements 23 parameters: 0..3 write `prior` / `mode`
  (with `SsUtReverbOff` / `SsUtReverbOn` on 0 and 4) / `min` / `max`; 4..14 go
  through `SsUnpackAdsr` → mutate → `SsPackAdsr` and cover AR/DR/SL/SR/RR with
  their linear-vs-exponential mode bits, the sustain direction, `vibT` and
  `porW`; 15..19 call the `SsUtSetReverb*` setters and touch no VagAtr at all.

The per-tick side: **`SsSeqCalledTbyT`** (func_80071018) takes the
`D_801E40AC` re-entrancy guard, flushes the voices and then walks every open
seq × sep, calling `SsSeqAdvanceChannelTick` plus, on the flag bits,
**`_SsSndCrescendo`** (func_8007128C, ramps the volume up to 0x7F and clears
flag 0x10) and **`_SsSndDecrescendo`** (func_80071568, the mirror image down to
1, flag 0x20).

libspu voice manager, all in `SpuVoice` (stride 0x34 off `D_8009E0B8`):

| name | addr | what settles it |
|---|---|---|
| `SpuVmInit` | 0x80075710 | the cold init: `SpuInitMalloc`, zero the 24×16-byte voice register shadow and the 16-byte VAB-id table, `D_801E42F8 = min(arg, 24)` (the voice count), per-voice defaults **and** the real SPU registers at `0x1F801C00 + v*16` (ADDR 0x200, PITCH 0x1000, ADSR1 0x80FF, ADSR2 0x4000), master volume 0x3FFF. Its only caller is `_SsInitTables`, with 0x18. The already-named `_SsVmInit` is the warm-reset subset. |
| `SpuVmNoiseKeyOn` | 0x80074348 | the branch `SsUtKeyOnV` takes when `vag == 0xFF`. It is the only place in the image that programs the SPU noise clock: it clears SPUCNT bits 13:8 and writes `(note - center) & 0x3F`, sets the voice status to 2 (the value `SsUtFlush` tests before calling `SpuSetNoiseVoice`) and writes the NON registers directly. |
| `SpuVmAutoVol` / `SpuVmAutoVolTick` | 0x80074D1C / 0x80074ECC | arm and advance the `+0x1C…0x26` ramp; the tick handler feeds the ramped value into the volume multiply chain. |
| `SpuVmAutoPan` / `SpuVmAutoPanTick` | 0x8007521C / 0x800753CC | the same shape over `+0x28…0x32`; the tick handler feeds the ramped value in as the third pan factor. |
| `SpuVmPitchBendVoice` | 0x80074B68 | uncalled. Scales the bend by the tone's `pbmax`/`pbmin` and divides by 127 (the `0x81020409` magic multiply). Its negative branch sets `fine` from the **quotient** rather than the remainder, where the matched sibling `SpuVmApplyPitchBendToVoice` uses the remainder — Sony's bug, preserved byte-exact, and plausibly why this entry point was superseded. |
| `SsUtKeyOff` | 0x80077A88 | see 17b item 2. Requires all four of vabId/prog/tone/note to match the voice record before keying off. |

Every `Ss*` / `Spu*` spelling in this section that is not in LibRef47 is a
*descriptive* name in the style `lib/libsnd/` already uses, not a recovered Sony
static. That is stated here so nobody later mistakes them for recovered symbols.

`func_80074D0C` and `func_80074D14` (two words each, `jr ra`) stay raw: empty
stubs with no caller and nothing to name them from.

### 17e. `sdk/`: libcd, libcard, libpress, libapi

* **`CdRead2` (func_8006CD0C)** — this is the pass's best single result, because
  section 15g had written it off. LibRef47 10-31: `int CdRead2(int mode)`,
  "seeks to the position specified by CdlSetloc and starts reading … **starts
  streaming when the CdlModeStream flag is set in mode**", returning 1/0. The
  body is one `int` in; `CdControl(CdlSetmode, &(u8)mode)`; `if (mode & 0x100)`
  install the data-ready and ready callbacks; `return CdControl(CdlReadS)`.
  `CdlModeStream = 0x100` and `CdlModeStream2 = 0x120` are LibRef's own Table
  10-4, and the `mode & 0x20` test writes the flag that `StGetBackloc`
  (documented as "valid **only** for CdlModeStream2 mode", −1 otherwise) reads.
  The single call site passes `0x1E0` = Stream2 | Speed | RT, the textbook STR
  mode. `DsRead2` is excluded by LibRef's own note that it takes a position.
* **The libcard block is entirely BIOS stubs and is now labelled**: `_bu_init`
  (A0 70h, func_80063180), `_card_info` (A0 ABh), `_card_load` (A0 ACh),
  `InitCARD` (B0 4Ah), `StartCARD` (B0 4Bh), `_card_write` (B0 4Eh,
  func_80063E24), `_new_card` (B0 50h, func_80063E34) and their composite
  `_card_clear` (func_80063DEC = `_new_card(); _card_write(chan, 0x3F, NULL);`,
  which is LibRef's "dummy write to the system management area"). The B0
  numbering is anchored by eight stubs this repo had already identified
  (OpenEvent 08h … DisableEvent 0Dh, BiosFileOpen 32h … BiosNextFile 43h),
  with no contradictions.
* **libpress**: `_new_card` shares a unit with `DecDCTReset`, `DecDCTGetEnv`,
  `DecDCTPutEnv`, `DecDCTBufSize` and `DecDCTin`, all of which were already
  aliased and all of which now check out against LibRef47 argument for argument.
  `DecDCTin`'s `mode` handling is the decisive one: bit 0 flips the 24-bit
  output bit in the MDEC command word and bit 1 the STP bit, exactly as
  documented.
* **`DeliverEvent` (func_8006A3D8)** — B0(07h); its three libcd callers pass
  `(0xF0000003, 0x20 | 0x40)`, i.e. `HwCdRom` with `EvSpCOMP` / `EvSpDR`.
* **`StopKernelInterrupts` (func_8006E4E4)** — the vector at `0x8009A498` is
  self-identifying: word 0 is the RCS string
  `$Id: intr.c,v 1.73 1995/11/10 05:29:40 suzu Exp $`, so the module is libapi's
  `intr.c`. Slot +0x10 is this function and the public thunk that reaches it is
  `StopCallback()`; the body saves I_MASK and DPCR, zeroes I_MASK, acks I_STAT,
  clears the enable bit of all seven DMA channels and calls `ResetEntryInt` —
  LibRef's "disable all interrupts", and the exact inverse of its partner
  `StartKernelInterrupts`, which restores the two saved words.
* **`Gpu_ArmTimeout` / `Gpu_CheckTimeout` (func_80067F04 / func_80067F38)** —
  arm `VSync(-1) + 240` and, past the deadline, print the
  `"GPU timeout:que=%d,stat=%08x,chcr=%08x,madr=%08x,"` /
  `"func=(%08x)(%08x,%08x)\n"` pair, mask interrupts, reset the queue and issue
  GP1(02h)+GP1(01h). Both are reached from five driver-table workers. **No Sony
  name is claimed** — neither string self-names, and `_status` is definitely
  wrong because section 16 already pinned that to driver slot +0x38.
  `Gpu_*` is this project's established prefix for exactly this situation.
* **`Gte_PatchExceptionHandler` (func_80069FA8)** — called only from `InitGeom`
  (func_80068928, itself named this pass). It `EnterCriticalSection`s, fetches
  the C0 table with B0(56h), overwrites the BIOS `ExceptionHandler` entry with
  14 words of hand-written code that saves only `at`/`v0`/`v1`/`ra` into the TCB
  instead of the full register spill, then `FlushCache` (A0 44h) and
  `ExitCriticalSection`. Descriptive name, new `Gte_` prefix by analogy with
  `Gpu_`; no Sony symbol is claimed.
* **`CD_namecmp` (func_8006C53C) / `CD_strncmp` (func_8006CC8C)** — the 12-byte
  filename compare `DsSearchFile` runs over the 64-entry `CdlFILE` cache
  `CD_cachefile` fills, and the null-safe `strncmp` under it. Descriptive names
  in libcd's existing `CD_*` style; not Sony symbols.
* **`CD_dmastart` (func_8006DB74)** — spins on the channel's CHCR busy bit,
  prints `"DMA STATUS ERROR %x\n"` on timeout, then programs MADR/BCR/CHCR. All
  three call sites use channel 3 with an 8-word header transfer and a 0x1F8-word
  body transfer — the libds sector header/body split. Descriptive name.

### 17f. `render/`: the two-page track texture swap, and the rear-view mirror

**The VRAM texture swap is solved as a mechanism.** The rectangle is
`Rect{x=576, y=256, w=448, h=256}` — the whole right-hand half of the lower VRAM
bank, 0x38000 bytes — and the row stride 0x380 is one 448-pixel line. The RAM
shadow `D_801E42D0` is produced by `func_8001A40C`'s `StoreImage` of exactly that
rectangle, and the install sequence in `GameInstallCourseAssets` is decisive:
upload image blocks 0..3 of the track `.1ST`, snapshot the region into the
shadow, then upload block 4 **on top**. So the shadow holds the region without
block 4 and VRAM holds it with block 4; `GameStepTrackTextureSwap` (func_8001A030,
called once a frame from `GameMainLoop`) exchanges them one row per iteration
until `VSync(1) >= 471` eats the frame's spare time, and
`g_TrackTextureRowState[256]` makes each row's exchange idempotent.

The trigger is authored per course: `GameLoadTrackTexturePageRange` (func_8001D30C)
copies words 0 and 1 of the `.2ND` pack's sub-block 0 into
`g_TrackTextureSectionLo` / `…Hi`, and `GameSelectTrackTexturePage` (func_80019D24)
returns 0 or 0x100 according to whether the car's track section falls inside that
window. The clincher that the two pages are two *regions of the course* is the
attract camera: `GameCycleAttractCameraCar` and `GameCycleBgmSelectCameraCar` roll
a random car and **refuse to cut to it unless it maps to the same texture page as
the car currently being watched**.

Named with it: `GameRequestTrackTexturePage` (func_80019EFC — the "8 files" entry
section 7 left generic), `GameSetTrackTexturePageNow` (func_80019E84, used by scene
entries and camera teleports), `GameSwapTrackTexturePageNow` (func_80019D7C),
`GameSwapTrackTextureRow` (func_80019F24), `GameResetTrackTextureSwap` (func_80019EBC),
and the globals `g_TrackTexturePageWanted`, `g_TrackTextureTargetRow`,
`g_TrackTextureCursorRow`, `g_TrackTextureShadow`, `g_TrackTextureSectionLo/Hi`.

**What is still not settled is which pixels differ** — that is block 4 of the
`.1ST` pack, which is on the disc. Three cheap checks would close it: dump
`offsets[4]` and block 4's image rects; compare words 0/1 of the matching `.2ND`
against `g_TrackLength` to see what fraction of a lap the window is; render both
sets. A tunnel would show as a short window, a course split as one near half the
lap. The code is agnostic, so no name asserts either.

**`render/func_8001A980.c` is the rear-view mirror**, and every number agrees:
`GameBeginMirrorPass` (func_8001A9A8) installs `D_8019CB18` — which section 7
already documents as the camera matrix pre-multiplied by a 180° Y turn — sets
`SetGeomOffset(0xA0, 0x24)` / `SetGeomScreen(0xC0)`, and clips to a 148 × 36
window centred on x = 320, mirrored into the second `DRAWENV` of both frame
contexts so the panel can slide in from above. `GameEndMirrorPass` (func_8001ABD8)
is its exact inverse. `GameDrawMirrorFrame` (func_8001ACE4) draws a 152 × 40 black
`TILE` — a 2-pixel border exactly bounding the viewport — plus one 8-px sprite
picked by car model through `D_8007C728[13]` into four styles. `GameDrawRearViewMirror`
(func_8001ADF4) runs the whole pass: unlock at `g_SceneTimer >= 0x169` (361 frames,
after the countdown), slide `g_MirrorPanelY`, then sky, frame, terrain, course
objects and cars into the mirror viewport. `GameResetMirrorState` (func_8001A980)
seeds it at race entry. `g_MirrorViewEnabled` is the player's toggle — the only
other writer is `GameUpdateRaceScene`, where a shoulder button plus `g_PadEdge2`
sets it.

Also in `render/`: **`GameDrawPlayerCarModel`** (func_8001DAB0) is the hero car,
not a duplicate of `GameDrawCar` — all four callers do
`GameSelectModelBank(0); func_8001DAB0(&g_PlayerCar)` where `GameDrawCars` selects
bank **1**, and it has no LOD ladder at all. **`GameBuildRaceHudPrims`**
(func_80032D5C) expands 12 (GP) or 11 (time trial) `GameSpriteDesc` rows into
both frame contexts at `frame + 0x236F8 + 0x14*i`, which is precisely the block
`GameDrawLapTimes`, `GameDrawRaceHudLabels` and `GameDrawRacePosition` index into;
**`GameDrawRaceHudLabels`** (func_80032E9C) links prims 6..8 or 6..11 every frame.
`GameBuildSpriteFromDesc` (func_80032FF0), `GameDrawTimeRemaining` (func_800331F8,
`GameDrawMinuteSecondTime` at (14, 210) turning to the warning colour below 1500
ticks) and `GameDrawSplitDelta` (func_80033308) complete that unit.

SDK routines named in `render/` from LibRef47, all instruction-verified:
**`CompMatrix`** (func_80068A38 — `[m2] = [m0][m1]`, `m2->t = [m0]·m1->t + m0->t`,
with `m1->t` going through the 16-bit V0 registers exactly as the documented
(1,15,0) restriction requires), **`ApplyMatrixLV`** (func_80068F80, the 32-bit
hi/lo split), **`ApplyMatrixSV`** (func_800696C8, SVECTOR in and out, returns the
third argument), **`ScaleMatrix`** (func_80069728, `m[i][j] *= v[j]`) and
**`ScaleMatrixL`** (func_80069110, `m[i][j] *= v[i]`, unreferenced). The
ScaleMatrix pair was decided by LibRef47 8-150/8-151, which print the two
matrices side by side; nothing in the image distinguishes them.

### 17g. `menu/`, `boot/`, `save/`, `asset/`, `audio/`

Section 15g's reason for leaving five `menu/` widgets raw was that they are
"reached only through a table". They are not in any table. Every one is `jal`-ed
from exactly one screen handler, and the screen entry code sets
`g_MenuHandlerIndex = <id>` and calls the widget with all-zero arguments in the
next instruction — which is what ties each widget to one screen:

| function | new name | screen |
|---|---|---|
| func_8004E368 | `GameDrawLogoSamplePanel` | id 8, TEAM LOGO sample picker (`g_MenuHandlerIndex = 8` then `func_8004E368(0,0)` on adjacent lines). Draws a two-digit sample number 01..20, a caption plate and the 15 colour swatches of the live logo CLUT `D_801E444C`. |
| func_8004F650 | `GameDrawCarShopPricePanel` | id 11, car shop. Row 1 is `g_PlayerMoney`, row 2 the car price from `g_CarPriceTable`. |
| func_8004F99C | `GameDrawEngineerShopPricePanel` | id 12, engineer shop. Structurally identical, 211 words each; the only differences are the accumulator and row 2's caption width. |
| func_8005026C | `GameDrawClassChangeCurtain` | id 1, COURSE SELECT. Two opaque 320×240 bars in the menu red (0x95,0x25,0x1E) closing from top and bottom over the 320×480 menu frame; its only trigger is the GRAND PRIX class change, whose `>= 0x19` branch commits `g_GrandPrixClass`. |
| func_800506BC | `GameFlipCourseCard` | id 1, GRAND PRIX branch only. A 64×80 quad at (228, 88) spun about Y by exactly half a turn per course change (`0x1F4000 / 1000` = 2048 GTE units), swapping its texture mid-flip; keyed off `g_CourseProgress`. |

`GameClearTeamNameTexture` / `GameUploadTeamNameTexture` (func_8001D4E8 /
func_8001D530) blank and refill a 12-word × 8-line VRAM strip at (0x282, 0x37)
from `g_TeamNameChars` / `g_TeamNameLength`; the start x of `0x288 - len` centres
up to six 8-pixel glyphs, which is exactly `g_TeamNameLength`'s cap.
`GameRampTeamLogoCanvas` (func_8004B8B4) is the parameter ramp feeding
`GameDrawTeamLogoCanvas` — it drives `D_8009B298`, already documented in section 1
as that function's fade level.

`boot/`: **`_start` (0x800630B4)** is the executable entry — the PS-EXE header's
`pc0` field at file offset 0x10 holds exactly that address. It clears BSS
0x8009AED8..0x801F2A10, sets `sp = 0x80200000` from the four-entry RAM-size table
at 0x80063160, `gp`, `fp`, calls `InitHeap` and then `main`. **`__main`
(func_800630AC)** is an empty function called as the very first `jal` in `main`,
before any argument setup — the GCC 2.x convention. `GameDrawBootLogo`,
`GameUpdateBootLogoScene` (scene table slot 1), `GameInstallSceneLighting` and
`GameEnterAttractScene` (slot 22) complete the boot unit.

`save/`: **`GamePollMemoryCardStatus` (func_8005ECE0)** builds
`chan = (port << 4) + slot` — LibRef's own "Port number × 16 + Card number" — and
runs `_card_info` → poll → `_card_load` → poll, mapping the four event codes onto
LibRef Table 4-2: `EvSpIOE` proceeds, `EvSpTIMOUT` returns −1 (no card),
`EvSpERROR` −3, and `EvSpNEW` at the second poll returns −2 (present but
unformatted), which is what drives the "New Memory card." / "Format Memory card?"
prompts. **`GameDrawMemoryCardMessage` (func_80027D84)** is settled by its own
20-entry line table `D_8007D99C`: "Select file to save.", "No Memory card.",
"Memory card full.", "Format Memory card?", "Overwrite old file?",
"Now accessing Memory card.", "LOAD/SAVE/FORMAT DATA OK!" and so on, with
indices 16..18 drawing a banner sprite instead. **`GameStoreSaveStateBlock`
(func_8005F88C)** is the exact inverse of `GameLoadSaveStateBlock`: it serialises
every save-backed global into the 0x1000-byte block and ends with
`checksum = ~Σ u16[0..0x7FD]`.

`audio/`: **`GameStartAudioSlotLoad` / `GamePollAudioSlotLoad`** (func_8005B768 /
func_8005B89C) are a start/poll pair over `SsVabOpenHeadSticky` →
`SsVabTransBody` → `SsVabTransCompleted`, with the per-slot SPU destination table
`D_800125EC = {0x1000, 0x20000, 0x20000, 0x6A000}`. Every asset loader uses them
as `case n: start(...); state++` / `case n+1: if (poll()) state++`.

`asset/`: **`GameUploadImageBlock` (func_8001A2E0)** is the per-record worker
under the already-named chain walker `GameUploadImageAsset`; it uploads the CLUT
sub-block first when `flags & 8` and then the pixel block.

### 17h. A note on the `0x80063200` rule

The convention "everything below 0x80063200 is Namco game code and gets a `Game*`
name" has a documented exception at **0x800630AC–0x80063220**: `__main`, `_start`,
`InitHeap`, `_bu_init`, `SetMemSize`, the B0 event stubs, `BiosInitPad`,
`BiosStartPad` and `ExitCriticalSection` are all crt0/BIOS objects that the linker
happened to place in the middle of the game text. They must not be "fixed" into
`Game*` names. Section 13c already broke the rule the same way for libcd's
`cdread.c` at 0x80027238.

### 17i. Left unnamed on purpose

* **`func_8006AB5C`** — the libcd interrupt decoder (350 words, drained in a
  `while` loop by `CD_sync`, `CD_ready`, `CD_cw` and the IRQ2 handler). Section 15d
  already corrected section 7's "error/trace helper" reading; this pass confirms
  it and strengthens the reason to leave it raw. Every other libcd internal in
  this image was pinned because it stores *its own name* into a trace slot or is
  named inside another routine's message. This one owns two strings,
  `"DiskError: "` and `"CDROM: unknown intr"`, and neither is an identity string.
  `CD_intr`, `CD_getintr`, `CD_status` and `CD_readIntr` are all unfalsifiable
  here, and `CD_status` is additionally implausible — the status accessor is
  `CdStatus`, which reads the byte this function writes. What would settle it: a
  libcd object with a surviving symbol table or `$Id:` string (libcd left none in
  this image; only libapi's `intr.c` did), or another PS1 decompilation with
  libcd symbols at matching offsets. **This is the only one of the 57 units still
  on a `func_` name.**
* **`func_800271EC`** (19 words, scene-table slot 32) keeps section 13h's ruling:
  it is a per-sequence step driver dispatching `D_8007D778[D_801E4178]`, and the
  four steps are not identified. Its unit is named after the libcd `cdread.c`
  statics that dominate it, which is what section 14c already decided about the
  file's placement.
* **`func_8004A17C`** (51 words, in `menu/GameDrawTeamLogoCanvas.c`) has **no
  caller anywhere**, and its two globals `D_8009B280` / `D_8009B284` are
  referenced nowhere else in the image. It ramps a level and draws a 248×480
  semi-transparent grey curtain at (72, 0). Dead code with a private accumulator:
  no feature can be attributed to it.
* **`func_80052128`** (12 words, in `menu/GameDrawCarEngineSpec.c`) also has no
  caller and no data reference. It prints one of the 13 strings
  `"CAR-0"`..`"CAR-C"` from `D_80082E70` through `GameDrawText8x8`. A leftover
  debug label printer; naming it would imply it is reachable.
* **`func_80050400`** (175 words, in `menu/GameInitMenuMode.c`) provably belongs
  to screen id 11 — its reset is welded to `g_MenuHandlerIndex = 11` and its only
  live caller is `GameUpdateCarShopScreen`. But *what its two unfolding textured
  plates depict* is not proven: plate A is gated on `g_CarModelAsset[8]`, an
  undocumented car-asset-header byte, and **plate B never opens in the shipped
  build** — `D_8009B330` is only ever assigned −1 or 0, never a positive step.
  Any content-bearing name would be a guess.
* **`func_800509C4`** (86 words) draws a 48×24 plate at (76, 215) that unfolds
  only in TIME ATTACK COURSE SELECT and only while the course series index is
  ≥ 1. Section 15g declined it; this evidence does not overturn that, because the
  tpage-0x20F artwork is on the disc.
* **`func_800332E0`** (10 words, no callers) writes one CLUT word into HUD prim 8.
  Dead; belongs with the section 15f list, not in a name.
* **`func_8001A40C`** does `LoadImage` of the team-logo CLUT and then
  `StoreImage`s the whole 448×256 page into `g_AssetBase`. The mechanism is
  unambiguous but nothing in the decompiled tree reads that captured page, so
  what it is *for* is unproven.
* **`func_80074D0C` / `func_80074D14`** — two-word empty libsnd stubs, no callers.
* **`func_800731CC`** — see 17b item 8; it is very probably `SsUtFlush`, but
  freeing that name means renaming an already-named unit file, so it is recorded
  rather than done.
* **`func_80076B30` / `func_80076C1C`** — the pair displaced from
  `SsUtKeyOn`/`SsUtKeyOff`. They are `SpuVmSeKeyOn/SeKeyOff(0x21, …)` wrappers
  with no callers and no matching LibRef signature.

### 17j. New naming conventions introduced

* **`GameEmit*`** for the hand-written GTE engine leaves that write one primitive
  packet and are reached only through a jump table, as distinct from the C-level
  `GameQueue*` / `GameAddTilePrim` helpers. `GameSubmit*` stays for the entry
  points that walk a model's opcode list.
* **`Gte_*`** for libgte internals whose Sony name cannot be pinned, by analogy
  with the existing `Gpu_*`.
* **`Ss*` / `Spu*` descriptive names for libsnd/libspu internals**, matching
  what `src/main/PAL/lib/libsnd/` and `lib/libspu/` already do. LibRef47 documents
  no internal statics, so these are descriptions, not recovered symbols, and this
  section says so explicitly rather than letting a later reader assume otherwise.
