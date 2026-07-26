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
together and `make check VERSION=PAL` stayed byte-identical. The only stub still
on a `func_` name on purpose is `func_8007010C` (see the SDK table).

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
| func_8007010C | 0x8007010C | 360 | [INCLUDE_ASM] | **Deliberately left generic.** libsnd internal, so there is no public `Ss*` name to claim. Behaviourally it is the MIDI **Control Change #6 (Data Entry MSB)** handler: func_8006F1E0 routes status 0xB0 to func_8006F5F4, whose `case 6:` is this. It applies the pending RPN/NRPN to the channel's VAB program by rewriting the `VagAtr` of every tone — `SsUtGetProgAtr` for the tone count, then per tone `SsUtGetVagAtr` → mutate → `SsUtSetVagAtr` — with the field chosen by `SeqStruct + 0x13`: 0 → +0x0C/+0x0D (`pbmin`/`pbmax`, i.e. RPN 0 pitch-bend sensitivity), 1 → +0x05 (`shift`), 2 → +0x04 (`center`), all gated on `play_mode == 0`. **Was described here as the "libsnd sequence tick/step"** — that is `SsSeqCalledTbyT` at 0x800731CC. |
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
