#include "common.h"
#include "game/race.h"

void func_8003E590(void);
void GameDrawFlybyScenery(void) asm("func_8003EAF4");
void GameUpdateRouteScenery(void) asm("func_8003EC98");
void GameDrawRouteScenery(void) asm("func_8003F02C");
void func_8003F9C4(void);
void GameDrawPathScenery(void) asm("func_80040730");

void GameDrawScriptedScenery(s32 arg0) asm("func_8003F608");
void GameDrawScriptedScenery(s32 arg0) {
    switch (g_GrandPrixClass % 5) {
    case 0:
        if (arg0 != 0) {
            GameUpdateRouteScenery();
        }
        GameDrawRouteScenery();
        break;
    case 1:
    case 2:
        if (arg0 != 0) {
            GameUpdateRouteScenery();
            func_8003E590();
        }
        GameDrawRouteScenery();
        GameDrawFlybyScenery();
        break;
    case 3:
    case 4:
        if (arg0 != 0) {
            GameUpdateRouteScenery();
            func_8003E590();
            func_8003F9C4();
        }
        GameDrawRouteScenery();
        GameDrawFlybyScenery();
        GameDrawPathScenery();
        break;
    case 5:
        break;
    }
}

typedef struct {
    s32 w[4];
} Blk16;

typedef struct {
    s16 h[4];
} Blk8;

/*
 * Frame counters and rate signs of the two keyframe tracks. func_8003F9C4
 * compares posFrame against g_PathSceneryPosSpan and rotFrame against
 * g_PathSceneryRotSpan to decide when to step to the next keyframe;
 * posRateNeg / rotRateNeg record that the keyframe's rate field was negative
 * (the rate itself is stored as a magnitude).
 */
typedef struct {
    s16 posFrame;
    s16 rotFrame;
    s16 posRateNeg;
    s16 rotRateNeg;
} PathSceneryClock;

extern u8 *g_PathSceneryPosData asm("D_801E4BA4");
extern u8 *g_PathSceneryRotData asm("D_801E4174");
/*
 * The looping prop's two keyframe tracks for the running series: 20-byte
 * position records { s32 x, y, z; u16 loopTo @0x0C; u16 span @0x10;
 * s16 rate @0x12 } and 12-byte rotation records { s16 x, y, z; ...;
 * u16 span @0x08; s16 rate @0x0A }. `Data` is the whole table (a per-series
 * s16 offset header), `Keys` the record array this series starts at, and
 * g_PathSceneryPosIndex / …RotIndex the record currently being eased out of.
 * A span field of -1 terminates and sends the index back to `loopTo`.
 */
extern u8 *g_PathSceneryPosKeys asm("D_801E42DC");
extern u8 *g_PathSceneryRotKeys asm("D_801E42E8");
extern PathSceneryClock g_PathSceneryClock asm("D_801E4DB0");
extern Blk16 g_PathSceneryX asm("D_801E4DB8");
extern Blk8 g_PathSceneryRot asm("D_801E4DC8");
/* The two tracks' cursors, interleaved pos/rot at 0x801E4DE0: phase, span, rate
 * and index are each a two-halfword pair with position first. Left as eight
 * scalars because nothing in the image indexes them by track. Phase is the
 * cosine-ease argument, 0..0x1000. */
extern s16 g_PathSceneryPosPhase asm("D_801E4DE0");
extern s16 g_PathSceneryRotPhase asm("D_801E4DE2");
extern u16 g_PathSceneryPosSpan asm("D_801E4DE4");
extern u16 g_PathSceneryRotSpan asm("D_801E4DE6");
extern u16 g_PathSceneryPosRate asm("D_801E4DE8");
extern u16 g_PathSceneryRotRate asm("D_801E4DEA");
extern s16 g_PathSceneryPosIndex asm("D_801E4DEC");
extern s16 g_PathSceneryRotIndex asm("D_801E4DEE");
/* Slew state of the prop's positional sound: func_80040590 turns the distance
 * from g_PlayerCar to g_PathSceneryX into 100 - dist/1024, clamps it to
 * [0, 100] and walks this value at most 20 a frame towards it. */
extern s32 g_PathSceneryVolume asm("D_801E4DF0");
/* Half of (next keyframe - this keyframe), i.e. the amplitude of the cosine
 * ease that carries the prop from one keyframe to the next. Two xyz triples,
 * each padded to 8 bytes: the rotation one at 0x801E4DD0, the position one at
 * 0x801E4DD8. */
