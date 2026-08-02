#include "common.h"
#include "game/track.h"
#include "game/car.h"
#include "game/race.h"
#include "game/menu.h"
#include "game/render.h"
#include "psyq/gte.h"

typedef struct GameInitVec4
{
  s32 words[4];
} GameInitVec4;
typedef struct GearCurveRow
{
  s32 values[16];
} GearCurveRow;
extern GearCurveRow gearRows[] asm("D_801E8884");
typedef struct GameInitShortVector
{
  s16 vx;
  s16 vy;
  s16 vz;
  s16 pad;
} GameInitShortVector;
/* The player object shares GameCarRuntime's 0x19C-byte footprint, but several
 * fields have player-only meanings.  Keep this file-local view for the init
 * sequence while using the shared GameCarDrive view for the +0xBC block. */
typedef struct GamePlayerCarInit
{
  s32 f00;
  s32 f04;
  s32 f08;
  u8 p0C[4];
  s32 f10;
  s32 f14;
  s32 f18;
  u8 p1C[4];
  s32 f20;
  s32 f24;
  s32 f28;
  s32 f2C;
  s32 f30;
  u8 p34[0xC];
  s32 f40;
  s32 f44;
  s32 f48;
  u8 p4C[4];
  s32 f50;
  s32 f54;
  s32 f58;
  s32 f5C;
  s32 f60;
  s32 f64;
  s32 f68;
  s32 f6C;
  s32 f70;
  s32 f74;
  u8 p78[0x20];
  s16 f98;
  u8 p9A[6];
  s32 fA0;
  s32 fA4;
  s32 fA8;
  u8 pAC[2];
  s16 fAE;
  u8 pB0[8];
  s16 fB8;
  u8 pBA[0xA];
  s32 fC4;
  s32 fC8;
  s32 fCC;
  u8 pD0[4];
  s32 fD4;
  s32 fD8;
  s32 fDC;
  u8 pE0[8];
  s16 fE8;
  s16 fEA;
  s16 fEC;
  s16 fEE;
  s16 fF0;
  s16 fF2;
  s16 fF4;
  u8 pF6[2];
  s16 fF8;
  u16 fFA;
  s16 fFC;
  s16 fFE;
  s32 f100;
  s32 f104;
  u8 p108[0x1C];
  s32 f124;
  s32 f128;
  u8 p12C[6];
  s16 f132;
  s32 f134;
  s32 f138;
  s32 f13C;
  u8 p140[0xC];
  s32 f14C;
  s32 f150;
  s32 f154;
  u8 p158[8];
  s16 f160;
  s16 f162;
  s32 f164;
  s16 f168;
} GamePlayerCarInit;
typedef struct GamePlayerCarSpecInit
{
  s32 curve[16];
  union
  {
    s32 w[16];
    u16 h[32];
  } f40;
  u8 p80[0x28];
  s32 fA8[10];
  s32 fD0[6];
  s32 fE8[6];
  s16 f100;
  s16 f102;
  s16 f104;
  s16 f106;
  s16 f108;
  u8 p10A[8];
  s16 f112;
  s16 f114[6];
  u8 p120[0x3C];
  s32 f15C;
} GamePlayerCarSpecInit;
extern GamePlayerCarSpecInit *g_PlayerCarInitSpec asm("D_801E42D8");
extern s32 g_GearTorqueCurve[] asm("D_801E8884");
extern char D_800113B0[];
extern char D_800113BC[];
extern char D_800113C4[];
extern char D_800113CC[];
extern char D_800113D4[];
extern char D_800113DC[];
extern char D_800113E0[];
extern char D_800113E8[];
extern char D_800113F0[];
extern char D_800113F8[];
extern char D_80011400[];
extern char D_80011408[];
extern s32 D_801F17B8;
extern s16 D_8019CAB0;
extern s32 D_801E8AA0;
extern s32 D_8007DA78;
extern s32 D_801E4BF4;
extern s16 D_8019C798;
extern s16 D_801E4B90;
extern s16 D_801E6F18;
extern s16 D_801E6F1A;
extern s16 D_801E4114[];
extern s16 D_801E4154[];
extern s16 D_8007DAD4[];
extern s32 D_801E4170;
extern s32 D_8019CAB4;
extern s32 D_801E4194;
extern s32 D_8019CA04;
extern s32 D_8019C998;
extern u16 D_801E4130;
extern s16 D_801E4FB4;
extern s16 D_801F17A4;
extern s16 D_801E4BA0;
extern s16 D_8019C9AC;
void func_80032BD0(void);
void func_80038AB8(void *);
s32 func_80030EB4(void *, s32);
void func_8002BF68(void *, s32);
void func_80031298(void *, s32, s16 *);
s32 GameIsCarFacingBackwards(GameCarTrackAngleWindow *) asm("func_8002CD08");
void GameInitPlayerCar(GameCarRuntime *car)
{
  s16 trackState[2];
  int scaledGearRatio;
  Matrix rotationMatrix;
  Matrix axisMatrix;
  GameInitShortVector rotationOffset;
  GamePlayerCarInit *player;
  GameCarDrive *drive;
  u8 *startData;
  s32 speedBandOffset;
  s32 i;
  s32 headingBase;
  s32 divisor;
  s32 speedThreshold;
  s16 *revLimit;
  s32 peakRpm;
  s32 value;
  s32 j;
  s32 k;
  GamePlayerCarSpecInit *carSpec;
  GamePlayerCarSpecInit *curveSpec;
  s16 *torqueBand;
  s16 *accelBand;
  s16 *accelBandOut;
  s32 bandSpeed;
  player = (GamePlayerCarInit *) car;
  startData = g_TrackEventData;
  GameDebugPrintf(D_800113B0);
  value = g_GrandPrixSeries;
  g_RacePhase = 2;
  g_RaceSeries = value & 1;
  func_80032BD0();
  func_80038AB8(car);
  D_801F17B8 = 0;
  D_8019CAB0 = 0;
  D_801E8AA0 = 0;
  D_8007DA78 = 0;
  player->fAE = 0x17;
  player->fCC = 0;
  player->fC8 = 0;
  player->fC4 = 0;
  player->fDC = 0;
  player->fD8 = 0;
  player->fD4 = 0;
  player->f18 = 0;
  player->f14 = 0;
  player->f10 = 0;
  player->f48 = 0;
  player->f44 = 0;
  player->f40 = 0;
  player->fA4 = 0;
  player->fA8 = 0;
  player->f168 = 0;
  player->fFA = 0;
  player->f68 = 0;
  player->f6C = 0;
  player->f70 = 0;
  GameDebugPrintf(D_800113BC);
  startData += g_RaceSeries * 0x90;
  player->f30 = *((s16 *) (startData + 0x35C));
  player->f00 = *((s32 *) (startData + 0x354));
  player->f08 = *((s32 *) (startData + 0x358));
  player->f04 = 0;
  player->f30 = func_80030EB4(car, player->f30);
  player->f20 = 0;
  headingBase = 0xC00 - (g_RaceSeries << 11);
  player->f24 = (headingBase - g_TrackPoints[player->f30].angle) & 0xFFF;
  player->f28 = 0;
  player->f64 = 0;
  player->f164 = player->f30;
  player->fA0 = player->f24;
  player->f14C = player->fA0;
  func_8002BF68(car, 0);
  trackState[0] = 0;
  trackState[1] = 0;
  func_80031298(car, player->f30, trackState);
  player->f74 = player->f70;
  *((GameInitVec4 *) (&player->f50)) = *((GameInitVec4 *) (&player->f20));
  player->f60 = player->f04;
  GameBuildRotMatrixY(&rotationMatrix, player->f24);
  GameBuildRotMatrixX(&axisMatrix, player->f20);
  MulMatrix2(&axisMatrix, &rotationMatrix);
  GameBuildRotMatrixZ(&axisMatrix, player->f28);
  MulMatrix2(&axisMatrix, &rotationMatrix);
  rotationOffset.vx = 0;
  rotationOffset.vy = 0;
  axisMatrix.m[0][0] = rotationMatrix.m[0][0];
  axisMatrix.m[0][1] = rotationMatrix.m[1][0];
  axisMatrix.m[0][2] = rotationMatrix.m[2][0];
  axisMatrix.m[1][0] = rotationMatrix.m[0][1];
  axisMatrix.m[1][1] = rotationMatrix.m[1][1];
  axisMatrix.m[1][2] = rotationMatrix.m[2][1];
  axisMatrix.m[2][0] = rotationMatrix.m[0][2];
  axisMatrix.m[2][1] = rotationMatrix.m[1][2];
  axisMatrix.m[2][2] = rotationMatrix.m[2][2];
  rotationOffset.vz = (-player->fFA) - 0x32;
  ApplyMatrix(&axisMatrix, &rotationOffset, &player->f10);
  player->f162 = -1;
  player->f154 = 3;
  player->fE8 = 0;
  player->fEA = 1;
  player->fF2 = 0;
  player->fEE = 0;
  player->fFE = 0;
  player->fFC = 0;
  player->fF4 = 0;
  player->fF0 = 0;
  player->f100 = 0;
  player->f104 = 0;
  player->f124 = 0;
  player->f128 = 0;
  player->f132 = 1;
  player->f134 = 0;
  player->f13C = 0;
  player->f150 = 0;
  player->f138 = 0;
  player->fFA = 0;
  player->f160 = 1;
  player->f00 = player->f00 + player->f10;
  player->f08 = player->f08 + player->f18;
  player->fB8 = GameIsCarFacingBackwards((GameCarTrackAngleWindow *) car);
  player->fF4 = 0;
  player->fF0 = 0;
  player->fEC = 1;
  player->fF8 = 0;
  D_801E4BF4 = 0;
  drive = (GameCarDrive *) (((u8 *) car) + (divisor = 0xBC));
  GameDebugPrintf(D_800113C4);
  carSpec = g_PlayerCarInitSpec;
  if (carSpec->f104 < 6)
  {
    if (carSpec->f104 <= 0)
    {
      carSpec->f104 = 6;
    }
  }
  else
  {
    carSpec->f104 = 6;
  }
  drive->unk8C = (g_PlayerCarInitSpec->f15C * 0x490) / 160;
  GameDebugPrintf(D_800113CC);
  j = 0;
  for (i = 0; i < 16; i++)
  {
    g_GearTorqueCurve[i] = g_PlayerCarInitSpec->curve[i] / 20;
    if (j < g_GearTorqueCurve[i])
    {
      D_8019C798 = i;
      j = g_GearTorqueCurve[i];
    }
  }

  D_801E4B90 = j;
  peakRpm = g_PlayerCarInitSpec->f40.h[D_8019C798 * 2];
  D_801E6F18 = (((s16) peakRpm) - g_PlayerCarInitSpec->f106) / 2;
  revLimit = &g_PlayerCarInitSpec->f100;
  D_801E6F1A = ((*revLimit) - ((s16) peakRpm)) / 2;
  D_8019C798 = peakRpm;
  GameDebugPrintf(D_800113D4);
  GameDebugPrintf(D_800113DC, g_PlayerCarInitSpec->f104);
  for (j = 0; j < 6; j++)
  {
    scaledGearRatio = (g_PlayerCarInitSpec->fE8[j] * 0x490) / 160;
    g_PlayerCarInitSpec->fD0[j] = (((scaledGearRatio * 6) / 100) << 17) / 10000;
    value = (g_PlayerCarInitSpec->f114[j] * g_PlayerCarInitSpec->fE8[j]) / 100;
    divisor = value;
    divisor = (divisor > 0) ? (divisor) : (g_PlayerCarInitSpec->fE8[j]);
    for (i = 0; i < 16; i++)
    {
      gearRows[j + 1].values[i] = g_PlayerCarInitSpec->curve[i] / divisor;
    }

  }

  if (g_PlayerCarInitSpec->f112 < 2)
  {
    g_PlayerCarInitSpec->f112 = 1;
  }
  GameDebugPrintf(D_800113E0);
  curveSpec = g_PlayerCarInitSpec;
  accelBand = D_801E4154;
  speedBandOffset = 0;
  speedThreshold = 0x3E8;
  torqueBand = D_801E4114;
  do
  {
    for (i = 0; i < 16; i++)
    {
      if ((curveSpec->f40.w[i] / speedThreshold) > 0)
      {
        *torqueBand = i;
        break;
      }
    }

    i = 0;
    bandSpeed = speedThreshold;
    accelBandOut = accelBand;
    while (i < 10)
    {
      if ((curveSpec->fA8[i] / bandSpeed) > 0)
      {
        *accelBandOut = i;
        break;
      }
      i++;
    }

    accelBand++;
    speedBandOffset += 2;
    speedThreshold += 0x3E8;
    torqueBand++;
  }
  while (speedBandOffset < 20);
  GameDebugPrintf(D_800113E8);
  drive->unk84 = D_8007DAD4[drive->unk28 % 5] * 0xE;
  drive->unk88 = g_PlayerCarInitSpec->f108;
  GameDebugPrintf(D_800113F0);
  player->f98 = 0;
  drive->unk9E = 0;
  drive->unk9C = 0;
  D_801E4170 = 0;
  D_8019CAB4 = 0;
  D_801E4194 = 0;
  D_8019CA04 = 0;
  D_8019C998 = 0;
  if (drive->manual != 0)
  {
    D_801E4130 = 0x7800;
  }
  else
  {
    D_801E4130 = 0x78CF;
  }
  D_801E4FB4 = 0x3E8;
  D_801F17A4 = 0;
  D_801E4BA0 = 0;
  g_WrongWayTimer = 0;
  D_8019C9AC = 0;
  GameDebugPrintf(D_800113F8);
  GameDebugPrintf(D_80011400, player->f68);
  GameDebugPrintf(D_80011408);
}

