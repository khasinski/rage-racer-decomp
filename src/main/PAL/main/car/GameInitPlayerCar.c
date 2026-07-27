#include "common.h"
#include "game/track.h"
#include "game/car.h"
#include "game/race.h"
#include "game/menu.h"
#include "game/render.h"

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/car/GameInitPlayerCar", func_8002C478);

/*
 * Wrong-way / spin check: compares the car's headingAngle against the current
 * track point's forward direction (0xC00 - track angle) and returns whether the
 * delta falls inside the 0x401..0x7FF window (i.e. facing roughly backwards).
 * Uses the 0x19C-stride GameCarTrackAngleWindow view onto the car array.
 */
s32 GameIsCarFacingBackwards(GameCarTrackAngleWindow *arg0) asm("func_8002CD08");
s32 GameIsCarFacingBackwards(GameCarTrackAngleWindow *arg0) {
    register s32 index asm("$3") = arg0->trackPointIndex;
    register s32 scaled asm("$2");
    register GameTrackPoint *table asm("$3");
    register s32 complement asm("$3");
    register s32 target asm("$5");
    s32 diff;

    asm volatile("" : "=r"(index) : "0"(index));
    scaled = index * 3;
    table = g_TrackPoints;
    target = ((GameTrackPoint *)((scaled << 3) + (s32)table))->angle;
    complement = 0xC00;
    complement -= target;
    diff = arg0->headingAngle - complement;
    diff &= 0xFFF;
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
    if (g_PadType != 0x41) goto L_1C4;

    if (g_MirrorMode != 0) {
        a1 = g_PadHeld & g_PadButtonMapping[0];
        v1 = g_PadHeld & g_PadButtonMapping[1];
    } else {
        v1 = g_PadHeld & g_PadButtonMapping[0];
        a1 = g_PadHeld & g_PadButtonMapping[1];
    }

    if (v1 == 0) goto L_11c;
    a0v = 2;
    if (ctx->fB8 != 0) a0v = 1;
    v1 = p->x1C;
    p->x40 = a0v;
    if (v1 > 0) { p->x1C = 0; goto L108; }
    if (v1 < -4095) goto L108;
    p->x1C = v1 - 1536;
L108:
    ctx->f64 = ctx->f64 - 6;
    goto L_194;

L_11c:
    if (a1 == 0) goto L_174;
    a0v = 1;
    if (ctx->fB8 != 0) a0v = 2;
    v1 = p->x1C;
    p->x40 = a0v;
    if (v1 < 0) { p->x1C = 0; goto L160; }
    if (v1 >= 4096) goto L160;
    p->x1C = v1 + 1536;
L160:
    ctx->f64 = ctx->f64 + 6;
    goto L_194;

L_174:
    p->x40 = 0;
    p->x1C = p->x1C / 3;

L_194:
    ctx->f44 = -p->x1C;
    if (ctx->f64 != 0) {
        ctx->f64 = (ctx->f64 * 7) / 8;
    }
    goto L_53C;

L_1C4:
    if (g_PadType != 0x23) goto L_43C;
    a1 = ((s32)(g_NegconSteer * 13) << 9) / g_NegconSteerRange[g_NegconMaxTwist];
    if (g_MirrorMode != 0) a1 = -a1;
    if (a1 >= 0) goto L_310;

    /* a1 < 0 */
    a0v = 2;
    if (ctx->fB8 != 0) a0v = 1;
    v1 = ctx->sub.x1C;
    ctx->sub.x40 = a0v;
    if (v1 > 0) {
        ctx->sub.x1C = 0;
        ctx->f44 = 0;
        goto L_2FC;
    }
    if ((a1 - 256) < v1) {
        s32 t;
        if (v1 >= 4097) v1 = 4096;
        t = func_80068634(v1 / 8);
        p->x1C = p->x1C - (t / 4);
        ctx->f44 = ctx->f44 + 1536;
        goto L_2FC;
    }
    ctx->sub.x1C = v1 / 3;
L_2FC:
    ctx->f64 = ctx->f64 - 6;
    goto L_414;

L_310:
    if (a1 <= 0) goto L_3E0;
    a0v = 1;
    if (ctx->fB8 != 0) a0v = 2;
    v1 = ctx->sub.x1C;
    ctx->sub.x40 = a0v;
    if (v1 < 0) {
        ctx->sub.x1C = 0;
        ctx->f44 = 0;
        goto L_3CC;
    }
    if (v1 < (a1 + 256)) {
        s32 t;
        s32 c = v1;
        if (v1 < -4096) { v1 = -4096; c = v1; }
        t = func_80068634(c / 8);
        p->x1C = p->x1C + (t / 4);
        ctx->f44 = ctx->f44 - 1536;
        goto L_3CC;
    }
    ctx->sub.x1C = v1 / 3;
L_3CC:
    ctx->f64 = ctx->f64 + 6;
    goto L_414;

L_3E0:
    ctx->sub.x40 = 0;
    ctx->f44 = ctx->f44 / 2;
    ctx->sub.x1C = ctx->sub.x1C / 6;

L_414:
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

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/car/GameInitPlayerCar", func_8002D398);
