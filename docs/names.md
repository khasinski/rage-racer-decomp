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

## How to read this document

**The code is the authority, not this file.** Where the two disagree, the code
wins. Everything here was written by a pass that has since been overtaken by
other passes, and the checks that settle a claim are cheap: grep `src/` for the
symbol, look at whether the function still carries an `INCLUDE_ASM`, run
`tools/scripts/progress_report.py` for what actually counts as decompiled.

Two kinds of section live here and they age very differently:

- **Conventions and mechanics** — rules the project follows, and toolchain
  behaviour that is a property of gcc 2.6.3 rather than of a particular
  function. Sections 3b ("the gcc 2.6.3 rule that decides whether a struct can
  be applied"), 4, 12c, 17h, 17j, 20a's three bullets, 21a and 24 are of this
  kind. They stay true until the toolchain or the policy changes.
- **Per-pass findings** — what one naming or typing pass established about
  specific symbols. Sections 1, 3, 3a, 5, 6, 7, 12, 13–20, 21b–21e, 22 and 23
  are of this kind. They are snapshots, and a later pass may have moved the
  ground under them.

Claims are marked in place:

- **DISPROVEN** — the claim was made, acted on, and is wrong. The wrong reading
  is left visible with the counter-evidence, because someone may remember having
  read it.
- **SUPERSEDED** — folded into the statement it corrects; the later correction
  block is gone and only the evidence survives.
- **UNVERIFIED** — stated as fact by an earlier pass and not checkable from this
  repo today (usually because the source it rests on is a scratch file or a disc
  asset that is not in the tree).

**Every "deliberately unnamed" / "left raw on purpose" list in this document is a
snapshot, and most of them have expired.** Of the 107 `D_` symbols covered by such
a claim, **59 now carry a readable name** — sections 12e, 18b, 19 and 23 took them,
usually by loosening the policy the earlier list was defending rather than by
finding new evidence. The lists are still worth reading for *what was observed*
about each symbol; they are not evidence that anything is unnameable. Check with

```sh
grep -rn 'asm("D_8019CB0C")' src/ include/
```

Three `D_` symbols the document names do not exist anywhere in the tree —
`D_80012748` (section 1's sound-cue table end), `D_8007DF32` (a scroll
accumulator in `GameDrawRaceOptionMenu`) and `D_801E4B70` (the second pad-mapping
row). They are plausibly interior addresses of larger objects that the split
never gave their own symbol, so treat them as **UNVERIFIED** rather than wrong.

An audit against the tree at `0568a8af` folded the corrections in and marked
what was wrong; the notes below carry that date where the reading changed.

### Sections

| # | Section | Kind |
|---|---|---|
| 1 | Remaining functions (what each IS) | per-pass |
| 2 | Struct inventory | per-pass |
| 3 | Global memory map (key regions) | per-pass |
| 3a | The menu-mode screen table (identified by emulation) | per-pass |
| 3b | Typing pass: raw offsets replaced by real structs | mixed — the gcc 2.6.3 aliasing rule is a convention |
| — | Change note (this consolidation pass) | per-pass |
| 4 | Translation units | convention |
| 5 | The title screen / main menu, and the animated course scenery | per-pass |
| 12 | Globals naming, round 3 | per-pass |
| 6 | Disc asset archive (RAGE.BIN) | per-pass |
| 7 | High-fanout names | per-pass |
| 13 | Subsystem naming pass (asset loader, FMV, cdread, CD-DA, logo editor) | per-pass |
| 14 | Subsystem directory pass | per-pass |
| 15 | Gameplay core pass (`race/`, `car/`, `track/`) | per-pass |
| 16 | `sdk/`, `render/` and `menu/` naming pass | per-pass |
| 17 | Finishing the unit files | per-pass; 17h/17j are conventions |
| 18 | Globals pass over `race/`, `car/` and `track/` | per-pass |
| 19 | Front-end globals pass | per-pass |
| 20 | Types pass: aggregation and semantics | per-pass; 20a's bullets are conventions |
| 21 | Endgame pass: the remaining functions | per-pass; 21a is a convention |
| 22 | One address, one name (duplicate-name pass) | per-pass |
| 23 | Library globals pass | per-pass |
| 24 | Shiftability: the program no longer contains its own addresses | convention |

**Function names in this file lag the tree.** Roughly two-thirds of the
`func_XXXXXXXX` spellings below now have a readable name in `src/`, because most
sections were written before the naming pass that gave it one. The address is
always the durable fact; resolve the current name with

```sh
grep -rn 'asm("func_8003F9C4")' src/ include/
```

and if that finds nothing, the function is either still raw or (like
`GameSelectTrackTexturePage`) defined under its readable name with no alias.

Two numbering accidents are left alone because other sections cite them by
these labels: **sections 8–11 do not exist**, and **section 16's
subsections are labelled 15a–15g**, colliding with section 15's own subsections.
"See 15d" in section 7 and section 17i means section 16's `15d` (libcd);
"section 15d" in section 18d means section 15's own (named this pass, by
directory).

---

## 1. Remaining functions (what each IS)

Every non-handwritten `INCLUDE_ASM` stub now carries a real name; the segment
name, the source file name and the `INCLUDE_ASM` path argument were renamed
together and `make check VERSION=PAL` stayed byte-identical. `func_8007010C` is
now `SsSeqApplyDataEntry` (section 17d); in `src/main/PAL/main` the only symbol
left on a `func_` name as a deliberate decision is `func_8006AB5C`
(section 17i). Two qualifications the sentence needs:

- Four `lib/` units are also still `func_`-named
  (`lib/libspu/func_80078F4C.c`, `lib/libsnd/func_8006F90C_8006FA44.c`,
  `lib/libsnd/func_80070A1C.c`, `lib/libsnd/func_800771AC.c`) — 17i's "the only
  one" was scoped to that pass's directories, not to the tree.
- It is about *unit files*, not symbols. About **55** addresses still have no
  readable name anywhere in `src/` or `include/` — the ones sections 13h, 15g,
  17i and 19h left generic on purpose, plus a handful of small library statics.

Several rows below correct earlier descriptions in this file; where a name
supersedes a wrong one the old claim is called out, because it is also repeated
in the headers and in commit messages.

**This is no longer a list of the remaining functions.** It was written when the
table's rows were all `INCLUDE_ASM`; ten of them have since been converted and
several functions that are still assembly were never in it. The list that is
current by construction is `tools/scripts/progress_report.py`, which classifies
every function individually. As of `0568a8af` that is **50** functions still
carrying assembly out of 1105, plus **47** documented `HANDWRITTEN_ASM`
(see the note under "SDK library" below, which said 48 and was wrong).

Rows below whose function has since been **converted to C** — they are kept
because the *description* is still the best account of what each one does, but
they are no longer outstanding work: `GameDrawLeftArrow`, `GameDrawRightArrow`,
`GameDrawPadConfigSelector`, `GameDrawPadConfigLabels`,
`GameDrawPadConfigCallouts`, `GameDrawPadConfigDiagram`,
`GameDrawNegconConfigDiagram`, `GameBeginControllerConfig` (the whole
controller-configuration block), `GameUpdateTeamLogoCanvas` and
`GameUpdateFlybyScenery`.

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
| `GameUpdatePathScenery` (**name not in the tree** — 0x8003F9C4 is still `INCLUDE_ASM(…, func_8003F9C4)` in `track/GameDrawScriptedScenery.c` with no alias; this row proposed the name and nobody applied it) | 0x8003F9C4 | 859 | The course's **permanently looping** prop, seeded by func_8003F700 and drawn by func_80040730. Two keyframe tracks: A at `D_801E42DC` stride 0x14 `{s32 x,y,z; s16 loopIndex; s16 duration; s16 easeFlag}` drives position `D_801E4DB8/BC/C0`, B at `D_801E42E8` stride 0x0C `{s16 rx,ry,rz; …}` drives rotation `D_801E4DC8/CA/CC`. Motion is a **sinusoidal ease** between waypoints (`P[i+1] - half - half*rcos(t*0x800/dur)>>12` then `P[i] + half + half*rsin(…-0x400)>>12`), not a lerp. Sound is cue 0, culled outside a ±0x1000 box, `vol = 0x64 - (sqrt(dx²/4 + dy²/8 + dz²/4) >> 10)`, slew-limited ±0x14 against `D_801E4DF0`, with pitch `((delta/2) + 0x3C) << 7` — an approximated Doppler shift. **Was described here as a "dual-channel record/marker state machine … stride 20"**, wrong on the framing and on both strides. |

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
| `GameDrawCar` | 0x8001DFC0 | 445 | Draws one car, from the func_800389F0 loop over the 11 runtime entries (`activeFlag != -1 && field_BC == 1`). Culls on `out[2] >= 0`, then picks a LOD by Manhattan camera distance: < 3328 gives the full body plus three extra prims and a two-pass mirrored sub-part (the pass negates matrix columns 0 and 2 and the +0xC offset — the left/right wheels), < 9472 a single low-detail prim, beyond that nothing. Every submission is `*(s32 *)0x1F800084 = colour; func_80028DEC(0x1F800000, primId)` with primId clamped against the object-bank size `D_801E4168`. **Was described here as a "render-object transform".** What it reads through `D_8019C9A8` is described at §2's `CamRow` entry, which this row corrected. |

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

> The full `HANDWRITTEN_ASM` set is **47** functions (see PROGRESS.md /
> ASM_AND_GTE_POLICY.md). This said 48, which was right when it was written and
> is not now: the report changed from counting subsegments to counting
> functions (`5335913d`), then to per-function attribution (`4f34e098`), taking
> the figure 48 → 57 → 46, and `func_80069D18` (`RotMatrix`) joined the set in
> `3d5e2847` to make 47. See 21e, whose "left unchanged here" no longer applies.
> Do not quote a handwritten count from this file; `make progress` regenerates
> it. The GTE engine and libgte matrix families above are their bulk. All are
> byte-matched and excluded from the C-progress totals.

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
| CamRow | 0x14 | `u8 pad0[0xC]`; `s16 axis0`@0xC; `u16 axis1`@0xE; `u16 axis2`@0x10; `s16 horizon`@0x12 | The layout is as declared in `include/game/render.h`, but **the prose that came with it was wrong** and is corrected here: `D_8019C9A8` is a **pointer** (`GameSetTrackCameraTable` stores sub-block 0 of the track `.2ND` pack into it, 13a), the index is the per-object model selector `D_8007D3AC[g_CourseIndex][obj->field_AE]` rather than a screen number, `+0xC/0xE/0x10` is a mirrored sub-part offset vector and `+0x12` a Y bias restored on exit. Read by `GameDrawCar` (func_8001DFC0). |
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
| 0x8019C9A8 | `D_8019C9A8` | A **pointer** to the `CamRow` table, installed by `GameSetTrackCameraTable` from sub-block 0 of the track `.2ND` pack; indexed by the per-object model selector, not by a screen number. (Said "camera/horizon rows, indexed `+8*screen`" — see §2's `CamRow` entry for what was wrong.) |
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

> **SUPERSEDED as a list.** Every symbol below except `D_801E42F8` has since been
> named — sections 12b, 18b and 19 took them, and the *reasoning* here (what was
> known, and why it was not enough at the time) is what survives. Current names:
> `D_8019CB0C` → `g_MenuAltLayout`, `D_8009B338` → `g_MenuAltLayoutSetting`,
> `D_801E4030` → `g_IsEnvironmentMode4`, `D_8019C768` → `g_FrameSyncThreshold`,
> `D_801E6F2C` → `g_TeamLogoCanvas`, `D_8009E6D4` → `g_PlayerCar`,
> `D_8009E67C` → `g_CourseProgress`, and the SDK block below is section 12e's,
> which is likewise superseded. Do not cite this list as evidence that something
> is unnameable.

High-reach globals whose *meaning* was not settled when this was written, with
what was actually known — naming these would have been a guess:

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
- ~~**`D_8019C768` (12 files)** — written `0x80` or `0x180` on entry to almost
  every scene; the only reader in the whole image is
  `GameAdvanceSaveHeaderCounter`, which advances the play-time counter by 1 when
  it is `0x80` and by 2 otherwise. Not enough to say what the number *is*.~~
  **DISPROVEN.** It is `g_FrameSyncThreshold`, the per-frame pacing deadline —
  see 12d for the evidence and why three passes in a row got it wrong.
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
- ~~**SDK data** — `D_800941E0`/`E4`/`EA`, `D_800942BC` (libgpu), `D_801E79CC`
  (libsnd `SeqStruct[]`), `D_8009AB7C`, `D_801E416C`, `D_801E42F8`, and everything
  at or above 0x80063200 need matching against Sony sources, not invented
  names.~~ **SUPERSEDED by section 23**, which named the library globals from
  external ground truth (the hardware map in `6BE64.data.s`, the psyq function
  names the files already carry, and the rodata trace strings) rather than from
  Sony sources. All of these are named now except `D_801E42F8`:
  `D_800941E0` → `g_GpuFuncs`, `D_800942BC` → `g_GpuGp1`,
  `D_801E79CC` → `g_SndSeqTable`, `D_8009AB7C` → `g_SpuRegBase`,
  `D_801E416C` → `g_SndCurrentToneTable`. `D_8009E674` is referenced *only* from
  0x80074xxx–0x80078xxx, i.e. purely from library code, and is still raw.

---

## 3a. The menu-mode screen table (identified by emulation)

Everything the front end draws while `g_MainState == 3` (i.e. after GRAND PRIX or
TIME ATTACK is chosen) is one of fourteen screens. `func_8005ACA0` dispatches
them through **two parallel tables indexed by the same screen id in
`g_MenuScreen`** (`func_8005ACA0` is `GameUpdateMenuMode`):

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

Names below were refreshed against the tree at `0568a8af`; the addresses are the
durable fact and the C spellings follow from them.

| id | `GameUpdate…Screen` | `GameDraw…Screen` | accumulator | on-screen title / rows |
|---:|---|---|---|---|
| 0 | `GameEnterCourseSelectScreen` (0x80052778) | – | – | menu-mode bootstrap; falls straight into id 1 |
| 1 | `GameUpdateCourseSelectScreen` (0x80053730) | `GameDrawCourseSelectScreen` | (`D_8009B2F0`, shared) | **COURSE SELECT** (TIME ATTACK header in TA mode) |
| 2 | `GameUpdateRankingScreen` (0x80054D10) | `GameDrawRankingScreen` (0x80054C84) | `D_8009B2C4` | **RANKING** — total time / lap time / exit |
| 3 | `GameEnterCarSelectScreen` (0x80055618) | – | – | one-frame bridge into id 4 |
| 4 | `GameUpdateCarSelectScreen` | `GameDrawCarSelectScreen` (0x800551BC) | `D_8009B2CC` | **CAR SELECT** — race start / customize / car shop / engineer shop / course select |
| 5 | `GameUpdateCustomizeScreen` (0x800563A0) | `GameDrawCustomizeScreen` (0x800562C8) | `D_8009B2D0` | **CUSTOMIZE** — tire / transmission / exit |
| 6 | `GameUpdateDesignModeScreen` (0x80057198) | `GameDrawDesignModeScreen` (0x80056E64) | `D_8009B2D4` | **DESIGN MODE** — logo / name / color / exit |
| 7 | `GameUpdateTeamLogoScreen` (0x80057748) | `GameDrawTeamLogoScreen` (0x800576BC) | `D_8009B2D8` | **TEAM LOGO** — sample / paint / exit |
| 8 | `GameUpdateLogoSampleScreen` (0x800580C8) | `GameDrawLogoSampleScreen` (0x8005803C) | `D_8009B2DC` | **TEAM LOGO** (sample picker) — character / background / exit |
| 9 | `GameUpdateTeamNameScreen` (0x8005873C) | `GameDrawTeamNameScreen` (0x800586B0) | `D_8009B2E0` | **TEAM NAME** — 4x11 character grid, 0x2A = BS, 0x2B = ED |
| 10 | `GameUpdatePaintColorScreen` (0x80058C14) | `GameDrawPaintColorScreen` (0x80058B88) | `D_8009B2E4` | **PAINT COLOR** — body color 1 / body color 2 / exit |
| 11 | `GameUpdateCarShopScreen` (0x80059558) | `GameDrawCarShopScreen` (0x80059248) | `D_8009B2E8` | **SHOP** (car shop) — buy / exit |
| 12 | `GameUpdateEngineerShopScreen` (0x8005A3A4) | `GameDrawEngineerShopScreen` (0x8005A2CC) | `D_8009B2EC` | **SHOP** (engineer shop) — tune-up / exit |
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
in `D_80082EF0`: `GameDrawCarSpecGraph` (called unconditionally by
`GameUpdateMenuMode` (func_8005ACA0) every frame, but only visible on
CUSTOMIZE — see section 1), `func_8004CF30`
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

The `2.7.2` branch in `tools/scripts/cc.sh` has since been **deleted**
(`08a34db5`); `RAGE_CC1_VERSION` survives only so an experiment can point at
another cc1 by path, and any value but `2.6.3` is now a hard error. The
`RAGE_CC1_VERSION_OBJ` rules that remain in the `Makefile` all pin `2.6.3`, i.e.
they are inert and kept only as documentation of which objects were once
suspect. Note that `(cc=2.7.2)` annotations in the function table above predate
all of this and should be read as "was compared against 2.7.2 output at the
time", not as a requirement.

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
| `D_801E4388` | `g_TimeAttackCars` (this row proposed `g_CarTable3`; the tree settled on `g_TimeAttackCars`, in 4 files, and that is the current name) | per-file | 6 | save block `+0x128`, the third 13 x `GameCarEntry`; `g_CarTable` is repointed at it |
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

> **SUPERSEDED as a list.** All five have since been named:
> `D_8019CB0C` → `g_MenuAltLayout`, `D_8019C768` → `g_FrameSyncThreshold` (and
> the entry for it was wrong, not merely conservative — see below),
> `D_8009B31C` → `g_CarNamePlateStep`, `D_801E4BC8` → `g_VisibleCellList`,
> `D_801E4028` → `g_EnvSpareLerp` (18b reversed 12a's objection: the name states
> only what the flag *does*). The observations are kept; the verdicts are not
> current.

* **`D_8019CB0C` (20 files)** - an alternate menu layout selector: it shifts
  panel x from `0xA8` to `0x69`, adds a `0x2C` wide offset, and makes
  `GameDrawScriptedSprite` skip script element types 9/19/29/39. It is only ever
  assigned from `D_8009B338`, which round 2 proved is only ever written zero, so
  the alternate layout is unreachable in retail and cannot be characterised.
  Confirms and keeps the round-2 entry.
* **`D_8019C768` — DISPROVEN, it is `g_FrameSyncThreshold`.** This entry read:
  "written `0x80` on entry to eleven scenes and `0x180` in three race-side
  inits, and read in exactly one place (`GameAdvanceSaveHeaderCounter`: `+= 1`
  if `0x80`, else `+= 2`) which nothing in the image calls. Write-many,
  read-never-reached; no recoverable meaning." Both halves are wrong.

  A full objdump cross-reference finds **nineteen** references, and the load at
  `0x80016684` is inside `GameMainLoop`
  (`src/main/PAL/main/boot/GameInitSubsystems.c`):

      frameLimit = g_FrameSyncThreshold;
      while (VSync(1) < frameLimit) { }

  It is the per-frame pacing deadline in `VSync(1)` units (scanlines since the
  last `VSync(0)`): `0x80` = 128 lines, comfortably inside one PAL field, so the
  frame is not held; `0x180` = 384 lines, past one 312-line field, so the race
  scenes are held to half rate. `GameAdvanceSaveHeaderCounter` is not
  unreachable either — `GameMainLoop` calls it every frame, four lines above the
  deadline wait — and it is the confirmation rather than a curiosity: it adds
  **1** to the saved play-time counter when the threshold is `0x80` and **2**
  otherwise, exactly the compensation a one-field / two-field frame needs. It
  reaches 13 files and was the highest-fanout raw global left in this territory.

  Why three passes missed it: `GameMainLoop` sat in an address range the split
  config classified as `.rodata`, so it was not in any `.c` and no "read in the
  whole image" sweep over `src/` could see the reader. **The lesson generalises:
  a "nothing reads this" claim made by grepping `src/` is only as complete as
  the split, and needs an objdump cross-reference before it is stated as fact.**
  Sections 3, 15g and 18f repeated this entry and are corrected in place.
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

### 12e. SDK data: identified, deliberately not renamed — SUPERSEDED

> **The "deliberately not renamed" policy was reversed by section 23**, which
> named the library globals descriptively from external ground truth rather than
> waiting for Sony sources. **20 of the 21 rows below now carry a name in the
> tree** (`g_CdDebugLevel`, `g_CdCommandNames`, `g_CdIntrNames`,
> `g_CdSyncCallback`, `g_CdReadyCallback`, `g_CdSyncStatus`, `g_CdReg0`,
> `g_CdSyncResult` / `g_CdReadyResult`, `g_SpuRegBase`, `g_IrqMask`,
> `g_SndVoiceRegs`, `g_SndVoiceState`, `g_SndCurrentAttr`, `g_SndTickResolution`,
> `g_SndUpdateLock`, `g_SndMonoMode`, `g_SndCurrentProgTable` /
> `g_SndCurrentVabHeader`, `g_SndVabStatus`, …); the exception is
> `D_801F2A08` / `D_801F2A0C`, kept raw because inline-asm `%hi`/`%lo` pairs
> stringify them (23, "libsnd"). **The identifications below are still good** —
> that is what the table was for, and it is why the naming was possible at all.
> What is stale is only the "left raw" verdict.

These are Sony library globals. Their meanings are pinned - in most cases by
a public API accessor in the same file. Recorded here so the identification is
not lost.

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

`SetDefDrawEnv` at 0x80064B78 was misnamed: it writes a `disp`/`screen` Rect
pair plus `isinter`/`isrgb24` at +0x10/+0x11 — an 0x14-byte **DISPENV** — so it
is `SetDefDispEnv`. The real `SetDefDrawEnv` is func_80064AA8, which writes
clip/ofs/tw/tpage/dtd/dfe/isbg/rgb (0x1C bytes) and derives `dfe` from the buffer
height and the DMA interrupt state. `GameSetupDisplay240` calls them in exactly
that order (draw env first, disp env at base + 0x5C). **Both names are in the
tree that way now** — both definitions are in
`sdk/Gpu_LoadTexImageAndGetTPage.c`, and `psyq/gpu.h` carries the corrected
bindings.

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

`func_8006DF94` was declared `ResetCallback` in `psyq/kernel.h`; it is really
`DMACallback`, a kernel-table thunk whose wrappers pass 0, 1, 2, 3 and 4 —
MDECin, MDECout, GPU, CD-ROM and SPU. **It carries that name in the tree now**,
and the freed name `ResetCallback` belongs to func_8006DF34 (17b item 4). That
settles several names at once:

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

> Partly overtaken: the two handler tables are named — `D_8007D6B8` is
> `g_BgmSelectSteps` and `D_8007D6D0` is `g_AttractDemoSteps` — and section 17f
> named `func_80019EFC` `GameRequestTrackTexturePage`. The *functions* below are
> still raw.

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
`g_PrologueCameraCuts`. (A `g_PrologueLines` was promised here for the
`{x, y, text}` table whose strings are the RAGE RACER opening narration; it does
not exist, because the table is only ever addressed through its three column
symbols `g_PrologueLine{X,Y,Text}` — see 18d.) Race spine:
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

Verified against a full `objdump -d build/PAL/main.elf`, checking both the symbol
and the encoded `jal` word, and against the scene table `D_8007C268`. **Re-checked
at `0568a8af` and still true** — every address below has zero `jal`/`j` sites in
the linked image. The check is one command, so re-run it rather than trusting
this paragraph:

```sh
objdump -d build/PAL/main.elf | grep -cE 'jal[ \t]+8003cdf4 '
```


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

> Three entries below have since been named and are corrected in place:
> `D_8019C90C` is `g_TileStripBuffers`, `D_801E4FB4` is `g_DragScale` (it *is*
> read — see 18e) and `D_8019C768` is `g_FrameSyncThreshold` (12d).

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
- **`D_801E4194`, `D_801E8A4C`, `D_8019C998`, `D_801E4D84`** are each written but
  never read anywhere in the image (`D_8019C998` is read but only ever written
  zero). Naming them would be inventing a feature. **`D_801E4FB4` was in this
  list and should not have been:** it is read at `0x8002B840` inside
  `GameUpdateCarDrivetrain` as `drag = v^2 / (g_CarSpec->unk110 * 1000 /
  D_801E4FB4)`, and the same function resets it to 1000 immediately afterwards,
  so the writers elsewhere (`func_8002CB30`, `func_8002D398`) change the drag for
  exactly one frame. It is now `g_DragScale`.
- ~~**`D_8019C768`** keeps the entry from section 3: still only `0x80` / `0x180`
  writes and one reader that cannot pin the quantity.~~ **DISPROVEN** — it is
  `g_FrameSyncThreshold`, the frame pacing deadline read in `GameMainLoop`. See
  12d.
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

**func_8006AB5C is the libcd interrupt decoder.** It reads the 8-byte response
FIFO, decodes intr codes 1..5 into `D_80099318` and the two result buffers, and
is drained in a `while (...)` loop by `CD_sync`, `CD_ready`, `CD_cw` and
`CD_getsector2`. Its Sony name is still unknown, so it keeps the raw symbol.
(This pass found it; an earlier reading in section 7 called it "a libcd
error/trace helper" and section 7 now states this one. 17i strengthens the case
for leaving it raw.)

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
`g_PrizeAmount` and then `g_PromotionBonus` into the save block at
`g_PrizeCountStep` / `g_BonusCountStep` per frame (x4 while confirm is held).
(This paragraph originally used `g_PendingPrizeMoney` / `g_PendingClassBonus` /
`g_PrizeTickRate` / `g_BonusTickRate`; section 22 retired all four spellings and
none of them exists in the tree.)

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
  text and kept at `scratchpad/psyq_libref47.txt`. **UNVERIFIED: that file is not
  in this repo** (there is no `scratchpad/` directory) — it was a session-local
  scratch copy, so every "LibRef47 <page>" citation below is a claim about a
  document the next reader has to fetch again. The page numbers are given so it
  can be re-checked. It gives the exact signature,
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

   **Still open, and worse than recorded.** `lib/libsnd/SsUtKeyOn.c` is still
   called that, still holds func_80076B30 / func_80076C1C, and additionally
   declares and defines `void SsUtKeyOnV(void) asm("func_80076C50")` — an empty
   two-word stub. The real `SsUtKeyOnV` is func_80077C7C
   (`sdk/SsUtKeyOnV.c`, and `psyq/snd.h` binds it there). So **one C identifier
   is bound to two different addresses in two translation units**, which is the
   exact failure the alias-collision check in 19a exists to catch; it links only
   because that file includes `common.h` rather than `psyq/snd.h`, so the two
   declarations never meet. Section 22 did not reach it because it was looking at
   duplicate names for one *address*, not one name across two addresses.
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
* **`CD_namecmp` (func_8006C53C) / `LibcStrncmp` (func_8006CC8C)** — the 12-byte
  filename compare `DsSearchFile` runs over the 64-entry `CdlFILE` cache
  `CD_cachefile` fills, and the null-safe `strncmp` under it. Descriptive names
  in libcd's existing `CD_*` style; not Sony symbols. (This entry proposed
  `CD_strncmp` for the second one; the tree spells it `LibcStrncmp`, in three
  files, alongside the other `Libc*` runtime routines.)
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
  on a `func_` name** — of this pass's directories. Four `lib/` units are also
  still `func_`-named and were outside its scope: `lib/libspu/func_80078F4C.c`,
  `lib/libsnd/func_8006F90C_8006FA44.c`, `lib/libsnd/func_80070A1C.c` and
  `lib/libsnd/func_800771AC.c`.
