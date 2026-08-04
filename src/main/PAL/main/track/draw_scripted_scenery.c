#include "common.h"
#include "game/vector.h"
#include "game/race.h"

void UpdateFlybyScenery(void) asm("func_8003E590");
void DrawFlybyScenery(void) asm("func_8003EAF4");
void UpdateRouteScenery(void) asm("func_8003EC98");
void DrawRouteScenery(void) asm("func_8003F02C");
void UpdatePathScenerySound(void) asm("func_8003F9C4");
void DrawPathScenery(void) asm("func_80040730");

void DrawScriptedScenery(s32 arg0) asm("func_8003F608");
void DrawScriptedScenery(s32 arg0) {
    switch (g_GrandPrixClass % 5) {
    case 0:
        if (arg0 != 0) {
            UpdateRouteScenery();
        }
        DrawRouteScenery();
        break;
    case 1:
    case 2:
        if (arg0 != 0) {
            UpdateRouteScenery();
            UpdateFlybyScenery();
        }
        DrawRouteScenery();
        DrawFlybyScenery();
        break;
    case 3:
    case 4:
        if (arg0 != 0) {
            UpdateRouteScenery();
            UpdateFlybyScenery();
            UpdatePathScenerySound();
        }
        DrawRouteScenery();
        DrawFlybyScenery();
        DrawPathScenery();
        break;
    case 5:
        break;
    }
}

/*
 * Frame counters and rate signs of the two keyframe tracks. UpdatePathScenerySound
 * compares posFrame against g_PathSceneryPosSpan and rotFrame against
 * g_PathSceneryRotSpan to decide when to step to the next keyframe;
 * posRateNeg / rotRateNeg record that the keyframe's rate field was negative
 * (the rate itself is stored as a magnitude).
 */
