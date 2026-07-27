#include "common.h"
#include "game/track.h"
#include "psyq/gte.h"
#include "game/race.h"
#include "game/render.h"

/* Per-path authored data; see GameDrawRouteScenery.c for the layout.
 * g_ShuttlePathTravelMax is the leg length in steps: the divisor of the
 * endpoint-to-endpoint lerp and the value travelStep counts up to. */
extern u8 g_ShuttlePathPoints[] asm("D_8007E360");
extern s16 g_ShuttlePathTravelMax[] asm("D_8007E3D8");
extern s16 g_ShuttlePathDwellMax[] asm("D_8007E3E0");

void GameUpdateShuttleScenery(s32 arg0) asm("func_8003F2A4");

void GameUpdateShuttleScenery(s32 arg0) {
    GameShuttleScenery *entry;
    s32 phase;
    s32 side;
    s32 step;
    s32 baseIndex;
    s32 altIndex;
    s32 phaseOffset;
    s32 phaseShift;
    s16 *limitPtr;
    s16 *tailLimitPtr;
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 denom asm("$5");
    register s32 temp asm("$3");
    register s32 value asm("$2");

    entry = &g_ShuttleScenery[arg0];
    /* This barrier is load-bearing: removing it changes .text. */
    asm("" : "=r"(entry) : "0"(entry));
    limitPtr = g_ShuttlePathTravelMax;
    side = entry->startEndpoint;
    phase = entry->pathIndex;
    step = entry->travelStep;
    baseIndex = side << 4;
    phaseShift = phase << 5;
    baseIndex += phaseShift;
    phaseOffset = phase << 1;
    limitPtr = (s16 *)((s32)limitPtr + phaseOffset);
    denom = *limitPtr;
    temp = *(s32 *)(g_ShuttlePathPoints + baseIndex);
    value = (denom - step) * temp;
    altIndex = (1 - side) << 4;
    altIndex += phaseShift;
    value = (value + (step * *(s32 *)(g_ShuttlePathPoints + altIndex))) / denom;
    entry->x = value;

    denom = *limitPtr;
    temp = *(s32 *)(g_ShuttlePathPoints + baseIndex + 4);
    value = (denom - step) * temp;
    value = (value + (step * *(s32 *)(g_ShuttlePathPoints + altIndex + 4))) / denom;
    entry->y = value;

    denom = *limitPtr;
    temp = *(s32 *)(g_ShuttlePathPoints + baseIndex + 8);
    value = (denom - step) * temp;
    value = (value + (step * *(s32 *)(g_ShuttlePathPoints + altIndex + 8))) / denom;
    entry->z = value;

    if (entry->travelStep >= *limitPtr) {
        entry->travelStep = 0;
        entry->dwellCounter = 0;
        entry->startEndpoint ^= 1;
        return;
    }

    phase = (s32)g_ShuttlePathDwellMax;
    tailLimitPtr = (s16 *)(phaseOffset + phase);
    if (entry->dwellCounter >= *tailLimitPtr) {
        entry->travelStep = entry->travelStep + 1;
        entry->dwellCounter = *tailLimitPtr;
        return;
    }
    entry->dwellCounter = entry->dwellCounter + 1;
}

extern u32 *g_VisibleCellMask asm("D_801E6828");
extern s32 g_CourseModelCount asm("D_801E40E4");

void func_80017794(void *arg0, void *arg1, Matrix *mtx);

void GameDrawShuttleScenery(s32 arg0) asm("func_8003F4BC");

void GameDrawShuttleScenery(s32 arg0) {
    Matrix mtx0;
    Matrix mtx1;
    GameShuttleScenery *state;
    GameShuttleScenery *base;
    Matrix *mtx1Ptr;
    /* This pin is load-bearing: removing it changes .text. */
    register s32 drawArg asm("$16");
    s32 drawValue;
    s32 offset;
    /* This pin is load-bearing: removing it changes .text. */
    register s32 bucket asm("$2");
    s32 bit;
    s32 firstValue;
    s32 value;
    u32 *visibility;
    u32 *wordPtr;
    s32 visible;
    s32 frameValue;

    offset = (((arg0 * 3) << 2) + arg0) << 2;
    base = g_ShuttleScenery;
    state = (GameShuttleScenery *)((u8 *)base + offset);
    firstValue = state->z;
    bucket = firstValue + 0x400;
    if (bucket < 0) {
        bucket = firstValue + 0xBFF;
    }
    bucket >>= 11;
    bucket <<= 2;
    value = state->x;
    visibility = g_VisibleCellMask;
    bit = value + 0x400;
    wordPtr = (u32 *)(bucket + (s32)visibility);
    if (bit < 0) {
        bit = value + 0xBFF;
    }
    bucket = bit >> 11;
    visible = 1 << bucket;
    visible &= *wordPtr;

    if ((visible != 0) || (g_CourseIndex == 2)) {
        drawArg = 0x3F;
        GameBuildRotMatrixY(&mtx0, state->angleY);
        mtx1Ptr = &mtx1;
        GameBuildRotMatrixZ(mtx1Ptr, state->angleZ);
        MulMatrix2(&mtx0, mtx1Ptr);
        MulMatrix2((Matrix *)0x1F800028, mtx1Ptr);
        if ((g_CourseIndex & 3) >= 2) {
            drawArg = 0x3C;
        }
        func_80017794((void *)0x1F80011C, &state->x, mtx1Ptr);
        frameValue = g_CourseModelCount;
        *(s32 *)0x1F800084 = 0;
        drawValue = 1;
        if (drawArg < frameValue) {
            drawValue = drawArg;
        }
        GameSubmitCourseModel((void *)0x1F800000, drawValue);
    }
}
