#include "common.h"
#include "game/vector.h"
#include "game/race.h"
#include "game/audio.h"
#include "game/menu.h"
#include "game/track.h"
#include "psyq/gte.h"

void DrawScriptedScenery(s32 flags) {
    switch (g_GrandPrixClass % 5) {
    case 0:
        if (flags != 0) {
            UpdateRouteScenery();
        }
        DrawRouteScenery();
        break;
    case 1:
    case 2:
        if (flags != 0) {
            UpdateRouteScenery();
            UpdateFlybyScenery();
        }
        DrawRouteScenery();
        DrawFlybyScenery();
        break;
    case 3:
    case 4:
        if (flags != 0) {
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

extern u8 *g_PathSceneryPosData;
extern u8 *g_PathSceneryRotData;
extern PathSceneryClock g_PathSceneryClock;
/* The three words are the scenery position: g_PathSceneryY and
 * g_PathSceneryZ alias +4 and +8. It stays a Block16 because respelling
 * w[0] as a named member of a Vec4 does not match. */
extern Block16 g_PathSceneryX;
extern SVec g_PathSceneryRot;

extern s32 g_PlayerCar;

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

extern PathSceneryCursor g_PathSceneryPosCursor asm("g_PathSceneryPosPhase");
extern PathSceneryCursor g_PathSceneryRotCursor asm("g_PathSceneryRotPhase");


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

    if (g_PathSceneryClock.posFrame == g_PathSceneryPosSpan) {
        idx = (u16)g_PathSceneryPosIndex;
        keys = g_PathSceneryPosKeys;
        g_PathSceneryPosPhase = 0;
        idx = idx + 1;
        g_PathSceneryPosIndex = idx;
        /* Preserve the original address-calculation allocation. */
        __asm__ volatile("" : "=r"(idx) : "0"(idx));
        stepRec = (u8 *)(idx * 20 + (s32)keys);
        if (*(s16 *)(stepRec + 0x10) == -1) {
            idx = *(u16 *)(stepRec + 0xC);
            g_PathSceneryClock.posFrame = 0;
            g_PathSceneryPosIndex = idx;
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
        g_PathSceneryPosPhase = g_PathSceneryPosPhase + 1;
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

    if (g_PathSceneryClock.rotFrame == g_PathSceneryRotSpan) {
        idx = (u16)g_PathSceneryRotIndex;
        keys = g_PathSceneryRotKeys;
        g_PathSceneryRotPhase = 0;
        idx = idx + 1;
        g_PathSceneryRotIndex = idx;
        /* Preserve the original address-calculation allocation. */
        __asm__ volatile("" : "=r"(idx) : "0"(idx));
        stepRec = (u8 *)(idx * 12 + (s32)keys);
        if (*(s16 *)(stepRec + 0x8) == -1) {
            idx = *(u16 *)(stepRec + 0x6);
            g_PathSceneryClock.rotFrame = 0;
            g_PathSceneryRotIndex = idx;
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
        g_PathSceneryRotPhase = g_PathSceneryRotPhase + 1;
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
    dx = g_PlayerCar - g_PathSceneryX.w[0];
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

    if (g_RacePhase >= 3) {
        pitch = 0;
        vol = 0;
    }
    SetPitchedSoundCue(0, pitch, vol);
}