* **`func_800271EC`** (19 words, scene-table slot 32) keeps section 13h's ruling:
  it is a per-sequence step driver dispatching `D_8007D778[g_PrologueStep]`
  (`D_801E4178`, named since), and the four steps are not identified. Its unit is named after the libcd `cdread.c`
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

---

## 18. Globals pass over `race/`, `car/` and `track/`

Section 15 named the *functions* in the three gameplay directories; their
globals were left for later. This pass closed that: of the **284** distinct
`D_XXXXXXXX` globals still spelled raw in those directories, **268 now carry a
name** and 16 stay raw on purpose (18f). Nothing but identifiers changed —
every declaration keeps its own file's type, `volatile`, array shape and
pointer-ness verbatim, with the alias appended:

```c
extern s16 D_8009B21C;   ->   extern s16 g_ChaseTargetYaw asm("D_8009B21C");
```

`make check VERSION=PAL` printed `build/PAL/main.exe: OK`, sha1
`2913e15648eddef40821c5f666460abc04155ee6`, after every batch and at the end.

**By reach and directory.** Measured *before* the pass, over the three
directories only:

| reach | candidates | named | left raw |
|---|---|---|---|
| 3+ files | 1 | 0 | 1 (`D_8019C768` — wrongly, see 12d; section 19 named it) |
| 2 files | 20 | 18 | 2 |
| 1 file | 263 | 250 | 13 |

Counted once per directory a symbol appears in: **race 77, car 51, track 145**.
The near-absence of high-reach targets is itself the result — section 12 and
section 15 had already taken every gameplay global that crossed three files, so
what was left is almost entirely the single-file tail, which is exactly the part
that turns an individual file from unreadable into readable.

### 18a. The camera work block at `0x8009B1B8` (37 names, one file)

`track/GameUpdateCamera.c` alone held 38 raw globals, all of one contiguous
block, and reading it settles what the five camera modes are.

`GameUpdateCamera(mode, object)` takes modes 0 and 1 straight from
`g_CameraViewMode`; for anything `>= 2` the mode comes from the nearest track
camera's own `+0x20` field. So:

* **mode 0** — bumper/cockpit: the eye is the car plus a rotated `(0, -0x1C0, 0)
  >> 4` and the yaw gets `car->field_8C`.
* **mode 1** — the chase view, and the only mode with smoothing. Named
  `g_ChaseTargetYaw` / `g_ChaseYaw` / `g_ChaseYawLag` / `g_ChaseYawRampNeg` /
  `…Pos` / `g_ChaseYawStepLimit` / `g_ChaseYawStep` / `g_ChaseYawDamping` /
  `g_ChaseCarSpeed`. The camera walks towards the car's heading by a quadratic
  ramp `((ramp + 8)^2) / damping`, clamped to `g_ChaseYawStepLimit` (max 0x40),
  and **the damping is a function of speed**: 222 at rest falling to 1 at
  1250 units, so the camera lags at parking speed and snaps at racing speed.
  Two ramps exist because the sign of the yaw error picks one and zeroes the
  other.
* **mode 2 / mode 4** — fixed trackside cameras; 4 dollies toward the car over
  `node->duration` frames.
* **mode 3** — a keyframed camera *around* the car. Named `g_CamPath*`: an
  offset triple and a pitch/yaw/roll/distance quad, each with a `Start`, a
  `Delta` and a current value, eased by `1 - cos()` over `node->duration`.
  This proves the 0x24-byte track-camera record is a **union**: modes 2 and 4
  read `+0x00..0x0C` as a world position, mode 3 reads the same four words as
  angles + distance (they are wrapped to +-0x800, a position never would be).
* **mode 5** — a rear/orbit camera at a fixed yaw and pull-back.

`g_ChaseCameraPreset` (`D_8007F610`), `g_OrbitCameraYaw` (`D_8007F614`) and
`g_OrbitCameraDistance` (`D_8007F618`) are `.data` constants **with no writer
anywhere in the image**: 0, 0 and 330. So the three chase distances
(eye 0x3A/0x59/0x97, pull-back 0x118/0x140/0x190) are authored but retail always
uses the nearest one.

### 18b. The environment colour timeline, finished

Section 12a described the nine `{ cur, from, to }` RGB slots at `D_801E3FB6` but
left them raw. All 27 are now `g_EnvFogColor{,From,To}` and
`g_EnvColor1..8{,From,To}`, plus `g_EnvSpare` for the fourth byte of slot 0.
Reading `GameUpdateEnvironment`'s eight `GameLerpEnvColor` calls adds one fact
section 12a did not have: **slots 5/6 and 7/8 are alternates, chosen by
`g_CourseIndex == 2`** — only six of the nine slots are ever lerped on a given
course.

`D_801E4028` is now `g_EnvSpareLerp`, reversing 12a's decision to leave it raw.
12a's objection was that naming it asserts what the flag is *for*; the name
chosen asserts only what it *does* — it is the gate on the `g_EnvSpareFrom ->
g_EnvSpareTo` lerp of `g_EnvSpare`, which is provable and complete. Same
mechanical style as `g_IsEnvironmentMode4`.

`D_8009B24C` is `g_FogNear`, `D_801E40E8` is `g_EnvScriptCursor`.

### 18c. Two controller findings

**The live button mapping is `D_801E4B60[16]`.** `GameLoadPadButtonMapping`
copies one of eight authored presets per controller into it: the standard pad's
eight `u16` masks at `+0x00`, the NeGcon's eight at `+0x10`. Reading the
consumers assigns every slot a job:

| slot | name | evidence |
|---|---|---|
| 0, 1 | `g_PadButtonMapping[0]` / `[1]` (this pass named them `g_PadSteerLeftMask` / `g_PadSteerRightMask`; section 22 folded both into the aggregate and neither spelling is in the tree) | `g_MirrorMode` swaps exactly these two |
| 2, 3 | `g_PadAccelMask` / `g_PadBrakeMask` (`g_Negcon…` for row 1) | drive `accelBtn` / `brakeBtn` |
| 4, 5 | `g_PadShiftMasks[type][0/1]` | gear up / gear down, hence the 8-halfword row stride |
| 6 | `g_PadMirrorMasks[type * 8]` | held + D-pad up/down toggles `g_MirrorViewEnabled` while paused |

**The NeGcon analog channels are I, II and shoulder L.** `func_80014014` writes
`raw[5] - centre` to `g_NegconAnalogI`, `raw[6] - centre` to `…II` and
`raw[7] - centre` to `…L`, and the *digital* pad path writes a flat **0x6A**
into the same three slots when the mapped button is held. That constant is what
proves the scale: every consumer divides by 106. The eight-way
`g_NegconConfigIndex` switch then reads as real control layouts (I=throttle
II=brake, swapped, brake-only on L, II+L, …).

`g_NegconSteer` is the twist after the calibrated centre, a dead zone and a
clamp to `g_NegconSteerRange[…]` = `{25, 38, 75, 113}`, which the save file
keeps. A higher setting needs more twist for full lock.

**The index is `g_NegconMaxTwist` (`D_801E418C`), not `g_NegconSteerPlay`.**
This paragraph originally said "the 0..3 setting the NEGCON STEER PLAY screen
edits", and `car/GameInitPlayerCar.c` declared the symbol `g_NegconSteerPlay` to
match. Both were wrong; the image settles it at `0x80014464`, where the two
settings index two different tables at two different strides:

    lh   v0, D_8019CAD0        # NEGCON STEER PLAY, 0..3
    sll  v0, v0, 2             # stride 4
    lhu  v0, 0x8007C128(at)    # subtracted from / added to the raw twist
    ...
    lh   v0, D_801E418C        # MAXIMUM TWIST, 0..3
    sll  v0, v0, 1             # stride 2
    lh   v0, 0x8007C020(at)    # g_NegconSteerRange, the clamp

`D_8019CAD0` is the dead zone (hence "play"), edited by
`GameUpdateNegconSteerPlayScreen` and drawn against `g_NegconPlayPercent`;
`D_801E418C` is the twist range, edited by `GameUpdateNegconMaxTwistScreen`,
backed up in `g_NegconMaxTwistSaved`, and divided into the twist by
`GameInitPlayerCar`. Every other file already called `D_801E418C`
`g_NegconMaxTwist`; `car/` was the only dissenter and is fixed. Found by the
alias-collision check 19a asks for (one name per address, one address per name).

### 18d. Judgement calls, and the readings they beat

* **`D_8009B1EC` left raw although its whole block is named.** The slot carries
  the mode-3 camera path's *yaw delta* and the mode-1 chase camera's *previous
  yaw*. Both roles are live, in the same function, and no name is honest for
  both, so the block has a hole and a comment saying why. Aliasing one address
  to two identifiers in one TU would compile, but it would be worse to read.
* **`D_801E40B8` left raw in `car/GameUpdateCarTrafficAvoidance.c`.** It is
  `g_RankedCars - 1` and the walker never loads the byte at that address. But
  `0x801E40B8` already has a name for what lives there — `g_SceneTimer` — so a
  second alias would tell a reader the scene timer is a car pointer. Raw plus a
  comment beats both.
* **`D_8019CAB0` -> `g_TrackZoneDark`, not `g_InTunnel`.** `GameGetTrackZoneBlend`
  sets it to 3 when the player is inside a zone whose code is 0; its only reader
  is `GameDrawPlayerTachometer`, where it forces dial mode 2 — the mode the
  time-of-day test otherwise only picks at night. "Tunnel" is the obvious guess
  and probably right, but the executable only proves "a zone that wants the lit
  dial".
* **`D_801E8AA0` -> `g_ShiftSoundLevel`, not `g_WheelspinLevel`.** It is
  `shiftTick & 0x3F` latched when a gear change lands under power, and its only
  effect is `func_8005C104(0, 0x1800, level + 25)` while the car is airborne.
  The trigger is a shift and the effect is a sound volume; the *content* of
  continuous sound 0 is on the disc, so the name does not claim it is a squeal.
* **The lowercase captions.** `D_80010E1C` etc. contain bytes like `"hai"`,
  `"hegi"`, `"hfgi"`, `"hci"`. `func_80016B7C` routes 'a'..'u' through a
  separate word-sprite bank (4-byte records at `0x8007C438`) and 'v'..0xFF
  through a second one at `0x8007C460`, so these are pre-rendered captions whose
  artwork is on the disc. They are named for **what they label**, which the call
  sites do prove: `"hegi"` is always immediately followed by the total time
  (`"T/…"`) and `"hfgi"` by the per-lap list, in two independent panels, so
  `g_CaptionTotalTime` / `g_CaptionLapTime`; `"hai"` heads the five ranking rows,
  so `g_CaptionRanking`; and in the prize panel `"hci"`, `"hebi"`, `"hji"` are
  each followed by `%dv` of `g_PrizeAmount`, the running total and
  `g_PromotionBonus`.
* **`D_801E4112` / `D_801E4114` are one table read two ways.** `func_8002C478`
  fills entry `b` with how many points of a car-spec rpm curve sit at or below
  rpm band `b` (`band = rpm / 1000`); `D_801E4112` is that same table one
  halfword earlier. So `[g_TorqueBandStart[b], g_TorqueBandEnd[b])` is the search
  range, which is why two symbols two bytes apart are both indexed by `b`. Same
  shape at `D_801E4152` / `D_801E4154` for the second curve.
* **`D_8007D6DC/DE/E0` are `g_PrologueLine{X,Y,Text}`**, split symbols of one
  14-entry 8-byte table `{ s16 x, s16 y, char *text }`. The table is addressed
  only through the three column symbols, so those are what exist; there is no
  `g_PrologueLines`.