typedef struct PathSceneryClock {
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
/* The three words are the scenery position: g_PathSceneryY and
 * g_PathSceneryZ alias +4 and +8. It stays a Block16 because respelling
 * w[0] as a named member of a Vec4 does not match. */
extern Block16 g_PathSceneryX asm("D_801E4DB8");
extern SVec g_PathSceneryRot asm("D_801E4DC8");
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

/* Unsigned views used when the two tracks advance their counters. */
extern u16 g_PathSceneryPosPhaseU asm("D_801E4DE0");
extern u16 g_PathSceneryRotPhaseU asm("D_801E4DE2");
extern s16 g_PathSceneryPosSpanS asm("D_801E4DE4");
extern s16 g_PathSceneryRotSpanS asm("D_801E4DE6");
extern u16 g_PathSceneryPosIndexU asm("D_801E4DEC");
extern u16 g_PathSceneryRotIndexU asm("D_801E4DEE");
extern u16 g_PathSceneryRotFrameU asm("D_801E4DB2");

extern s32 g_PathSceneryY asm("D_801E4DBC");
extern s32 g_PathSceneryZ asm("D_801E4DC0");
extern s16 g_PathSceneryRotY asm("D_801E4DCA");
extern s16 g_PathSceneryRotZ asm("D_801E4DCC");
extern s32 g_PlayerCarX asm("D_8009E6D4");
extern s32 g_PlayerCarY asm("D_8009E6D8");
extern s32 g_PlayerCarZ asm("D_8009E6DC");
extern s16 g_PathSceneryRacePhase asm("D_801E6E74");

/*
 * The position and rotation cursors are the same interleaved eight-halfword
 * object viewed from offsets zero and two.
 */
typedef struct PathSceneryCursor {
    s16 phase;
    s16 otherPhase;
    u16 span;
    u16 otherSpan;
    u16 rate;
    u16 otherRate;
    s16 index;
    s16 otherIndex;
} PathSceneryCursor;

extern PathSceneryCursor g_PathSceneryPosCursor asm("D_801E4DE0");
extern PathSceneryCursor g_PathSceneryRotCursor asm("D_801E4DE2");

s32 rsin(s32 angle) asm("func_80068568");
s32 rcos(s32 angle) asm("func_80068634");
s32 SquareRoot12(s32 value) asm("func_8006888C");
void SetPitchedSoundCue(s32 cue, s32 pitch, s32 volume) asm("func_8005C914");

void InitPathScenery(void) asm("func_8003F700");
void InitPathScenery(void) {
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
    g_PathSceneryX = *(Block16 *)g_PathSceneryPosKeys;

    {
        u8 *copySrc;
        u8 *entryA;
        u8 *entryB;
        s16 sv;

        copySrc = g_PathSceneryRotKeys;
        entryA = g_PathSceneryPosKeys;
        g_PathSceneryRot = *(SVec *)copySrc;
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
 * keyframe tracks (position at g_PathSceneryPosKeys, rotation at
 * g_PathSceneryRotKeys) are eased sinusoidally between waypoints. The sound is
 * cue zero with distance attenuation and a Doppler-like pitch shift.
 */
void UpdatePathScenerySound(void) asm("func_8003F9C4");
void UpdatePathScenerySound(void) {
    s32 delta[3];
    u8 *keys;
    register u8 *rec asm("$4");
    s16 rate;
    register s16 idx asm("$2");
    s32 dx;
    s32 dy;
    s32 dz;
    s32 vol;
    s32 pitch;
    register s32 slew;
    register u16 *frames asm("$5");
    register u16 posFrame;
    register u16 rotFrame;
    u8 *sinRec;
    s32 product;
    register s32 oldVolume;
    u8 *stepRec;

    if (g_PathSceneryClock.posFrame == g_PathSceneryPosSpanS) {
        idx = g_PathSceneryPosIndexU;
        keys = g_PathSceneryPosKeys;
        g_PathSceneryPosPhaseU = 0;
        idx = idx + 1;
        g_PathSceneryPosIndexU = idx;
        /* Preserve the original address-calculation allocation. */
        __asm__ volatile("" : "=r"(idx) : "0"(idx));
        stepRec = (u8 *)(idx * 20 + (s32)keys);
        if (*(s16 *)(stepRec + 0x10) == -1) {
            idx = *(u16 *)(stepRec + 0xC);
            g_PathSceneryClock.posFrame = 0;
            g_PathSceneryPosIndexU = idx;
            if (idx > 0) {
                g_PathSceneryClock.posFrame = *(u16 *)(idx * 20 + keys - 4);
            }
        }
        rate = *(s16 *)(g_PathSceneryPosKeys + g_PathSceneryPosIndex * 20 + 0x12);
        if (rate < 0) {
            rate = -rate;
            g_PathSceneryPosRate = rate;
            g_PathSceneryClock.posRateNeg = 1;
        } else {
            if (rate == 0) {
                rate = 1;
            }
            g_PathSceneryPosRate = rate;
            g_PathSceneryClock.posRateNeg = 0;
        }
        rec = g_PathSceneryPosKeys + g_PathSceneryPosIndex * 20;
        g_PathSceneryPosSpan = *(u16 *)(rec + 0x10);
        g_PathSceneryHalfDelta[0] =
            (*(s32 *)(rec + 0x14) - *(s32 *)(rec + 0x0)) / 2;
        g_PathSceneryHalfDelta[1] =
            (*(s32 *)(rec + 0x18) - *(s32 *)(rec + 0x4)) / 2;
        g_PathSceneryHalfDelta[2] =
            (*(s32 *)(rec + 0x1C) - *(s32 *)(rec + 0x8)) / 2;
    } else {
        g_PathSceneryPosPhaseU = g_PathSceneryPosPhaseU + 1;
    }

    if (g_PathSceneryPosCursor.phase <= (s16)g_PathSceneryPosCursor.rate) {
        if (g_PathSceneryPosCursor.phase <=
            (s16)g_PathSceneryPosCursor.rate / 2) {
            g_PathSceneryX.w[0] =
                *(s32 *)(g_PathSceneryPosKeys +
                         g_PathSceneryPosCursor.index * 20 + 0x14) -
                g_PathSceneryHalfDelta[0] *
                    rcos((s32)(g_PathSceneryPosCursor.phase << 11) /
                         (s16)g_PathSceneryPosCursor.rate) /
                    4096 -
                g_PathSceneryHalfDelta[0];
            g_PathSceneryY =
                *(s32 *)(g_PathSceneryPosKeys +
                         g_PathSceneryPosCursor.index * 20 + 0x18) -
                g_PathSceneryHalfDelta[1] *
                    rcos((s32)(g_PathSceneryPosCursor.phase << 11) /
                         (s16)g_PathSceneryPosCursor.rate) /
                    4096 -
                g_PathSceneryHalfDelta[1];
            g_PathSceneryZ =
                *(s32 *)(g_PathSceneryPosKeys +
                         g_PathSceneryPosCursor.index * 20 + 0x1C) -
                g_PathSceneryHalfDelta[2] *
                    rcos((s32)(g_PathSceneryPosCursor.phase << 11) /
                         (s16)g_PathSceneryPosCursor.rate) /
                    4096 -
                g_PathSceneryHalfDelta[2];
        } else {
            product =
                g_PathSceneryHalfDelta[0] *
                rsin((s32)(g_PathSceneryPosCursor.phase << 11) /
                         (s16)g_PathSceneryPosCursor.rate -
                     0x400);
            sinRec = g_PathSceneryPosKeys +
                     g_PathSceneryPosCursor.index * 20;
            if (product < 0) {
                product += 0xFFF;
            }
            g_PathSceneryX.w[0] =
                (product >> 12) + *(s32 *)(sinRec + 0x0) +
                g_PathSceneryHalfDelta[0];

            product =
                g_PathSceneryHalfDelta[1] *
                rsin((s32)(g_PathSceneryPosCursor.phase << 11) /
                         (s16)g_PathSceneryPosCursor.rate -
                     0x400);
            sinRec = g_PathSceneryPosKeys +
                     g_PathSceneryPosCursor.index * 20;
            if (product < 0) {
                product += 0xFFF;
            }
            g_PathSceneryY =
                (product >> 12) + *(s32 *)(sinRec + 0x4) +
                g_PathSceneryHalfDelta[1];

            g_PathSceneryZ =
                *(s32 *)(g_PathSceneryPosKeys +
                         g_PathSceneryPosCursor.index * 20 + 0x8) +
                g_PathSceneryHalfDelta[2] *
                    rsin((s32)(g_PathSceneryPosCursor.phase << 11) /
                             (s16)g_PathSceneryPosCursor.rate -
                         0x400) /
                    4096 +
                g_PathSceneryHalfDelta[2];
        }
    } else {
        g_PathSceneryX =
            *(Block16 *)(g_PathSceneryPosKeys +
                       g_PathSceneryPosCursor.index * 20 + 0x14);
    }

    if (g_PathSceneryClock.rotFrame == g_PathSceneryRotSpanS) {
        idx = g_PathSceneryRotIndexU;
        keys = g_PathSceneryRotKeys;
        g_PathSceneryRotPhaseU = 0;
        idx = idx + 1;
        g_PathSceneryRotIndexU = idx;
        /* Preserve the original address-calculation allocation. */
        __asm__ volatile("" : "=r"(idx) : "0"(idx));
        stepRec = (u8 *)(idx * 12 + (s32)keys);
        if (*(s16 *)(stepRec + 0x8) == -1) {
            idx = *(u16 *)(stepRec + 0x6);
            g_PathSceneryClock.rotFrame = 0;
            g_PathSceneryRotIndexU = idx;
            if (idx > 0) {
                g_PathSceneryClock.rotFrame = *(u16 *)(idx * 12 + keys - 4);
            }
        }
        rate = *(s16 *)(g_PathSceneryRotKeys + g_PathSceneryRotIndex * 12 + 0xA);
        if (rate < 0) {
            rate = -rate;
            g_PathSceneryRotRate = rate;
            g_PathSceneryClock.rotRateNeg = 1;
        } else {
            if (rate == 0) {
                rate = 1;
            }
            g_PathSceneryRotRate = rate;
            g_PathSceneryClock.rotRateNeg = 0;
        }
        rec = g_PathSceneryRotKeys + g_PathSceneryRotIndex * 12;
        g_PathSceneryRotRate = *(u16 *)(rec + 0xA);
        g_PathSceneryRotSpan = *(u16 *)(rec + 0x8);
        g_PathSceneryRotHalfDelta[0] =
            (*(s16 *)(rec + 0xC) - *(s16 *)(rec + 0x0)) / 2;
        g_PathSceneryRotHalfDelta[1] =
            (*(s16 *)(rec + 0xE) - *(s16 *)(rec + 0x2)) / 2;
        g_PathSceneryRotHalfDelta[2] =
            (*(s16 *)(rec + 0x10) - *(s16 *)(rec + 0x4)) / 2;
    } else {
        g_PathSceneryRotPhaseU = g_PathSceneryRotPhaseU + 1;
    }

    if (g_PathSceneryRotCursor.phase <= (s16)g_PathSceneryRotCursor.rate) {
        if (g_PathSceneryRotCursor.phase <=
            (s16)g_PathSceneryRotCursor.rate / 2) {
            g_PathSceneryRot.vx =
                *(u16 *)(g_PathSceneryRotKeys +
                         g_PathSceneryRotCursor.index * 12 + 0xC) -
                g_PathSceneryRotHalfDelta[0] *
                    rcos((s32)(g_PathSceneryRotCursor.phase << 11) /
                         (s16)g_PathSceneryRotCursor.rate) /
                    4096 -
                g_PathSceneryRotHalfDelta[0];
            g_PathSceneryRotY =
                *(u16 *)(g_PathSceneryRotKeys +
                         g_PathSceneryRotCursor.index * 12 + 0xE) -
                g_PathSceneryRotHalfDelta[1] *
                    rcos((s32)(g_PathSceneryRotCursor.phase << 11) /
                         (s16)g_PathSceneryRotCursor.rate) /
                    4096 -
                g_PathSceneryRotHalfDelta[1];
            g_PathSceneryRotZ =
                *(u16 *)(g_PathSceneryRotKeys +
                         g_PathSceneryRotCursor.index * 12 + 0x10) -
                g_PathSceneryRotHalfDelta[2] *
                    rcos((s32)(g_PathSceneryRotCursor.phase << 11) /
                         (s16)g_PathSceneryRotCursor.rate) /
                    4096 -
                g_PathSceneryRotHalfDelta[2];
        } else {
            g_PathSceneryRot.vx =
                *(u16 *)(g_PathSceneryRotKeys +
                         g_PathSceneryRotCursor.index * 12 + 0x0) +
                g_PathSceneryRotHalfDelta[0] *
                    rsin((s32)(g_PathSceneryRotCursor.phase << 11) /
                             (s16)g_PathSceneryRotCursor.rate -
                         0x400) /
                    4096 +
                g_PathSceneryRotHalfDelta[0];
            g_PathSceneryRotY =
                *(u16 *)(g_PathSceneryRotKeys +
                         g_PathSceneryRotCursor.index * 12 + 0x2) +
                g_PathSceneryRotHalfDelta[1] *
                    rsin((s32)(g_PathSceneryRotCursor.phase << 11) /
                             (s16)g_PathSceneryRotCursor.rate -
                         0x400) /
                    4096 +
                g_PathSceneryRotHalfDelta[1];
            g_PathSceneryRotZ =
                *(u16 *)(g_PathSceneryRotKeys +
                         g_PathSceneryRotCursor.index * 12 + 0x4) +
                g_PathSceneryRotHalfDelta[2] *
                    rsin((s32)(g_PathSceneryRotCursor.phase << 11) /
                             (s16)g_PathSceneryRotCursor.rate -
                         0x400) /
                    4096 +
                g_PathSceneryRotHalfDelta[2];
        }
    } else {
        g_PathSceneryRot =
            *(SVec *)(g_PathSceneryRotKeys +
                      g_PathSceneryRotCursor.index * 12 + 0xC);
    }

    /*
     * These pinned views and empty constraints reproduce the original
     * instruction schedule without generating code of their own.
     */
    frames = (u16 *)&g_PathSceneryClock;
    posFrame = frames[0];
    rotFrame = g_PathSceneryRotFrameU;
    __asm__ volatile("" : "=r"(rotFrame) : "0"(rotFrame));
    dx = g_PlayerCarX - g_PathSceneryX.w[0];
    posFrame = posFrame + 1;
    frames[0] = posFrame;
    g_PathSceneryRotFrameU = rotFrame + 1;
    delta[0] = dx;
    delta[1] = dy = g_PlayerCarY - g_PathSceneryY;
    delta[2] = dz = g_PlayerCarZ - g_PathSceneryZ;

    if (dx < 0x1000 && dz < 0x1000 && dx >= -0xFFF && dz >= -0xFFF) {
        slew =
            SquareRoot12(dx * dx / 4 + dy * dy / 8 + dz * dz / 4) >> 10;
        vol = 0x64 - slew;
        if (vol >= 0x65) {
            vol = 0x64;
        }
        if (vol < 0) {
            vol = 0;
        }
        oldVolume = g_PathSceneryVolume;
        slew = vol - oldVolume;
        if (slew < -0x14) {
            slew = -0x14;
        }
        if (slew >= 0x15) {
            slew = 0x14;
        }
        oldVolume = slew / 2 + 0x3C;
        g_PathSceneryVolume = vol;
        pitch = oldVolume << 7;
    } else {
        vol = 0;
        g_PathSceneryVolume = 0;
        pitch = 0;
    }

    if (g_PathSceneryRacePhase >= 3) {
        pitch = 0;
        vol = 0;
    }
    SetPitchedSoundCue(0, pitch, vol);
}