/*
 * Wrong-way / spin check: compares the car's headingAngle against the current
 * track point's forward direction (0xC00 - track angle) and returns whether the
 * delta falls inside the 0x401..0x7FF window (i.e. facing roughly backwards).
 * Uses the 0x19C-stride GameCarTrackAngleWindow view onto the car array.
 */
s32 GameIsCarFacingBackwards(GameCarTrackAngleWindow *arg0) asm("func_8002CD08");
s32 GameIsCarFacingBackwards(GameCarTrackAngleWindow *arg0) {
    s32 index = arg0->trackPointIndex;
    s32 complement = 0xC00 - g_TrackPoints[index].angle;
    s32 diff = (arg0->headingAngle - complement) & 0xFFF;
    return (u32)(diff - 0x401) < 0x7FFU;
}

typedef struct SubB {
    char _p0[0x1C];
    s32 x1C;
    char _p1[0x20];
    s16 x40;
} SubB;

typedef struct A {
    char _a0[0x24];
    s32 f24;
    char _a1[0x0C];
    s32 f34;
    char _a2[0x0C];
    s32 f44;
    char _a3[0x1C];
    s32 f64;
    char _a4[0x3C];
    s32 fA4;
    char _a5[0x0C];
    s32 fB4;
    s16 fB8;
    char _a6[2];
    SubB sub;
} A;

