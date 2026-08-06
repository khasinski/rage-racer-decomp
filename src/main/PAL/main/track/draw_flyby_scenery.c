#include "common.h"
#include "psyq/gte.h"
#include "game/render.h"
#include "game/track.h"

extern s32 g_FlybyScenery[];

void DrawFlybyScenery(void) {
    Matrix mtx0;
    Matrix mtx1;
    s32 *state;

    state = g_FlybyScenery;
    if (state[0] > 0) {
        BuildRotMatrixY(&mtx0, 0x800 - g_FlybySceneryRotY);
        BuildRotMatrixX(&mtx1, g_FlybySceneryRotX);
        MulMatrix2(&mtx0, &mtx1);
        MulMatrix2((Matrix *)0x1F800028, &mtx1);
        BuildRotMatrixZ(&mtx0, g_FlybySceneryRotZ);
        MulMatrix2(&mtx1, &mtx0);
        SelectModelBank(2);
        SetGteObjectMatrix((void *)0x1F80011C, state + 4, &mtx0);
        *(s32 *)0x1F800084 = 0;
        SubmitModel((void *)0x1F800000, g_ModelBankCount < 1);
    }
}

extern volatile s32 g_RaceSeries;
extern u8 *g_RouteSceneryData;
/* 0 while the route prop is not running; the seeder sets it to 1 and
 * UpdateRouteScenery increments it every frame, so it is both the enable
 * and the frame count since the seed. */
extern volatile s32 g_RouteSceneryClock;
extern volatile s16 g_RouteSceneryKeyIndex;
extern s32 g_RouteSceneryRotY;
extern u8 *g_RouteSceneryKeyframe;

void SeedRouteScenery(void) {
    register s32 index0 asm("$2");
    s32 index1;
    register u8 *base asm("$4");
    register u8 *src asm("$2");
    register u8 *record asm("$4");
    s32 value;

    D_801E433C = 1;
    g_RouteSceneryClock = 1;

    index0 = g_RaceSeries;
    base = g_RouteSceneryData;
    index1 = g_RaceSeries;
    src = (u8 *)((index0 * 32) + (s32)base);
    *(Vec4 *)&g_RouteSceneryX = *(Vec4 *)(src + 0x10);

    index1 = index1 << 2;
    index1 = index1 + (s32)base;
    g_RouteSceneryKeyIndex = 0;
    g_RouteSceneryFrame = 0;

    index1 = *(s16 *)(index1 + 8);
    value = ((index1 * 3) << 2) + 0x50;
    record = base + value;

    g_RouteSceneryRotX = *(s16 *)(record + 0);
    g_RouteSceneryRotY = *(s16 *)(record + 2);
    value = *(s16 *)(record + 4);
    g_RouteSceneryKeyframe = record;
    g_RouteSceneryRotZ = value;
}
