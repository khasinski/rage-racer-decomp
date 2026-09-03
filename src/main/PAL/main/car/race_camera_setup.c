#include "common.h"
#include "game/asset.h"
#include "game/car.h"
#include "game/car_internal.h"
#include "game/race.h"
#include "game/render.h"
#include "game/render_internal.h"
#include "game/scratchpad.h"
#include "game/state.h"
#include "game/track.h"
#include "game/vector.h"
#include "psyq/gte.h"





void RunRaceIntroCamera(PlayerCarRuntime *car, s32 mode) {
    PlayerCarPositionView target;
    s32 *spad = &SCRATCH_PRIM_CURSOR_WORD;
    register s32 s0v asm("$16");
    s32 delta[3];

    target.car = car;
    __asm__("" : "=r"(s0v) : "0"(28), "r"(spad));
    if (mode < 90) {
        if (mode < 2) {
            RaceIntroCameraScript *script = g_RaceIntroCameraScript;
            s16 n = script->firstKeyIndex[ReadStableRaceSeries()];
            s32 off = n * sizeof(RaceIntroCameraKey) + sizeof(script->firstKeyIndex);
            RaceIntroCameraScriptAddress keyAddress;
            RaceIntroCameraKey *p;
            RaceIntroCameraKey *q;
            keyAddress.scriptPointer = script;
            keyAddress.value = off + keyAddress.value;
            p = keyAddress.keyPointer;
            g_RaceIntroCameraCursor = p;
            *SCRATCH_VIEW_POSITION_BLOCK = *keyAddress.vectorPointer;
            q = g_RaceIntroCameraCursor;
            g_RaceIntroCameraDelta.vx = -q[0].x.half.value + q[1].x.half.value;
            g_RaceIntroCameraDelta.vy = -q[0].y.half.value + q[1].y.half.value;
            g_RaceIntroCameraDelta.vz = -q[0].z.half.value + q[1].z.half.value;
            g_RaceIntroCameraTimer = q[0].duration;
        } else {
            RaceIntroCameraKey *a = g_RaceIntroCameraCursor;
            if (mode == a->startFrame) {
                g_RaceIntroCameraCursor = &a[1];
                g_RaceIntroCameraTimer = a[1].duration;
                if (a[1].mode == 1) {
                    g_RaceIntroCameraDelta.vx = -a[1].x.half.value + target.position->x.half.low;
                    g_RaceIntroCameraDelta.vy = -a[1].y.half.value - 28 + target.position->y.half.low;
                    g_RaceIntroCameraDelta.vz = -a[1].z.half.value + target.position->z.half.low;
                } else {
                    g_RaceIntroCameraDelta.vx = -a[1].x.half.value + a[2].x.half.value;
                    g_RaceIntroCameraDelta.vy = -a[1].y.half.value + a[2].y.half.value;
                    g_RaceIntroCameraDelta.vz = -a[1].z.half.value + a[2].z.half.value;
                }
            }
        }

        g_RaceIntroCameraTimer = g_RaceIntroCameraTimer - 1;
        if (g_RaceIntroCameraTimer <= 0) {
            g_RaceIntroCameraTimer = 0;
        }

        if (g_RaceIntroCameraCursor->mode == 0) {
            spad[2] = g_RaceIntroCameraCursor->x.word
                      + (g_RaceIntroCameraDelta.vx * rcos((g_RaceIntroCameraTimer << 10) / g_RaceIntroCameraCursor->duration)) / 4096;
            spad[3] = g_RaceIntroCameraCursor->y.word
                      + (g_RaceIntroCameraDelta.vy * rcos((g_RaceIntroCameraTimer << 10) / g_RaceIntroCameraCursor->duration)) / 4096;
            spad[4] = g_RaceIntroCameraCursor->z.word
                      + (g_RaceIntroCameraDelta.vz * rcos((g_RaceIntroCameraTimer << 10) / g_RaceIntroCameraCursor->duration)) / 4096;

            delta[0] = rsin(car->bodyYaw) / 128 + car->x - spad[2];
            delta[1] = car->y - s0v - spad[3];
            delta[2] = rcos(car->bodyYaw) / 128 + car->z - spad[4];
            s0v = 0x400;
            spad[7] = s0v - Atan2(delta[0], delta[2]);
            s0v = s0v - Atan2(delta[1], SquareRoot12(delta[0] * delta[0] + delta[2] * delta[2]) >> 6);
            spad[6] = s0v;
            spad[8] = 0;
            SetCameraRotMatrix();
            SelectModelBank(0);
            DrawPlayerCarModel(car);
        } else {
            DrawFullscreenFadeTile(g_RaceIntroCameraTimer * 26, 0x29);
            {
                s32 c0 = car->x;
                s32 c1 = car->y;
                s32 c2 = car->z;
                s32 c3 = car->positionW;
                spad[2] = c0;
                spad[3] = c1;
                spad[4] = c2;
                spad[5] = c3;
            }
            __asm__ volatile("");
            spad[3] -= s0v;
            {
                s32 c0 = car->bodyPitch;
                s32 c1 = car->bodyYaw;
                s32 c2 = car->bodyRoll;
                s32 c3 = car->bodyRotationW;
                spad[6] = c0;
                spad[7] = c1;
                spad[8] = c2;
                spad[9] = c3;
            }
            __asm__ volatile("");
            SetCameraRotMatrix();
        }
    } else {
        UpdateCamera(car, 0);
    }
}