/* Deliberately raw: both writes in the image store zero, so this branch --
 * ignore the pad and freeze the steering -- is unreachable in retail. */
extern s16 D_8019C9AC;
extern u8 g_PadType asm("D_801E4369");
/* The live button mapping; masks 0 and 1 steer, g_MirrorMode swaps them. */
extern s16 g_PadButtonMapping[] asm("D_801E4B60");
/* NeGcon steering: the raw twist minus the calibrated centre, minus the dead
 * zone indexed by the separate NEGCON STEER PLAY setting at D_8019CAD0 (a
 * word table at 0x8007C128), then clamped to
 * +-g_NegconSteerRange[g_NegconMaxTwist].
 * g_NegconMaxTwist is the 0..3 setting the MAXIMUM TWIST screen edits and the
 * save file keeps; the range table is { 25, 38, 75, 113 }, so a higher setting
 * needs more twist for full lock. This file used to spell D_801E418C
 * g_NegconSteerPlay, which is the name of that other setting -- see
 * docs/names.md 20. */
extern s16 g_NegconSteer asm("D_801E437E");
extern s16 g_NegconMaxTwist asm("D_801E418C");
extern s16 g_NegconSteerRange[] asm("D_8007C020");

s32 func_80068634(s32);

/*
 * Steering-lean / body-roll state machine for the car `ctx`: drives the lean
 * (f44) and roll (f64) from the drive-block steer input (sub.x1C), branching on
 * the control mode g_RacePhase (0x41 = player, 0x23 = demo). The local A / SubB
 * typedefs are raw-offset overlays onto the car runtime (drive block at +0xBC)
 * shaped to match; retyping them to GameCarRuntime would break the match.
 */