extern s16 g_PathSceneryRotHalfDelta[3] asm("D_801E4DD0");
extern s16 g_PathSceneryHalfDelta[3] asm("D_801E4DD8");

void GameInitPathScenery(void) asm("func_8003F700");
void GameInitPathScenery(void) {
    s32 lev;
    u8 *tblA;
    u8 *tblB;
    s32 ia;
    s32 ib;

    lev = g_RaceSeries;
    tblA = g_PathSceneryPosData;
    tblB = g_PathSceneryRotData;
    ia = *(s16 *)(tblA + (lev * 2));
    ib = *(s16 *)(tblB + (lev * 2));
    g_PathSceneryClock.rotFrame = 0;
    g_PathSceneryClock.posFrame = 0;
    g_PathSceneryPosKeys = tblA;
    g_PathSceneryPosKeys = g_PathSceneryPosKeys + ((ia * 20) + 4);
    g_PathSceneryRotKeys = tblB;
    g_PathSceneryRotKeys = g_PathSceneryRotKeys + ((ib * 12) + 4);
    g_PathSceneryX = *(Blk16 *)g_PathSceneryPosKeys;

    {
        u8 *copySrc;
        u8 *entryA;
        u8 *entryB;
        s16 sv;

        copySrc = g_PathSceneryRotKeys;
        entryA = g_PathSceneryPosKeys;
        g_PathSceneryRot = *(Blk8 *)copySrc;
        g_PathSceneryPosPhase = 0;
        g_PathSceneryRotPhase = 0;
        g_PathSceneryPosSpan = *(u16 *)(entryA + 0x10);
        entryB = g_PathSceneryRotKeys;
        g_PathSceneryRotSpan = *(u16 *)(entryB + 0x8);
        g_PathSceneryPosRate = *(u16 *)(entryA + 0x12);
        g_PathSceneryRotRate = *(u16 *)(entryB + 0xA);

        sv = *(s16 *)(entryA + 0x12);
        if (sv < 0) {
            sv = -sv;
            g_PathSceneryPosRate = sv;
            g_PathSceneryClock.posRateNeg = 1;
        } else {
            if (sv == 0) {
                sv = 1;
            }
            g_PathSceneryPosRate = sv;
            g_PathSceneryClock.posRateNeg = 0;
        }
    }

    {
        u8 *entryB;
        s16 sv;

        entryB = g_PathSceneryRotKeys;
        sv = *(s16 *)(entryB + 0xA);
        if (sv < 0) {
            sv = -sv;
            g_PathSceneryRotRate = sv;
            g_PathSceneryClock.rotRateNeg = 1;
        } else {
            if (sv == 0) {
                sv = 1;
            }
            g_PathSceneryRotRate = sv;
            g_PathSceneryClock.rotRateNeg = 0;
        }
    }

    {
        u8 *entry;

        entry = g_PathSceneryPosKeys;
        g_PathSceneryVolume = 0;
        g_PathSceneryPosIndex = 0;
        g_PathSceneryRotIndex = 0;
        g_PathSceneryHalfDelta[0] = (*(s32 *)(entry + 0x14) - *(s32 *)(entry + 0x0)) / 2;
        g_PathSceneryHalfDelta[1] = (*(s32 *)(entry + 0x18) - *(s32 *)(entry + 0x4)) / 2;
        g_PathSceneryHalfDelta[2] = (*(s32 *)(entry + 0x1C) - *(s32 *)(entry + 0x8)) / 2;
        entry = g_PathSceneryRotKeys;
        g_PathSceneryRotHalfDelta[0] = (*(s16 *)(entry + 0xC) - *(s16 *)(entry + 0x0)) / 2;
        g_PathSceneryRotHalfDelta[1] = (*(s16 *)(entry + 0xE) - *(s16 *)(entry + 0x2)) / 2;
        g_PathSceneryRotHalfDelta[2] = (*(s16 *)(entry + 0x10) - *(s16 *)(entry + 0x4)) / 2;
    }
}

/*
 * Ticks the course's permanently looping prop and its positional sound. Two
 * keyframe tracks (position at g_PathSceneryPosKeys, rotation at g_PathSceneryRotKeys) eased
 * sinusoidally between waypoints; the sound is GameSetPitchedSoundCue cue 0 with
 * a Doppler-approximating pitch. See docs/names.md 1.
 */
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/track/GameDrawScriptedScenery", func_8003F9C4);
