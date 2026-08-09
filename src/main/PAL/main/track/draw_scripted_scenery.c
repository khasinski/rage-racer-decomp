#include "common.h"
#include "game/vector.h"
#include "game/race.h"
#include "game/audio.h"
#include "game/menu.h"
#include "game/track.h"
#include "game/track_internal.h"
#include "game/player_car_internal.h"
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
/* The position stays a Block16 because respelling w[0] as a named member of a
 * Vec4 does not match. */

/*
 * The position and rotation cursors are the same interleaved eight-halfword
 * object viewed from offsets zero and two.
 */
void InitPathScenery(void) {
    s32 lev;
    PathSceneryPositionData *tblA;
    PathSceneryRotationData *tblB;
    s32 ia;
    s32 ib;

    lev = g_RaceSeries;
    tblA = g_PathSceneryPosData;
    tblB = g_PathSceneryRotData;
    ia = RAW(tblA->firstKey[lev]);
    ib = RAW(tblB->firstKey[lev]);
    g_PathSceneryClock.rotFrame = 0;
    g_PathSceneryClock.posFrame = 0;
    g_PathSceneryPosKeys = (PathSceneryPositionKey *)tblA;
    g_PathSceneryPosKeys =
        &((PathSceneryPositionData *)g_PathSceneryPosKeys)->keys[ia];
    g_PathSceneryRotKeys = (PathSceneryRotationKey *)tblB;
    g_PathSceneryRotKeys =
        &((PathSceneryRotationData *)g_PathSceneryRotKeys)->keys[ib];
    g_PathSceneryTransform.position = g_PathSceneryPosKeys->position;

    {
        PathSceneryRotationKey *copySrc;
        PathSceneryPositionKey *entryA;
        PathSceneryRotationKey *entryB;
        s16 sv;

        copySrc = g_PathSceneryRotKeys;
        entryA = g_PathSceneryPosKeys;
        g_PathSceneryTransform.rotation = copySrc->rotation;
        g_PathSceneryPosPhase = 0;
        g_PathSceneryRotPhase = 0;
        g_PathSceneryPosSpan =
            RAW(entryA->fields.span);
        entryB = g_PathSceneryRotKeys;
        g_PathSceneryRotSpan =
            RAW(entryB->fields.span);
        g_PathSceneryPosRate =
            RAW(entryA->fields.rate);
        g_PathSceneryRotRate =
            RAW(entryB->fields.rate);

        sv = RAW(entryA->fields.rate);
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
        PathSceneryRotationKey *entryB;
        s16 sv;

        entryB = g_PathSceneryRotKeys;
        sv = RAW(entryB->fields.rate);
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
        PathSceneryPositionKey *positionKeys;
        PathSceneryRotationKey *rotationKeys;

        positionKeys = g_PathSceneryPosKeys;
        g_PathSceneryVolume = 0;
        g_PathSceneryPosIndex = 0;
        g_PathSceneryRotIndex = 0;
        g_PathSceneryHalfDelta[0] =
            (RAW(positionKeys[1].fields.x) - RAW(positionKeys[0].fields.x)) / 2;
        g_PathSceneryHalfDelta[1] =
            (RAW(positionKeys[1].fields.y) - RAW(positionKeys[0].fields.y)) / 2;
        g_PathSceneryHalfDelta[2] =
            (RAW(positionKeys[1].fields.z) - RAW(positionKeys[0].fields.z)) / 2;
        rotationKeys = g_PathSceneryRotKeys;
        g_PathSceneryRotHalfDelta[0] =
            (RAW(rotationKeys[1].fields.x) - RAW(rotationKeys[0].fields.x)) / 2;
        g_PathSceneryRotHalfDelta[1] =
            (RAW(rotationKeys[1].fields.y) - RAW(rotationKeys[0].fields.y)) / 2;
        g_PathSceneryRotHalfDelta[2] =
            (RAW(rotationKeys[1].fields.z) - RAW(rotationKeys[0].fields.z)) / 2;
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
    PathSceneryPositionKey *positionKeys;
    PathSceneryRotationKey *rotationKeys;
    register PathSceneryKey *rec asm("$4");
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
    PathSceneryPositionKey *sinRec;
    s32 product;
    register s32 oldVolume;
    PathSceneryKey *stepRec;
    PathSceneryKeyAddress positionStepAddress;
    PathSceneryKeyAddress rotationStepAddress;

    if (g_PathSceneryClock.posFrame == g_PathSceneryPosSpan) {
        idx = (u16)g_PathSceneryPosIndex;
        positionKeys = g_PathSceneryPosKeys;
        g_PathSceneryPosPhase = 0;
        idx = idx + 1;
        g_PathSceneryPosIndex = idx;
        __asm__ volatile("" : "=r"(idx) : "0"(idx));
        positionStepAddress.positionPointer = positionKeys;
        positionStepAddress.byteOffset =
            idx * sizeof(PathSceneryPositionKey) + positionStepAddress.byteOffset;
        stepRec = positionStepAddress.pointer;
        if (stepRec->position.fields.span == -1) {
            idx = stepRec->position.fields.loopIndex;
            g_PathSceneryClock.posFrame = 0;
            g_PathSceneryPosIndex = idx;
            if (idx > 0) {
                g_PathSceneryClock.posFrame =
                    RAW(positionKeys[idx - 1].fields.span);
            }
        }
        rate = RAW(g_PathSceneryPosKeys[g_PathSceneryPosIndex].fields.rate);
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
        rec = (PathSceneryKey *)&g_PathSceneryPosKeys[g_PathSceneryPosIndex];
        g_PathSceneryPosSpan =
            RAW(rec->position.fields.span);
        g_PathSceneryHalfDelta[0] =
            (RAW(rec[1].position.fields.x) - RAW(rec[0].position.fields.x)) / 2;
        g_PathSceneryHalfDelta[1] =
            (RAW(rec[1].position.fields.y) - RAW(rec[0].position.fields.y)) / 2;
        g_PathSceneryHalfDelta[2] =
            (RAW(rec[1].position.fields.z) - RAW(rec[0].position.fields.z)) / 2;
    } else {
        g_PathSceneryPosPhase = g_PathSceneryPosPhase + 1;
    }

    if (g_PathSceneryPosCursor.phase <= (s16)g_PathSceneryPosCursor.rate) {
        if (g_PathSceneryPosCursor.phase <=
            (s16)g_PathSceneryPosCursor.rate / 2) {
            g_PathSceneryTransform.position.w[0] =
                RAW(g_PathSceneryPosKeys[g_PathSceneryPosCursor.index + 1].fields.x) -
                g_PathSceneryHalfDelta[0] *
                    rcos((g_PathSceneryPosCursor.phase << 11) /
                         (s16)g_PathSceneryPosCursor.rate) /
                    4096 -
                g_PathSceneryHalfDelta[0];
            g_PathSceneryTransform.position.w[1] =
                RAW(g_PathSceneryPosKeys[g_PathSceneryPosCursor.index + 1].fields.y) -
                g_PathSceneryHalfDelta[1] *
                    rcos((g_PathSceneryPosCursor.phase << 11) /
                         (s16)g_PathSceneryPosCursor.rate) /
                    4096 -
                g_PathSceneryHalfDelta[1];
            g_PathSceneryTransform.position.w[2] =
                RAW(g_PathSceneryPosKeys[g_PathSceneryPosCursor.index + 1].fields.z) -
                g_PathSceneryHalfDelta[2] *
                    rcos((g_PathSceneryPosCursor.phase << 11) /
                         (s16)g_PathSceneryPosCursor.rate) /
                    4096 -
                g_PathSceneryHalfDelta[2];
        } else {
            product =
                g_PathSceneryHalfDelta[0] *
                rsin((g_PathSceneryPosCursor.phase << 11) /
                         (s16)g_PathSceneryPosCursor.rate -
                     0x400);
            sinRec = &g_PathSceneryPosKeys[g_PathSceneryPosCursor.index];
            if (product < 0) {
                product += 0xFFF;
            }
            g_PathSceneryTransform.position.w[0] =
                (product >> 12) + sinRec->fields.x +
                g_PathSceneryHalfDelta[0];

            product =
                g_PathSceneryHalfDelta[1] *
                rsin((g_PathSceneryPosCursor.phase << 11) /
                         (s16)g_PathSceneryPosCursor.rate -
                     0x400);
            sinRec = &g_PathSceneryPosKeys[g_PathSceneryPosCursor.index];
            if (product < 0) {
                product += 0xFFF;
            }
            g_PathSceneryTransform.position.w[1] =
                (product >> 12) + sinRec->fields.y +
                g_PathSceneryHalfDelta[1];

            g_PathSceneryTransform.position.w[2] =
                RAW(g_PathSceneryPosKeys[g_PathSceneryPosCursor.index].fields.z) +
                g_PathSceneryHalfDelta[2] *
                    rsin((g_PathSceneryPosCursor.phase << 11) /
                             (s16)g_PathSceneryPosCursor.rate -
                         0x400) /
                    4096 +
                g_PathSceneryHalfDelta[2];
        }
    } else {
        g_PathSceneryTransform.position =
            g_PathSceneryPosKeys[g_PathSceneryPosCursor.index + 1].position;
    }

    if (g_PathSceneryClock.rotFrame == g_PathSceneryRotSpan) {
        idx = (u16)g_PathSceneryRotIndex;
        rotationKeys = g_PathSceneryRotKeys;
        g_PathSceneryRotPhase = 0;
        idx = idx + 1;
        g_PathSceneryRotIndex = idx;
        __asm__ volatile("" : "=r"(idx) : "0"(idx));
        rotationStepAddress.rotationPointer = rotationKeys;
        rotationStepAddress.byteOffset =
            idx * sizeof(PathSceneryRotationKey) + rotationStepAddress.byteOffset;
        stepRec = rotationStepAddress.pointer;
        if (stepRec->rotation.fields.span == -1) {
            idx = stepRec->rotation.fields.loopIndex;
            g_PathSceneryClock.rotFrame = 0;
            g_PathSceneryRotIndex = idx;
            if (idx > 0) {
                g_PathSceneryClock.rotFrame =
                    RAW(rotationKeys[idx - 1].fields.span);
            }
        }
        rate = RAW(g_PathSceneryRotKeys[g_PathSceneryRotIndex].fields.rate);
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
        rec = (PathSceneryKey *)&g_PathSceneryRotKeys[g_PathSceneryRotIndex];
        g_PathSceneryRotRate =
            RAW(rec->rotation.fields.rate);
        g_PathSceneryRotSpan =
            RAW(rec->rotation.fields.span);
        g_PathSceneryRotHalfDelta[0] =
            (RAW(((PathSceneryRotationKey *)rec)[1].fields.x) -
             RAW(rec[0].rotation.fields.x)) / 2;
        g_PathSceneryRotHalfDelta[1] =
            (RAW(((PathSceneryRotationKey *)rec)[1].fields.y) -
             RAW(rec[0].rotation.fields.y)) / 2;
        g_PathSceneryRotHalfDelta[2] =
            (RAW(((PathSceneryRotationKey *)rec)[1].fields.z) -
             RAW(rec[0].rotation.fields.z)) / 2;
    } else {
        g_PathSceneryRotPhase = g_PathSceneryRotPhase + 1;
    }

    if (g_PathSceneryRotCursor.phase <= (s16)g_PathSceneryRotCursor.rate) {
        if (g_PathSceneryRotCursor.phase <=
            (s16)g_PathSceneryRotCursor.rate / 2) {
            g_PathSceneryTransform.rotation.vx =
                RAW(g_PathSceneryRotKeys[g_PathSceneryRotCursor.index + 1].fields.x) -
                g_PathSceneryRotHalfDelta[0] *
                    rcos((g_PathSceneryRotCursor.phase << 11) /
                         (s16)g_PathSceneryRotCursor.rate) /
                    4096 -
                g_PathSceneryRotHalfDelta[0];
            g_PathSceneryTransform.rotation.vy =
                RAW(g_PathSceneryRotKeys[g_PathSceneryRotCursor.index + 1].fields.y) -
                g_PathSceneryRotHalfDelta[1] *
                    rcos((g_PathSceneryRotCursor.phase << 11) /
                         (s16)g_PathSceneryRotCursor.rate) /
                    4096 -
                g_PathSceneryRotHalfDelta[1];
            g_PathSceneryTransform.rotation.vz =
                RAW(g_PathSceneryRotKeys[g_PathSceneryRotCursor.index + 1].fields.z) -
                g_PathSceneryRotHalfDelta[2] *
                    rcos((g_PathSceneryRotCursor.phase << 11) /
                         (s16)g_PathSceneryRotCursor.rate) /
                    4096 -
                g_PathSceneryRotHalfDelta[2];
        } else {
            g_PathSceneryTransform.rotation.vx =
                RAW(g_PathSceneryRotKeys[g_PathSceneryRotCursor.index].fields.x) +
                g_PathSceneryRotHalfDelta[0] *
                    rsin((g_PathSceneryRotCursor.phase << 11) /
                             (s16)g_PathSceneryRotCursor.rate -
                         0x400) /
                    4096 +
                g_PathSceneryRotHalfDelta[0];
            g_PathSceneryTransform.rotation.vy =
                RAW(g_PathSceneryRotKeys[g_PathSceneryRotCursor.index].fields.y) +
                g_PathSceneryRotHalfDelta[1] *
                    rsin((g_PathSceneryRotCursor.phase << 11) /
                             (s16)g_PathSceneryRotCursor.rate -
                         0x400) /
                    4096 +
                g_PathSceneryRotHalfDelta[1];
            g_PathSceneryTransform.rotation.vz =
                RAW(g_PathSceneryRotKeys[g_PathSceneryRotCursor.index].fields.z) +
                g_PathSceneryRotHalfDelta[2] *
                    rsin((g_PathSceneryRotCursor.phase << 11) /
                             (s16)g_PathSceneryRotCursor.rate -
                         0x400) /
                    4096 +
                g_PathSceneryRotHalfDelta[2];
        }
    } else {
        g_PathSceneryTransform.rotation =
            g_PathSceneryRotKeys[g_PathSceneryRotCursor.index + 1].rotation;
    }

    /*
     * These pinned views and empty constraints reproduce the original
     * instruction schedule without generating code of their own.
     */
    frames = (u16 *)&g_PathSceneryClock;
    posFrame = frames[0];
    rotFrame = frames[1];
    __asm__ volatile("" : "=r"(rotFrame) : "0"(rotFrame));
    dx = g_PlayerCar.x - g_PathSceneryTransform.position.w[0];
    posFrame = posFrame + 1;
    frames[0] = posFrame;
    frames[1] = rotFrame + 1;
    delta[0] = dx;
    delta[1] = dy = g_PlayerCar.y - g_PathSceneryTransform.position.w[1];
    delta[2] = dz = g_PlayerCar.z - g_PathSceneryTransform.position.w[2];

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
