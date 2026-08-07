#include "common.h"
#include "game/race.h"
#include "game/render.h"
#include "game/scratchpad.h"
#include "game/track.h"
#include "game/vector.h"
#include "psyq/gte.h"

extern volatile s32 g_RouteSceneryRotY;

void DrawRouteScenery(void) {
    Matrix mtx0;
    Matrix mtx1;
    Matrix *mtx1Ptr;
    s32 frameValue;
    s32 drawId;

    BuildRotMatrixY(&mtx0, 0x800 - g_RouteSceneryRotY);
    mtx1Ptr = &mtx1;
    BuildRotMatrixX(mtx1Ptr, g_RouteSceneryRotX);
    MulMatrix2(&mtx0, mtx1Ptr);
    MulMatrix2(SCRATCH_VIEW_MATRIX_GTE, mtx1Ptr);
    BuildRotMatrixZ(&mtx0, g_RouteSceneryRotZ);
    MulMatrix2(mtx1Ptr, &mtx0);
    SelectModelBank(1);
    SetGteObjectMatrix((void *)0x1F80011C, &g_RouteSceneryX, &mtx0);
    frameValue = g_ModelBankCount;
    SCRATCH_ENV_MODE4 = 0;
    drawId = 1;
    if (frameValue >= 0x26) {
        drawId = 0x25;
    }
    SubmitModel((void *)SCRATCHPAD_ADDR, drawId);
}

#define PATH(byteOffset) (*(ShuttlePath *)((u8 *)g_ShuttlePathPoints + (byteOffset)))
#define ANGLES(byteOffset) (*(SVec *)((u8 *)g_ShuttlePathAngles + (byteOffset)))

void InitShuttleScenery(void) {
    GameShuttleScenery *state;
    s32 index;
    register s32 value asm("$2");
    register s32 v1 asm("$3");

    state = &g_ShuttleScenery[0];
    if ((g_CourseIndex & 3) == 2) {
        g_ShuttleScenery[1].pathIndex = 2;
        *(Vec4 *)&g_ShuttleScenery[1].x = *(Vec4 *)g_ShuttlePath2Points;

        asm(".globl func_8003F1D0\nfunc_8003F1D0 = InitShuttleScenery + 0xD8");
        index = g_ShuttleScenery[1].pathIndex;
        v1 = index * 8;
        g_ShuttleScenery[1].angleX = ANGLES(v1).vx;
        g_ShuttleScenery[1].angleY = ANGLES(v1).vy;
        value = ANGLES(v1).vz;
        index <<= 1;
        g_ShuttleScenery[1].startEndpoint = 0;
        g_ShuttleScenery[1].travelStep = 0;
        g_ShuttleScenery[1].angleZ = value;
        v1 = *(s16 *)((u8 *)g_ShuttlePathDwellMax + index);
        state->pathIndex = 1;
        g_ShuttleScenery[1].dwellCounter = v1;
    } else {

    state->pathIndex = 0;
    }
    value = state->pathIndex;
    value <<= 5;
    *(Vec4 *)&state->x = PATH(value).endpoint[0];
    value = state->pathIndex;
    value <<= 3;
    v1 = RAW(ANGLES(value).vx);
    asm("" ::: "memory");
    value = state->pathIndex;
    value <<= 3;
    state->angleX = v1;
    v1 = RAW(ANGLES(value).vy);
    value = state->pathIndex;
    value <<= 3;
    state->angleY = v1;
    v1 = RAW(ANGLES(value).vz);
    value = state->pathIndex;
    state->startEndpoint = 0;
    state->travelStep = 0;
    value <<= 1;
    state->angleZ = v1;
    value = *(s16 *)((u8 *)g_ShuttlePathDwellMax + value);
    state->dwellCounter = value;
}
