#include "common.h"
#include "game/render.h"
#define GAME_ROUTE_SCENERY_QUALIFIER volatile
#include "game/track_internal.h"
#define GAME_RACE_SERIES_QUALIFIER volatile
#include "game/race.h"
#include "game/scratchpad.h"
#include "game/track.h"
#include "psyq/gte.h"


void DrawFlybyScenery(void) {
    Matrix mtx0;
    Matrix mtx1;
    FlybySceneryState *state;

    state = &g_FlybyScenery;
    if (state->timer > 0) {
        BuildRotMatrixY(&mtx0, 0x800 - state->rotationY);
        BuildRotMatrixX(&mtx1, state->rotationX);
        MulMatrix2(&mtx0, &mtx1);
        MulMatrix2(SCRATCH_VIEW_MATRIX_GTE, &mtx1);
        BuildRotMatrixZ(&mtx0, state->rotationZ);
        MulMatrix2(&mtx1, &mtx0);
        SelectModelBank(2);
        SetGteObjectMatrix((void *)0x1F80011C, &state->position, &mtx0);
        SCRATCH_ENV_MODE4 = 0;
        SubmitModel((void *)SCRATCHPAD_ADDR, g_ModelBankCount < 1);
    }
}

/* 0 while the route prop is not running; the seeder sets it to 1 and
 * UpdateRouteScenery increments it every frame, so it is both the enable
 * and the frame count since the seed. */

void SeedRouteScenery(void) {
    register s32 index0 asm("$2");
    s32 index1;
    u8 *base;
    s32 value;

    g_RouteSceneryArmed = 1;
    g_RouteSceneryClock = 1;

    index0 = g_RaceSeries;
    base = g_RouteSceneryData;
    index1 = g_RaceSeries;
    index0 = (index0 * 32) + (s32)base;
    *(Vec4 *)&g_RouteSceneryX = *(Vec4 *)((u8 *)index0 + 0x10);

    index1 = index1 << 2;
    index1 = index1 + (s32)base;
    g_RouteSceneryKeyIndex = 0;
    g_RouteSceneryFrame = 0;

    index1 = *(s16 *)(index1 + 8);
    value = ((index1 * 3) << 2) + 0x50;
    base += value;

    g_RouteSceneryRotX = *(s16 *)(base + 0);
    g_RouteSceneryRotY = *(s16 *)(base + 2);
    value = *(s16 *)(base + 4);
    g_RouteSceneryKeyframe = (SceneryMotionKeyframe *)base;
    g_RouteSceneryRotZ = value;
}
