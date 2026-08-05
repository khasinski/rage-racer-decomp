#ifndef GAME_CAR_H
#define GAME_CAR_H

#include "common.h"

/*
 * Per-car entry. The two setup bytes are what the CUSTOMIZE screen edits and
 * the save file keeps; whether the transmission row can be opened at all is a
 * property of the car's own loaded data (byte 8 of it), not of this entry.
 */
typedef struct GameCarEntry {
    u8 modelVariant;
    u8 tireCompound;  /* +0x01 CUSTOMIZE row 0, five settings */
    u8 transmission;  /* +0x02 CUSTOMIZE row 1, 0 automatic, 1 manual */
    u8 paintColor1;
    u8 paintColor2;
    u8 enabled;
    u8 pad6[2];
} GameCarEntry;

typedef struct GameCarRuntime {
    s32 x;
    /* +0x04 32 bits wide, not 16: `lw`/`sw` at nine sites in each of
     * UpdateRaceCars / UpdateAttractCars, `s32 unk04` in
     * UpdatePlayerCar's own layout, and the `*(s32 *)&ent->y` cast in
     * InitRivalCar that the old `s16` forced. See names.md 30. */
    s32 y;
    s32 z;
    /* +0x10..+0x18: per-frame motion, measured in-race. Not the world
     * velocity GameCarDrive documents at +0xC8/+0xD0. */
    s32 field_0C;
    s32 motionX;
    s32 motionY;
    s32 motionZ;
    s32 field_1C;
    s32 field_20;
    s32 field_24;
    s32 field_28;
    s32 field_2C;
    s32 trackPointIndex;
    s32 field_34;
    s32 field_38;
    s32 field_3C;
    s32 field_40;
    s32 field_44;
    s32 field_48;
    s32 field_4C;
    s32 field_50;
    s32 field_54;
    s32 field_58;
    s32 field_5C;
    s32 field_60;
    s32 field_64;
    s32 field_68;
    s32 field_6C;
    s32 trackProgress;
    s32 previousTrackProgress;
    s16 field_78;
    s16 field_7A;
    s16 velocityX;
    s16 velocityZ;
    s16 motionActive;
    u16 motionTimer;
    s16 motionMode;
    s16 motionModeTimer;
    s16 motionValue;
    s16 field_8A;
    s16 field_8C;
    s16 field_8E;
    s16 field_90;
    s16 field_92;
    s16 field_94;
    s16 field_96;
    s16 field_98;
    s16 field_9A;
    s16 field_9C;
    s16 field_9E;
    s32 headingAngle;
    s32 field_A4;    /* +0xA4 speed; km/h readout is field_A4 * 160 / 1168 */
    s32 field_A8;
    s16 activeFlag;
    s16 field_AE;
    s32 field_B0;
    s32 field_B4;
    /* +0xB8 0 = travelling with the course, 1 = against it. Seeded to
     * g_RaceSeries for every car by BuildStartingGrid and recomputed each
     * frame for the player from IsCarFacingBackwards; `!= g_RaceSeries`
     * is the wrong-way test. */
    s16 facingBackwards;
    u8 padBA[2];
    s32 field_BC;
    s32 field_C0;
    s32 field_C4;
    s32 field_C8;
    s32 field_CC;
    s32 field_D0;
    s32 field_D4;
    s32 field_D8;
    s32 field_DC;
    s32 field_E0;
    s32 field_E4;
    s16 field_E8;
    s16 field_EA;
    s32 field_EC;
    s16 field_F0;
    s16 field_F2;
    s32 field_F4;
    s32 field_F8;
    s32 field_FC;
    s32 routeIndex;
    s16 field_104;
    u8 pad106[2];
    s32 field_108;
    s16 field_10C;
    s16 field_10E;
    s16 field_110;
    s16 field_112;
    s16 field_114;
    s16 field_116;
    s16 field_118;
    s16 field_11A;
    s16 field_11C;
    s16 field_11E;
    s16 field_120;
    s16 field_122;
    s16 field_124;
    s16 field_126;
    s16 field_128;
    s16 field_12A;
    s16 field_12C;
    s16 field_12E;
    s16 field_130;
    s16 field_132;   /* +0x132 current gear */
    s32 field_134;
    s16 field_138;
    s16 field_13A;
    s32 field_13C;
    u8 pad140[0xC];
    u8 field_14C;
    u8 field_14D;
    u8 field_14E;
    u8 field_14F;
    s32 field_150;
    s32 field_154;
    u8 pad158[4];
    s16 field_15C;
    s16 field_15E;
    s16 field_160;
    s16 field_162;
    s32 field_164;
    s16 field_168;
    u8 pad16A[0x32];
} GameCarRuntime;