void GameUpdateCarBodyRoll(A *ctx) asm("func_8002CD4C");
void GameUpdateCarBodyRoll(A *ctx) {
    SubB *p = &ctx->sub;
    s16 mode = g_RacePhase;
    s32 v1, a1;
    s32 a0v, r, s2v;

    if (mode < 2) {
        ctx->sub.x1C = 0;
        ctx->f44 = 0;
        goto L_53C;
    }
    if (mode >= 4) goto L_448;
    if (D_8019C9AC != 0) goto L_448;
    if (!(g_PadType != 0x41)) {

    if (g_MirrorMode != 0) {
        a1 = g_PadHeld & g_PadButtonMapping[0];
        v1 = g_PadHeld & g_PadButtonMapping[1];
    } else {
        v1 = g_PadHeld & g_PadButtonMapping[0];
        a1 = g_PadHeld & g_PadButtonMapping[1];
    }

    if (v1 != 0) {
    a0v = 2;
    if (ctx->fB8 != 0) a0v = 1;
    v1 = p->x1C;
    p->x40 = a0v;
    if (v1 > 0) {
        p->x1C = 0;
    } else if (v1 >= -4095) {
        p->x1C = v1 - 1536;
    }
    ctx->f64 = ctx->f64 - 6;
    } else if (a1 != 0) {
    a0v = 1;
    if (ctx->fB8 != 0) a0v = 2;
    v1 = p->x1C;
    p->x40 = a0v;
    if (v1 < 0) {
        p->x1C = 0;
    } else if (v1 < 4096) {
        p->x1C = v1 + 1536;
    }
    ctx->f64 = ctx->f64 + 6;
    } else {
    p->x40 = 0;
    p->x1C = p->x1C / 3;
    }
    ctx->f44 = -p->x1C;
    if (ctx->f64 != 0) {
        ctx->f64 = (ctx->f64 * 7) / 8;
    }
    goto L_53C;
    }

    if (g_PadType != 0x23) goto L_43C;
    a1 = ((s32)(g_NegconSteer * 13) << 9) / g_NegconSteerRange[g_NegconMaxTwist];
    if (g_MirrorMode != 0) a1 = -a1;
    if (!(a1 >= 0)) {

    /* a1 < 0 */
    a0v = 2;
    if (ctx->fB8 != 0) a0v = 1;
    v1 = ctx->sub.x1C;
    ctx->sub.x40 = a0v;
    if (v1 > 0) {
        ctx->sub.x1C = 0;
        ctx->f44 = 0;
    } else if ((a1 - 256) < v1) {
        s32 t;
        if (v1 >= 4097) v1 = 4096;
        t = func_80068634(v1 / 8);
        p->x1C = p->x1C - (t / 4);
        ctx->f44 = ctx->f44 + 1536;
    } else {
        ctx->sub.x1C = v1 / 3;
    }
    ctx->f64 = ctx->f64 - 6;
    } else if (!(a1 <= 0)) {
    a0v = 1;
    if (ctx->fB8 != 0) a0v = 2;
    v1 = ctx->sub.x1C;
    ctx->sub.x40 = a0v;
    if (v1 < 0) {
        ctx->sub.x1C = 0;
        ctx->f44 = 0;
    } else if (v1 < (a1 + 256)) {
        s32 t;
        s32 c = v1;
        if (v1 < -4096) { v1 = -4096; c = v1; }
        t = func_80068634(c / 8);
        p->x1C = p->x1C + (t / 4);
        ctx->f44 = ctx->f44 - 1536;
    } else {
        ctx->sub.x1C = v1 / 3;
    }
    ctx->f64 = ctx->f64 + 6;
    } else {
    ctx->sub.x40 = 0;
    ctx->f44 = ctx->f44 / 2;
    ctx->sub.x1C = ctx->sub.x1C / 6;
    }
    if (ctx->f64 != 0) {
        ctx->f64 = (ctx->f64 * 7) / 8;
    }
    goto L_53C;

L_43C:
    ctx->f64 = 0;
    ctx->sub.x1C = 0;
    goto L_53C;

L_448:
    {
        s32 arg1 = (ctx->fB8 << 11) + 3072;
        r = GameGetAngleDelta(ctx->f24, arg1 - ctx->fB4);
    }
    s2v = r * 32;
    r = func_80068634(ctx->f34 * 2);
    a0v = 4096 - r;
    if (ctx->fA4 < 800) {
        a0v = a0v * 6;
    } else {
        a0v = a0v * 4;
    }
    if (ctx->fA4 >= 81) {
        if (ctx->fB8 != 0) {
            if (ctx->f34 < 0) a0v = -a0v;
        } else {
            if (ctx->f34 > 0) a0v = -a0v;
        }
        a0v = a0v + s2v;
    } else {
        p->x1C = 0;
        a0v = 0;
    }
    if (a0v < -4096) a0v = -4096;
    if (a0v > 4096) a0v = 4096;
    p->x1C = a0v;
    ctx->f44 = a0v;
    ctx->f64 = a0v / 128;

L_53C:
    v1 = ctx->fA4;
    if (v1 < 800) {
        s32 f = ctx->f64;
        ctx->f64 = (f * v1) / 800;
    }
}