void SeedFinishCamera(PlayerCarRuntime *car) {
    register u32 word0;
    Block16 *src;
    Block16 *dst;
    Block16 *end;
    GameCarRuntimeAddress sourceAddress;
    GameTrackPoint *track;
    TrackPointTableAddress pointAddress;
    TrackPointTableAddress trackAddress;
    GameCarRuntimeAddress destinationAddress;
    GameTrackPoint *point;
    register s32 index asm("$3");
    s32 lastIndex;

    sourceAddress.player = car;
    destinationAddress.runtime = &g_CameraCar;
    dst = destinationAddress.blocks;
    src = sourceAddress.blocks;
    end = src + sizeof(GameCarRuntime) / sizeof(*src);
    do {
        *dst = *src;
        src++;
        dst++;
    } while (src != end);

    sourceAddress.blocks = src;
    destinationAddress.blocks = dst;
    *destinationAddress.vector = *sourceAddress.vector;

    index = car->trackPointIndex;
    track = g_TrackPoints;
    pointAddress.pointOffset = (index * 3) << 3;
    trackAddress.pointPointer = track;
    pointAddress.value = pointAddress.pointOffset + trackAddress.value;
    point = pointAddress.pointPointer;
    g_CameraCar.x = point->x;

    index = car->trackPointIndex;
    pointAddress.pointOffset = (index * 3) << 3;
    trackAddress.pointPointer = track;
    pointAddress.value = pointAddress.pointOffset + trackAddress.value;
    point = pointAddress.pointPointer;
    g_CameraCar.z = point->z;

    index = car->trackPointIndex;
    pointAddress.pointOffset = (index * 3) << 3;
    trackAddress.pointPointer = track;
    pointAddress.value = pointAddress.pointOffset + trackAddress.value;
    point = pointAddress.pointPointer;
    index = g_CameraCar.speed;
    word0 = point->y;
    index += 0x40;
    word0 -= 0x40;
    g_CameraCar.speed = index;
    g_CameraCar.y = word0;

    index = car->facingBackwards;
    lastIndex = car->trackPointIndex;
    index <<= 11;
    pointAddress.pointOffset = (lastIndex * 3) << 3;
    trackAddress.pointPointer = track;
    pointAddress.value = pointAddress.pointOffset + trackAddress.value;
    point = pointAddress.pointPointer;
    index += 0xC00;
    index -= point->angle;
    g_CameraCar.headingAngle = index;
    g_CameraCarSeedYaw = index;
    g_CameraCar.bodyYaw = index;
}