/* Per-car runtime state, player in slot 0. Individual slots and single fields
 * also have their own split symbols; see docs/names.md section 3b. */
extern GameCarRuntime g_Cars[11];

typedef struct GameCarRuntimeProgressWindow {
    s32 field_6C;
    u8 pad4[0x3C];
    s16 activeFlag;
    u8 pad42[0x15A];
} GameCarRuntimeProgressWindow;

/* The four contenders ordered by race progress (`field_68 + field_6C`), best
 * first; re-sorted every frame by RankContenders to rubber-band the AI. */
extern GameCarRuntime *g_RankedCars[4];

/* Active car-entry table; repointed at one of the three 13-entry tables below
 * per title-menu row, so it is a pointer rather than a fixed array. */
extern GameCarEntry *g_CarTable;

/* The three saved car-entry tables, one per title-menu race row (0 GRAND PRIX,
 * 1 EXTRA GRAND PRIX, 2 TIME ATTACK); save block +0x50 / +0xC0 / +0x128. The
 * shops raise the Time Attack row to the best spec reached in either GP file. */
extern GameCarEntry g_GrandPrixCars[];
extern GameCarEntry g_ExtraGrandPrixCars[];
extern GameCarEntry g_TimeAttackCars[];

/* g_Cars index the replay / attract camera is following. */
extern s32 g_CameraCarIndex;

/* Index into g_CarTable of the car the player drives; selects the model and
 * texture pack to install. Distinct from g_CarListCursor (names.md 3). */
extern s32 g_PlayerCarIndex;

/* Cursor of the car list being browsed in the shop; steps to the next entry
 * with `enabled == 0`. Buying it copies it into g_PlayerCarIndex. */
extern s32 g_CarListCursor;

/* Index of each car model's first grade in the 32-entry asset list; thirteen
 * entries, one per model. GetCarAssetIndex adds the owned grade to it. */
extern u8 g_CarModelBaseIndex[];

/* Per-model base of the progress level a purchase requires; the level needed is
 * this plus the grade being bought (GetCarUnlockLevel). */
extern u8 g_CarModelUnlockBase[];

/* One automatic-gearbox shift point; `spec->shiftPoints[gear - 1]`. */
typedef struct GameCarSpecShiftPoint {
    s16 downshiftSpeed;
    s16 upshiftSpeed;
} GameCarSpecShiftPoint;

/* The loaded car's spec block (`g_CarSpec`), from its asset pack. Nothing below
 * 0xCC is read yet; the gear tables are indexed by the 1-based gear number. */
typedef struct GameCarSpec {
    u8 unk00[0xCC];
    s32 gearLoad[6];      /* +0xCC engine-load divisor per gear */
    s32 gearRatio[7];     /* +0xE4 final-drive ratio per gear (rpm divisor) */
    s16 revLimit;         /* +0x100 rev ceiling; the tacho and cut-out use it */
    s16 unk102;           /* +0x102 scale applied to car->field_A8 */
    s16 topGear;          /* +0x104 highest selectable gear */
    s16 redline;          /* +0x106 redline warning rpm */
    u8 unk108[2];
    u16 steerResponse;    /* +0x10A divisor of the AI heading correction */
    s16 unk10C;           /* +0x10C */
    s16 unk10E;           /* +0x10E */
    s16 unk110;           /* +0x110 */
    s16 unk112;           /* +0x112 */
    u8 unk114[0xC];
    GameCarSpecShiftPoint shiftPoints[6]; /* +0x120, index = gear - 1 */
    s16 tachoNeedleX;     /* +0x138 tachometer needle pivot */
    s16 tachoNeedleY;     /* +0x13A */
    u16 tachoFaceDX;      /* +0x13C added to the pivot for the dial sprite */
    u16 tachoFaceDY;      /* +0x13E */
    u16 tachoDigitsX;     /* +0x140 origin of the numeric readout */
    u16 tachoDigitsY;     /* +0x142 */
    u8 unk144[8];
    u8 needleQuad[4];      /* +0x14C needle half-width/height corner offsets */
    s16 needleAngleMin;   /* +0x150 needle sweep, 1/10000 of a turn */
    s16 needleAngleMax;   /* +0x152 */
    u8 needleColor[4];    /* +0x154 rgb + primitive code */
    u8 needleColorAlt[4]; /* +0x158 */
} GameCarSpec;

extern GameCarSpec *g_CarSpec;