s32 func_80069C98(s32 arg0, s32 arg1, s32 arg2);

/*
 * Point-in-quad test: returns 1 if point `pt` is inside the quad with corners
 * p0,p1,p3,p2 (four chained half-plane sign checks via func_80069C98), else 0.
 */
s32 GameIsPointInQuad(s32 p0, s32 p1, s32 p2, s32 p3, s32 pt) asm("func_8002D2E8");
s32 GameIsPointInQuad(s32 p0, s32 p1, s32 p2, s32 p3, s32 pt) {
    s32 result;
    s32 ret = 0;

    if (func_80069C98(p0, p1, pt) >= 0) {
        if (func_80069C98(p1, p3, pt) >= 0) {
            if (func_80069C98(p3, p2, pt) >= 0) {
                result = func_80069C98(p2, p0, pt) >= 0;
                ret = result;
            }
        }
    }

    return ret;
}

typedef struct SVec
{
  s16 vx;
  s16 vy;
  s16 vz;
  s16 pad;
} SVec;
typedef struct Vec4
{
  s32 x;
  s32 y;
  s32 z;
  s32 w;
} Vec4;
typedef struct CPt
{
  s16 x;
  s16 y;
} CPt;
typedef struct CollisionGeometry
{
  SVec rotation;
  Vec4 transformed;
  Matrix matrix;
  SVec delta;
  CPt polygon[6];
  CPt grid[4][4];
  CPt opponentGrid[10];
  CPt opponentPolygon[4];
} CollisionGeometry;
typedef struct CollisionContext
{
  SVec rotation;
  Vec4 transformed;
  Matrix matrix;
  SVec delta;
  CPt polygon[6];
  CPt grid[4][4];
  CPt opponentGrid[10];
  CPt opponentPolygon[4];
  s32 carIndex;
  s32 outerIndex;
  s32 playerProgress;
  s32 innerIndex;
  s32 playerTrack;
  s32 polygonOffset;
  s32 playerX;
  s32 opponentPolygonOffset;
  s32 trackDelta;
} CollisionContext;
/* GCC 2.6.3 lays the separately named geometry locals out as this context.
 * The typed view gives the three point-test passes one in-bounds coordinate
 * system and names the final collision-response delta at the same location. */