* **Split symbols named as such, not invented as new objects.**
  `g_Shuttle1*` (`g_ShuttleScenery[1]`'s fields), `g_CarProgressA/B`,
  `g_CarTrackProgress`, `g_CarTrackSection`, `g_CarMarkerIndex/Flag` (bases of
  0x19C-stride walks over `g_Cars`), `g_BestSectorTime1/2`,
  `g_RankingCars`, `g_TimeRecordTimes/Cars`, `g_ClassRecord5/6`, `g_ClassClears`,
  `g_PrizeMoney3rd`, `g_DefaultRecordTimes/Cars`, `g_ShuttlePath2Points`,
  `g_StaticSceneryYaw`, `g_SpinningSceneryYaw`, `g_EnvFogColorG/B`.

### 18e. Corrections to earlier sections

* **`D_801E4FB4` is `g_DragScale`.** Found here; folded into the 15g entry that
  claimed it was never read, so the document states it once.
* **A second `%hi`/`%lo` trap, in this territory.** `LA_ORDERED(dst, sym, dep)`
  in `include/asm_macros.h` **stringifies** its symbol into an inline-asm `la`.
  `track/GameSeekEnvironmentScript.c` uses it on `D_801E6DA4` (the 16-entry sky
  CLUT), so that symbol must keep the raw spelling exactly like the
  `%hi`/`%lo` cases in 12c. Renaming it links clean at compile time and fails at
  link. Any future pass touching a `LA_ORDERED` argument must know this.
* **`include/game/sound.h` claims `D_801E6DA4` as "+0x24 s16 table" of the sound
  work area.** Its only user in the tree is the sky-CLUT upload. The header
  declaration is left alone (it is another pass's territory) but the
  identification is recorded here as doubtful.
* **`D_801E4112`/`D_801E4152` overlap `D_801E4110`**, which section 12e
  identifies as libsnd's open-VAB `ProgAtr` pointer. Both cannot be live at
  once. The drivetrain reading is direct (`func_8002C478` builds the tables, and
  the disassembly shows the `addiu`s), so the 12e entry is the one to re-check.

### 18f. Left raw on purpose

| symbol | why |
|---|---|
| ~~`D_8019C768`~~ | **DISPROVEN — it is named.** This row said "section 12d; unchanged". It is `g_FrameSyncThreshold` (12d) and carries that name in 13 files. |
| `D_8009B1EC` | two live roles in one slot — see 18d |
| `D_801E40B8` | already named `g_SceneTimer`; used only as `g_RankedCars - 1` |
| `D_8019CB38` / `D_8019CB3A` | referenced from `%hi`/`%lo` inline asm (12c). Since named `g_PaintBlendShade0` / `…1` for the *C* uses in the same file; the inline-asm references still spell them raw, which is the point of the entry. |
| `D_801E6DA4` | referenced from a `LA_ORDERED` inline asm — see 18e |
| `D_8019C9AC` | both writes in the image store zero, so the one reader (skip the pad, freeze the steering) can never fire |
| `D_8019C998` | same shape: initialised to zero, only ever decremented |
| `D_801E4194`, `D_801E4248`, `D_801E4CF8`, `D_801E4D84`, `D_801E433C`, `D_801E3F60`, `D_801E8A4C` | written, never read anywhere in the image |
| `D_8009EC88` | its only reader is the guard on its own write, so it has no effect — and it lives in the unreachable waypoint mode (15f) |

## 19. Front-end globals pass (`menu/ save/ asset/ cd/ fmv/ audio/ pad/ boot/`)

Function naming was essentially finished; globals were not. 374 globals carried a
name and ~1040 were still raw `D_XXXXXXXX`. This pass took the eight front-end
directories and named **259 more**, bringing the project total to **633**.
Everything is an `asm()` alias with the file's own declaration type left
byte-for-byte alone, so `make check VERSION=PAL` stayed at
`build/PAL/main.exe: OK`, sha1 `2913e15648eddef40821c5f666460abc04155ee6`,
verified after every batch with the affected objects deleted first.

Of the 259: **54 reach three or more files, 71 reach two, 134 are single-file.**
By directory: `save` 76, `menu` 64, `audio` 50, `fmv` 35, `boot` 25, `cd` 23,
`pad` 19, `asset` 11 (a symbol used from two directories is counted in both).

### 19a. Corrections to earlier sections

1. **`D_8019C768` is `g_FrameSyncThreshold`.** Found by this pass; the evidence
   and the reason section 12d got it wrong are folded into 12d itself, so the
   document states it once.

2. **One existing name was ambiguous**: `D_8007C464` / `D_8007C474` had been
   named `g_CarModelBaseIndex`
   / `g_CarModelUnlockBase` in `car/`. This pass initially coined
   `g_CarAssetBase` / `g_CarUnlockLevelBase` for the same two symbols in
   `asset/GameGetCarAssetIndex.c`; that was caught by an alias-collision check
   and reverted to the existing spellings. **A per-file *type* is allowed; a
   per-file *name* is not.** The check is worth keeping: grep every
   `NAME asm("D_XXXXXXXX")` in `src/` + `include/` and assert one name per
   address.

### 19b. `cd/` — the CD-DA mixer, and the ten looping BGM tracks

`CdlATV` (LibRef47 10-3) is `{val0 L->L, val1 L->R, val2 R->R, val3 R->L}`, and
that pins the whole mixer:

| address | name | evidence |
|---|---|---|
| `D_8007F5A8` | `g_CdMixPresets` | `u8[2][4]` in `.data`: `{0x7F,0,0x7F,0}` (stereo) and `{0x3F,0x3F,0x3F,0x3F}` (mono fold-down) |
| `D_8007F5FC` | `g_CdMixPreset` | the row index; `GameSetStereoOutput` writes 0, `GameSetMonoOutput` 1 |
| `D_8009B174..80` | `g_CdMixLL/LR/RR/RL` | the live 12.12 values; `CdMix` gets `>> 12` |
| `D_8009B184..90` | `g_CdMixFullLL/...` | the same four at full setting volume; a fade-in ramps live toward these |
| `D_8009B1B4` | `g_CdFadeFrames` | `> 0` frames left of a fade-out, `< 0` of a fade-in, clamped to ±0xFFF |

`D_8007F568` is `g_CdAudioFileNames` — sixteen `"\CDDA\DAnnXXXX.DA;1"` paths in
track order — and `GameBuildCdTrackTable` `DsSearchFile`s each one into
`g_CdBgmTrackLocs`, which is `&g_CdTrackLocs[2]`, so **`g_CdTrackLocs` is indexed
by CD track number**: track 2 is `DA02PRO`, track 17 is `DA17EXTR`.

That settles `D_8007F5B0` = **`g_CdTrackLoopPoint`**, eighteen `CdlLOC` in
`.data` that are `0:00` for every track except 3..11 and 17, which are `5:00`.
Those nine plus one are exactly `g_BgmTrackCount`'s `9`, or `10` once five class
records are grade 1 — independent confirmation from two directions.
`GameTickCdAudio` uses "entry is non-zero" (spelled as a `CdPosToInt` compare
against entry 0) as "this track loops", and `GameStepCdPauseRequest` uses the
5:00 value as a real threshold: `D_8007F5F8` = **`g_CdRestartOnResume`** is set
when the pause happened past 5:00 into a looping track, and `GameResumeCdAudio`
then re-issues a play from the top instead of a `CdlPlay` un-pause.

`D_8009B168` stays raw: its only reference is an inline-asm `%hi`/`%lo` pair
(see 12c). It is the `CdlSetmode` byte, `0x07` = `CdlModeDA | CdlModeAP |
CdlModeRept`, which is why `GameTickCdAudio` can detect end-of-track as status 4.

### 19c. `audio/` — the engine-sound curve table

`GameLoadAudioParameterTable` (in `menu/GameUpdateMenuMode.c`) writes the shape
out longhand: **`g_EngineSoundCurves` (`D_801E446C`) is `[2 banks][12][9][2]`** —
bank stride `0x360`, row stride `0x48`, nine `{position, value}` breakpoints per
row — and its last word becomes **`g_EngineSoundMaxRpm`** (`D_801E6CC4`, clamped
to `1..0x2800`). `GameUpdateLoadedAudioVoices` is called from
`GameUpdatePlayerCar` as `(g_EngineRpm + g_EngineRpmJitter, bank)` and normalises
that onto `0..10240` with `((rpm * 5) << 11) / g_EngineSoundMaxRpm`, so the twelve
rows are six sound slots × {pitch curve, volume curve} and the domain is engine
revs. Hence `g_EngineSoundPosition` (`D_801E6CB8`) and `g_EngineSoundBank`
(`D_801E6CBC`, latched so a bank change re-keys the six voices).

Also named here: `g_SoundSlotActive[6]` (`D_801E6CC8`), `g_SoundSlotVolumeScale`
(`D_801E6CE0`, which is `g_SoundSlotActive[6]` read as `base[6]` in two files and
is the per-car engine loudness from **`g_CarSoundVolumeScales`**, `D_800125FC`,
indexed the same way `g_CarPriceTable` is), the pan voice (`g_PanVoiceVolumeR`,
`g_PanVoiceActive`), the indexed effect voice (`g_IndexedEffectIndex` / `…Prev` /
`…Pitch` / `…Volume` over the 3×3-word table `g_IndexedEffectTones` /
`g_IndexedEffectVolumes`), the VAB loader (`g_VabSpuAddress` =
`{0x1000, 0x20000, 0x20000}` + `g_VabSpuAddressExtra` = `0x6A000`,
`g_AudioLoadSlot`, `g_VabTransferDone`, `g_ExtraVabLoaded`), and the sequence
side in `game/sound.h`: `g_MusicChannels`, `g_EffectVoices`, `g_ReverbDepthL/R`,
`g_SeqVolume`, `g_SeqVolumeSetting`, `g_SeqVolumeFadeStep`.

**`D_801E6D8C` deliberately keeps the raw spelling in `game/sound.h`.** As a
value it is `g_ReverbFadeStep` (the per-frame delta `GameUpdateSequenceFadeOut`
adds to `g_ReverbDepthL/R`, `-3` while a fade runs), and it is named that in the
two files that read it as one. But `GameForceBasicEffectVoicesEnabled` also uses
`&D_801E6D8C` as the **end address of `g_EffectVoices`** (`= &g_EffectVoices[4].pitch`),
and spelling that loop bound `&g_ReverbFadeStep` would actively mislead. The
header says so at the declaration.

`D_801E6CAE/B0/B2` are `g_VabIds3/4/5`, split symbols of `g_VabIds` — the same
`+0xC` off `&g_AudioSlotMask` that `GameCloseAudioSlot` indexes. Named
mechanically on purpose (the `g_RefSectorTime0/1/2` precedent of 15b), because
"the extra VAB" is not one slot: the loader writes slot 3 and the closer closes
slot 5.

### 19d. `pad/` — the NeGcon calibration chain

`GameUpdateNegconNeutralScreen` latches four raw axis bytes out of the BIOS pad
buffer and every one of the six persisted settings falls out of it:

| address | name | how |
|---|---|---|
| `D_801E4040..43` | `g_NegconAxisSteer/I/II/L` | `g_PadBuffers + 4..7`, the NeGcon's twist and three analog buttons |
| `D_801E4BF0` | `g_NegconSteerNeutral` | `= g_NegconAxisSteer - 128`, the signed centre offset |
| `D_8019CA08/0A/0C` | `g_NegconNeutralI/II/L` | the other three, latched raw |
| `D_8019CAD0` | `g_NegconSteerPlay` | the 0..3 dead-zone setting mode 10 edits; indexes `g_NegconPlayPercent` (`D_8007C260`) for the gauge |
| `D_801E418C` | `g_NegconMaxTwist` | the 0..3 range mode 11 edits; `car/GameInitPlayerCar` divides by `D_8007C020[…]` with it |

`GameBeginNegconCalibration` parks all six in `g_NegconSteerNeutralSaved`,
`g_NegconNeutralISaved/IISaved/LSaved`, `g_NegconSteerPlaySaved`,
`g_NegconMaxTwistSaved` so a cancel restores them, and the two button-mapping
selections in `g_PadMappingIndexSaved` / `g_NegconMappingIndexSaved`. The live
selections are `g_PadMappingIndex` (`D_8019CE08`) and `g_NegconMappingIndex`
(`D_8019CB08`); `GameLoadPadButtonMapping` copies one row of
`g_PadButtonPresets` / `g_NegconButtonPresets` (8 presets × 8 masks each) into
`g_PadButtonMapping` (`D_801E4B60`, pad masks at +0, NeGcon at +0x10).

**`D_801E8AA4` and `D_801E8A9C` are rotation angles, not counters.**
`GameDrawControllerSetupScene` feeds them to `GameBuildRotMatrixY(m, angle+1024)`
and `GameBuildRotMatrixX(m, angle-64)`, so they are
`g_ControllerSceneAngleY` / `g_ControllerSceneAngleX`. A left/right press adds
±2048 (half a turn) to the Y angle and it decays `*15/16` each frame; that is why
`GameDrawPadConfigCallouts` draws only while `|angle| < 16`, i.e. once the
diagram has settled. Its companions are `g_PadConfigFlipTimer` (`D_801E7A4C`, a
30-frame window) and `g_PadConfigFlipPhase` (`D_801E6C7C`, `(timer >> 2) & 1`).

### 19e. `save/` — the memory-card menu, and play time

`GameOpenMemoryCardEvents` opens eight handles as `D_8009B538[0..7]`, so the four
`Hw` and four `Sw` slots that 17b identified now carry the spec in the name:
`g_McHwEventIoe / …Error / …Timeout / …New` and the `g_McSw…` four. The same
address is declared `s32 g_McEvents[]` in `GamePollMemoryCardStatus.c`, which
indexes it — a textbook 12c per-file type.

`GameEnterMemoryCardMenu` and `GameEnterMemoryCardMenuFromLoad` differ in exactly
three writes, which names the whole menu: `g_McMenuRowCount` (`D_8009B744`, 2 or
3), `g_McMenuRowCursor` (`D_80082F54`, 0 or 2) and `g_McFromLoadMenu`
(`D_8009B730`). `g_McMenuPage` (`D_80082F50`) is 0 for row-select and 1 for the
save/load action machine, whose sub-state is `g_McActionState` (`D_80082FA4`),
countdown `g_McActionTimer`, result `g_McActionResult`, target slot
`g_McSlotCursor`, and whose direction is `g_McSaveMode` (`D_8009B734`: the
zero branch prompts LOAD, the non-zero branch prompts OVERWRITE).
`g_McFadeStep` / `g_McFadeLevel` (`D_8009B9A0` / `D_8009B9A4`) are the ±8 and
0..0xFF the entry and exit fades ride; `g_McSlotUsedMask` (`D_8009B564`) is
`GameRefreshMemoryCardSaveStatus`'s return, tested as `(mask >> slot) & 1`;
`g_McSaveHeaders` (`D_8009B568`) is the `GameSaveHeaderRow[3]` the whole file
indexes with `slot << 7`; `g_McCardFileCount` / `g_McFreeBlocks`
(`D_8009B738` / `D_8009B73C`) come straight out of that function.

**`D_801E7A54` is `g_SaveElapsedTicks`**: `GameAdvanceSaveHeaderCounter`
increments it once per frame, `GameWriteSaveHeaderRow` stores it in the header,
and `GameFormatSaveElapsedTime` prints it as `"%5d:%02d:%02d"` — the play time on
the memory-card screen. See 18a for why its increment is 1 or 2.

The three `GameRaceProgress` slots are now readable end to end. `game/race.h`
already typed the struct; its fields 1..4 exist as separate symbols in the two
serialisers, and they are named per slot:
`g_GrandPrixSaveCar/Class/MaxClass/Time` (`D_801E4098..A4`),
`g_ExtraGrandPrixSave…` (`D_801E6E80..8C`) and `g_TimeAttackSaveCar/Class/
MaxClass/Series` (`D_8019C984..990`) — the last one is `Series` rather than
`Time` because `race.h` already records that Time Attack reuses `unk10` for
`g_GrandPrixSeries`.

### 19f. `fmv/` — the decode context is one struct

`GameStartFmvPlayback` passes `&D_8009AF20` as the context to
`GameInitFmvContext` / `GamePresentFmvFrame`, and those index it as
`ctx[0..5]`, so the block is one record and the raw symbols are its fields:
`g_FmvVlcBuffers` (`ctx[0..1]`), `g_FmvVlcIndex` (`ctx[2]`),
`g_FmvStripBuffers` (`ctx[3..4]`), `g_FmvStripIndex` (`ctx[5]`).
`GameSetupFmvBuffers` lays the memory out from one base and names the templates:
`g_FmvVlcBuffer0/1` at `+0` / `+0x28000` (the VLC-expanded bitstream `DecDCTin`
consumes), `g_FmvStripBuffer0/1` at `+0x50000` / `+0x52D00` (the MDEC output
strips `DecDCTout` fills) and `g_FmvRingBuffer` at `+0x55A00` (`StSetRing`).
With them: `g_FmvStripRects` / `g_FmvStripRectIndex` / `g_FmvUploadRect`
(+ its `X`/`Y` split symbols) / `g_FmvStripWidth` / `g_FmvStripHeight` /
`g_FmvStripDone`, `g_FmvFrameWidth` / `g_FmvFrameHeight` and
`g_FmvStreamEnded`. `g_FmvState` (`D_8009F094`) is the 0/1/2 state `state.h`
already documents; `g_StreamLoc` (`D_801E8A90`) and `g_StreamSectorCount`
(`D_8019CA1C`) are the `CdlLOC` and length the three `GameBegin*Fmv` wrappers
copy out of `g_StreamCdEntries`.

`D_8019CE94..9A` and `D_801C067C..82` are the `DISPENV` rect of frame contexts 0
and 1 — `g_FrameContexts + 0x5C` and `+ 0x237E8 + 0x5C` — so they are
`g_DispEnv0X/Y/W/H` and `g_DispEnv1X/Y/W/H`; `D_8019CE50` / `D_801C0638` are
`+0x18` of the `DRAWENV` (`g_DrawEnv0Dither` / `g_DrawEnv1Dither`) and
`D_8019CEA5` / `D_801C068D` are `+0x6D` of the `DISPENV`
(`g_DispEnv0Rgb24` / `g_DispEnv1Rgb24`, set to 1 for the 24-bit FMV mode).

### 19g. `boot/` and `menu/`

`GameMainLoop`'s locals are all named now: `g_FrameContexts` (the two
0x237E8-byte contexts), `g_FrameParity`, `g_FrameCounter`, `g_GameClock` (the
running `1 + elapsed/256` accumulator) and `g_SceneHandlers` — the table 14a
documents, finally spelled that way at its only dispatch site. `g_BootLogoState`
/ `g_BootLogoTimer` / `g_BootLogoHoldTimer` drive the logo scene;
`g_DefaultColorMatrix` / `g_DefaultLightMatrix` are the two constant matrices
`GameInstallSceneLighting` copies into `g_SceneColorMatrix` and
`g_SceneLightMatrix`.

In `menu/`: `g_PrevOwnedCarIndex` / `g_NextOwnedCarIndex` (`D_8019CA18` /
`D_801E41A4`) are the scan up and down `g_CarTable` for the nearest `enabled`
car — up moves the car-shop cursor on D-pad up, down on D-pad down, and both
light the browse arrows. `g_CarSwapFromIndex` / `g_CarSwapToIndex`
(`D_8009B374` / `D_8009B378`, `-1` idle) are the showroom turntable's source and
destination while it spins between two cars; `g_CourseSwapDelay` (`D_8009B354`,
0..19) is the equivalent hold at the far side of the course carousel before
`g_MenuCourseModelIndex` takes `g_MenuPendingCourseIndex`.
`g_MenuHintBarScript` / `g_MenuHintBarProgress` / `g_MenuHintBarStep` /
`g_MenuHintButtonsVisible` are the bottom button-hint bar that
`GameUpdateMenuMode` runs after every screen handler.
`g_FrontendState` (`D_8009F098`, 0..3 through `g_FrontendDrawHandlers`),
`g_TitleAttractTimer` (`0x190` frames), `g_TitleExitTimer` (`0x1E`),
`g_TitlePulse` and `g_MainMenuSlide` cover the title screen;
`g_ScreenOffsetEditX/Y` are the live copies the screen-adjust panel edits before
committing to `g_ScreenOffsetX/Y`.

`g_GrandPrixSeriesU16` in `menu/GameDrawNowLoadingText.c` is the one place the
12c "same name, per-file type" rule could not be applied literally: that file
includes `game/race.h`, which already declares `g_GrandPrixSeries` as `s16`, and
the unit needs a `u16` view of the same address. A second C spelling was the only
way to keep both the type and the compile.

### 19h. Left raw on purpose

* **`D_8009B33C`, and its neighbours in the menu-mode block.** Written `0` by
  `GameInitMenuMode` and *never* written anything else in the whole image (four
  references, one store, three loads), so the branches it guards in
  `GameUpdateRankingScreen` and `GameUpdateLogoSampleScreen` are unreachable in
  retail. Exactly the reasoning that left `D_8009B338` and `D_8009B31C` raw in
  rounds 2 and 3.
* **The seven per-screen "secondary timed-draw script" pointers**
  (`D_8019C764`, `D_801E40B4`, `D_8019C794`, `D_801E8A44`, `D_8009F0B0`,
  `D_8019CB00`, `D_801E4188`). `GameInitMenuMode` seeds all seven to
  `&D_80082568` and each is then repointed by exactly one screen handler at a
  different script table. The mechanism is certain; which *panel* each one draws
  is not, because the tables are `GameRunTimedDrawScript` records whose contents
  are sprite rows, so a name would assert artwork this repo cannot see.
* **`D_8019C754` → `g_AssetBlockPtr2`, chosen mechanically.** In `asset/` it is
  sub-block 2 of a pack and becomes `GameStartAudioSlotLoad`'s fourth argument
  (the SEQ pointer for slot 1, the parameter table for slot 3); in
  `car/GameLoadUpgradedCarModel` it is a plain load destination. No single
  semantic name covers all four uses, so the name says only that it is another
  asset-block cursor alongside `g_AssetBlockPtr` and `g_AssetSubBlockPtr`.
* ~~**`D_8019CA00`** is read by `GameUploadFmvSlice` but is libds state owned by
  `sdk/CdRead2.c`; 12e's rule keeps SDK globals raw.~~ **Overtaken:** section 23
  dropped 12e's rule and named it `g_StInterruptPending`.
* **`D_801E4D14`, `D_8019CB10`** — written by `GameInitSubsystems` and otherwise
  touched only inside `GameUpdatePadState`, which is still `INCLUDE_ASM`. There
  is nothing to read them against until that unit is decompiled.
* **`D_8019C86C` / `D_8009EC94`** (the main-view visible-cell mask and list that
  `GameInitRenderState` points `g_VisibleCellMask` / `g_VisibleCellList` at) are
  referenced from `asset/` but belong to `track/` and `render/`. Left for the
  pass that owns those directories, so the two agents cannot coin different
  names for the same address.
* **The `D_80082FBC/C0/C8` tail of the memory-card action machine**, the
  `D_8009B36x` menu-mode accumulators and the remaining single-file `D_8009Bxxx`
  block: each is a private counter inside one screen, readable in place, and
  naming them would mostly restate the surrounding code.

After this pass 232 raw globals are still referenced from the eight front-end
directories: 10 reach three files, 38 reach two, and 184 are single-file.

## 20. Types pass: aggregation and semantics

Sections 18 and 19 gave every reachable global a name. What a name cannot say
is that a *run* of globals is one object, and what byte-exactness does not pin
is the meaning of a value whose width is already forced. This pass took the
largest runs of consecutive named globals and tried to collapse each into the
struct or array it really is, then annotated the values whose unit the code
proves.

Width and signedness were not revisited: byte-exactness already fixes them at
every site that really loads or stores, and same-width spellings (`int`,
`long`, `s32`) compile identically here. Aggregation is different — it can
change codegen, so **every conversion was verified on its own** with
`make check VERSION=PAL`, the affected object deleted first, and reverted if
the sha1 moved off `2913e15648eddef40821c5f666460abc04155ee6`.

### 20a. Aggregations that hold

| run | was | is now | where |
|---|---|---|---|
| `D_801E3FB6` 27 words | 27 `PackedWord` externs | `GameEnvColorSlot g_EnvColors[9]`, each `{ cur, from, to }` of `GameEnvColor` | `include/game/render.h`; users `track/GameLoadEnvironmentCue.c`, `track/GameSeekEnvironmentScript.c` |
| `D_8009B1B8/C8/D8` | `g_CamPathOffset{,Start,Delta}{X,Y,Z}` | `s32 g_CamPathOffsetDelta[3]` / `…Start[3]` / `g_CamPathOffset[3]` | `track/GameUpdateCamera.c` |
| `D_8009B1E8/F8/B208` | `g_CamPath{Pitch,Yaw,Roll,Dist}{Delta,Start,}` | `s32 g_CamPathAngleDelta[4]` / `…Start[4]` / `g_CamPathAngle[4]` + `CAMPATH_PITCH/YAW/ROLL/DIST` | same |
| `D_801E4DC8` | `g_PathSceneryRot{X,Y,Z}` and a `Blk8` under a fourth name | one `g_PathSceneryRot`, `s16[3]` in the drawer and `Blk8` in the initialiser | `track/GameDrawPathScenery.c`, `track/GameDrawScriptedScenery.c` |
| `D_801E4DD0` / `D_801E4DD8` | `g_PathScenery{Rot,}HalfDelta{X,Y,Z}` | `s16 g_PathSceneryRotHalfDelta[3]` / `g_PathSceneryHalfDelta[3]` | `track/GameDrawScriptedScenery.c` |
| `D_8019C7D4` 8 halfwords | `g_TachoNeedleQuad{X,Y}{0..3}` | `s16 g_TachoNeedleQuad[4][2]` | `car/GameGetTrackSurfaceHeight.c` (writer) and `race/GameDrawWrongWayWarning.c` (reader) |
| `D_801E4094`, `D_801E6E7C`, `D_8019C980` | three `s32` bases plus 12 split field symbols | three `GameRaceProgress` objects, fields as members | `include/game/race.h`; `save/GameStoreSaveStateBlock.c`, `save/GameLoadSaveStateBlock.c`, `menu/GameUpdatePrizeMoneyScreen.c`, `menu/GameEnterFrontend.c` |
| `D_8009AF84/88` | `g_SectorTime1/2` | `g_SectorTimes[1]/[2]` | `race/GameUpdateLapAndFinish.c` |
| `D_801E41EC/F0` | `g_BestSectorTime1/2` | `&g_BestSectorTimes[0][0][1]/[2]` | same |

Sixty-eight declarations become nineteen. Three things are worth recording
about *why* these held where others did not:

* **Arrays are safer than structs, and a base+index is safer than a member.**
  An array subscript produces an ordinary `MEM`; a struct member reference sets
  `MEM_IN_STRUCT_P`, which feeds gcc 2.6.3's aliasing decisions. The one struct
  conversion that held (`GameRaceProgress`) is in two straight-line serialisers
  whose only other memory traffic is through a `s32 *` the compiler cannot
  disambiguate anyway.
* **Half-aligned words survive if the member is packed.** `g_EnvColors` starts
  at `0x801E3FB6`, i.e. 2 mod 4, so every word in the block is loaded with
  `lwl`/`lwr`. Wrapping the `u32 __attribute__((packed))` in a struct keeps the
  aggregate's alignment at 1 and the same instruction pair comes out.
* **A per-file *type* is still allowed, and now carries the aggregation.**
  `D_801E4DC8` is `s16 g_PathSceneryRot[3]` in the drawer and `Blk8
  g_PathSceneryRot` in the initialiser that copies the whole keyframe; one
  name, two views. Same for `g_FmvUploadRect` (a `Rect`) against its volatile
  `X`/`Y` halves.

`g_CamPathAngleDelta[CAMPATH_YAW]` incidentally closes the hole section 18d
left open. `D_8009B1EC` carries the mode-3 camera path's yaw delta *and* the
mode-1 chase camera's previous yaw, and 18d left the whole slot raw because no
single name is honest for both. The array names only the mode-3 role, which is
the honest half; the mode-1 code keeps the raw spelling, and the two now sit
side by side with a comment saying so.

### 20b. Aggregations that do not hold, and what the compiler did instead

* **`g_RefSectorTime0/1/2` (`D_8009AF90`) cannot become `g_RefSectorTimes[k]`**
  in `race/GameUpdateLapAndFinish.c`. With three symbols gcc emits three
  independent `%hi`/`%lo` pairs. With one array symbol it CSEs the base into a
  register, and the resulting live range reshuffles the register allocation of
  the whole surrounding block — the diff is not just the relocation form, it
  moves `lui a1` to `lui a2` and inserts an `addiu` that materialises the base.
  The `g_SectorTimes[1]/[2]` fold in the same function is fine, because
  `g_SectorTimes[0]` is already spelled that way there, so nothing changes
  about how many bases are live.
* **The eight libcard event descriptors at `D_8009B538` cannot become
  `g_McEvents[k]`** in `save/GameClearMemoryCardHwEvents.c`, even though
  `save/GamePollMemoryCardStatus.c` already opens them as an array. The four
  pollers call `TestEvent` in a row; with an array base gcc keeps that base in
  a callee-saved register across the calls, and
  `GamePollMemoryCardHwEvent`'s frame grows from 24 to 32 bytes. The eight
  scalars stay, with the array layout and the index-plus-one return convention
  written at the declaration.
* **`g_Shuttle1*` cannot become `g_ShuttleScenery[1].field`** in
  `track/GameDrawRouteScenery.c`, although `GameShuttleScenery` is already
  typed and `GameUpdateShuttleScenery` indexes the array happily. As member
  references the loads and stores get `MEM_IN_STRUCT_P`, stop aliasing the
  `g_ShuttlePath*` table reads interleaved with them, and gcc hoists one base
  register for the block: 0x34-relative offsets (52, 60, 64, 66, 84, 88, 92)
  appear where retail has separate `%hi`/`%lo` pairs, and the branch layout
  moves with it. The eleven symbols stay, with their offsets documented.

In all three cases the declarations now say, at the declaration, which object
the scalars are fields of and at what offset, so a reader gets the structure
even though the compiler would not take it.

### 20c. Runs deliberately left as scalars

* **`g_PathScenery{Pos,Rot}{Phase,Span,Rate,Index}`** (`D_801E4DE0`..`DEE`) are
  four two-halfword pairs, position first — a structure-of-arrays over the
  prop's two keyframe tracks. Nothing in the image indexes them by track, so
  turning them into `[2]` arrays would trade eight readable names for eight
  equally readable subscripts and buy no structure the comment cannot state.
* **`g_CdMix{,Full}{LL,LR,RR,RL}`** (`D_8009B174`) are already recognised as a
  `CdlATV` (19b) and `GameStepCdVolumeFade` already walks them through a
  hand-pinned `p = &g_CdMixLL`. An array would fight that.
* **The `D_801E4B84` and `D_8007F5F8` runs are not objects at all.** Both look
  like long runs of adjacent named globals, but reading them shows unrelated
  neighbours in `.bss` — `g_BgmSelectCursor`, `g_CarListCursor`,
  `g_ScreenOffsetX`, `g_PeakOutputValue`… — that happen to be allocated next to
  each other. Adjacency in `.bss` is not evidence of aggregation; only a walker
  is.

### 20d. Semantics recorded this pass

Every one of these is stated at the declaration, with the instruction pattern
that proves it:

* **12-bit angles (4096 to the turn).** `g_CamPathAngle[CAMPATH_PITCH/YAW/ROLL]`
  and their `Start`/`Delta` companions: the deltas are wrapped into
  `[-0x800, 0x800)` on load (`>= 0x800` subtracts 0x1000, `< -0x7FF` adds it)
  and the interpolated values are masked `& 0xFFF` on store. Element 3 of the
  same quads gets *neither*, which is what proves `CAMPATH_DIST` is a length
  and not a fourth angle — and, with it, that the 0x24-byte track-camera record
  really is the union 18a claims. Same unit for `g_PathSceneryRot[3]` and
  `g_RouteSceneryRot{X,Y,Z}`, both consumed as `0x800 - yaw`, i.e. reflected
  about half a turn.
* **12.12 fixed point.** `g_CdMix*` / `g_CdMixFull*` are shifted right 12 to
  make the 0..0x7F bytes `CdMix` wants, so 0x7F000 is full scale.
* **Cosine-ease phase, 0..0x1000.** `g_PathSceneryPosPhase` /
  `g_PathSceneryRotPhase`.
* **Unsigned 0..255 with 0x80 the mechanical centre.** The four raw NeGcon axis
  bytes at `D_801E4040`; `GameUpdateNegconNeutralScreen` latches the steering
  one as `axis - 128` while the three analog buttons keep their raw range and
  are re-centred against the latched `g_NegconNeutral*` instead.
* **Packed RGB, half-aligned.** The 27 `g_EnvColors` words; the block base is
  2 mod 4, so they are `lwl`/`lwr` traffic and the type has to say `packed`.
* **Position vector plus spare word.** `D_801E4340` is x, y, z and a fourth
  word copied verbatim out of the series header, written through one cursor;
  `D_8009B1B8`/`C8`/`D8` are three xyz triples each padded to 16 bytes.

### 20e. A wrong name found, and corrected

**`D_801E418C` is `g_NegconMaxTwist`, not `g_NegconSteerPlay`.** Found by this
pass's alias-collision check; the disassembly evidence is folded into 18c, which
is where the wrong reading was stated, so the document states it once.

The same check reported eleven **pre-existing** addresses carrying two different
names from different directories. **Section 22 resolved ten of them** — see its
"Resolved (one name now)" table for which spelling won and why: `D_8019CB08`,
`D_8019CB74`, `D_8019CE0C`, `D_801F17B0`, `D_801E6C78`, `D_801E6DA0`,
`D_8019C754`, `D_8009B1B0`, `D_801E4B34` and `D_800941E8`. The eleventh,
`D_801E6CA4` (`g_EffectVolumeScale` / `g_SoundScale`), is **kept split on
purpose**: a struct member reference is non-aliasing to gcc 2.6.3
(`MEM_IN_STRUCT_P`) and the volume arithmetic in `GameSetPitchedSoundCue.c`
reorders, in either direction. Section 22 records the experiment.

The remaining duplicates are the sanctioned array-base-plus-element-zero pattern
(`g_EnvColors`/`g_EnvFogColor`, `g_RefSectorTimes`/`g_RefSectorTime0`,
`g_PadButtonMapping`/`g_PadSteerLeftMask`, …) or a deliberate volatile/non-
volatile pair, and are fine. **The check itself is worth re-running rather than
trusting this list**, which is a snapshot: grep every `NAME asm("D_XXXXXXXX")`
in `src/` + `include/` and assert one name per address and one address per name.

## 21. Endgame pass: the remaining functions (findings, not conversions)

This section records what was *established* about the remaining non-plain
functions so the analysis does not have to be redone. Nothing in it changed the
ROM: `make check VERSION=PAL` is still `OK` at
`2913e15648eddef40821c5f666460abc04155ee6`.

It was written when 67 functions were outstanding. **That count is now 50**
(`0568a8af`); `tools/scripts/progress_report.py` is the list that is current by
construction. Several functions this section discusses have since been
converted, and where a subsection says a function "cannot" be matched, treat
that as "was not matched by this pass" — the record of such claims in this file
is bad. `func_8005E4EC`, `func_8005B070` and `func_8005B768` were all written
down as permanently crutch-bound and are all plain C now (see the note under
"Measurement" below); `func_80069D18` was left in the outstanding list here and
is now `HANDWRITTEN_ASM`.

### Measurement — and why the figures below are unreliable

The method: compile the single translation unit with `tools/scripts/cc.sh`,
then compare the function's `.text` words against the retail words in
`asm/PAL/main/nonmatchings/**/<func>.s`, masking the immediate field of any word
that carries an `R_MIPS_HI16` / `LO16` / `26` relocation. That is
layout-independent, so a candidate whose instruction *count* differs still gives
a usable per-word residual instead of shifting the whole image.

**UNVERIFIED / UNRELIABLE: every residual figure quoted in 21a–21e was measured
before two harness bugs were fixed in `0568a8af`, and understates.** The harness
(a) did not resolve relocations, so an address that links to the same word as
retail counted as different, and (b) truncated the retail extent at interior
`.globl` labels, which several stubs carry mid-function — one function was being
compared against 158 words of a 345-word body. Do not quote "N words out" from
the subsections below as fact; re-measure. `0568a8af` re-measured all nineteen
functions that were open at the time, but those figures were not written down
here, so the only current source is the harness itself.

**A rule this section is the wrong place to look for.** An earlier note in this
project held that the `addu` operand-order class — retail has the base register
first, the compiler insists on it second — was bound by CSE's canonicalisation of
a constant `symbol_ref` into operand 2, was unreachable from plain C, and that a
single inline-`addu` was the sanctioned remedy; it named `func_8005E4EC`,
`func_8005B070` and `func_8005B768` as permanent crutches on that basis.
**That is wrong** (`de09c458`). Every *inline* spelling of the address does give
base-second — the shift and multiply forms, both operand orders, `(s32)p + …`,
`((s16 *)p)[i + 6]` — but hoisting the offset base into **its own pointer
variable** and indexing that gives base-first, with no inline asm and no pins,
because the intermediate pointer changes what CSE has an equivalence for. All
three named functions are plain C in the tree today. See
`docs/DECOMPILATION_GUIDE.md`, which carries the same warning next to the asm
lever it still sanctions for `func_80032098`.

### 21a. The phantom 8-byte stack frame (`addiu $sp,$sp,-8` with no stack use)

Twenty-nine retail functions contain `addiu $sp,$sp,-8` / `addiu $sp,$sp,8`
around a body that never touches the frame. At `a038e0c7`, the known remaining
crutches discussed in this subsection are two hand-written stack adjustments
(`SsUtSetVVol` in `lib/libsnd/SsUtPitchBend.c` and `SpuVmSetSeqVol`) and two
fabricated arrays (`GameInstallTrackPoints` and
`SpuVmApplyPitchBendByTone`). `SsUtChangeADSR` and
`render/Gpu_WriteGp0Words` are now crutch-free; see the rewrites below.

**The mechanism is now known.** gcc emits `.frame $sp,8,$31 # vars= 8`, i.e.
`get_frame_size()` is non-zero, because `alter_reg` in reload gave a stack slot
to a pseudo that

1. still has `REG_N_REFS > 0` from the last `reg_scan`, but
2. has no surviving insn at `regclass` time, so all its class costs are zero, so
   `regclass` picks the last class it scans — `ST_REGS` — which has no
   allocatable register on this soft-float target, so `find_reg` fails and the
   pseudo is spilled to a slot that nothing ever reads.

Confirmed with `-da` RTL dumps on a two-line reproducer:

```c
void f5(int n) { int i; for (i = 0; i < n; i++); }   /* .frame $sp,8 */
```

`f5`'s `.lreg` dump shows `Register 74 used 2 times across 2 insns in block 0;
dies in 0 places; ST_REGS or none`, `.greg` lists it under
`3 regs to allocate: 72 71 74` with `74 conflicts:` (none) and then omits it from
`Register dispositions`. Reg 74 is the *duplicate* of the loop exit test that
`jump.c:duplicate_loop_exit_test` copies above `NOTE_INSN_LOOP_BEG`; `combine`
later folds it into `branch_zero` and the reference count is never refreshed.

Practical consequence, verified by compiling each shape:

| source shape | frame |
|---|---|
| `for (i = 0; i < n; i++) …` (pre-test loop) | **8** |
| `while (n > 0) { …; n--; }` | 0 |
| `while (n--) …` | **8** |
| `i = 0; do { … } while (i < n);` | 0 |
| `if (n > 0) { i = 0; do { … } while (i < n); }` | 0 |
| `for (i = 0; i < 10; i++) …` (constant bound) | 0 |

This immediately retired one fabrication. `render/Gpu_WriteGp0Words`
(func_800675B0) is now

```c
s32 Gpu_WriteGp0Words(u32 *src, s32 count) {
    s32 i;

    *g_GpuGp1Volatile = 0x04000000;
    for (i = count - 1; i != -1; i--) {
        *g_GpuGp0 = *src;
        src++;
    }
    return 0;
}
```

with no `volatile s32 unused;` and no `if (count != 0) { … do … while … }`
rewrite: the `beqz $5,…` guard in retail *is* the duplicated exit test, folded by
`combine` from `count - 1 != -1` back to `count != 0`, and the frame comes free.
Still byte-identical.

`track/GameInstallTrackPoints` (func_8002A6B0) is the same story but does not
close. Replacing `s32 pad[2]; asm volatile("" : "=m"(pad));` and the
`if (count > 0) { … do … while … }` with

```c
    limit = count;
    for (i = 0; i < limit; i++) {
        index = i % limit;
        point = (GameTrackPoint *)((index * sizeof(GameTrackPoint)) + (s32)points);
        g_TrackLength += point->segmentLength;
    }
```

(register pins unchanged) gets the frame and the right instruction count, and
leaves **three words**: retail emits `move $4,$3` (`limit = count`) *after* the
`blez $3` guard, in the loop preheader, while gcc schedules it before the
`sw g_TrackPointCount` / `sw g_TrackArcCenters` pair. Tried and rejected:
dropping the `$4` pin, dropping the `limit` variable entirely (loses the copy and
a word), assigning `limit` at the top of the function, `for (limit = count, i = 0; …)`,
an outer `if (count > 0)` around the `for` (adds a word), and a `while` form.

**The loop is not the cause; it is only one carrier.** The general rule, read
out of the gcc 2.6.3 sources (`ftp.gnu.org/old-gnu/gcc/gcc-2.6.3.tar.bz2`) and
confirmed against `-da` dumps:

* `flow_analysis` sets `reg_n_refs`; only `combine` runs between it and
  `regclass`, so `combine` is the only pass that can leave a pseudo with refs
  and no insns.
* `combine.c` normally repairs this: after a successful `try_combine` it does
  `reg_n_sets[i2dest]--; if (… == 0 …) reg_n_refs[i2dest] = 0;` — but only
  `if (! added_sets_2 && newi2pat == 0 && ! i2dest_in_i2src)` (combine.c:2280).
* So the stale case is exactly a **three-insn combine `(i1, i2, i3)` in which
  `i1dest` is still live after `i3`**. `added_sets_1` then forces the merged
  pattern into a `PARALLEL` that combine splits, `newi2pat` becomes i1's set
  (moved into i2's slot, i1 deleted) and `i2dest` — the intermediate — vanishes
  from the RTL with `reg_n_refs` still at its old value.
* `regclass` then sees all-zero costs for it, `reg_class_subunion` walks down
  from `ST_REGS`, `find_reg` fails, and `alter_reg` gives it a 4-byte slot;
  `MIPS_STACK_ALIGN` rounds `get_frame_size()` up to 8.

**Source-and-control verification, 2026-07-28 (`TASK_FOR_CODEX_2H`).** The
reading above is confirmed, including the previously missing discriminator.
The exact source path is:

1. `jump.c:587-603` calls `duplicate_loop_exit_test` only immediately after
   regscan; `jump.c:2097-2125` gives exit-only values fresh pseudos and
   `jump.c:2128-2165` copies the test before `NOTE_INSN_LOOP_BEG`.
2. In crutch-free `SpuVmSeKeyOff`, `.jump` creates pseudo 230 for
   `ltu(zero_extend(voice), zero_extend(D_801E42F8))`. The loop pass notices
   that both `voice` and the separately live SI `count` start at zero and uses
   `count` as the comparison's SI left operand.
3. Combine therefore sees the three-insn chain `count = 0; p230 = count < bound;
   branch_zero(p230)`. Because `count` remains live after the branch,
   `added_sets_1` is true (`combine.c:1430-1445`); the preserved `count = 0`
   becomes `newi2pat` while the branch becomes a direct zero test
   (`combine.c:1622-1663, 2138-2156`).
4. The normal repair for the vanished comparison destination is guarded by
   `newi2pat == 0` (`combine.c:2275-2285`), so it is skipped. This is exactly
   the rare stale-`reg_n_refs` case admitted by `combine.c:51-58`.
5. The resulting `.lreg` line is `Register 230 used 2 times across 2 insns in
   block 0; dies in 0 places; ST_REGS or none`; `.greg` lists 230 for
   allocation with no conflicts, then omits it from the dispositions.
   `regclass.c:703-705, 921-948` explains the all-zero costs/class tie, and
   `reload1.c:646-656, 2306-2316` assigns a stack home to an unallocated pseudo
   whose stale `reg_n_refs` is positive. MIPS `BIGGEST_ALIGNMENT` and
   `STACK_BOUNDARY` are 64 bits, so the four-byte SI home rounds to eight.

The discriminator is therefore not merely “pre-test loop over a nonconstant
bound.” It is a successful three-insn combine whose first set must survive.
For the loop carrier, that means a distinct, same-mode value equal to the
initial induction value and live after the copied entry test. In
`SpuVmSeKeyOff`, that value is the real SI match count. A pre-test loop without
such a carrier can have its copied compare folded and accounted for normally.

In the `for`-loop carrier the triple is `i1 = (set i 0)`, `i2 = (set t (lt i n))`,
`i3` = the duplicated exit branch; `i` is live in the loop, so `newi2pat` is
`(set i 0)` and the compare temp `t` is the phantom. `while (n > 0)` produces no
frame because a compare against 0 needs no `slt` temp at all.

**Loop-free carrier, which is what the `SsUt*` setters use.** Indexing *one*
scaled array base at two constant offsets inside a single basic block:

```c
extern short A[];
void t07(int a, short v) { int o = a << 4; A[o] = v; A[o + 1] = v; }   /* vars= 8 */
```

`expand` forces the symbol into a register (`(set r75 (symbol_ref A))`) because
`(plus symbol reg)` is not a valid *value*, then materialises the address
(`(set r78 (plus r77 r75))`). Combine folds the symbol back into the MEM —
legal, `sh v,A(r77)` — and `r75` is still needed for the `A+2` access, so
`added_sets_1` fires and `r78` is the phantom. Its `.lreg` line is
`Register 78 used 2 times across 2 insns in block 0; dies in 0 places;
ST_REGS or none`, and `.greg` omits it from `Register dispositions`. Writing the
two stores through two *separate* symbols (`D_8009DF20` and `D_8009DF22`) gives
`vars= 0`; writing them as `base[j]` / `base[j + 1]` on a shared halfword view
gives the retail frame with no other change. The base must be a scaled type
(a `u8` base needs no shift, so `EXPAND_SUM` keeps it inside the MEM), the two
accesses must be in the same basic block (LOG_LINKS do not cross blocks), and
the MEMs must not be `volatile` (combine refuses to rewrite a volatile MEM, so
the fold — and with it the phantom — never happens).

This retires the crutch in `SsUtSetDetVVol` (func_80078430), which is now plain
C: `j = index * 8; g_SndVoiceRegs16[j + 1] = volr; … g_SndVoiceRegs16[j] = voll;`
plus the two pre-existing `$2` / `$7` pins.

**Still open, with the distance measured.**

* `SsUtSetVVol` (func_80078528): same rewrite gets the frame and the right 35
  words, 17 differ. The residual is purely allocation — retail spills `volr`
  (`move $3,$6` in the guard's delay slot) because `voll * 129` is computed
  first and lands in `$6`; ours computes `volr * 129` first and spills `voll`
  instead. Swept `x`/`y`/`index`/`j`/`flags` pins × three statement orders.
* `SsUtChangeADSR` (func_80078300): **CLOSED 2026-07-28**, byte-exact and in
  the tree with no stack crutch. `g_SndVoiceRegs16[j + 4]` / `[j + 5]` is the
  right carrier, as recorded here; what closed the last word was the *control
  flow*, not the carrier. The nested-`&&` form lets `dbr` steal `li $2,-1` into
  the third `bne`'s delay slot (the 55-vs-54 above). Writing each guard as its
  own `if` with `ret = -1; goto done;` gives every `bne` its own `li $2,-1` to
  fill from, which is what retail has. The `sched` interleave of the flags `lbu`
  between the two `sh` is held off by the pre-existing `asm("" ::: "memory")`
  barrier, and the `$3` pin stays; both were re-measured as load-bearing (26 and
  2 words respectively).
* `SpuVmSetSeqVol` (func_80076C58): the crutch is unnecessary — the retail
  `beqz D_801E42F8` guard *is* a duplicated exit test, so the body is a pre-test
  `for` and the three preheader insns after the guard (`andi`, `la D_8009DF20`,
  `addiu +2`) are `move_movables` hoists, i.e. they must be written *inside* the
  loop.

  **Re-measured 2026-07-28 with both sides linked, and the residual is now
  pinned to exactly two words.** With `short i` as the counter and the stores
  kept as the two hoisted `u_char *` pointers, the candidate is 91 words and
  **every one of them is byte-identical to retail** once retail's two frame
  instructions are removed and branch displacement fields are normalised: zero
  differences. The whole gap is `addiu $sp,$sp,-8` / `addiu $sp,$sp,8`.

  What was swept, all inert or worse:

  - counter type: `short` 91 words body-exact; `u_short` 88; `u_char` 86;
    `long`/`int` 89 **with the frame** (see below); declaring `i` at function
    scope instead of inside the `if (arg3 == 1)` block changes nothing.
  - the 21a array carrier (`g_SndVoiceRegs16[k]` / `[k + 1]`) inside the loop:
    90 words and **no** frame. It cannot fire here, because `move_movables`
    hoists the base out of the loop, so the two accesses no longer share a basic
    block with the address materialisation, and 21a's fold needs LOG_LINKS that
    do not cross blocks.

  **`long i` and `int i` do produce the phantom frame**, which 21a did not
  record. They cost four words elsewhere and so are not the answer: the guard
  degrades from `beqz` to `blez` (this is the `nonzero_bits` half below, which
  `short i` gets right for free), and the exit test loses the `(short)`
  truncation, dropping retail's `sll`/`sra` pair. Routing the index through the
  `$2`-pinned `offset` in two steps keeps `exact` at 32 rather than 88 but does
  not recover those four words.

  RTL evidence for what is actually missing, from `-dl` dumps against
  `SpuVmSeKeyOff` (crutch-free, same `D_801E42F8` bound, and it *does* get the
  frame): the template's phantom is pseudo 230, `used 2 times across 2 insns in
  block 0; dies in 0 places; ST_REGS or none`, and it appears nowhere in the
  final RTL — a dead pseudo whose `REG_N_REFS` combine never refreshed, showing
  up as `vars= 8` in `.frame`. The `short i` candidate here has `vars= 0` and
  every pseudo allocated to a hard register. This narrowed the then-open
  question to getting combine to leave that dead pseudo while the guard still
  folded to `beqz` and the counter stayed `short`; the 2H closure below answers
  why no real value in this function can do all three.

  **2026-07-28, second pass: the frame IS reachable, and the residual is now five
  named defects.** A two-variable counter,
  `for (counter = 0; (i = (short)counter) < D_801E42F8; counter++)` with `long counter`
  and `short i`, produces a **93-word** candidate, i.e. retail's exact size, frame
  included. `exact 27`. Aligned against retail it differs in only five places:

  1. an inserted `move a1,zero` at +0xAC, initialising the second variable;
  2. `blez` instead of `beqz` at +0xB8 (the `nonzero_bits` half, below);
  3. `sll v0,a1,16` instead of `sll v0,a2,16` at +0xD0;
  4. a missing `addiu v0,a2,1` in the delay slot at +0x100;
  5. the increment/move pair at +0x13C using the wrong registers.

  Defects 1, 3, 4 and 5 are all one cause: **retail has a single counter variable**
  living in `a2`, sign-extended into `a1` at each use, whereas this candidate has two.
  So the shape to look for is one variable that both yields the frame and truncates to
  `short` — a `short` counter alone gives the truncation and no frame (91 words,
  everything else byte-identical), a `long` counter alone gives the frame and loses the
  truncation. Six loop-header spellings were swept on top of the two-variable form
  (`i = counter` uncast, bound cast to `long`, comparison on `counter` with `i` assigned
  in the body, an explicit `!= 0` conjunct, an unsigned bound); all still emit `blez`,
  and the three that changed the body were worse. Candidate kept at
  `scratch/decomp-work/func_80076C58/codex2-93w-exact27.c`.

  The `blez` vs `beqz` half needs `nonzero_bits` to know the bound's sign bit is
  clear, which `combine` only manages when the compared pseudo has
  `reg_n_sets > 1` (`set_nonzero_bits_and_sign_copies` ignores single-set
  pseudos, and the `reg_last_set` fast path is barred because `subst_low_cuid`
  is the counter's `i = 0`, which always precedes the duplicated load).
  Assigning the bound to a local both before and at the end of the loop supplies
  the second set and closes it, but that shape is invented C. With `short i` the
  question does not arise.

  **Closed as a proven negative by `TASK_FOR_CODEX_2H`.** The body-exact
  `short i` candidate's copied comparison is pseudo 156. The loop pass reduces
  the sign extension of the initial HI zero only to a temporary SI zero; there
  is no distinct live SI value to preserve. Combine deletes the temporary and
  pseudo 156 normally, `.lreg` has no `ST_REGS` orphan, and `.frame` remains
  `vars=0`. Reusing the function's genuine `u_long index` as the sole counter
  with `(short)index` in the condition also fails: GCC transforms the induction
  sequence, emits no phantom, and measures 90 words (`exact 88`, `aligned 11`).
  A single `long` counter creates the frame but removes retail's short
  truncation; adding a second long carrier is exactly the fabricated-local
  solution forbidden by the task and already has the five code-generation
  defects above. There is no legal, byte-exact source shape among the real
  values in this function, so the two-instruction stack crutch remains.

* `SpuVmApplyPitchBendByTone` (func_80075EB4): **also closed as a proven
  negative by `TASK_FOR_CODEX_2H`.** Removing only `stack_pad[2]` and its
  `"=m"` constraint gives the full 60-word retail body; all 16 positional
  differences are the frame size and save/restore offsets, and its compiler
  frame is `vars=0`. Here the real SI `sum` is a valid carrier: the natural
  source order `sum = 0; i = 0; while ((short)i < D_801E42F8) ...` reproduces
  the bug without any fake local. Its `.lreg` has orphan pseudo 121 as
  `ST_REGS or none` and its compiler frame is the required 64 bytes with
  `vars=8`. But that legal loop changes the initialization, allocation, and
  increment body (58 words, `exact 45`, `aligned 21`; the hoisted-voice form is
  59 words, `exact 34`, `aligned 20`). The hash-exact body is emitted from the
  existing explicit guard plus do-loop; that RTL has no unconditional jump
  immediately after `NOTE_INSN_LOOP_BEG`, so `duplicate_loop_exit_test` has
  nothing to copy. The real carrier and the retail body are thus reachable
  separately, not together. The fabricated padding remains pending a future
  compiler-faithful source discovery.

**Explicit-inline follow-up, 2026-07-29 (`TASK_FOR_CODEX_1_PHANTOM`).** An
explicit `inline` helper can change a caller's frame, but it is not a carrier
that imports this phantom frame. GCC 2.6.3 enables automatic inlining only at
`-O3`; this build uses `-O2`, so only a function declared `inline` enters
`integrate.c`. `finish_inline` records `DECL_FRAME_SIZE` while saving the
callee's RTL, before the caller's `combine` and `reload` passes. During
expansion, `copy_rtx_and_substitute` reserves that recorded size only when the
callee's virtual stack-vars register is actually referenced. The stale
`REG_N_REFS` slot described above does not exist yet and therefore cannot be
copied from an inline callee. An inlined pre-test loop can still create its own
phantom later, because its copied RTL runs through the caller's normal
`combine`/`reload`; that is the same carrier, not a second mechanism.

This was checked on all three remaining libsnd cases, with each candidate
linked and compared to the committed byte-exact object:

* Moving `SpuVmApplyPitchBendByTone`'s natural pre-test loop into an explicit
  inline helper produced the real 64-byte frame, but the `long`-counter version
  was 56 words (`exact 59`, `aligned 41`) and the `short`-counter version was
  58 words with the same residual. The helper boundary changed allocation but
  was otherwise erased; it did not recover the retail body.
* In `SpuVmSetSeqVol`, inline helpers around the condition, bound, increment
  and short sign-extension did not create the missing frame on the
  body-exact short-counter form. The least disruptive increment helper stayed
  at 91 words with no frame (`aligned 5`); the other forms fell to 83-84 words.
* In `SsUtSetVVol`, an inline scaling helper could move the natural frame to
  the function entry, confirming that integration changes pseudo lifetimes,
  but the best such form was 36 words (`exact 27`, `aligned 26`). Inlining the
  entire successful path was folded back to the already measured natural C
  allocation (36 words, `exact 24`, `aligned 22`). The first 1,000
  permutations of the seven genuine local declarations were also inert, so
  that sweep was stopped rather than spending time on equivalent RTL.

No production source change was accepted from this follow-up. In particular,
an inline helper with a dummy local or a semantically empty loop would merely
replace the existing stack crutch with another fake frame carrier and is not a
decompilation.

### 21b. `SsUtChangeADSR` (func_80078300) — crutch-free body

> **Closed 2026-07-28.** The function is byte-exact in the tree with no stack
> crutch: both `addiu $sp,$sp,±8` and both hand-written `lhu` loads are gone.
> The body below is right about the carrier and about the `u16` parameters, but
> its nested-`&&` control flow is what cost the extra word; see the
> `SsUtChangeADSR` bullet in 21a for the fix. Note also that the parameter
> typing and the frame crutch are **mutually exclusive**: `assign_parms` emits
> the `lhu` entry loads in the prologue, ahead of any `asm("addiu $sp,$sp,-8")`
> in the body, whereas retail adjusts `$sp` first. That is why the `lhu` crutch
> survived so long, and why only the real frame dissolves both at once.

The `lhu`-from-stack-slot crutch is unnecessary. Declaring arguments 5 and 6 as
`unsigned short` makes `assign_parms` emit the zero-extending entry loads by
itself, and the whole body then falls out of ordinary C:

```c
s16 SsUtChangeADSR(s16 vc, s16 vabId, s16 prog, s16 tone, u16 adsr1, u16 adsr2) {
    s32 off;
    if ((u16)vc < 24) {
        off = vc * 0x34;
        if (*(s16 *)((u8 *)D_8009E0CE + off) == vabId &&
            *(s16 *)((u8 *)D_8009E0CA + off) == prog &&
            *(s16 *)((u8 *)D_8009E0C4 + off) == tone) {
            *(volatile s16 *)(D_8009DF28 + (vc << 4)) = adsr1;
            *(volatile s16 *)(D_8009DF2A + (vc << 4)) = adsr2;
            D_8009E0A0[vc] |= 0x30;
            return 0;
        }
    }
    return -1;
}
```

This compiles to retail's instruction sequence exactly — `andi/sltiu`, the
`lhu $9,16($sp)` / `lhu $10,20($sp)` entry loads, the three `lh` compares with
their `-1` delay-slot fills, the two `sh` stores and the `ori 0x30` — with every
stack offset 8 lower, because the retail frame is 8 bytes and this body's is 0.
`SsUtSetDetVVol` did then convert on 21a's mechanism and is plain C, and
`SsUtChangeADSR` closed on 2026-07-28. `SsUtSetVVol` has **not** closed: the
same natural-C reduction (`voll * 129` / `volr * 129` with the `volr` store
first, which is what retail does) reaches the frame and retail's shape but sits
at 37 words against 35, with two surplus `move`s copying incoming argument
registers. Re-swept 2026-07-28 against the linked-both-sides harness: five
statement orders, `short` vs `long` parameter types, dropping the `$2` pin, and
inlining the repeated `index`/`offset` all give **identical** output, so the
residual is allocation, not source order. It is still the hand-written asm block
with the `2:` label hack in `lib/libsnd/SsUtPitchBend.c`.

### 21c. `func_80016754` GameDrawText8x8 — converted; GIV question closed

**Converted with both siblings on 2026-07-28.** A second audit removed six of
the eleven register pins (`index` and `fontUCell` in every function), with a
forced clean link after every trial and a second cumulative pass. The accepted
source still does not make `packet` a giv. Instead, a tied empty output makes
the first font base a source preheader operation before the explicit
sprite-cursor assignment:

```c
asm("" : "=r"(font) : "0"(g_Font8x8Cells));
sprt = (TextSprt8 *)packet;
```

The opaque output supplies the U lookup while the V lookup independently
rematerialises `g_Font8x8Cells + 1`. Thus the exact `.loop` dump orders the
font `asm_operands` before `sprt = packet`; allocation emits retail's
`lui/addiu s6` then `move s2,s3`. The `v0`/`v1` allocation now falls out
naturally; the short-lived `t0` pin retains the second lookup's base. This same
shape gives exact 0 for `func_80016754`
(86 words), `func_800168AC` (91), and `func_80016A18` (89). Full details and
the shaded sibling's two additional pressure pins are in
`docs/TASK_FOR_CODEX_2F_REPORT.md`.

**Re-measured 2026-07-28: 86/86 words, exact residual 3, and the residual is now
characterised exactly.** Retail's loop preheader is `lui/addiu s6` (the hoisted
`D_8007C2F8` font base) followed by `move s2,s3`; every candidate emits the `move`
first and the hoist after. The two are independent, so this is an emission-order tie
between a `move_movables` hoist and the preheader statement that initialises the sprite
cursor, not a scheduling decision inside a block.

Retail's register roles, for reference: `s3` is the packet read from the scratchpad,
`s2` the sprite cursor copied from it, both advancing by 16; `s6` is the hoisted font
base used for the first byte, while the second byte rematerialises `D_8007C2F9` inside
the loop at `0x800167CC`.

Ruled out on top of the nine mechanism classes already swept (see
`scratch/decomp-work/func_80016754/metrics.md`):

* **An explicit `u8 *font = g_Font8x8Cells;` collapses the function to 83 words**, in
  either declaration position and whether indexed or dereferenced. Naming the base stops
  the second byte rematerialising `D_8007C2F9` in the loop, which is three of retail's
  words. The base must stay spelled as array indexing.
* **Dropping the explicit cursor and writing through `((TextSprt8 *)packet)->field`
  does make gcc create the second induction variable by strength reduction, but it
  biases it**: `addiu s2,s3,14` with negative field offsets, exact 7. That is the same
  bias pathology recorded for `func_8005C914`. A `volatile` cast in that form drops back
  to 83 words.
* Declaration order of `sprt` relative to `packet` and `scratch`, splitting the
  declaration from the assignment, and initialising `sprt` from a second read of the
  scratchpad slot are all **inert**: four spellings, all exact 3.

So the shape needed is an *unbiased* second induction variable whose initialisation is
emitted after the invariant hoist. The explicit cursor gives unbiased-but-early; the
cast gives late-but-biased. Nothing tried yields both.

**Why, from the RTL.** `-dL` on the best candidate shows the preheader as:

```
insn 27   (set (reg 78) (reg 76))                     ; sprt = packet, from source
insn 180  (set (reg 83) (symbol_ref "D_8007C2F8"))    ; hoisted font base
insn 182  (set (reg 91) (const (plus D_8007C2F8 1)))  ; hoisted font base + 1
insn 188  (set (reg 107) (plus (reg 76) (const_int 14)))  ; biased giv init
note 29   NOTE_INSN_LOOP_BEG
```

`move_movables` places every hoist with `emit_insn_before (..., loop_start)`, so hoists
always land *after* whatever the source already put in the preheader. An explicitly
initialised cursor is a preheader source insn, therefore **it can never precede the
hoist** — that is a property of the pass, not a matter of finding the right spelling, and
it closes off the whole explicit-cursor family rather than one more candidate.

It follows that the original source did **not** initialise a cursor before the loop: the
second register must come from `loop` itself. Note that gcc already builds such a giv
here (insn 188) even in the explicit-cursor candidate, and it builds it biased by 14, the
offset of the last field written. The open question is therefore narrower than it looked:
**what source shape makes that giv unbiased?** Retail's `move s2,s3` with positive field
offsets 8 through 14 is a bias of 0.

**The bias rule, established by experiment and reusable.** `combine_givs` takes the head
of `bl->giv` as the representative and points the others at it, and `record_giv` builds
that list by prepending, so **the representative is the last giv recorded, and the bias
is the offset of the last field written through the pointer in source order.** Verified
directly: writing `x0,y0,u0,v0,clut` gives `addiu s2,s3,14`, and reversing the five
writes to `clut,v0,u0,y0,x0` gives `addiu s2,s3,8`. This is the same pathology recorded
for `func_8005C914`.

It also shows why bias 0 is unreachable with this structure. `packet` is the basic
induction variable; the field accesses at 8 through 14 are its givs, and there is no giv
at offset 0 because a base-offset use *is* the biv and is never recorded as a giv.
Passing the cast pointer to `AddPrim` does not create one: four placements of the cast
across `SetSprt8`, `SetShadeTex` and `AddPrim` all leave the residual at 7.

**Closed from GCC 2.6.3 source and new dumps (2G).** A zero-offset memory-address
GIV is not merely absent in the candidates: `loop.c:find_mem_givs`, lines
4135-4137, explicitly refuses `DEST_ADDR` when `mult_val == 1 && add_val == 0`.
Thus a direct `packet` address can never seed a bias-zero entry.

The proposed alternative, making `packet` a register GIV over another BIV, is
legal and was reached. `2g-count-units-01.i.loop` records the count-derived
packet as `mult 16, add (reg 76)` and reduces it to a new register. It still
cannot derive the field addresses: the packet's additive term is the invariant
runtime base loaded from scratchpad, and `simplify_giv_expr` lines 5060-5075 can
combine that invariant with a constant field offset only when the invariant is
`CONSTANT_P`. A register is not, so `(count * 16 + base) + 8` returns failure.
The byte-count version is rejected independently as `not worth while, 0 vs 33`
by the benefit test at lines 3739-3769.

An actual BIV copy gives the desired `mult 1, add 0`, but ordinary dominating
copies are propagated away before `loop`; the two `2g-prim-giv` dumps again
contain only field GIVs and select offset 14. A loop-condition copy survives and
is recorded in both `2g-loop-condition-giv` dumps, proving that zero-addend
register GIVs are accepted, but it is conditional/non-derivable across a label
(`update_giv_derive`, lines 4706-4709) and rejected as unprofitable
(`-8400 vs 33`, or `-1176 vs 33` even when its final value is replaceable).

That exhausts the legal zero-bias carriers here: address GIV excluded; packet
over a count unable to derive through its runtime-base addend; pointer-BIV copy
either propagated away or conditional and rejected. Preventing propagation of
the useful copy requires an opaque asm/volatile/fabricated dependency. Under
the task's source rules the bias therefore cannot be driven to zero. Full
candidates, dumps, clean-link hashes, and the remaining-crutch accounting are
in `docs/TASK_FOR_CODEX_2G_REPORT.md`.

Down from six residual words to three — **but both figures came from the
pre-`0568a8af` harness and understate; re-measure before quoting them.** The
*shape* of the residual below is what matters and is independent of the count.
The two solved sub-problems:

* **Use the `str` parameter as the walking cursor**, not a local copy. A local
  `p = str;` is a body insn, so `sched2` interleaves the callee-saved stores
  differently and `sw $fp,56($sp)` lands one slot early. With the parameter
  itself as the cursor the prologue comes out as retail's
  `sw $21 / move $21 / sw $23 / move $23 / sw $20 / move $20 / sw $fp / …`.
  gcc 2.6.3 **segfaults** on `register T x asm("$n")` applied to a parameter, so
  the pin cannot be moved onto `str`.
* **Hold the scratchpad pointer in a variable**: `u8 **scratch = (u8 **)0x1F800000;`
  used for both the initial read and the final write. This is what flips the
  `$19`/`$20` allocation so `str` lands in `$20` and `packet` in `$19`, and it
  also fixes the `addiu $2,$19,12` at the tail. (Found by decomp-permuter, which
  proposed the equivalent `int new_var = 0x1F800000;`.)

Current best body (86/86 words, 3 differ):

```c
void GameDrawText8x8(s32 x, s32 y, u8 *str, s32 clutIndex) {
    u8 **scratch = (u8 **)0x1F800000;
    u8 *packet;

    packet = *scratch;

    if (*str != 0) {
        volatile SPRT_8 *sprt = (SPRT_8 *)packet;

        do {
            s32 cell = *str - 0x20;

            str++;
            if (cell != 0) {
                s32 u = D_8007C2F8[cell * 2] * 8;
                s32 v = D_8007C2F8[cell * 2 + 1] * 8;

                SetSprt8(packet);
                SetShadeTex(packet, 1);
                sprt->x0 = x;
                sprt->y0 = y;
                sprt->u0 = u;
                sprt->v0 = v;
                sprt->clut = clutIndex;
                AddPrim(g_DrawBuffer + 0xCC, (void *)sprt);
                sprt++;
                packet += 16;
            }
            x += 8;
        } while (*str != 0);
    }
    SetDrawMode((DrawPacket *)packet, 0, 1, 9, D_8007BED0);
    AddPrim(g_DrawBuffer + 0xCC, packet);
    *scratch = packet + 12;
}
```

**The residual** is the order of two independent insns in the loop preheader at
`0x800167a0`:

```
retail:  lui $22,%hi(D_8007C2F8) ; addiu $22,$22,%lo(…) ; move $18,$19
ours:    move $18,$19            ; lui $22,%hi(…)       ; addiu $22,$22,%lo(…)
```

`la $22,D_8007C2F8` is `loop.c`'s LICM hoist of the first font-table address (the
second, `D_8007C2F8+1`, is deliberately *not* hoisted by gcc and is rebuilt each
iteration into `$8` — ours reproduces that exactly). `move_movables` emits every
hoisted insn immediately before `NOTE_INSN_LOOP_BEG`, therefore after
`sprt = packet`, which is the last statement of the preheader. Retail has it
before, which is where `strength_reduce` puts *giv initialisers* (they run after
`move_movables`) — but `packet`/`sprt` are incremented inside `if (cell != 0)`, so
neither is a biv and no giv is available.

Ruled out by compilation, all still 3 words: declaring `sprt` at function scope
and assigning it inside the `if`; initialising `sprt` from `*scratch` instead of
from `packet`; hoisting `cell` out of the loop body; extra casts on the
initialiser; folding `u`/`v` into their declarations; `*(D_8007C2F8 + …)` instead
of the array form. Swapping the `u`/`v` computations makes it worse (7 words).
Introducing an explicit `u8 *font = D_8007C2F8;` fixes the order but costs a word,
because with only one symbolic address left in the loop gcc drops the explicit
`la` for `D_8007C2F8+1` and uses the `lbu $17,D_8007C2F8+1($3)` assembler macro
(3 words) instead of retail's `la`+`addu`+`lbu` (4 words).

decomp-permuter ran ~280k iterations on this base (weights per
`docs/`-sanctioned settings, all fabrication weights zero) without beating it.

**What the ordering actually implies** (2026-07-27). Everything emitted before
`NOTE_INSN_LOOP_BEG` goes through `emit_insn_before (…, loop_start)`, so later
passes land *closer* to the loop: expand's preheader statements first, then
`jump.c:duplicate_loop_exit_test`'s copies, then `loop.c:move_movables`, then
`strength_reduce`'s giv initialisers. Retail has `la $22` before `move $18,$19`,
so either the `la` is a preheader statement (source order) and `sprt = packet`
follows it, or `sprt = packet` is emitted by `strength_reduce`. Both were tried:
`u8 *font = D_8007C2F8;` declared *before* `sprt` still yields the same three
words, because cse folds the symbol back into the loop body and `move_movables`
re-creates the `la` afterwards; and making `sprt` a real giv (`sprt =
(Sprt8 *)packet;` inside the `if`) makes `strength_reduce` collapse it into
`packet` altogether, losing three words (83 vs 86). Retail keeps two registers
stepping by 16 from the same start, so they are two bivs, not a biv and a giv.
Also confirmed here: `D_8007C2F8 + 1` being rebuilt with `lui`/`addiu` every
iteration is reproduced by a pointer local declared *inside* `if (cell != 0)`.

Solving this also unblocks `func_800168AC` and `func_80016A18`, which are the
same function with one extra argument.

### 21d. `func_800155EC` GameUpdateControllerConfigScreen — one scheduling tie

(The "8 words / 199 of 207" figures below are pre-`0568a8af` harness numbers and
understate; the scheduling tie they describe is the durable part.)

Retail keeps `&g_PadEdge2` in `$16` for the first three of the five reads and
rematerialises `lui/lhu` for the last two. That is reproduced by
`u16 *edge = &g_PadEdge2;` used for the `0x90`, `0x860` and `0x800` tests and the
plain global for the `0x8000` and `0x2000` tests; with that, 199 of the 207 words
match, and the instruction count is right (using `*edge` for all five loses two
words, using none loses the `la` entirely).

**The residual** is the first basic block. Retail issues the pad load first:

```
retail:  lhu $3,0($16) ; addu $2,$2,1 ; sw $2,g_AnimTimer ; lw $2,D_8007C13C ;
         andi $3,$3,0x90 ; addu $2,$2,96 ; sw $2,D_8007C13C ; beq …
ours:    addu $2,$2,1 ; sw $2,g_AnimTimer ; lw $2,D_8007C13C ; lhu $3,0($16) ;
         addu $2,$2,96 ; andi $3,$3,0x90 ; sw $2,D_8007C13C ; beq …
```

Both are valid schedules of the same DAG; gcc's list scheduler breaks the
priority tie between `lhu $3,0($16)` and `lw $2,D_8007C13C` the other way.
Reading the pad word into a local *before* the two counter updates does not move
it (verified: byte-identical output), and swapping the two counter updates only
swaps them. Note that without the pointer the same source schedules the `lhu`
first — i.e. the tie flips purely on the MEM being `(mem (reg))` rather than
`(mem (symbol_ref))`. That is also the reason to be suspicious of the
three-pointer-reads/two-direct-reads split as *original* source: it is the only
shape found that reproduces retail's instruction count, but it is odd C, and the
scheduling residual may be telling us the real source materialises the address
some other way.

decomp-permuter has this base too; ~44k iterations, no improvement.

### 21e. `func_80069D18` (in `render/TransposeMatrix.c`) — now reclassified

Was listed among the outstanding functions, and should not have been: every
fixed-point product is `multu`+`mflo` with no `mfhi`, and gcc 2.6.3 only emits
`multu` for a true 64-bit widening multiply. It is `RotMatrix`, hand-written
assembly in the original — 14 `multu`, 15 `mflo`, no `mfhi`, a load split by a
shift pair where the compiler folds to `lh`, and a bare jump joining two branch
arms with a shared shift in its delay slot, matching the idiom and register file
of two siblings already documented as hand-written.

This section originally said it was "left unchanged here because reclassifying
it moves the headline number without decompiling anything". **That was
reversed** (`3d5e2847`): it now carries `HANDWRITTEN_ASM` and is the 47th
member of that set, out of the outstanding list.

## 22. One address, one name (duplicate-name pass)

24 addresses carried more than one name, mostly because parallel passes named
the same word from different directories. Most were plain naming accidents and
are now single-named; the ones in the second table are **forced** by gcc 2.6.3
and are kept deliberately, each documented at its declaration. Every step in
this section was verified byte-identical with `make check VERSION=PAL`.

**The two tables below are a snapshot, and the snapshot has moved.** Re-run the
check rather than reading it off this page:

```python
# one name per address, and one address per name
import re, pathlib, collections
pat = re.compile(r'\b([A-Za-z_]\w*)\s*(?:\[[^\]]*\])?\s*(?:\([^;{]*?\))?\s*asm\("(D_[0-9A-Fa-f]{8})"\)')
a2n, n2a = collections.defaultdict(set), collections.defaultdict(set)
for root in ('src', 'include'):
    for p in pathlib.Path(root).rglob('*'):
        if p.suffix in ('.c', '.h'):
            for name, addr in pat.findall(p.read_text(errors='ignore')):
                a2n[addr].add(name); n2a[name].add(addr)
```

At `0568a8af` that reports **18** addresses carrying more than one name, not the
eight this section left behind. The extras are `D_8007D78C`, `D_8009A588`
(`g_SndSpuRegs` / `g_SndSpuRegsBytes`), `D_8009DF20` (`g_SndVoiceRegs` /
`g_SndVoiceRegs16`), `D_8009E6D4` (`g_PlayerCar` / `g_PlayerCarX`),
`D_801E408C` (`g_RaceSeries` / `g_RaceSeriesNV`) and
`D_801E431C`/`20`/`24` (`g_FlybySceneryRot{X,Y,Z}` and their `2` siblings) —
mostly non-volatile or differently-typed views added by conversions after this
pass, i.e. the same sanctioned pattern, but nobody has checked them one by one.

**Two unresolved name collisions this pass could not have caught**, because it
only asked "does one address have two names" and not the reverse:

- **`g_McCardStatus` is bound to two different addresses.** `game/menu.h` and
  `save/GameUpdateMemoryCardMenu.c` bind it to `D_8009B720`;
  `save/GamePollMemoryCardStatus.c` binds it to `D_801E825C`. Both are live, and
  a reader who follows the header into the poller reads the wrong global.
- **`SsUtKeyOnV` is bound to two different addresses** — `func_80077C7C` in
  `psyq/snd.h` and `sdk/SsUtKeyOnV.c`, and `func_80076C50` (an empty two-word
  stub) in `lib/libsnd/SsUtKeyOn.c`. See 17b item 2.

Neither is fixed here; both are recorded so the next pass over `save/` and
`lib/libsnd/` knows.

### Resolved (one name now)

| Address | Kept | Dropped | Why this reading |
|---|---|---|---|
| `D_800941E0` | `g_GpuFuncs` | `g_GpuCallbacks`, `g_GpuFuncsStore`, `g_GpuFuncsMove`, `g_GpuFuncsClearOTagR` | Not five slots: five `GpuCallbacks *` externs for the *same* pointer, one per function of `sdk/LoadImage.c` and `sdk/SetGraphQueue.c`. Same type, different functions, so merging is free. |
| `D_800941E8` | `g_GraphType` | `g_GraphTypeArray` | The scalar and the `u_char[]` are the same byte; `_get_mode.c` and `render/SetDrawTPage.c` now both use the array spelling (`g_GraphType[0]` where a scalar was wanted). Also names the two raw uses in `SetDrawTPage.c` / `SetGraphQueue.c`. |
| `D_800942BC` | `g_GpuGp1` | `g_GpuGp1Volatile` | One `volatile u_long *` serves both `Gpu_WriteGp1` and `Gpu_WriteGp0Words`; the non-volatile spelling was not needed. |
| `D_8009AF34` | `g_FmvStripIndex` | `g_FmvStripIndexRaw` | The second symbol looked like a CSE break inside `GameUploadFmvSlice`; it is not, the single symbol matches. |
| `D_8009B1B0` | `g_CdCurrentTrack` | `g_CdTrack` | The track the CD-DA driver last selected (`GameStepCdTrackRequest` writes it from `g_CdTrackPending`); two of the three files already said "current". |
| `D_8009B720` | `g_McCardStatus` (+`V`) | `g_McMenuSlotData` | It is the last `GamePollMemoryCardStatus` (func_8005ECE0) return code — 0 no card yet, 1/2 present, -1/-2/-3 error — compared against those constants and copied into `g_McMenuState`. It is not a pointer to a save-slot record, so `game/menu.h`'s `void *` was wrong as well as the name. |
| `D_8019C754` | `g_AssetBlockPtr2` | `g_SharedAssetPtr` | Third pointer of the asset triple handed to func_8005B768 next to `g_AssetBlockPtr` / `g_AssetSubBlockPtr`; three files out of four already named it that way. |
| `D_8019CB08` | `g_NegconMappingIndex` | `g_NegconConfigIndex` | Same reading, seven files to one. |
| `D_8019CB74` | `g_PrizeScreenState` | `g_PrizeScreenStep` | `GameUpdatePrizeMoneyScreen` switches on it over eight states 0..7; it is not a step size. |
| `D_8019CE0C` | `g_PromotionBonus` | `g_PendingClassBonus` | Loaded from `g_PromotionBonusTable[class]` and printed under the screen's own "PROMOTION BONUS" caption (`D_80010E40`). That it counts down to zero is documented, not encoded in the name. |
| `D_801E3FB6` | `g_EnvColors` | `g_EnvFogColor`, and its `G`/`B` siblings | `track/GameSeekEnvironmentScript.c` needed the three bytes of `g_EnvColors[0].cur` individually for SetFarColor; it now reaches them through `ENV_FOG_RGB` = `((u8 *)g_EnvColors)`, which matches. |
| `D_801E4B34` | `g_FrameParity` | `g_DrawBufferParity` | Index of the current frame context in `g_FrameContexts`, written beside `g_DrawBuffer`; the 240-line y bias in `race/` is a consequence, not the definition. |
| `D_801E4B60` | `g_PadButtonMapping` | `g_PadSteerLeftMask`, `g_PadSteerRightMask` | The aggregate works in `car/GameInitPlayerCar.c` too: masks 0 and 1 are now `g_PadButtonMapping[0]` / `[1]`. |
| `D_801E4D98` | `g_SectorEndDistance` | `g_SectorEndDistance0/1/2` | The array spelling was assumed to be impossible in `race/GameUpdateLapAndFinish.c` by analogy with `g_RefSectorTimes`; it was retested and matches. |
| `D_801E6C78` | `g_BonusCountStep` | `g_BonusTickRate` | `bonus / 250`, subtracted from the counter every frame: an amount per tick, not a rate. |
| `D_801E6DA0` | `g_PrizeCountStep` | `g_PrizeTickRate` | Same, `prize / 80`. |
| `D_801F17B0` | `g_PrizeAmount` | `g_PendingPrizeMoney` | The figure under the "PRIZE MONEY" caption (`D_80010E30`), counted down into the total. |

### Kept split, with the experiment that forced it

Each of these compiles fine under one name; it is the *object file* that changes.

| Address | Names | Forcing reason |
|---|---|---|
| `D_8009AF4C` | `g_FmvUploadRect` / `g_FmvUploadRectX` | Whole-`Rect` copy and a `volatile s16` x in the same function. `g_FmvUploadRect.x` does not match. |
| `D_8009AF90` | `g_RefSectorTimes` / `g_RefSectorTime0` | Retested: `g_RefSectorTimes[k]` in `GameUpdateLapAndFinish` still shifts the whole surrounding allocation. |
| `D_8009B538` | `g_McEvents` / `g_McHwEventIoe` | Retested: `g_McEvents[k]` in `GameClearMemoryCardHwEvents.c` keeps the base live in a callee-saved register and grows the frame. |
| `D_8009B720`, `D_8009B72C`, `D_8009B740` | `g_McCardStatus` / `g_McMenuSubState` / `GameMenuLoadPhase` plus a `…V` alias each | `save/GameUpdateMemoryCardMenu.c` reads each of the three **both** ways, and only the volatile spelling forces the reload retail has at those sites. A redeclaration cannot add the qualifier: gcc 2.6.3 keeps the first declaration's type (that is also why the `volatile` at `GameWriteMemoryCardSaveFile.c:196` is a no-op), so the alias needs its own identifier. Convention adopted: the volatile alias is the base name plus `V`. |
| `D_8019CABC` | `g_GrandPrixSeries` / `g_GrandPrixSeriesU16` | `menu/GameDrawNowLoadingText.c` includes `game/race.h`, and `u16` vs `s16` is a hard "conflicting types" error in gcc 2.6.3, not a warning. |
| `D_801E6CA4` | `g_EffectVolumeScale` / `g_SoundScale` | `SoundScale` is just `{ g_EffectVolumeScale; g_VabIds[3] }`. Spelling the three reads in `GameSetPitchedSoundCue.c` as those two existing globals compiles but does not match, in either direction: a struct member reference is non-aliasing to gcc 2.6.3 (`MEM_IN_STRUCT_P`) and the volume arithmetic reorders. |
| `D_8009AB7C` | `g_SpuRegBase` / `g_SpuRegBaseNv` | `_spu_FgetRXXa` reads the SPU register file through a non-volatile pointer; the volatile spelling does not match. |

### Hardware register mirrors

Twenty-six library globals are just pointers to PS1 hardware, and the data
segment (`asm/PAL/main/data/main/6BE64.data.s`) states which:

| Address | Name | Register |
|---|---|---|
| `D_800942B8` / `D_800942BC` | `g_GpuGp0` / `g_GpuGp1` | 0x1F801810 / 0x1F801814 |
| `D_800942C0/C4/C8` | `g_GpuDmaMadr/Bcr/Chcr` | DMA2 0x1F8010A0/A4/A8 |
| `D_800942CC/D0/D4` | `g_OtcDmaMadr/Bcr/Chcr` | DMA6 (OTC) 0x1F8010E0/E4/E8 |
| `D_800942D8` | `g_GpuDpcr` | DPCR 0x1F8010F0 |
| `D_8009A4BC/C0/C4` | `g_IrqStatus` / `g_IrqMask` / `g_KernelDpcr` | I_STAT 0x1F801070, I_MASK 0x1F801074, DPCR |
| `D_8009AB7C` | `g_SpuRegBase` | SPU register file 0x1F801C00 |
| `D_8009AB80/84/88` | `g_SpuDmaMadr/Bcr/Chcr` | DMA4 0x1F8010C0/C4/C8 |
| `D_8009AB8C` / `D_8009AB90` | `g_SpuDpcr` / `g_SpuDelayReg` | DPCR, SPU_DELAY 0x1F801014 |
| `D_80099300..0C` | `g_CdReg0..g_CdReg3` | CD ports 0x1F801800..03 |
| `D_80099310` / `D_80099334` | `g_ComDelayReg` / `g_CdromDelayReg` | 0x1F801020 / 0x1F801018 |
| `D_80099338` | `g_CdDpcr` | DPCR |
| `D_8009933C/40/44` | `g_CdDmaMadr/Bcr/Chcr` | DMA3 0x1F8010B0/B4/B8 |

Three libraries keep their own pointer to DPCR, which is why `g_GpuDpcr`,
`g_KernelDpcr`, `g_SpuDpcr` and `g_CdDpcr` are four names: four distinct
addresses holding the same value.

## 23. Library globals pass (`sdk/`, `lib/kernel`, `lib/libspu`, `render/`)

Naming pass over the raw `D_` globals of the Sony libraries, where there is
external ground truth: the hardware map (initialised pointers in
`asm/PAL/main/data/main/6BE64.data.s` literally hold `0x1F801xxx`), the psyq
function names the files are already named after, and the trace strings in
rodata. Every batch below was verified with `make check VERSION=PAL`.

### libgpu env block (`0x800941E0`–`0x80094304`)

`ResetGraph` memsets 0x80 bytes from `0x800941E8` and its own trace string calls
that pointer `env=%08x`; `0x800941E8 + 0x80 = 0x80094268`, which is exactly the
end of the block below. That is why the two environment caches sit where they do.

| Address | Name | Evidence |
|---|---|---|
| `D_800941A0` | `g_GpuJumpTable` | the driver table `g_GpuFuncs` points at; `ResetGraph` prints it as `jtb=%08x` |
| `D_800941EC` / `D_800941EE` | `g_VramWidth` / `g_VramHeight` | loaded from the per-type tables below; every draw-area builder clamps against them |
| `D_800941F0` / `D_800941F4` | `g_DrawSyncCbPending` / `g_DrawSyncCallback` | set by `Gpu_AddQueue`, cleared and invoked by `Gpu_ExecuteQueue` when the ring drains; `DrawSyncCallback` swaps the second |
| `D_800941F8` / `D_80094254` | `g_DrawEnvCache` / `g_DispEnvCache` | the 0x5C and 0x14 bytes `PutDrawEnv`/`GetDrawEnv` and `PutDispEnv`/`GetDispEnv` copy |
| `D_80094268` / `D_8009427C` | `g_VramWidthTable` / `g_VramHeightTable` | five longs each, indexed by graph type |
| `D_80094298/9C/A0` | `g_MoveImageSrc/Dst/Size` | the three words `MoveImage` fills before sending a 0x14-byte GP0(80h) list |
| `D_800942A4` | `g_OtagTerminator` | its 24-bit address is the last "next" tag written by `ClearOTag`/`ClearOTagR`; the data is a dead-end packet |
| `D_800942DC/E0/E4` | `g_GpuLastCb/Arg/Data` | what `Gpu_ExecuteQueue` records and the timeout dump prints as `func=(%08x)(%08x,%08x)` |
| `D_800942EC` / `D_800942F0` | `g_GpuQueueWriteIdx` / `g_GpuQueueReadIdx` | the mod-64 pair whose difference `& 0x3F` is the printed queue depth |
| `D_800942F4/F8/FC` | `g_AddQueueIntrMask` / `g_ExecQueueIntrMask` / `g_GpuResetIntrMask` | each holds a `SetIntrMask(0)` result restored on the matching exit |
| `D_80094300` / `D_80094304` | `g_GpuTimeoutDeadline` / `g_GpuTimeoutPolls` | `VSync(-1) + 240`, and the poll counter that forces the timeout past 0xF0000 |
| `D_801E5024` | `g_GpuQueue` | the 64 x 96-byte ring both indices address; `Gpu_Reset` clears exactly 0x1800 bytes |
| `D_8009B9B0` | `g_ClearImagePacket` | the 12-word list `Gpu_ClearImage` builds |
| `D_80094308` / `D_8007B664` | `g_SinTable` / `g_AtanTable` | 0x401-entry quarter-wave tables on 4096 units per turn, used by `rsin`/`rcos` and `GameAtan2` |

`sdk/PutDispEnv.c` keeps a `GfxState` struct over the head of this block instead
of the six scalars the rest of the tree uses. Replacing it with those scalars
compiles and does not match — same `MEM_IN_STRUCT_P` effect as `g_SoundScale` —
so it stays, aliased to the base's one name.

### Kernel (`lib/kernel`, `sdk/StopKernelInterrupts.c`)

`g_IntrState` (`D_80099430`) is the interrupt block: `[0]` installed flag,
`[1]` `g_IntrInDispatch`, `[2..]` `g_IntrCallbacks` (11 slots), `+0x30`
`g_IntrCallbackMask` (which slots are filled — the dispatcher ANDs it with
I_STAT & I_MASK), `+0x38` the register frame `HookEntryInt` is pointed at.
`g_IntrSavedIrqMask` / `g_IntrSavedDpcr` (`D_80099462/64`) are what
`StopKernelInterrupts` parks and `StartKernelInterrupts` restores;
`g_IntrStuckCount` (`D_8009A4C8`) is the 0x801-iteration stuck-IRQ guard;
`g_IntrRpNode` (`D_8009A4B8`) is the node handed to `SysEnqIntRP`.
VSync: `g_VSyncCallbacks` / `g_VSyncCount` (`D_8009A4CC` / `D_8009A4EC`),
`g_VSyncGpuStat` and `g_Timer1CountReg` with their two sampled `*Base` words.
DMA: `g_DmaCallbacks` (`D_8009A4F8`), `g_DmaIrqControl` (DICR),
`g_DmaChannelRegs` (0x1F801080). Root counters: `g_RootCounterRegs` (0x1F801100,
three 16-byte banks), `g_IrqRegs`, `g_RootCounterIrqBits`.

### libmdec (`sdk/MDEC_reset.c`, `sdk/_new_card.c`)

DMA channel 0 is MDEC-in and channel 1 MDEC-out, hence
`g_MdecInDmaMadr/Bcr/Chcr` and `g_MdecOutDmaMadr/Bcr/Chcr`, plus `g_MdecCmdReg`
(0x1F801820) and `g_MdecCtrlReg` (0x1F801824). The two parameter blocks are a
command word followed by its table: `g_MdecQuantCmd` = 0x40000001 (command 2,
set quant tables, colour) with `g_MdecQuantLuma` / `g_MdecQuantChroma`, and
`g_MdecIdctCmd` = 0x60000000 (command 3) with `g_MdecIdctTable`, whose first
entry 0x5A82 is sqrt(2) x 2^14.

### libcd accessors and streaming

`CdStatus`, `CdMode` and `CdLastPos` are one-line accessors, which names
`g_CdStatusByte`, `g_CdModeByte` and `g_CdLastPos` outright. `StSetRing`'s two
arguments name `g_StRingBase` and `g_StRingSize`; `g_StRingSlot` is the slot
index `StGetNext` walks and wraps.

### Name propagation

97 addresses that already had a name in one file were still spelled `D_…` in
others; those uses now use the name. Two were reverted and are commented in
place: `car/GameUpdateCarTrafficAvoidance.c` reuses `D_801E40B8` for something
other than `g_SceneTimer`, and `sdk/SpuVmAlloc.c` includes `game/audio.h`, whose
`g_SndVoiceFlags` is `volatile` — and gcc 2.6.3 rejects a requalified *array*
redeclaration outright, where it merely warns for a scalar.

### libspu (`lib/libspu`, `sdk/Spu*.c`)

`g_SpuRegBase` (0x1F801C00) is the register file; `_spu_FsetRXX`/`_spu_FgetRXXa`
index it, which is where the psyq names already in these files come from. The
state named in this pass: `g_SpuTransferMode` / `g_SpuTransferByIo` (the
`SpuSetTransferMode` selector, which `_spu_Fw` uses to pick DMA or the
`_spu_writeByIO` loop), `g_SpuTransferStartAddr` (register 0xD3, in 8-byte
units), `g_SpuTransferCompleted` / `g_SpuTransferIsRead` /
`g_SpuTransferCallback` / `g_SpuTransferEvent` (EvSpEND, 0xF0000009),
`g_SpuDmaTransferAddr` / `g_SpuDmaBlockCount`, `g_SpuKeyStatus` (the bit-per-voice
mask `SpuGetKeyStatus` turns into the LibRef Table 15-1 values), the reverb group
`g_SpuRevReserveWa` / `g_SpuRevWorkAreaAddr` /
`g_SpuRevAttr` / `g_SpuRevWorkAreaStartAddr` / `g_SpuRevAttrTable`, the
`g_SpuMemMode` unit/shift/mask quartet, `g_SpuZeroBuf` (0x400 zero bytes DMA'd
over the reverb work area), `g_SpuDummyAdpcmBlock` (16 x 0x07, the silent ADPCM
block written at SPU RAM 0x1000) and the four `SPU:T/O [%s]` message strings.

### libsnd (`lib/libsnd`, `sdk/Ss*.c`)

`g_SndSpuRegs` is libsnd's own 0x1F801C00 pointer. The tick group
(`g_SndTickMode`, `g_SndTickResolution` 50/60/120/240, `g_SndNoTickFlag`,
`g_SndTickCallback` = `SsSeqCalledTbyT`, `g_SndTickUsesVSync`, `g_SndTickIrq`,
`g_SndTickVSyncToggle`, `g_SndPrevVSyncCallback`) comes straight off
`SsSetTickMode` and the SS_TICK* / SS_NOTICK spec bits. `g_SndSeqTable` with
`g_SndSeqTableSMax` / `g_SndSeqTableTMax` are `SsSetTableSize`'s three arguments;
`g_SndMarkCallbacks` is `SsSetMarkCallback`'s `[32][16]` table, dispatched on
controller 0x28. The VAB side is `g_SndVabProgTable` / `g_SndVabHeader` /
`g_SndVabToneTable` per id, latched into the `g_SndCurrent*` set by
`SpuVmVSetUp`, plus `g_SndVabStatus`, `g_SndVabOpenCount`, `g_SndVabBodySize`,
`g_SndVabSpuAddr`, `g_SndVabBodyAddr`. `g_SndPitchTable` is the note-to-pitch
table indexed `[semitone * 16 + fine/8]`.

Two corrections this pass produced:

- `sdk/SsUtFlush.c` had its four key-register locals swapped. `spu[0xC4]/[0xC5]`
  is KON (0x1F801D88) and `spu[0xC6]/[0xC7]` is KOFF, so `D_8009E670/74` are
  key-**on** and `D_801F2A08/0C` key-**off**, and `spu[0xCC]/[0xCD]` is EON, not
  noise. All four key words keep their raw `D_` spelling — inline-asm `%hi`/`%lo`
  pairs elsewhere stringify them.
- `include/game/audio.h` described the 0x8009DF20 block as "note at +0, fine
  detune at +2" and the 0x34 record as "pitch +0xC, level +0x10, program +0x14".
  Both were wrong: the first block is a straight SPU voice-register shadow
  (volL, volR, pitch, address, ADSR x2) and the second has note at +0xC, actual
  program at +0x10 and tone at +0x14. The error came from the mislabelled
  `SpuVmKeyOnCore(voice, note, fine, …)` prototype, whose 2nd and 3rd arguments
  are really the left and right volumes.

### libcd (`sdk/CD_*.c`, `sdk/Cd*.c`, `sdk/cd_read.c`)

Six 32-entry tables indexed by the `CdlCOM` code drive the whole command layer,
and their contents identify them: `g_CdCommandNeedsSetloc` (send CdlSetloc
first), `g_CdCommandParamCount` (3 for CdlSetloc, 2 for CdlSetfilter, 1 for
CdlSetmode/CdlGetTD), `g_CdCommandHasComplete` (an INT2 is still owed),
`g_CdCommandClearsReady`, `g_CdCommandAckHasStatus` (0 exactly at CdlGetlocL and
CdlGetlocP, which return position data rather than a status byte), plus the
`g_CdCommandNames` / `g_CdIntrNames` string tables behind CdComstr/CdIntstr.
State: `g_CdSyncCallback` / `g_CdReadyCallback`, the `g_Cd*Status` /
`g_Cd*Result` triples for INT1/2/3/4/5, `g_CdDebugLevel` (CdSetDebug),
`g_CdErrorByte`, `g_CdShellOpenCount`, and the `g_CdTimeoutDeadline` /
`g_CdTimeoutCounter` watchdog. The directory cache is one contiguous run:
`g_CdFileCache[64]` CdlFILE records, `g_CdPathTable[128]` ISO path-table records,
`g_CdSectorBuf` (one 0x800-byte staging sector) — the address arithmetic between
them is exact. `cd_read.c`'s statics become the `g_CdRead*` set (callback, sector
count, buffer and cursor, mode, words per sector, remaining, the two VSync
watchdog stamps, expected sector, and the two saved libcd callbacks). libds adds
`g_StBackLoc` / `g_StBackFrame` (what `StGetBackloc` returns), `g_StColorMode`
(bit 0 of the StSetStream mode: 0 = 16-bit, 1 = 24-bit), `g_StNotStream2Mode`,
`g_StFrameCallback`, `g_StDmaBusy` and `g_StInterruptPending`.

### Left raw on purpose

- Every symbol in the `%hi`/`%lo` and `LA_ORDERED` set: renaming one compiles
  clean and fails only at link.
- `D_8009A710`, `D_8009AB98`, `D_8009ABB4` (libspu), `D_8009A569` (libsnd):
  written zero and never read anywhere in the image. `D_8009ABB4` is shaped like
  `SpuSetIRQCallback`'s slot but this build has no call site to prove it.
- The field aliases inside `g_SndVoiceState`, `g_SndCurrentAttr`,
  `g_CdPathTable` and `g_DispEnvCache`: they are struct members reached through
  their own labels, and naming them separately would fight the struct.
- The auto-vol / auto-pan ramp words `D_8009E0D6..DE` and `D_8009E0E2..EA`, the
  libds ring cursors `D_801E6C74` / `D_801E6C84`, and the `StSetStream`
  end-of-stream callback `D_8019C9A0`: every remaining reader is inside
  `StCdInterrupt` / `SpuVmAutoVol`, which are still assembly, so the roles are
  not settled.
- The `render/Game*.c` game-side globals (camera, HUD, glyph tables, track
  texture streaming). They are game state, not library state, and pinning them
  down needs callers outside this pass.

## 24. Shiftability: the program no longer contains its own addresses

The build used to hand the linker 1618 assignments of the form
`D_8009B168 = 0x8009B168;`, generated by taking the address out of the symbol's
own name. A linker-script assignment overrides a definition, so each of those
lines nailed one address down for good: nothing in .text or .data could change
size without silently corrupting whatever came after it.

They are gone. One survives, and one is correct:

| was | now |
|---|---|
| 834 uninitialised globals past the end of the image | defined in `.bss`, laid out by the linker |
| 121 bss names nothing referenced | defined alongside them, no longer dangling |
| 615 references into `.data` / `.rodata` / `.text` | resolved to the object that already defines them |
| 63 mid-function entry points and branch targets | `func_X = func_Y + 0x40;` in `undefined_syms_manual.txt` |
| 12 `lui`/`ori` pairs no `%hi`/`%lo` can spell | linker-computed halves, `addr_halves.main.txt` |
| `D_1F800084` | still absolute, and rightly so: scratchpad RAM |

### The bss layout

`configs/PAL/bss.main.txt` is the table, `tools/scripts/gen_bss.py` turns it
into `asm/PAL/main/bss.s` and adds it to `.main_bss`. It covers
`0x8009B000..0x801F2A10`, 1.34 MB, in one run: the image ends at `0x8009B000`
and bss picks up 0x168 bytes later.

Sizes come from the gap to the next referenced address, which makes them upper
bounds on the true object size and never lower bounds, so an over-wide entry
wastes nothing and cannot overlap its neighbour. What is *not* referenced is
emitted as anonymous padding, including the three large runs that look like
declared arrays: 0xFD648 after `D_8009F0B8`, and two consecutive gaps of
exactly 0x23656 around `0x801C0504` -- equal to the byte, which is what a
`[2]` of something looks like. Two more gaps are round in decimal (12000 and
24000 bytes), the signature of a C array bound rather than a hardware address.

The evidence that the region is linker-allocated rather than hand-placed is
that none of the six cluster bases is a round number: `0x8019C700`,
`0x801C0504`, `0x801E3CEC`, `0x801F179C`. Hard-coded work areas land on
boundaries a human chose; these are where a linker happened to stop.

The layout is verified, not assumed. Every one of these addresses reaches
.text through a `%hi`/`%lo` pair, so a single wrong offset changes instruction
bytes and `make check` fails. A green build is a byte-exact proof of the table.

### `lui`/`ori` address halves

The hand-written GTE engine builds a few addresses with `lui`+`ori`. `%lo`
relocation arithmetic assumes the sign extension of `addiu`, so when bit 15 of
the address is set, `%hi` computes a different upper half than the original
`lui` holds and no relocation pair spells the instruction. `lui $t2, sym_hi`
assembles to an `R_MIPS_LO16` against `sym_hi`, so defining
`sym_hi = (expr) >> 16` in the linker script reproduces the literal encoding
while leaving the value a function of the symbol. `gen_nonmatching_asm.py`
writes those definitions to `linkers/PAL/addr_halves.main.txt`.

### What is still absolute, and why

- `D_1F800084` -- the scratchpad, `0x1F800084`. Fixed hardware RAM; the render
  path writes it as a literal (`*(s32 *)0x1F800084 = colour` in `GameDrawCar`).
  It has nowhere to move to.
- 142 `lui`/`lo` pairs building scratchpad addresses `0x1F8000xx`, same reason.
- 74 pairs building plain 32-bit constants (0x7A120, 0xFFFFF, ...). Not
  addresses at all.
- 3 pairs pointing at jump tables in `.rodata` that the split-time symbol
  table has no name for.

### Checking it

Two link-only experiments, neither of which touches the shipped build:

- relink `.main` at `0x80410000`: all 4736 symbols move by exactly 0x400000,
  and only `D_1F800084` and the ROM-offset accounting symbols stay put;
- insert `. += 0x40;` in the middle of `.text`: everything at or after that
  point moves by 0x40 and nothing is left behind -- including bss, and
  including the `lui`/`ori` halves, whose encoded immediate tracks the symbol.

## 25. Global-allocation priorities are not recoverable from retail assembly

GCC 2.6.3's candidate-side allocation order is reproducible from `.lreg` and
`.greg`, but there is no corresponding retail order to extract from final
assembly. This matters for the six large remaining functions: treating
hard-register use ranges as retail pseudos would manufacture the inputs to the
comparison.

The exact comparator in `global.c:586-606` is:

```text
int(floor_log2(allocno_n_refs) * allocno_n_refs
    / allocno_live_length * 10000 * allocno_size)
```

Size is **multiplied**, not divided. Ties use allocno number. Nor is there
necessarily one allocno per pseudo: `global.c:383-423` groups `reg_may_share`
pseudos, sums their reference counts, and takes their maximum live length.

The required retail inputs disappear before assembly:

- `flow.c:1298-1364,1973-1983` derives weighted references from
  `NOTE_INSN_LOOP_BEG/END` and adds the current RTL loop depth per mention;
- live lengths, pseudo boundaries, allocno grouping, conflicts, and preferences
  all exist before global allocation and reload;
- `toplev.c:3094-3140` performs allocation/reload and dumps `.greg`, while
  `toplev.c:3251-3273` emits assembly only later.

The loss is demonstrably many-to-one. The two controls in
`scratch/decomp-work/gcc263-alloc-diff-impossibility/` use a noted `for` loop
and an equivalent backward `goto`. Both have allocnos 71/72, dispositions
`71 in 16, 72 in 17`, and byte-identical 76-byte `.text` sections. Their hidden
metadata differs:

| form | p71 refs/length/priority | p72 refs/length/priority |
|---|---|---|
| noted `for` | 9 / 7 / 38571 | 6 / 9 / 13333 |
| backward `goto` | 5 / 7 / 14285 | 4 / 9 / 8888 |

No assembly parser can decide which row generated those identical bytes.
Therefore a retail pseudo order, first inversion, or numeric percentage gap is
not observable.

The recorded `func_800418D4` 887-to-780 edit remains useful candidate evidence:
moving `var_t0_395 = 0` earlier changes pseudo 156 from 5 refs across 167 insns
(priority 598) to 5 across 206 (priority 485), moving it below pseudos 73, 151,
154, and 82 in `.greg`. It does not reveal the retail values. Such lifetime
edits must be evaluated by compiling and comparing linked `exact`, preferably
through permuters, rather than by assigning invented priorities to retail
assembly. See `docs/TASK_FOR_CODEX_2I_REPORT.md` for the full proof and six
remeasured starting points.

## 26. The remaining 27: two populations, not one

The progress counter reports 1078/1105. Running `classify_with_addresses` over
every unit and grouping the `asm` verdicts shows the 27 outstanding functions are
not one backlog but two, with very different economics.

**Sixteen are `INCLUDE_ASM` stubs — 11417 words.** These are undecompiled: the
retail assembly is included verbatim and no C exists for them. They run from
`func_800458CC` (258) to `func_8004A248` (1435).

**Eleven are already decompiled — 1224 words total.** They read as ordinary C and
count as unfinished only because they still carry an inline-assembly crutch.
Each is worth the same one point on the counter as a stub, for roughly a ninth of
the code. Sizes: `func_8001D338` 108, `func_8001D5F0` 65, `func_80027874` 94,
`func_800483D4` 107, `func_8005F6BC` 88, `func_8005F88C` 402, `func_8006BF00` 91,
`func_80075EB4` 60, `func_80076C58` 93, `func_80076ED8` 81, `func_80078528` 35.

Five of the eleven had never been examined. What follows is what their crutches
actually contain, decoded.

### 26a. `func_8001D5F0` — `GameBlendPaintColorThirds`, 65 words

The entire body is one `__asm__ volatile` block. It is not obscure: it is two
15-bit RGB blends written out longhand.

`0x55555556` with `mult`/`mfhi` and the `sra 31`/`subu` correction is signed
division by three (the same family as the `0x66666667` and `0x2AAAAAAB` cases in
21c). The block extracts each 5-bit channel from both arguments, sums the pair,
and stores two results: `g_PaintBlendShade0` gets `sum * 2 / 3`, and
`g_PaintBlendShade1` gets `sum / 3`. The trailing `addiu $5,$0,-0x8000` plus
`addu` before each `sh` sets the semi-transparency bit, since the accumulated
value never reaches bit 15.

So the function is two thirds of the way from colour A to colour B, and one third,
per channel. That is exactly what the name says. Expressed directly:

    r = (arg0 & 0x1F) + (arg1 & 0x1F);
    g = ((arg0 >> 5) & 0x1F) + ((arg1 >> 5) & 0x1F);
    b = ((arg0 >> 10) & 0x1F) + ((arg1 >> 10) & 0x1F);
    g_PaintBlendShade0 = (r*2/3) | ((g*2/3) << 5) | ((b*2/3) << 10) | 0x8000;
    g_PaintBlendShade1 = (r/3)   | ((g/3)   << 5) | ((b/3)   << 10) | 0x8000;

The `andi $4,$4,0xFFFF` before the green and blue extractions is redundant against
a `u32` argument and does not need reproducing as a cast.

### 26b. `func_8001D338` — `GameComposeSampleTeamLogo`, 108 words

Three `__asm__ volatile` blocks, and all three compute nothing but addresses. The
loops around them are already plain C. Every block multiplies a row index by 33
and shifts by 6, so the stride is 2112 bytes, and the third block offsets by 0x40
into it. The record behind `g_TeamLogoSampleData` is therefore:

    struct TeamLogoSample {
        u16 clut[2][16];    /* 64 bytes, one palette per parity */
        u16 canvas[64][16]; /* 2048 bytes, 4bpp pixels */
    };                      /* 2112 */

With `row0 = arg0 / 2`, `row1 = arg1 / 2 + 10`, and parity `arg & 1`, the three
blocks reduce to `&sample[row0].clut[arg0 & 1][1]`, then
`&g_TeamLogoClut[index]` paired with `&sample[row1].clut[arg1 & 1][index]`, then
`sample[row0].canvas` and `sample[row1].canvas`.

That also explains the composition: palette entries 1..11 come from the first
logo, 12..15 from the second, and the pixel loop fills any zero nibble of the
first logo's canvas from the second's. It is a two-layer overlay where colour zero
is transparent. `g_TeamLogoSwatches` (`D_801E444E`) is `&g_TeamLogoClut[1]`, which
is why the first copy needs no explicit index.

The two pins, `row1` in `$t2` and `adjusted` in `$v0`, exist only because the asm
blocks name `$t1`/`$t2` directly. They should fall away with the blocks.

### 26c. `func_80078528` — `SsUtSetVVol`, 35 words

The same shape as `SsUtChangeADSR` in the same unit before it was cleaned: a
phantom `addiu $sp,$sp,-8` frame, a matching `addiu $sp,$sp,8`, and an early
return spelled as `j 2f` past a hand-written label. The arithmetic is `sll 7` plus
`addu`, so both arguments are scaled by 129, after an explicit 16-bit
sign-extension of each.

`SsUtChangeADSR` is the precedent worth following rather than the phantom-frame
note in 21a: there the frame crutch and correctly typed parameters turned out to
be mutually exclusive, and typing the parameters won. The explicit `sll 16`/`sra
16` pairs here are the same signal, so the first thing to try is the parameter
types, not the frame.

### 26d. `func_8005F88C` — `GameStoreSaveStateBlock`, 402 words

Four `asm volatile` blocks, each an entire three-level loop nest written out with
numeric labels and clobber lists of fourteen to nineteen registers. They copy the
record arrays into the save block: best lap and total times, ranking and time
records, sector times, then the volume and progress fields. Mechanical rather than
subtle, but it is the largest of the eleven and the loop bounds (2 x 4 x 2, 2 x 4 x 3)
and strides (0x20, 0x30, 0xC) are all visible in the encoding.

### 26e. `func_80076ED8` — `SpuVmSeqKeyOff`, 81 words

The weakest of the five, and the one to leave alone. The loop itself is
hand-encoded: `asm volatile("1:")` as the head, a final block ending
`bnez $2,1b`, and a C `for(;;) { ... break; }` wrapped around them purely to give
the labels somewhere to live. Eight register pins feed it. This is the same
construction class that was rejected from a contributed `func_8003425C` on
2026-07-29, and it predates that rule; it is not evidence that the approach is
sanctioned.

## 27. `func_800418D4`: where schedule search runs out

Recorded because someone will otherwise repeat it. The largest remaining stub
(1211 words, `main/track/GameDrawTerrainCells.c`) was worked down from 663 to
**418** differing words, measured independently rather than taken from the
agent's own notes:

    retail 1211 words, candidate 1211 words
    exact 418, aligned 393, equal 939 of 1211
    first differing word at +0x88

Two things are settled by that. The candidate is **exactly the right size**, and
the first 34 words match, so the frame, the save set and the prologue are right;
that was the earlier 887 -> 780 result and it has held through everything since.

The method that got there was exhaustive machine search over legal statement
schedules: all 60 orders of the post-loop reset/scaling prefix, all 1260 orders
of the seven coordinate-rounding chains, then random jumps with insertion
descent over the packet field stores. Its trajectory was
452, 450, 445, 444, 441, 440, 439, 438, 424, 418 — one to five words per
exhaustive round, each round costing more than the last. At that rate zero is
not reachable, and the search is at a single-insertion local minimum on every
block it has touched.

**The diagnostic it did not use.** `aligned 393` is *lower* than `exact 418`.
Pure allocation divergence gives the two numbers close together, because
allocation changes which register a word names, not how many words there are.
A 25-word gap means allowing insertions realigns the stream, so there is a local
shape difference: an instruction present on one side and absent on the other.
Statement-schedule permutation cannot fix that by construction, since permuting
statements does not change the instruction count. That gap is where the next
attempt should start.

**A second cost, easy to miss.** The candidate is raw m2c-shaped output: 1265
lines carrying `goto block_14`, `goto block_153`, `loop_25` and names like
`var_a0_209`. None of that breaks the fabrication rules, but the project's goal
is readable, unit-compilable C, so reaching `exact 0` in this form would not be
a finished function. The readability pass is not free either: it changes
allocation, and would give back some of the 418.

## 28. Unaligned copies: how to make 2.6.3 emit `lwl`/`lwr`

This blocked one function for two passes and was recorded in 21a as an open
problem, so the answer belongs somewhere findable. Two source forms produce
retail's unaligned load/store pairs, and neither needs `packed`.

**An eight-byte `memcpy` between `u8 *` pointers emits `lwl`/`lwr` plus
`swl`/`swr`.** Proven in `GameStoreSaveStateBlock`, where the two course-progress
copies are now:

    memcpy(arg0 + 0xFC8, g_GrandPrixCourseProgress, 8);
    memcpy(arg0 + 0xFD0, g_ExtraGrandPrixCourseProgress, 8);

GCC 2.x expands a small block move of known size inline, and when it cannot
assume the operands are word-aligned it emits the unaligned pair rather than
`lw`/`sw`. Copying between `u8` arrays is exactly that situation. The same call
with a 16-byte length against word-aligned `s32` arrays emits ordinary `lw`/`sw`,
which is why the record copies in the same function also use `memcpy` and still
match.

**A four-byte struct assignment does the same for one word.** `GameDrawStartCountdown`
needed retail's unaligned four-byte palette write and reached it with

    typedef struct CountdownColorCode { u8 r, g, b, code; } CountdownColorCode;
    *(CountdownColorCode *)color = colors[pattern & 1];

which is an aggregate copy between `u8`-aligned addresses.

**What does not work, and should not be retried.** Struct-level
`__attribute__((packed))` **crashes `cc1`** in this compiler; member-level packed
is silently ignored with a warning. Four explicit byte loads and stores do not
produce the pair either, and they grow the function.

So the rule is that the unaligned pair follows from the compiler not being able
to prove alignment, and the way to arrange that is a byte-typed aggregate or
block move, not an attribute and not hand-written assembly.

## 29. `func_8005F6BC`: two words the scheduler will not give back

`GameBuildSaveIconBlock` reaches **88 words, exact 2, 86 of 88 equal** from ordinary
C. The remaining two words are a rotation of three adjacent instructions:

    retail:    mult s0,v0 ; addiu a1,s2,0x60 ; move s5,zero
    candidate: move s5,zero ; addiu a1,s2,0x60 ; mult s0,v0

The `LM43`/`LM44`/`LM45` markers confirm those are three consecutive source
statements, so retail's source did write the division first.

The cause is that the two versions give the scheduler different information.
In the committed crutch source the multiply is an opaque `asm_operands` insn with
priority 1 and no modelled HI/LO dependency on the later `mfhi`, so it stays put.
Written as real division, GCC expands it to `smulsi3_highpart_internal`, the R3000
multiply latency enters the model, and the scheduler places `mult` at the latest
slot that still avoids a stall before `mfhi`. Retail's multiply sits two slots
earlier than that, with slack the model says is unnecessary.

Moving it earlier would require the two intervening values to depend on the
quotient, but they are independently `block + 0x60` and zero. Manufacturing that
dependency needs an algebraic zero such as `iconTile - tileRow * 20 - tileX`,
which is fabrication.

Searched and rejected, all landing on exact 2: 2520 legal orders of the
divide/rect/counter/image/width/height initialisation, 1440 prefix and field-order
combinations, all 128 register-pin subsets, all 768 pin-subset plus tail-order
combinations, all 64 empty-barrier subsets, plus division spellings, declaration
orders, local types, storage views, inline helpers and use-site initialisers.

**Also tested and falsified afterwards, so do not retry it.** Retail synthesises
the times-twenty as `sll`/`addu`/`sll`, which suggested that spelling the remainder
explicitly as `tileX = iconTile - tileRow * 20` rather than `iconTile % 20` would
lengthen the visible dependency chain from the multiply and raise its scheduling
priority. It does not: the explicit form builds and gives **exactly the same two
differing words**. GCC 2.6.3 expands `%` by a constant into that same shift-add
chain, so the two spellings are the same RTL.

An inline-helper probe (`scratch/compiler-source/inline_frame_probe.c`) separately
confirmed that an inlined register-only helper adds no stack area; only an inlined
stack object or a wider outgoing-argument requirement enlarges the caller frame.

So this function is blocked on the scheduler's latency model, not on a missing
source shape, and closing it honestly needs either a construct that changes that
model or evidence that retail was built with different scheduling. It is 86/88 and
should be left alone rather than forced.

## 30. Types pass over `func_8003B0D4` / `func_8003BB50` (`GameUpdateRaceCars`, `GameUpdateAttractCars`)

These two stubs, 671 and 622 words, were picked because they carry the highest
density of the `sll`/`addu`/`sll` non-power-of-two stride idiom of the fifteen
remaining stubs, on the theory that a dense stride is an undiscovered
array-of-struct. **It is not. Every one of the six stride idioms in the two
functions multiplies by 412, and 412 is `sizeof(GameCarRuntime)`.** The pass
still produced results, but the headline is a negative one and it is worth
stating first so nobody re-runs the same reasoning on the other thirteen stubs.

### 30a. The stride arithmetic, worked out

The idiom appears at `8003B170`, `8003B76C`, `8003B7E0`, `8003BC00`, `8003C124`
and `8003C198`, identical in all six:

    sll  $v0, $i,  1      2i
    addu $v0, $v0, $i     3i
    sll  $v0, $v0, 2     12i
    addu $v0, $v0, $i    13i
    sll  $v0, $v0, 3    104i
    subu $v0, $v0, $i   103i
    sll  $v0, $v0, 2    412i

412 = 0x19C, which is the `g_Cars[]` stride already recorded in `car.h`, and the
`addiu $sN, $sN, 0x19C` at the tail of every walking loop in both functions
confirms it independently. Compiling `&cars[i]` for a 412-byte element and a
`short i` against this project's cc1 reproduces the seven instructions plus the
leading `sll 16`/`sra 16` exactly, which is also what proves the loop counters
are `short` and not `int` (`scratch/decomp-work/probe_idx.c`). The base symbol in
each case is one of the split field symbols: `D_801F1900` is
`&g_Cars[0].activeFlag` (0x1900 - 0x1854 = 0xAC), `D_801F18D6` is
`&g_Cars[0].motionTimer` (+0x82) and `D_801F1884` is
`&g_Cars[0].trackPointIndex` (+0x30).

So the two functions contain no aggregate that was not already typed. What they
do contain is a large, well-conditioned sample of *how* an already-typed
aggregate is addressed, and that turned out to be the useful part.

### 30b. Reading the biased base registers

Neither function addresses a car from its own base. Every walking loop keeps a
base register offset into the middle of the element: `+0x8A` in the first loop of
`func_8003B0D4`, `+0x68` in the first loop of `func_8003BB50`, `+0x24` in the
third and fourth loops of both, and `+0xA4` in the last. Offsets from those
registers run negative (`-0xA0`, `-0x66`, `-0x1C`), which no C member reference
can produce, so these are gcc's loop-strength-reduction givs and **the bias is
not in the source**.

The bias is not arbitrary either, and the rule is worth writing down because it
saves guessing at every future loop of this shape: **the giv's zero offset is
always a field the loop really touches.** `+0x8A` has `lhu 0x0` = `field_8A`,
`+0x68` has `lw 0x0` = `trackProgress`, `+0x24` has `lw 0x0` = `field_24`, and
`+0xA4` has `lw 0x0` = `field_A4`. Compiling a three-field walking loop over a
412-byte element reproduces the `+0x8A` case instruction for instruction, biased
register and all, from ordinary field references (`probe_idx.c`, function `m3`).
The consequence for the later conversion is that the bias must be *ignored* when
reading the assembly: add it back and the result is a plain `car->field`.

### 30c. `lhu` carries no signedness information here

This has to be settled before any width table is trustworthy. Compiled against
this project's cc1 (`scratch/decomp-work/probe_lhu.c`), gcc 2.6.3 emits `lhu` for
a 16-bit field in a read-modify-write *regardless of whether the field is signed*,
and also for a masked read:

| source | code |
|---|---|
| `s16 f; p->f = p->f - 1;` | `lhu` / `sh` |
| `u16 f; p->f = p->f - 1;` | `lhu` / `sh` |
| `s16 f; s16 v = p->f + 1; p->f = v; return v*v;` | `lhu` / `sll 16` / `sra 16` / `mult` / `sh` |
| `s16 f; return p->f & 1;` | `lhu` / `andi` |
| `s16 f; return p->f;` | `lh` |
| `u16 f; return p->f;` | `lhu` |

So `lh` at a value-read site proves the field is signed, and `lhu` proves
nothing. That kills three signedness readings that looked available in these two
functions (0x8A at `8003B11C`, 0x9A at `8003B930`, 0x12E at `8003B348`) and it
validates the `lh` readings that remain. It also confirms `s16 field_9A` as
declared: the retail sequence at `8003B930` is `lhu` / `sh` / `sll 16` / `sra 16`
/ `mult`, which is the fifth row of the table verbatim.

### 30d. Widths and signedness recovered

Every offset below is absolute in `GameCarRuntime`; the address column gives one
representative site per width, and the base column says whether the site went
through a giv over the car (`car`) or through the separate `+0xBC` register
(`drive`). Confidence is **high** where an instruction fixes the width and, for
16-bit fields, where an `lh` or a signed comparison fixes the sign; **medium**
where the width is fixed but the sign is not; nothing here is lower than that,
because a width is never a guess.

| offset | width | sign | site | base | agrees with `car.h`? |
|---|---|---|---|---|---|
| 0x00 | 32 | — | `8003B4CC lw 0x0($s2)` | car | yes (`x`) |
| 0x04 | 32 | — | `8003B8E8 lw -0xA0($s0)` | car | **no — declared `s16 y` + 2 pad** |
| 0x08 | 32 | — | `8003B4E0 lw -0x1C($s0)` | car | yes (`z`) |
| 0x0C | 32 | — | `8003B67C sw 0xC($s2)` | car | yes |
| 0x10 / 0x14 / 0x18 | 32 | — | `8003B4D0 lw -0x14($s0)`, `func_80069678` output | car | yes; the three are one `VECTOR` |
| 0x20 | 32 | — | `8003B534 lw -0x4($s0)` | car | yes |
| 0x24 | 32 | — | `8003B3B8 lw 0x0($s0)` | car | yes |
| 0x28 / 0x2C | 32 | — | `8003B8F4 lw -0x7C($s0)` | car | yes |
| 0x44 | 32 | — | `8003B680 lw 0x20($s0)` | car | yes |
| 0x48 | 32 | — | `8003B8C0 lw -0x5C($s0)` | car | yes |
| 0x50–0x5C | 32 | — | `8003B8FC..908 sw -0x54..-0x48($s0)` | car | yes; one 16-byte block |
| 0x60 | 32 | — | `8003B92C sw -0x44($s0)` | car | yes |
| 0x64 | 32 | — | `8003B694 lw 0x40($s0)` | car | yes |
| 0x68 | 32 | signed | `8003BB94 div` | car | yes (`trackProgress`) |
| 0x82 | 16 | **signed** | `8003B824 lh` + `blez` | car | **no — declared `u16 motionTimer`** |
| 0x8A | 16 | signed | `8003BA84 lh` + `bnez` | car | yes |
| 0x90 / 0x94 | 16 | — | `8003BA74 sh -0x14($s0)` | car | yes |
| 0x98 | 16 | signed | `8003B920 lh` + `beqz` | car | yes |
| 0x9A | 16 | — | `8003B930 lhu` (RMW) | car | yes, sign not proven here |
| 0x9C | 16 | signed | `8003B94C lh` + `mult` | car | yes |
| 0x9E | 16 | signed | `8003B9C0 lh` + `slt` | car | yes |
| 0xA0 | 32 | — | `8003B45C lw 0x7C($s0)` | car | yes (`headingAngle`) |
| 0xA4 / 0xA8 | 32 | signed | `8003B2FC lw`, `8003B304 slti` | car | yes |
| 0xAC | 16 | signed | `8003B2C8 lh` + `beq -1` | car | yes (`activeFlag`) |
| 0xC8 | 32 | — | `8003B490 sw 0xA4($s0)` / `8003B5F8 lw 0xC($s4)` | both | yes |
| 0xD0 | 32 | — | `8003B4B4 sw 0xAC($s0)` / `8003B62C lw 0x14($s4)` | both | yes |
| 0xEC | 32 | — | `8003B3D4 lw 0x30($s4)` | drive | yes in `car.h`; **no in `GameCarDrive`** |
| 0xF4 | 32 | — | `8003B4E8 lw 0xD0($s0)` / `8003B6E8 lw 0x38($s4)` | both | yes in `car.h`; **no in `GameCarDrive`** |
| 0xF8 | 32 | — | `8003B124 sw 0x6E($a1)` | car | yes |
| 0x108 | 32 | — | `8003B118 lw 0x7E($a1)` | car | yes |
| 0x126 / 0x128 | 16 | signed | `8003B374 lh`, `8003B2E8 lh` + `slt` | car | yes |
| 0x12C | 16 | signed | `8003B330 lh 0x70($s4)` + `addu` | drive | yes |
| 0x12E | 16 | signed | `8003B2D8 lh 0x10A($s0)` + `blez` | car | yes; **`GameCarAiBlock` had it as pad** |
| 0x130 | 16 | signed | `8003B318 lh 0x74($s4)` + `slt` | drive | yes in `car.h`; see 30f |

### 30e. What the two functions do with those fields

Recorded because several of the widths only make sense with the arithmetic
attached, and because the semantics are what make the `unkNN` names replaceable
later.

`field_C8` and `field_D0` are the world velocity components:
`field_C8 = func_80068568(headingAngle) * field_A4 / 256` and
`field_D0 = func_80068634(headingAngle) * field_A4 / 256`, with the
`bgez`/`addiu 0xFF`/`sra 8` rounding that `GameUpdateCarAirborne` already uses on
the same pair one slot lower (`drive->accelPos`/`brakePos`, 0xC4 / 0xCC). The
position integration is then `x += field_C8 * 6 / 1280` and
`z += field_D0 * 6 / 1280`; the divisor is the `0x66666667` / `sra 9` pair, i.e.
the /5 magic plus eight bits.

`field_F4` is a yaw rate: it is added to `field_44` (clamped to ±0x12C) and to
`field_24` in the same iteration, and `|field_F4| / 6` (`0x2AAAAAAB`, `mfhi`, no
shift) becomes the third component of the `SVECTOR` at `sp+0x88`.

`field_EC` is a target angle for `field_24`, not a value:
`field_24 += GameGetAngleDelta(field_24, field_EC) / 5`, which is character for
character the shape `GameUpdateCarAirborne` uses with `drive->unk90`.

The AI acceleration ramp is `field_A8`, capped by `field_130` and stepped by
`field_126` normally or by `field_12C` while the `field_12E` countdown is
positive; `field_A8` is zeroed instead when `field_128 < field_12E` and
`field_A4 >= 0x321`. `field_A4` itself decays by 94/100 per frame in the AI pass
(`0x51EB851F`, `sra 5`, on `94 * field_A4`) and by 97/100 twice in the last pass.
So `field_130` is a speed cap in acceleration units, which is the reading
`GameUpdateCarTrafficAvoidance` and `GameIsCarNearWaypoint` already take.

`field_98` is a four-state vertical-hop machine over `field_9A` (frame counter),
`field_9C` (launch rate) and `field_9E` (target height), writing `y` at 0x04:
state 1 integrates `field_9C * t + 72 t² / 100`, state 3 integrates
`field_9E + 216 t² / 100`, and each lands when `y` reaches the entry height minus
8, at which point `field_90`, `field_94` and `field_98` are cleared and
`func_80038F0C(1, car)` runs. The `72` and `216` are `(8v+v)*8` and
`((8v-v)*4-v)*8`; both are then divided by 100.

`field_48` is a 12-bit phase accumulator: `(field_48 + step) & 0xFFF` where
`step` is `3 * field_A4` clamped to `0x249` once `(s16)(3 * field_A4) >= 0x1001`,
with bit 12 set separately when `field_A4 >= 0x321`.

### 30f. Contradictions with existing types

**`GameCarRuntime.y` is 32 bits, not 16.** Nine `lw`/`sw` at absolute 0x04 in
each of the two functions (`8003B8E8`, `8003B914`, `8003B98C`, `8003B998`,
`8003B9D8`, `8003B9F4`, `8003BA44`, `8003BA70`, plus the `sw 0x4($s2)` of the
four-word block at `8003B674`, and the same nine at `8003C2A0`.. in
`func_8003BB50`) settle it, and two independent already-matched units agree:
`GameUpdatePlayerCar`'s own layout has `s32 unk04`, and `GameInitRivalCar` writes
it as `*(s32 *)&ent->y = 0`, a cast that only existed to work around the wrong
declaration. **Fixed in `car.h`; build re-verified at
`2913e15648eddef40821c5f666460abc04155ee6`.**

**`GameCarAiBlock` was missing four 32-bit fields and one halfword.** The `+0xBC`
register reads 0x0C, 0x14, 0x30 and 0x38 with `lw` (absolute 0xC8, 0xD0, 0xEC,
0xF4) and 0x72 with `lhu`/`sh` (absolute 0x12E); all five fell inside `pad0` or
`pad72`. **Named in `car.h`, size unchanged at 0xE0; build re-verified.**

**`GameCarDrive` is calibrated on `g_PlayerCar`, and using it for a `g_Cars[]`
element is wrong at four offsets.** `GameCarDrive` declares `s16 gearDisp` /
`s16 unk32` at +0x30 and `s16 unk38` / `s16 unk3A` at +0x38, and both readings
have matched-code support: `GameUpdatePlayerCar` does `p->gearDisp = p->gear` and
`GameSteerCarToTrackLine`'s `GameUpdateCarAirborne` does `r->unk38 * 2 + 80` and
`r->unk38 <= 0`. These two functions read the same two offsets as full words.
The resolution is not that one side is wrong: `GameUpdatePlayerCar` is called
only as `GameUpdatePlayerCar(g_PlayerCar)` / `(&g_PlayerCar)`, i.e. on
`D_8009E6D4`, which is a *different* 0x19C object from `g_Cars` at `D_801F1854`.
The two objects share the stride and much of the layout but not the meaning of
these bytes. The same split explains `manual` / `gear` at `GameCarDrive` +0x74 /
+0x76: absolute 0x130 and 0x132 are the transmission flag and the gear on the
player object, and an acceleration cap and a clamped speed floor on the AI cars
(`GameUpdateCarTrafficAvoidance`, `GameIsCarNearWaypoint`). **`GameCarDrive` was
therefore left alone**, and the four AI-side 32-bit fields were added to
`GameCarAiBlock` instead, which is the view that already exists for exactly this
purpose.

**`motionTimer` (absolute 0x82) needs opposite signs in two units, and `car.h`
must keep the unsigned one.** Retail reads it here as `lh` followed by `blez`
(`8003B824`, `8003C1DC`), which the probe table in 30c shows is only reachable
from a signed field or an explicit `(s16)` cast. Declaring it `s16` was tried and
**moved the sha1**: it changes `GameApplyCarKnockback` in
`GameBuildStartingGrid.c`, whose matched body is
`u32 timer = obj->motionTimer - 1; ... if ((s32)(timer << 16) <= 0)` and depends
on the unsigned promotion. So the declaration stays `u16`, with a comment, and
the conversion of these two functions must spell the test
`(s16)car->motionTimer > 0`. Verified by probe: a `u16` field under an explicit
`(s16)` cast compiles to `lh` + `blez`, identical to a genuine `s16` field.

Nothing found contradicts `GameTrackPoint`, `waypoint.h` or `track.h`; neither
function touches them.

### 30g. Aggregates on the stack

Four, all in the fourth loop of each function, and all of them ordinary PSY-Q
types rather than game structures:

* **Two `MATRIX` at `sp+0x48` and `sp+0x68`, 0x20 bytes each.** Proven by the
  nine `lhu`/`sh` pairs at `8003B574`–`8003B5C4`, which move `0x48/0x4A/0x4C`,
  `0x4E/0x50/0x52`, `0x54/0x56/0x58` into `0x68/0x6E/0x74`, `0x6A/0x70/0x76`,
  `0x6C/0x72/0x78` — a 3×3 halfword transpose — and by `ApplyMatrix`
  (`func_80069678`), whose already-decompiled body loads `matrix[0..4]`, i.e. the
  nine halfwords plus one word of padding.
* **One `SVECTOR` at `sp+0x88`**, written `{0, 0, -(|field_F4| / 6) - 0x32}` by
  three `sh` at `8003B59C`–`8003B5A4`, and passed as `ApplyMatrix`'s second
  argument.
* **A 16-byte vector at `sp+0x10`.** `ApplyMatrix` writes three words
  (`swc2 $25/$26/$27`) to `car+0x10`, so `field_10`/`field_14`/`field_18` is one
  `VECTOR`; the local at `sp+0x10` is the same shape and is copied as four words
  into `car+0x00`. **Only `sp+0x10` and `sp+0x18` are ever written** — the
  four-word copy at `8003B660`–`8003B67C` therefore stores two uninitialised
  stack words into `car->y` and `car->field_0C` every frame for cars 0..3. That
  is a retail defect, not a decoding error: no store to `sp+0x14` or `sp+0x1C`
  exists anywhere in either function, and the o32 argument-save area stops at
  `sp+0x0F` so a callee cannot have written them. Recorded here because a
  converter will be tempted to "fix" it.
* **A two-halfword constant pair at `sp+0x40`**, `{0x3C, -0x3C}`, hoisted out of
  the loop and passed as the third argument of `func_80031298`.

### 30h. Per-access-site spelling: which sites want a member and which want raw arithmetic

This is the part that carries over to the conversion. Two source-level pointers
are addressing the same car in the third and fourth loops of both functions: the
giv over the car itself, and a register that is materialised at the top of every
iteration as `addiu $s4, $s2, 0xBC`. Five absolute offsets are reached both ways
in the same function — 0xC8, 0xD0, 0xF4, 0x12E and 0x130 — and one of them,
0x130, is reached *through different bases in the two arms of one `if`*:
`8003B318 lh 0x74($s4)` in the boost arm and `8003B35C lh 0x10C($s0)` in the
other. A single source spelling cannot produce that, because the base register
choice for one field would be consistent across both arms of the branch. So the
retail source genuinely has two names for the second half of the car, and the
`+0xBC` name is used in the boost arm and the plain one in the normal arm.

| site | offset | verdict | why |
|---|---|---|---|
| `8003B318 lh 0x74($s4)` | 0x130 | `+0xBC` pointer | branch-correlated against `8003B35C`, see above |
| `8003B330 lh 0x70($s4)` | 0x12C | `+0xBC` pointer | same arm as the above; no car-base site exists for 0x12C |
| `8003B348 lhu 0x72($s4)`, `8003B358 sh` | 0x12E | `+0xBC` pointer | read-modify-write in the boost arm, against `8003B2D8 lh 0x10A($s0)` in the guard |
| `8003B3D4 lw 0x30($s4)` | 0xEC | `+0xBC` pointer | only reference to 0xEC; 32-bit, so `GameCarDrive` cannot be the type |
| `8003B5F8 lw 0xC($s4)`, `8003B62C lw 0x14($s4)` | 0xC8, 0xD0 | `+0xBC` pointer | written through the car base earlier in the same iteration, read back through this one |
| `8003B6E8`, `8003B71C lw 0x38($s4)` | 0xF4 | `+0xBC` pointer | against `8003B4E8 lw 0xD0($s0)` in the same loop |
| `8003B2D8 lh 0x10A($s0)`, `8003B35C lh 0x10C($s0)`, `8003B374 lh 0x102($s0)`, `8003B2E8 lh 0x104($s0)` | 0x12E, 0x130, 0x126, 0x128 | car pointer | all four in the guard and the non-boost arm |
| `8003B4E8 lw 0xD0($s0)` | 0xF4 | car pointer | inside the `i < 4` block, where the `+0xBC` register is otherwise unused |
| `8003B490 sw 0xA4($s0)`, `8003B4B4 sw 0xAC($s0)` | 0xC8, 0xD0 | car pointer | the writes; only the reads use `+0xBC` |
| everything else in the table in 30d | — | car pointer | single base, no competing spelling |

**What is *not* settled, and the experiment that would settle it.** Whether the
car-base sites want `car->field` or `*(s32 *)((u8 *)car + N)` is *not* decidable
from the base register, because both spellings compile to the same biased giv:
`probe_giv.c` (all raw casts) and `probe_giv2.c` (all member references) both
produce one base register at `cars+0x24`, with the same offsets. Worse,
`probe_giv2.c` and `probe_giv3.c` show that the obvious spelling of the second
pointer — `d = &c->drive` or `d = (D *)((char *)c + 0xBC)`, assigned at the top
of the loop — is *folded into the same giv*, so neither reproduces retail's two
live bases. Something else keeps `$s2` alive as a pure biv whose only consumer is
`addiu $s4, $s2, 0xBC`. Finding it is a one-variable search over how the second
pointer is introduced, and the measurement is cheap: compile the candidate and
count base registers in the loop body, before spending any effort on the
arithmetic. Until that is answered, the aliasing question that the whole types
pass exists to answer — `MEM_IN_STRUCT_P` on which sites — cannot be decided for
this pair of functions, and guessing it would waste a permuter run.

### 30i. Two smaller findings

**The fifth loop of `func_8003B0D4` walks `g_RankedCars` from `D_801E7740` down
to 1, never touching index 0.** The count is read with `lh`
(`8003B270 lh %lo(D_801E7740)`), the first iteration indexes with the
undecremented count (`8003B27C addu $s3, $v0`), and the guard is
`bgtz` on the *unshifted* `sll 16`, i.e. the loop runs while
`(s16)(count - 1)` is in 1..0x7FFF. With `D_801E7740 = 3` that is
`g_RankedCars[3]`, `[2]`, `[1]`, which fits the declared `[4]` and leaves the
leader alone. No contradiction, but the off-by-one look of it is deliberate.

**Cars 4..10 are time-sliced on `g_AnimTimer` parity, cars 0..3 are not.** The
second loop of `func_8003B0D4` skips the `func_8003A280` call when
`(i & 1) != (g_AnimTimer & 1)`, and jumps straight past that test when `i < 4`
(`8003B14C slti $v0, $a1, 0x4`). `func_8003BB50` has no such test at all, which
is the mechanical form of the note already in `car.h` that the attract variant
runs every car.

## 31. Types pass over `func_80032280` (`GameResetCarTrackState`) and `func_80039980`

Re-triaged targets: the first family was picked by stride-idiom density, which
section 30 showed is meaningless, so this round used **count of distinct
non-`$sp` field offsets** instead. That metric put `func_80032280` far in front
(53 offsets, 88 sites, 596 words, only 5 globals) and it was right to.

**`func_80032280` yielded one new named structure and three recovered fields in
an existing one.** The new structure is not the one the offset count implied: 34
of the 53 offsets belong to a **scratchpad work record at `0x1F80011C`**, and
that record is already fully spelled out in matched code — `func_80031298`
(`GameUpdateCarTrackState`, `car/GameUpdateCarTrackState.c`) is the *clamping
twin* of this stub and uses the same base with the same offsets. So the layout
came out corroborated rather than inferred, which is the strongest evidence this
pass has produced. The genuinely new type is small: a **12-byte arc-centre
record** behind `g_TrackArcCenters`.

### 31a. The two functions are one function with the clamp removed

`car.h` already described `func_80032280` as the "non-clamping twin" of
`GameUpdateCarTrackState`. That is exact, and it is checkable: the set of
scratchpad offsets the stub touches is a **strict subset** of the set the matched
twin touches, and the only two the twin has and the stub does not are 0x68 and
0x70 — the `ApplyMatrix` output that only the clamping path writes.

    stub offsets absent from the twin: (none)
    twin offsets absent from the stub: 0x68 0x70

Every other difference is on the writer side: the twin returns
`spad+0x3C` and writes `obj` +0x20/+0x28/+0x34/+0x38/+0x3C/+0x04, while the stub
returns nothing and writes `car` +0x50/+0x54/+0x58 (the reference triple `car.h`
already mentions) plus +0x6C/+0x70/+0x74/+0x78/+0xB4. That makes the twin a
line-by-line model for the conversion, and it is why this section proposes no
scratchpad typedef: see 31e.

### 31b. The scratchpad geometry record at `0x1F80011C`, 0x98 bytes

Offsets are from `0x1F80011C`. "Both" in the source column means the field is
written by one function and read by both, or written by both. Every width is
fixed by an instruction; the twin being matched C makes the signedness column
unusually solid, because it is a *declaration* rather than an inference.

| off | width | field | evidence |
|---|---|---|---|
| 0x00 | s32 | arc centre x | `sw 0x0($s2)` 80032394 from `arcCenter[i].x`; twin `FIELD(spad,s32*,0)` |
| 0x04 | s32 | arc centre z | `sw 0x120($at)` 800323A0 |
| 0x08 | s32 | car x - centre x | `sw 0x124($at)` 800323B4 |
| 0x0C | s32 | car z - centre z | `sw 0x128($at)` 800323C8 |
| 0x10 | s32 | car radius from the centre | `sw 0x12C($at)` 800324D8, `(cos*dx + sin*dz) >> 12` |
| 0x14 | s32 | this point's radius, then the interpolated radius | `sw 0x130($at)` 8003253C, rewritten 80032654 |
| 0x18 | s32 | next point's radius | `sw 0x134($at)` 800325A8 |
| 0x24 / 0x2C | s32 | this point x / z minus the centre | `sw 0x140/0x148($at)` 80032404/8003240C |
| 0x28 / 0x30 | s32 | next point x / z minus the centre | `sw 0x144/0x14C($at)` 80032420/80032434 |
| 0x34 | s32 | `sin(spad+0x8C)` | `sw 0x34($s2)` 80032A8C |
| 0x38 | s32 | `cos(spad+0x8C)` | `sw 0x38($s2)` 80032A6C |
| 0x3C | s32 | the twin's return value (knockback magnitude); the stub only zeroes it | `sw $zero, 0x158($at)` 800322A8, first store in the function |
| 0x40 | `MATRIX` 0x20 | rotation by the interpolated track angle | twin: `GameBuildRotMatrixY(spad+0x40, spad->f90)`; never built by the stub |
| 0x60 / 0x62 / 0x64 | `SVECTOR` | `{(carX - pX) * 4, 0, (carZ - pZ) * 4}` | `sh 0x60/0x62/0x64($s2)` 80032740/8003274C/8003275C |
| 0x68 / 0x6C / 0x70 | `VECTOR` | `ApplyMatrix` output; twin only | twin lines 228, 233, 234 |
| 0x78 | s16 | arc class, `point->arcRef & 3` | `sh 0x194($at)` 8003236C, then `beqz` skips the whole arc block |
| 0x7A | s16 | arc index, `(s16)point->arcRef >> 4` | `sh 0x196($at)` 80032358 |
| 0x7C | s16 | angular span of the arc, clamped `>= 1` | `sh 0x198($at)` 800325B8, `sll 16`/`bgtz`, then `ori 1` |
| 0x7E | s16 | atan2 of the car about the centre, then reused for the car's angular position in the arc | `sh 0x19A($at)` 800323DC and 8003266C |
| 0x80 / 0x82 | s16 | atan2 of this / the next point about the centre | `sh 0x19C/0x19E($at)` 80032458/80032474 |
| 0x84 | s16 | signed lateral offset on the arc path, negated when class == 2 | `sh 0x1A0($at)` 80032698 |
| 0x86 | s16 | `spad+0x88 + spad+0x8A`, the full track width | `sh 0x86($s2)` 800329C4 |
| 0x88 | s16 | interpolated `field_12` (right half-width) | `sh 0x88($s2)` 80032860 |
| 0x8A | s16 | interpolated `field_10` (left half-width) | `sh 0x8A($s2)` 800328CC |
| 0x8C | s16 | `car->field_24 - 0xC00 + spad+0x90`, the car's track-relative heading | `sh 0x8C($s2)` 80032958 |
| 0x8E | s16 | interpolated `field_E` (cross-slope gradient) | `sh 0x8E($s2)` 80032944 |
| 0x90 | s16 | interpolated track angle; seeded from `point->angle` and overwritten by the arc blend | `sh 0x1AC($at)` 80032340, `sh 0x1AC($at)` 80032728 |
| 0x92 | s16 | interpolated `field_C` | `sh 0x92($s2)` 800329CC |
| 0x94 | s16 | interpolated cross-slope *angle*, `atan2(width, field_E * width >> 7)` | `sh 0x94($s2)` 80032A60 |
| 0x96 | s16 | `point->segmentLength` clamped `>= 1`; the divisor of every interpolation | `sh 0x1B2($at)` 80032310, `ori 1` at 8003232C |

Gaps at 0x1C..0x23, 0x66, 0x72..0x77. The record therefore runs
`0x1F80011C..0x1F8001B3`.

**This address is a shared overlay, not a durable object.** `GameDrawCourseObjects`
uses `0x1F80011C` as an `SVECTOR` for a single vertex, `GameSubmitTerrainCells`
uses it as a screen-XY array (see §2's note), and six `func_80017794` callers
pass it as a vector argument. The layout above is what the *car track-state pair*
puts there and nothing more. Naming it globally would be wrong, and 31e says
what to do instead.

### 31c. `GameTrackPoint`: three fields recovered out of padding

`track.h` declared `u8 padC[4]` and `u8 pad14[2]`. All three halfwords in that
padding are real fields, and each is proven twice — once in the stub and once in
already-matched code, which is why the confidence is high rather than medium:

* **`s16 field_C` (+0x0C).** `lh 0xC($s5)` / `lh 0xC($s4)` at 8003295C /
  80032970, interpolated into `spad+0x92`. Matched confirmation:
  `FIELD(temp_s6, s16 *, 0xC)` in `GameUpdateCarTrackState` line 305. It is then
  rotated with `spad+0x94` by the car's track-relative heading into the two words
  at `obj` +0x20 and +0x28, so the two together are a surface-tilt vector and
  `field_C` is its pitch component. **High** on width and signedness, **medium**
  on that reading of the meaning.
* **`s16 field_E` (+0x0E).** `lh 0xE($s5)` / `lh 0xE($s4)` at 800328E8 /
  800328FC and again at 800329D0 / 800329EC. Matched confirmation in
  `GameUpdateCarTrackState` line 292, and — decisively — in the *same file* as
  the stub: the already-decompiled `GameGetTrackSurfaceHeight` returns
  `y + (((s16)fieldE * outZ) >> 7)`, and `GameSampleTrackSurfaceHeight` carries a
  whole local `TP` typedef whose stated reason for existing is that it "needs
  0x0E, which GameTrackPoint leaves unnamed". So `field_E` is the cross-slope
  gradient in 1/128 of a unit per unit of lateral offset. **High** throughout.
* **`u16 arcRef` (+0x14).** `lhu 0x14($s4)` twice at 80032344 / 8003235C, then
  `sll 16` / `sra 20` for one half and `andi 3` for the other. Matched
  confirmation: `FIELD(temp_s4, u16 *, 0x14)` in `GameUpdateCarTrackState` lines
  95 and 97, declared `u16` there. Bits 0..1 select the cornering model — 0 skips
  the entire arc block, 2 negates the lateral offset, so 2 is the mirrored hand —
  and bits 4..15 are a **signed** index into `g_TrackArcCenters`. Bits 2..3 are
  never read anywhere. **High** on the split, **medium** on "mirrored hand".

Per §30's rule, the `lhu` at +0x14 carries no signedness information by itself;
the `u16` comes from the matched twin's declaration, and the `sra 20` shows the
*index* is used signed regardless.

**Applied to `include/game/track.h`.** Size unchanged at 0x18; no unit named
`padC` or `pad14`, so nothing else moved. Verified by deleting all 30 objects
whose `.d` lists `include/game/track.h` and rebuilding:
`2913e15648eddef40821c5f666460abc04155ee6`.

### 31d. `g_TrackArcCenters` (`D_8019C7D0`): the element is 12 bytes, and one existing declaration says 24

The arc block is reached as `((s16)point->arcRef >> 4) * 0xC + g_TrackArcCenters`
and two words are read from it, +0x00 and +0x04, which the stub subtracts from
the car's and the two points' x and z before taking `atan2`. The stride 12 is
proven at three independent sites: 8003237C in the stub,
`FIELD` line 101 of `GameUpdateCarTrackState`, and line 755 of
`car/GameUpdateCarDrivetrain.c`, which performs the identical arc computation and
already spells the symbol `u8 *` with its own `* 0xC`.

**`track/GameInstallTrackPoints.c` declares it `extern GameTrackPoint
*g_TrackArcCenters`, i.e. a 24-byte element.** That is wrong, and harmless only
because that unit never subscripts it — it just publishes
`points + count * sizeof(GameTrackPoint)`, where the 24 is the *point* stride and
so is correct. Two units, one symbol, two incompatible element types; this is a
§22-class duplicate that §22 did not catch because the types differ rather than
the names.

`GameTrackArcCenter` is therefore **added as a typedef to `track.h` with no
`extern`**: declaring one would collide at compile time with the two existing
per-file declarations, which is exactly the collision worth documenting rather
than silently resolving. The third word is never read in the image, so it stays
`unk08`. Stride **high**, x and z **high**, +0x08 **unknown**.

### 31e. Per-access-site spelling — and why base-register counting cannot answer it here

§30 left the member-versus-raw question open and specified the experiment:
compile a candidate and count base registers. Run here, it returns a **negative
that is worth more than the positive would have been.**

The scratchpad base is a *compile-time constant*, so gcc constant-propagates it
and then chooses, per site, between materialising it in a register and emitting
`lui $at` plus a `%lo`-style displacement. Comparing the retail stub against the
built object of its matched twin — whose source spells essentially every one of
these accesses the same way, through one `spad` variable — the two forms come out
interleaved in both, and the split is nearly identical:

    stub  absolute (lui $at) form: 0x04 08 0C 10 14 18 24 28 2C 30 3C 78 7A 7C 7E 80 82 84 90 96
    twin  absolute (lui $at) form: 0x04 08 0C 10 14 18 24 28 2C 30 3C 78 7A 7C 7E 80 82    90 96
    stub  register ($s2) form:     0x00 34 38 60 62 64 86 88 8A 8C 8E 90 92 94 96
    twin  register ($s1) form:     0x00 10 14 34 38 3C 60 62 64 68 70 78 7C 7E 84 86 88 8A 8C 8E 90 92 94 96

The twin reaches eight offsets **both** ways (0x10, 0x14, 0x3C, 0x78, 0x7C, 0x7E,
0x90, 0x96) from a single uniform source spelling. So for a constant base the
instruction form is pure allocation and carries no information about the source
at all — the opposite of the §30h situation, where two registers over a *runtime*
base did discriminate. Anyone tempted to read meaning into `lui $at` versus a
held base register should stop.

What that buys is a concrete, evidence-backed prescription instead of a guess:

| group | verdict | why |
|---|---|---|
| all 34 scratchpad offsets | **raw**, one `void *spad = (void *)0x1F80011C` plus `FIELD(spad, T *, off)` | the twin uses exactly this, uniformly, and matches; and it produces retail's mixed instruction forms without being asked to |
| `spad+0x3C` at 800322A8 | **raw literal**, `*(s32 *)0x1F800158 = 0` | the twin's first statement is this literal and its object has `lui $at` there, same as the stub |
| the two `GameTrackPoint` bases `$s4` / `$s5` | **raw**, `FIELD(base, s16 *, off)` off a `void *` | the twin does this for all nine offsets; note the twin does *not* use `GameTrackPoint` even though the header exists |
| `car` +0x00 / +0x08 in the vector build | **raw with a `(u16)` narrowing**, `(u16)FIELD(car, s32 *, 0)` | `lhu 0x0($s3)` / `lhu 0x8($s3)` at 8003272C / 80032744 read the low half of 32-bit fields; the twin line 192 is character for character this |
| `car` +0x24 at 80032948 | **raw with `(u16)`** | `lhu 0x24($s3)`, twin line 300 |
| `car` +0x50/0x54/0x58/0x6C/0x70/0x74/0xB4/0x78 | **raw** | the twin writes the same shapes to `obj` through `FIELD` |
| `g_TrackArcCenters` element | **raw**, `u8 *` plus `* 0xC` | two matched units already do this; `GameTrackArcCenter` is for readers, not for the conversion |

The one thing the twin does *not* settle is the `$s6`-style biased giv, because
`func_80032280` has none — its three object bases are all real source pointers
(`$s3` the parameter, `$s4`/`$s5` the two points), which is itself why its offsets
are all non-negative.

### 31f. A dead `mult` survives in gcc 2.6.3, and it is not evidence of hand-written asm

Two `mult` at 800327CC and 800327D8 have no `mflo` or `mfhi` before the next
`mult` on any path — checked against both arms of the branch at 800327E8 and the
join at 80032804. Their results are unreachable. Read against the twin, the
expression is unmistakable: it is line 204,
`(-sin(f90) * (s16)spad->f60) + (cos(f90) * spad->f64)`, the **lateral** component
of the rotation whose along-track partner at 800327A8 is kept. The twin consumes
it (`var_a2`, then the boundary clamp); the non-clamping stub has no consumer, so
the sum, the rounding and the `>> 14` were all deleted and the two multiplies were
not.

The mechanism, confirmed by probe (`scratch/decomp-work/probe_deadmult.c`):

    s32 f1(s32 a, s32 b, s32 c, s32 d) {
        s32 v = a * b + c * d;
        if (v < 0) v += 0xFFF;
        v = h(a);              /* overwritten before use */
        return v;
    }

compiles to `mult $4,$5` … `jal h` … `mult $6,$7` with **no `mflo` at all**. gcc
2.6.3's MIPS `mulsi3` is a `mult` insn plus a separate `mflo`; RTL dead-code
elimination removes the `mflo`, but it will not remove the `mult`, because that
insn sets the HI/LO hard registers and hard-register liveness is not something it
will prove away. The companion `f2`, whose operands are globals, loses everything
— the difference is that in `f1` the operands are already live for other reasons,
so only the product dies.

Three consequences. A `mult` with no `mflo` is **a dead product in the source, not
hand-written assembly** — worth knowing before anyone marks such a function
`HANDWRITTEN_ASM`. Converting `func_80032280` **requires writing the dead
expression**, or the output is two words short. And, third, the negative:

**This is not the explanation for §27's `aligned 393 < exact 418` gap on
`func_800418D4`.** Scanning every remaining stub for `mult`/`div` with no
following `mflo`/`mfhi` finds candidates in only three functions, and after
discarding the delay-slot-then-`j` false positives (`800326C0`, `8003B504`,
`8003BEBC`, all of which reach an `mflo` through the jump) the genuine count is
**two, both in `func_80032280`**. `func_800418D4` has none. The missing-or-extra
instruction §27 is looking for is somewhere else.

### 31g. `func_80039980` (`GameCollideRivalCars`): a cross-check, not a new type

Surveyed rather than fully typed, which was the point of picking it: 18 offsets,
4 globals, and every one of them lands on `GameCarRuntime`. It **corroborates
four of §30's corrections and contradicts none.**

The shape: `$s7` is the `car` parameter, `$fp` is `&g_Cars[index + 1]`
(`D_801F19F0` is `g_Cars + 0x19C`, i.e. `&g_Cars[1]`, plus the same ×412 idiom),
and `$s6` is a `$fp + 0x24` biased giv over the inner loop — the same +0x24 bias
§30b explains, with `lw 0x0($s6)` = `field_24` at offset zero again.

* **`field_C8` / `field_D0` are the velocity components.** `lhu 0xC8($fp)` minus
  `lhu 0xC8($s7)` at 8003A050, `/32` with the `+0x1F` rounding, becomes the second
  argument of `GameSetCarKnockback` (`func_80038CE8`, named in
  `GameUpdateCarTrackState.c`); `field_D0` becomes the third. A knockback built
  from the *difference* of two cars' values at these offsets is a relative
  velocity and nothing else. That is independent of §30e's derivation, which came
  from `sin/cos(headingAngle) * field_A4 / 256`.
* **`field_A8` is an acceleration ramp.** A collision multiplies it by 90/100 on
  both cars (`0x51EB851F`, `sra 5`, on `90 * field_A8`, at 8003A028 and 8003A0EC).
  A collision penalising acceleration is coherent; a collision scaling a speed
  *limit* by 0.9 would not be.
* **`field_8A` is the collision flag.** Set to exactly 1 on *both* cars in both
  collision branches (8003A034 / 8003A04C and 8003A108 / 8003A10C), `sh`, 16-bit.
  §30's first-loop reading now closes: `GameUpdateRaceCars` does
  `field_8A &= 1` per frame and `GameUpdateAttractCars` does `field_8A = 0`, and
  the last pass of both branches on `field_8A == 0` to choose the body-kick path.
  So bit 0 is deliberately sticky in a race and cleared in attract.
* **`field_98` is not a gearbox field on these objects.** 8003A020 compares
  `lh 0x74($s6)` (the other car's +0x98) against `lh 0x98($s7)` and skips the pair
  unless they are equal. `GameUpdatePlayerCar` calls +0x98 `shiftState`; two rival
  cars gating collision on equal shift states is not a reading anyone would
  defend, and §30f's two-object explanation covers it.
* `z` at +0x08 and +0x20/+0x24/+0x28 are read with `lhu`, the low half of 32-bit
  fields, exactly as in `func_80032280` and its twin. No contradiction with the
  32-bit widths; §30f's `s32 y` is not exercised here.

One thing worth flagging for whoever takes this function: the stack block it
builds at `sp+0x18`/`sp+0x30` is copied with `lwl 0x9B($s2)` / `lwr 0x98($s2)`
into `swl 0x3($a0)` / `swr 0x0($a0)` at 80039B74. Both addresses are in fact
4-aligned, so this is gcc emitting an unaligned move because the *declared* type
said it could not prove alignment — the §28 unaligned-copy family, reached from a
different direction.

### 31h. Triage rule, recorded so it is not rediscovered a third time

Stride-idiom density was the wrong metric (§30). Distinct-field-offset count is a
better one but still has a failure mode at the bottom of the table rather than the
top. Of the fifteen remaining stubs, five are a menu cluster —
`func_8004A248`, `func_8004D384`, `func_8005290C` and neighbours — totalling
about 4700 words with **one** field-offset access between them, against 43
globals. They are global-and-array code, not aggregate code, and a types pass
there would find nothing. The rule: *before typing, check that the function
addresses objects through pointers at all*; a high global count with a near-zero
offset count means the data is already named and the work is elsewhere.

## 32. The phantom 8-byte frame: `volatile` blocks the carrier

21a recorded the phantom frame as an open problem and the family was withdrawn once
as a wall. It is now partly closed, and the mechanism is the opposite of what the
earlier attempts assumed.

**The carrier is a non-volatile view of the SPU register block, and `volatile`
destroys it.** Measured on `SsUtSetVVol` (`func_80078528`), all three spellings of the
same hardware at `D_8009DF20`:

| view of `D_8009DF20` | words | exact | frame |
|---|---:|---:|---|
| non-volatile `short[]` | 35 | **0** | retail's 8-byte frame appears |
| `volatile short[]` | 34 | 32 | none |
| separate volatile byte symbols with casts | 33 | 35 | none |

So the frame is not something to reserve and not evidence of a pre-test loop in this
case: it falls out of two ordinary halfword stores through a non-volatile array view,
exactly as it does for the already-solved twin `SsUtChangeADSR` fifteen lines away,
which uses the same `g_SndVoiceRegs16` spelling. Every earlier attempt reached for the
volatile byte views because the registers *are* hardware, and that is precisely what
prevented the frame from forming.

This does not contradict section 28's rule that `volatile` on genuinely shared or
hardware-backed state is description rather than fiction. It says something narrower
and more useful: **`volatile` is not free, and where retail's own code did not use it,
adding it changes codegen.** `D_8009DF20` is declared eleven times across this tree in
four element types, and only one of those declarations carries `volatile`; that
inconsistency is retail's, and it is load-bearing.

**Parameter typing is the other half.** Declaring the two volume arguments `long` and
casting to `short` at the point of use produces retail's `sll 16` / `sra 16` pairs and
reaches `exact 0`. Declaring them `short` gives 36 words and `exact 35`, because
`assign_parms` moves the sign extension ahead of the guard. Same lesson as
`GameDrawFlatTriangle`: width and signedness are per-use.

### 32a. The two that did not close, and how close they are

Both remain byte-exact in the tree with their crutches; neither was replaced by a
non-hashing candidate. Their best legal crutch-free candidates:

| function | words | exact | aligned | what is left |
|---|---:|---:|---:|---|
| `func_80076C58` (`SpuVmSetSeqVol`) | 93 | **1** | 1 | the zero-iteration entry branch at `+0xB8` |
| `func_80075EB4` (`SpuVmApplyPitchBendByTone`) | 60 | **10** | 7 | best over every measured statement order |

`func_80075EB4` does not touch `D_8009DF20` at all, so the volatility lever does not
apply to it and its residual is a different problem. Three of `func_80076C58`'s
residual words are one scheduling difference, which is the same class as section 29's
`func_8005F6BC` and may be equally hard.

These are recorded as low-residual negatives rather than as walls: exact 3 out of 93 is
not the same statement as "blocked", and either could fall to one more idea.

### 32b. Second confirmation, from the other direction

`SpuVmSeqKeyOff` (`func_80076ED8`) closed at 81 words with **zero pins, zero barriers and
no assembly**, from a starting position of eight pins, seven `asm` blocks carrying real
instructions, a hand-written `1:` label and a `bnez $2,1b` closing the loop by hand.

What it needed was `volatile` **removed** from all six globals the unit declared with it:
`D_801E42F8`, `g_SndCurrentVoice`, and the four key-on/key-off mask words `D_801F2A08`,
`D_801F2A0C`, `D_8009E670`, `D_8009E674`. So this is 32's corollary confirmed from the
opposite direction: in 32 a missing `volatile` was blocking the frame carrier, here a
spurious `volatile` was blocking ordinary codegen, and in both cases the fix was to match
retail's own inconsistent volatility rather than to reason about what the hardware
"should" need.

The rewrite also recovered the SPU voice record: a 52-byte struct with 28 named fields,
replacing four parallel byte arrays indexed by a hand-computed `52 * i`. It is declared
locally as `SpuVoice76ED8`; the name is address-derived and the type belongs in a header
once another unit needs it.

## 33. When C takes ownership of data that was disassembled

`GameDrawCarSpecGraph` (`func_800496F0`) is the first function whose conversion moved data
out of the disassembly and into C, and the procedure generalises to every remaining function
containing an ordinary `switch`.

**The symptom.** The function measures at the right size with a tiny residual, and the only
differing words are the `lui`/`lw` pair that loads the compiler-generated jump table. Here it
was 675/675 words at `exact 2`, differing at `800498A0` and `800498A8`.

**The cause.** A C `switch` emits a jump table into its unit's `.rodata`. Retail's copy of the
same table is still present in the disassembled data, so the linked image contains both. Here
that was 20 bytes of table plus 4 of alignment, and everything after it moved by `0x18` —
including the function itself, which is why only the table's address differed inside it.

**The fix is a config change, not an edit to `asm/`.** `asm/**` is gitignored and regenerated
by `make split`, so deleting the duplicated blocks by hand works locally and is undone the
next time anyone splits. Express it where it survives: in `configs/PAL/main.yaml`, a
`.rodata` entry with a leading dot names data owned by that C file, and a plain `rodata`
entry names data still disassembled. Each segment runs to the start of the next one, so
moving the boundary transfers ownership:

    - [0x205C, .rodata, PAL/main/menu/GameDrawTireCompoundSlider]
    - [0x2070, rodata, main/002070_main]     <- became 0x2098, main/002098_main

That one line hands `0x2070`..`0x2098` to the C unit, which then supplies those bytes itself:
the four graph colours as a `const` definition carrying the retail symbol via
`asm("D_80011870")`, and the jump table as a by-product of the `switch`.

**Verification that actually proves it.** Run `make split VERSION=PAL` to regenerate from the
config, confirm the first block in the new file is the one that should now follow the C-owned
range, then delete `main.exe` and `main.elf` and rebuild. A hash that holds only before a
split proves nothing.

**Do not compensate in the function body.** If the data layout is wrong, the residual appears
as an address difference, not a code difference; adjusting the body to absorb it would be
fixing the wrong thing.

### 32c. `SpuVmSetSeqVol` is down to one word, and the volatility question is settled

The `+0x144` schedule difference in 32a is gone. The best crutch-free candidate is now
**93 words, exact 1, aligned 1, 92 of 93 equal**, kept at
`scratch/decomp-work/func_80076C58/best-exact1.c`. The production source is unchanged and
still carries its crutch; no non-hashing candidate was installed.

**The volatility hypothesis is falsified, by controlled sweep rather than by argument.** All
four combinations, measured on the same source shape:

| `D_801E42F8` | `g_SndVoiceFlags` | words | exact | aligned |
|---|---|---:|---:|---:|
| volatile | volatile | 93 | 3 | 2 |
| plain | volatile | 93 | 6 | 5 |
| volatile | plain | 93 | 3 | 2 |
| plain | plain | 93 | 6 | 5 |

So removing `volatile` from the voice count is actively **harmful** here, changing the entry
and loop-bound code and doubling the residual, and the flags' volatility is neutral. This is
the counterweight to 32 and 32b: volatility is a lever in both directions and the answer is
per-symbol and per-function, never a rule of thumb. `D_801E42F8` should stay volatile.

**What the last word is.** The sole mismatch is the zero-iteration entry test:

    +0xB8   retail: beqz v0, ...      candidate: blez v0, ...

and it is a genuine trade-off rather than a missed spelling. Where GCC can see the unsigned
byte range of the bound it reduces the initial `0 < bound` to `bound != 0` and emits retail's
`beqz` — but that shape schedules the loop tail wrongly. Placing the assignment so the tail
matches routes the test through the signed comparison and yields `blez`. Supplying both
assignments was tried. Counter type was swept across `short`, signed `long`, `u_long` and
`u_char`; `u_long` is codegen-neutral at exact 1, and `short` is much worse at exact 55.

So the two remaining words in this family are each one instruction, and each sits on a
different compiler decision: this one on how the entry test is derived, and `func_8005F6BC`
(section 29) on the scheduler's multiply latency. Neither is a missing source construct.