/* Per-gear torque curve, one 16-entry row per gear: row 0 is the engine's own
 * curve, rows 1..6 are it divided by each gear's ratio. */
typedef struct GearCurveRow {
    s32 values[16];
} GearCurveRow;

extern GearCurveRow g_GearTorqueCurve[];

/* Drivetrain / input block at `car->field_BC`; the physics code addresses the
 * car's second half through this rather than through GameCarRuntime.
 *
 * Calibrated on g_PlayerCar (D_8009E6D4), which is a different 0x19C object from
 * g_Cars: it shares the stride but not the meaning of every byte.
 * +0x30, +0x38, +0x74 and +0x76 are 16-bit gearDisp/unk38/manual/gear on the
 * player object and 32-bit / AI-speed fields on the rival cars, so use
 * GameCarAiBlock for a g_Cars[] element. Evidence in names.md 30f. */
typedef struct GameCarDrive {
    s32 unk00;
    s32 unk04;
    s32 accelPos;    /* +0x08 */
    s32 unk0C;
    s32 brakePos;    /* +0x10 */
    s32 unk14;
    s32 unk18;
    s32 steerPos;    /* +0x1C */
    s32 unk20;
    s32 unk24;
    s32 unk28;       /* +0x28 index into the g_LaunchSpeedThresholds launch table */
    s16 unk2C;       /* engine load */
    s16 unk2E;
    s16 gearDisp;    /* +0x30 */
    s16 unk32;
    s16 clutch;      /* +0x34 */
    s16 unk36;
    s16 unk38;       /* +0x38 airborne height */
    s16 unk3A;
    s16 unk3C;
    u16 unk3E;
    s16 unk40;
    s16 unk42;
    s32 unk44;       /* +0x44 frames on the ground */
    s32 unk48;
    s32 unk4C;
    s32 unk50;       /* +0x50 spin rate, damped 31/32 per frame */
    s32 unk54;
    s32 unk58;       /* +0x58 launch heading */
    s32 unk5C;       /* +0x5C launch speed */
    s32 unk60;       /* +0x60 yaw offset */
    s32 unk64;
    s32 unk68;
    s16 unk6C;
    s16 unk6E;
    s16 unk70;
    s16 unk72;
    s16 manual;      /* +0x74 */
    s16 gear;        /* +0x76 */
    s32 unk78;       /* +0x78 engine rpm */
    u8 pad7C[8];
    s32 unk84;       /* +0x84 launch-energy threshold */
    s32 unk88;       /* +0x88 */
    s32 unk8C;       /* +0x8C player-car speed scale seeded from the loaded spec */
    s32 unk90;       /* +0x90 */
    s32 unk94;       /* +0x94 */
    s32 state98;     /* +0x98 0 driving, 1 launching, 2 airborne */
    s16 unk9C;
    s16 unk9E;
    s16 accelBtn;    /* +0xA0 */
    s16 brakeBtn;    /* +0xA2 */
    u8 padA4[4];
} GameCarDrive;

/* A second, halfword-wide view of that same block, for the code that loads
 * 0x104..0x134 as s16 where GameCarDrive declares s32. See names.md 3b. */
typedef struct GameCarAiBlock {
    u8 pad0[0xC];
    s32 field_C8;   /* +0x0C world velocity x, sin(headingAngle) * field_A4 / 256 */
    u8 pad10[4];
    s32 field_D0;   /* +0x14 world velocity z, cos(headingAngle) * field_A4 / 256 */
    u8 pad18[0x18];
    s32 field_EC;   /* +0x30 target angle: field_24 += GetAngleDelta(field_24, this) / 5 */
    u8 pad34[4];
    s32 field_F4;   /* +0x38 yaw rate, added to both field_44 and field_24 */
    u8 pad3C[0xC];
    s16 field_104;  /* set to 1 while another car blocks this one */
    u8 pad4A[6];
    u16 field_10C;  /* count of cars close enough to matter this frame */
    u8 pad52[0xA];
    s32 field_118;  /* grid-seeded target progress (g_TrackLength / 12 steps) */
    u16 field_11C;  /* accumulated steering bias */
    s16 field_11E;  /* steering bias target, +-0x50 */
    u16 field_120;  /* per-frame step towards the target */
    u8 pad66[2];
    s16 field_124;  /* grid-seeded speed, clamped >= 0 */
    u8 pad6A[4];
    s16 field_12A;
    s16 field_12C;  /* clamped to 0..15; the boost-branch step of field_A8 */
    s16 field_12E;  /* +0x72 slipstream-boost countdown, decremented while > 0 */
    s16 field_130;  /* speed scale, damped to 98% when boxed in; caps field_A8 */
    s16 field_132;  /* clamped to >= 0x3C */
    s16 field_134;  /* clamped to >= 0 */
    u8 pad7A[0x66];
} GameCarAiBlock;

