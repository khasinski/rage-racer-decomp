#include "common.h"
#include "game/vector.h"
#include "psyq/gte.h"
#include "game/render.h"
#include "game/track.h"
#include "game/race.h"

#include "game/track_internal.h"

void UpdateRouteScenery(void) {
    Matrix mtx0;
    Matrix mtx1;
    SVec vin;
    LVec vout;
    volatile s32 pad[6];
    volatile s32 *cnt;
    SceneryMotionData *base;
    SceneryMotionKeyframe *kp;
    s32 i;
    s32 counter;
    s32 c;
    s32 r4354;

    cnt = &g_RouteSceneryClock;
    c = *cnt;
    base = (SceneryMotionData *)g_RouteSceneryData;
    if (c <= 0) {
        return;
    }
    *cnt = c + 1;

    counter = g_RouteSceneryFrame;
    i = g_RouteSceneryKeyIndex;
    kp = g_RouteSceneryKeyframe;
    counter = counter + 1;
    {
        SceneryMotionKeyframe *rec0 =
            (SceneryMotionKeyframe *)(i * sizeof(*rec0) + (s32)kp);

        g_RouteSceneryFrame = counter;
        if (RAW(rec0->duration) == counter) {
            c = i + 1;
            g_RouteSceneryKeyIndex = c;
            g_RouteSceneryFrame = 0;
        }
    }

    {
        s32 keyIndex = g_RouteSceneryKeyIndex;
        if (kp[keyIndex].duration == -1) {
            s32 idx;
            SceneryMotionKeyframe *r3;
            s32 n;
            s32 value;

            idx = g_RaceSeries;
            n = base->firstKeyframe[idx][g_RouteSceneryKeyIndex = 0];
            r3 = &base->keyframes[n];
            value = r3->rotationX;
            g_RouteSceneryRotX = value;
            value = r3->rotationY;
            g_RouteSceneryRotY = value;
            value = r3->rotationZ;
            g_RouteSceneryKeyframe = r3;
            g_RouteSceneryRotZ = value;
            *(Vec4 *)&g_RouteSceneryX = (idx + base->start)->position;
            *cnt = 1;
            g_RouteSceneryFrame = 0;
        }
    }

    {
        SceneryMotionKeyframe *rec;
        s32 t;
        s32 t0v;

        rec = g_RouteSceneryKeyframe + (s32)g_RouteSceneryKeyIndex;
        t = g_RouteSceneryFrame;
        t0v = rec->duration - t;
        g_RouteSceneryRotX =
            (rec[1].rotationX * t + rec->rotationX * t0v) / rec->duration;
        r4354 = (RAW(rec[1].rotationY) * t + RAW(rec->rotationY) * t0v) /
                RAW(rec->duration);
        g_RouteSceneryRotY = r4354;
        g_RouteSceneryRotZ =
            (RAW(rec[1].rotationZ) * t + RAW(rec->rotationZ) * t0v) /
            RAW(rec->duration);
        vin.vx = 0;
        vin.vy = 0;
        vin.vz = -RAW(rec->speed) * 4;
        BuildRotMatrixY(&mtx0, 0x800 - r4354);
    }

    BuildRotMatrixX(&mtx1, g_RouteSceneryRotX);
    MulMatrix2(&mtx0, &mtx1);
    BuildRotMatrixZ(&mtx0, g_RouteSceneryRotZ);
    MulMatrix(&mtx1, &mtx0);
    ApplyMatrix((s32 *)&mtx1, (s32 *)&vin, (s32 *)&vout);

    g_RouteSceneryX += vout.x / 4;
    g_RouteSceneryY += vout.y / 4;
    g_RouteSceneryZ += vout.z / 4;
}