extern s16 D_801E4DAC;
extern GameCarRuntime D_801F1854[];
extern u16 D_8007DA88[];
extern u16 D_8007DA8A[];
extern u16 D_8007DAA0[];
extern u16 D_8007DAA2[];
extern s32 D_801E40D8;
extern s16 D_801E4FB4;
extern s16 D_801E6E74;
extern s32 D_8019CACC;
extern s16 D_801E4BA0;
extern s32 D_801E408C;
extern s16 D_801E8A8C;
void func_80069D18(void *rot, void *mtx);
s32 *func_80069678(void *mtx, void *vec, void *out);
s32 func_8002D2E8(s32 a, s32 b, s32 c, s32 d, s32 e);
void func_80038CE8(GameCarRuntime *car, s32 arg1, s32 arg2, s32 mode);
void func_8005D6EC(s32 id);
s32 GameCollidePlayerWithCars(GameCarRuntime *car)
{
  SVec rotation;
  s32 opponentX;
  Vec4 transformed;
  Matrix rotationMatrix;
  CPt velocityDelta;
  u8 *diagonalWalk;
  u8 *pointWalk;
  CPt playerOutline[6];
  CPt playerGrid[4][4];
  CPt opponentSamples[10];
  CPt opponentCorners[4];
  GameCarRuntime *opponent;
  s32 index;
  s32 sampleIndex;
  s32 quadIndex;
  s32 pointOffset;
  s32 cornerOffset;
  s32 collisionRegion;
  s32 progressDelta;
  s32 playerProgress;
  s32 playerTrackOffset;
  s32 playerX;
  s32 trackDelta;
  CollisionContext *context;
  if (D_801E4DAC == 0)
  {
    return 0;
  }
  opponent = D_801F1854;
  collisionRegion = 0;
  rotation.vx = *((u16 *) (((u8 *) car) + 0x20));
  rotation.vz = *((u16 *) (((u8 *) car) + 0x28));
  rotation.vy = *((u16 *) (((u8 *) car) + 0x24));
  func_80069D18(&rotation, &rotationMatrix);
  index = 0;
  pointWalk = (u8 *) (&rotation);
  diagonalWalk = pointWalk;
  pointOffset = 0;
  do
  {
    rotation.vx = *((u16 *) (((u8 *) D_8007DA88) + pointOffset));
    rotation.vz = *((u16 *) (((u8 *) D_8007DA8A) + pointOffset));
    rotation.vy = 0;
    func_80069678(&rotationMatrix, &rotation, &transformed);
    (*((CPt *) (((u8 *) playerOutline) + pointOffset))).x = transformed.x >> 1;
    (*((CPt *) (((u8 *) playerOutline) + pointOffset))).y = transformed.z >> 1;
    if (index < 4)
    {
      u8 *diagonalBase = diagonalWalk + 0x58;
      *((CPt *) ((u32) pointOffset + (u32) diagonalBase)) = *((CPt *) (pointWalk + 0x40));
    }
    pointWalk += 4;
    diagonalWalk += 16;
    pointOffset += 4;
    index++;
  }
  while (index < 6);
  playerGrid[0][1].x = (playerGrid[1][0].x = (playerOutline[0].x + playerOutline[1].x) / 2);
  playerGrid[0][1].y = (playerGrid[1][0].y = (playerOutline[0].y + playerOutline[1].y) / 2);
  playerGrid[0][2].x = (playerGrid[2][0].x = playerOutline[4].x);
  playerGrid[0][2].y = (playerGrid[2][0].y = playerOutline[4].y);
  playerGrid[1][3].x = (playerGrid[3][1].x = playerOutline[5].x);
  playerGrid[1][3].y = (playerGrid[3][1].y = playerOutline[5].y);
  playerGrid[2][3].x = (playerGrid[3][2].x = (playerOutline[2].x + playerOutline[3].x) / 2);
  playerGrid[2][3].y = (playerGrid[3][2].y = (playerOutline[2].y + playerOutline[3].y) / 2);
  playerGrid[0][3].x = (playerGrid[1][2].x = (playerGrid[2][1].x = (playerGrid[3][0].x = (playerOutline[4].x + playerOutline[5].x) / 2)));
  playerGrid[0][3].y = (playerGrid[1][2].y = (playerGrid[2][1].y = (playerGrid[3][0].y = (playerOutline[4].y + playerOutline[5].y) / 2)));
  playerProgress = *((s32 *) (((u8 *) car) + 0x70));
  index = 0;
  pointWalk = (u8 *) (&rotation);
  context = (CollisionContext *) pointWalk;
  playerTrackOffset = *((s32 *) (((u8 *) car) + 0x34));
  playerX = *((s32 *) (((u8 *) car) + 0x04));
  for (; index < 11; index++, opponent = (GameCarRuntime *) (((u8 *) opponent) + 0x19C))
  {
    if ((*((s16 *) (((u8 *) opponent) + 0xAC))) != (-1))
    {
      s32 aDist;
      *((s16 *) (((u8 *) opponent) + 0x8A)) = 0;
      progressDelta = ((s32) (((*((s32 *) (((u8 *) opponent) + 0x70))) + D_801E40D8) - playerProgress)) % ((s32) D_801E40D8);
      opponentX = *((s32 *) (((u8 *) opponent) + 0x04));
      trackDelta = (*((s32 *) (((u8 *) opponent) + 0x34))) - playerTrackOffset;
      if (trackDelta < 0)
      {
        trackDelta = -trackDelta;
      }
      aDist = opponentX - playerX;
      if (aDist < 0)
      {
        aDist = -aDist;
      }
      if (((*((s16 *) (((u8 *) opponent) + 0x98))) == (*((s16 *) (((u8 *) car) + 0x98)))) || (aDist < 0x1A))
      {
        if (((trackDelta < 0x64) && ((progressDelta < 0xC8) || ((D_801E40D8 - 0xC8) < progressDelta))) && (aDist < 0x3C))
        {
          if ((progressDelta < 0xC8) && (trackDelta < 0x32))
          {
            D_801E4FB4 = 0x2BC;
          }
          velocityDelta.x = (*((u16 *) (((u8 *) opponent) + 0x00))) - (*((u16 *) (((u8 *) car) + 0x00)));
          velocityDelta.y = (*((u16 *) (((u8 *) opponent) + 0x08))) - (*((u16 *) (((u8 *) car) + 0x08)));
          rotation.vx = *((u16 *) (((u8 *) opponent) + 0x20));
          rotation.vz = *((u16 *) (((u8 *) opponent) + 0x28));
          rotation.vy = *((u16 *) (((u8 *) opponent) + 0x24));
          func_80069D18(&rotation, &rotationMatrix);
          for (cornerOffset = 0; cornerOffset < 16; cornerOffset += 4)
          {
            rotation.vx = *((u16 *) (((u8 *) D_8007DAA0) + cornerOffset));
            rotation.vz = *((u16 *) (((u8 *) D_8007DAA2) + cornerOffset));
            rotation.vy = 0;
            func_80069678(&rotationMatrix, &rotation, &transformed);
            ((CPt *) (((u8 *) (&opponentCorners[0])) + cornerOffset))->x = (transformed.x >> 1) + (velocityDelta.x / 2);
            ((CPt *) (((u8 *) (&opponentCorners[0])) + cornerOffset))->y = (transformed.z >> 1) + (velocityDelta.y / 2);
          }

          opponentSamples[0].x = (opponentCorners[0].x + opponentCorners[1].x) / 2;
          opponentSamples[0].y = (opponentCorners[0].y + opponentCorners[1].y) / 2;
          opponentSamples[1].x = (opponentCorners[0].x + opponentCorners[2].x) / 2;
          opponentSamples[1].y = (opponentCorners[0].y + opponentCorners[2].y) / 2;
          opponentSamples[2].x = (opponentCorners[1].x + opponentCorners[3].x) / 2;
          opponentSamples[2].y = (opponentCorners[1].y + opponentCorners[3].y) / 2;
          opponentSamples[3].x = (opponentCorners[2].x + opponentCorners[3].x) / 2;
          opponentSamples[3].y = (opponentCorners[2].y + opponentCorners[3].y) / 2;
          opponentSamples[4].x = (opponentSamples[0].x + opponentSamples[2].x) / 2;
          opponentSamples[4].y = (opponentSamples[0].y + opponentSamples[2].y) / 2;
          opponentSamples[6].x = (opponentCorners[0].x + opponentSamples[1].x) / 2;
          opponentSamples[6].y = (opponentCorners[0].y + opponentSamples[1].y) / 2;
          opponentSamples[7].x = (opponentCorners[1].x + opponentSamples[2].x) / 2;
          opponentSamples[7].y = (opponentCorners[1].y + opponentSamples[2].y) / 2;
          opponentSamples[8].x = (opponentCorners[2].x + opponentSamples[1].x) / 2;
          opponentSamples[8].y = (opponentCorners[2].y + opponentSamples[1].y) / 2;
          opponentSamples[9].x = (opponentCorners[3].x + opponentSamples[2].x) / 2;
          opponentSamples[9].y = (opponentCorners[3].y + opponentSamples[2].y) / 2;
          for (sampleIndex = 0; sampleIndex < 4; sampleIndex++)
          {
            for (quadIndex = 0; quadIndex < 4; quadIndex++)
            {
              collisionRegion = func_8002D2E8(
                *((s32 *) ((u8 *) context + (u32) &((CollisionContext *) 0)->grid + 8 + (quadIndex * 16))),
                *((s32 *) ((u8 *) context + (u32) &((CollisionContext *) 0)->grid + 12 + (quadIndex * 16))),
                *((s32 *) ((u8 *) context + (u32) &((CollisionContext *) 0)->grid + (quadIndex * 16))),
                *((s32 *) ((u8 *) context + (u32) &((CollisionContext *) 0)->grid + 4 + (quadIndex * 16))),
                *((s32 *) ((u8 *) context + (u32) &((CollisionContext *) 0)->opponentPolygon + (sampleIndex * 4))));
              if (collisionRegion > 0)
              {
                collisionRegion = quadIndex + 1;
                break;
              }
            }

            if (collisionRegion > 0)
            {
              goto collision_found;
            }
          }

          for (sampleIndex = 0; sampleIndex < 5; sampleIndex++)
          {
            for (quadIndex = 0; quadIndex < 4; quadIndex++)
            {
              collisionRegion = func_8002D2E8(
                *((s32 *) ((u8 *) context + (u32) &((CollisionContext *) 0)->grid + 8 + (quadIndex * 16))),
                *((s32 *) ((u8 *) context + (u32) &((CollisionContext *) 0)->grid + 12 + (quadIndex * 16))),
                *((s32 *) ((u8 *) context + (u32) &((CollisionContext *) 0)->grid + (quadIndex * 16))),
                *((s32 *) ((u8 *) context + (u32) &((CollisionContext *) 0)->grid + 4 + (quadIndex * 16))),
                *((s32 *) ((u8 *) context + (u32) &((CollisionContext *) 0)->opponentGrid + (sampleIndex * 4))));
              if (collisionRegion > 0)
              {
                collisionRegion = quadIndex + 1;
                break;
              }
            }

            if (collisionRegion > 0)
            {
              goto collision_found;
            }
          }

          for (sampleIndex = 0; sampleIndex < 4; sampleIndex++)
          {
            for (quadIndex = 0; quadIndex < 4; quadIndex++)
            {
              collisionRegion = func_8002D2E8(
                *((s32 *) ((u8 *) context + (u32) &((CollisionContext *) 0)->grid + 8 + (quadIndex * 16))),
                *((s32 *) ((u8 *) context + (u32) &((CollisionContext *) 0)->grid + 12 + (quadIndex * 16))),
                *((s32 *) ((u8 *) context + (u32) &((CollisionContext *) 0)->grid + (quadIndex * 16))),
                *((s32 *) ((u8 *) context + (u32) &((CollisionContext *) 0)->grid + 4 + (quadIndex * 16))),
                *((s32 *) ((u8 *) context + (u32) &((CollisionContext *) 0)->opponentGrid + 24 + (sampleIndex * 4))));
              if (collisionRegion > 0)
              {
                collisionRegion = quadIndex + 1;
                break;
              }
            }

            if (collisionRegion > 0)
            {
              goto collision_found;
            }
          }

        }
        else
          if ((trackDelta < 0x32) && (progressDelta < 0x3E8))
        {
          s32 v = 0x3E8 - progressDelta;
          if (v < 0)
          {
            v += 3;
          }
          D_801E4FB4 = 0x3E8 - (v >> 2);
        }
      }
    }
  }

  if (collisionRegion <= 0)
  {
    return collisionRegion;
  }
  collision_found:
  if (((*((s16 *) (((u8 *) car) + 0x82))) < 0xB) && (D_801E6E74 < 3))
  {
    s32 sid;
    if (((u32) (trackDelta + 0x1D)) < 0x3B)
    {
      sid = 0xA;
      if (collisionRegion >= 3)
      {
        sid = 0xD;
      }
    }
    else
    {
      sid = 0xC;
      if ((collisionRegion & 1) != D_8019CACC)
      {
        sid = 0xB;
      }
    }
    func_8005D6EC(sid);
  }

  trackDelta = (*((s32 *) (((u8 *) car) + 0x34))) - (*((s32 *) (((u8 *) opponent) + 0x34)));
  D_801E4BA0 = 0;
  ((CollisionContext *) &rotation)->trackDelta = trackDelta;
  if (collisionRegion < 3)
  {
    if ((*((s16 *) (((u8 *) car) + 0xB8))) != D_801E408C)
    {
      *((s32 *) (((u8 *) car) + 0x150)) = 0;
      *((s32 *) (((u8 *) car) + 0xA8)) = 0;
    }
    else
    {
      *((s32 *) (((u8 *) car) + 0xA8)) = (*((s32 *) (((u8 *) car) + 0xA8))) / 2;
      *((s32 *) (((u8 *) car) + 0x150)) = ((*((s32 *) (((u8 *) car) + 0x150))) * 0x50) / 100;
    }
    if (((*((s32 *) (((u8 *) car) + 0xA4))) - (*((s32 *) (((u8 *) opponent) + 0xA4)))) >= 0x191)
    {
      D_801E4BA0 = 0x1E;
    }
    else
    {
      D_801E4BA0 = 0xF;
    }
    {
      s32 vx;
      s32 vz;
      vx = (s16) ((*((u16 *) (((u8 *) opponent) + 0xC8))) - (*((u16 *) (((u8 *) car) + 0xC4))));
      velocityDelta.x = vx / 0x20;
      vz = (s16) ((*((u16 *) (((u8 *) opponent) + 0xD0))) - (*((u16 *) (((u8 *) car) + 0xCC))));
      velocityDelta.y = vz / 0x20;
    }
    if ((((*((s16 *) (((u8 *) car) + 0xB8))) != D_801E408C) && ((*((s32 *) (((u8 *) car) + 0xA4))) >= 0x51)) && (D_801E8A8C >= 0xA))
    {
      func_80038CE8(opponent, 0, 0, 4);
      func_80038CE8(car, 0, 0, 4);
    }
    else
    {
      if ((*((s32 *) (((u8 *) car) + 0xA4))) >= 0x29)
      {
        func_80038CE8(car, 0, 0, 4);
      }
      else
      {
        func_80038CE8(car, -((s16) velocityDelta.x), -((s16) velocityDelta.y), 4);
      }
      func_80038CE8(opponent, (s16) velocityDelta.x, (s16) velocityDelta.y, 4);
    }
  }
  else
  {
    s32 vx;
    s32 vz;
    *((s32 *) (((u8 *) opponent) + 0xA4)) = (*((s32 *) (((u8 *) opponent) + 0xA4))) / 2;
    *((s32 *) (((u8 *) opponent) + 0xA8)) = (*((s32 *) (((u8 *) opponent) + 0xA8))) / 2;
    *((s16 *) (((u8 *) opponent) + 0x12E)) = *((u16 *) (((u8 *) opponent) + 0x12A));
    vx = (s16) ((*((u16 *) (((u8 *) opponent) + 0xC8))) - (*((u16 *) (((u8 *) car) + 0xC4))));
    velocityDelta.x = vx / 0x20;
    vz = (s16) ((*((u16 *) (((u8 *) opponent) + 0xD0))) - (*((u16 *) (((u8 *) car) + 0xCC))));
    velocityDelta.y = vz / 0x20;
    velocityDelta.x = velocityDelta.x - (*((u16 *) (((u8 *) opponent) + 0x7C)));
    velocityDelta.y = velocityDelta.y - (*((u16 *) (((u8 *) opponent) + 0x7E)));
    if ((((*((s16 *) (((u8 *) car) + 0xB8))) != D_801E408C) && ((*((s32 *) (((u8 *) car) + 0xA4))) >= 0x51)) && (D_801E8A8C >= 0xA))
    {
      func_80038CE8(opponent, 0, 0, 4);
      func_80038CE8(car, 0, 0, 4);
    }
    else
    {
      func_80038CE8(car, -((s16) velocityDelta.x), -((s16) velocityDelta.y), 4);
      func_80038CE8(opponent, 0, 0, 4);
    }
  }
  *((s16 *) (((u8 *) opponent) + 0x8A)) = 1;
  return collisionRegion;
}