typedef struct GameCarTrackAngleWindow {
    u8 pad0[0x30];
    s32 trackPointIndex;
    u8 pad34[0x6C];
    s32 headingAngle;
} GameCarTrackAngleWindow;

/*
 * The car pipeline. docs/names.md 1 has the evidence and the data layouts.
 */
/* Race-entry init for the player object: start pose plus the speed/gear lookup
 * tables g_GearTorqueCurve / g_TorqueBandEnd / g_TorqueLossBandEnd. Logs "init_car" .. "init_ok". */
/* race_scene.c passes a bare void *; an empty parameter list keeps both
 * units' spellings.  The body reads a GameCarRuntime *. */
void InitPlayerCar();
/* Non-clamping twin of UpdateCarTrackState: recomputes the track-relative placement
 * and writes the reference triple at +0x50, for the init/reset paths only. */
void ResetCarTrackState(GameCarRuntime *car);
/* The two variants of the rival-car driver over GameCarRuntime[11]. Race runs
 * only while `g_RacePhase >= 2 && g_GrandPrixMode`, adds three race-only passes
 * and time-slices cars 4..10; attract has no player so every car runs. */
void UpdateRaceCars(void);
void UpdateAttractCars(void);
/* Player-vs-field collision (detection, response and the crash cue), called
 * only from UpdatePlayerCar; returns the struck sub-quad 1..4 or 0. */
/* update_player_car.c spells the argument void *; the body reads
 * GameCarRuntime *. */
s32 CollidePlayerWithCars();
/* One row of the AI pairwise sweep: car[index] against car[index + 1 .. 10],
 * push-apart only - no sound, no damage globals, no mode gate. */
s32 CollideRivalCars(GameCarRuntime *car, s32 index);
/* Draws one car, from the DrawCars loop; two LOD tiers plus the mirrored
 * wheel pass, submitted through SubmitModel. */
void DrawCar(void *car);
/* Selects model bank 1 and calls DrawCar for each of the 11 runtime cars
 * whose activeFlag != -1 and field_BC == 1. */
void DrawCars(void);
/* Car motion-state handler for state98 == 1: the one-frame jump takeoff, which
 * hands over to the airborne handler UpdateCarAirborne.  Declared K&R because
 * the UpdateCarDrivetrain dispatch passes two arguments to every handler in
 * the table and this one reads only the first. */
void UpdateCarLaunch();

/*
 * The player's own car object and the fields of it that retail addresses as
 * separate absolute symbols. Per-file types; see docs/names.md 12c.
 *   g_PlayerCar           D_8009E6D4  0x19C bytes, the g_Cars[] stride
 *   g_PlayerProgressA/B   g_PlayerProgressA / g_PlayerProgressB  +0x68 / +0x6C, sum = progress
 *   g_PlayerTrackProgress g_PlayerTrackProgress  +0x70
 *   g_PlayerTrackSection  g_PlayerTrackSection  +0x78
 *   g_PlayerLap           g_PlayerLap  +0x168, also the lap-time record header
 */

/* Declared identically by 58 translation units before this
 * header carried them. */

extern s32 D_8019C998;
extern s32 D_801E4194;
extern s32 g_AutoShiftCooldown;
extern u8 *g_CarModelBuffer;
extern s16 g_DragScale;
extern s32 g_EngineRpm;
extern s32 g_EngineRpmJitter;
extern s16 g_GripLossTimer;
extern u16 g_HudGlyphClut;
extern s16 g_PeakOutputRpm;
extern s16 g_PeakOutputValue;
extern u8 *volatile g_RaceIntroCameraScript;
extern s32 g_RoadGrade;
extern s32 g_SharedAssetWord0;
extern s32 g_ShiftSoundLevel;
extern s32 g_ShiftTargetRpm;
extern s32 g_StandingStartSpin;
extern s16 g_SteerHoldFrames;
extern s16 g_TachoNeedleQuad[4][2];
extern s16 g_TrackZoneDark;

void SetCarImageSlot(void *arg0, s32 arg1);
void SetCarModelSlot(void *arg0, s32 arg1);
s32 SmoothTrackAngle(s32 arg0, s32 arg1);
void UpdateRivalRubberBand(void);

#endif
