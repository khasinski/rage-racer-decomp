#include "common.h"
#include "game/track.h"
#include "psyq/gte.h"
#include "game/race.h"
#include "game/render.h"
void SetGteObjectMatrix(void *arg0, void *arg1, Matrix *mtx) asm("func_80017794");

/* Per-path authored data; see DrawRouteScenery.c for the layout.
 * g_ShuttlePathTravelMax is the leg length in steps: the divisor of the
 * endpoint-to-endpoint lerp and the value travelStep counts up to. */
extern u8 g_ShuttlePathPoints[];
extern s16 g_ShuttlePathTravelMax[];
extern s16 g_ShuttlePathDwellMax[];

void UpdateShuttleScenery(s32 arg0);

void UpdateShuttleScenery(s32 arg0) {
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
    s16 denom;
    register s32 temp asm("$3");
    register s32 value asm("$2");

    entry = &g_ShuttleScenery[arg0];
    asm("" : "=r"(entry) : "0"(entry));
    limitPtr = g_ShuttlePathTravelMax;
    side = entry->startEndpoint;
    phase = entry->pathIndex;
    step = entry->travelStep;
    baseIndex = side << 4;
    phaseShift = phase * 32;
    baseIndex += phaseShift;
    phaseOffset = phase << 1;
    limitPtr = (s16 *)((u8 *)limitPtr + phaseOffset);
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

extern u32 *g_VisibleCellMask;
extern s32 g_CourseModelCount;


void DrawShuttleScenery(s32 arg0);

void DrawShuttleScenery(s32 arg0) {
    s32 drawArg;
    Matrix mtx0;
    Matrix mtx1;
    GameShuttleScenery *state;
    GameShuttleScenery *base;
    Matrix *mtx1Ptr;
    s32 drawValue;
    s32 offset;
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
        BuildRotMatrixY(&mtx0, state->angleY);
        mtx1Ptr = &mtx1;
        BuildRotMatrixZ(mtx1Ptr, state->angleZ);
        MulMatrix2(&mtx0, mtx1Ptr);
        MulMatrix2((Matrix *)0x1F800028, mtx1Ptr);
        if ((g_CourseIndex & 3) >= 2) {
            drawArg = 0x3C;
        }
        SetGteObjectMatrix((void *)0x1F80011C, &state->x, mtx1Ptr);
        frameValue = g_CourseModelCount;
        *(s32 *)0x1F800084 = 0;
        drawValue = 1;
        if (drawArg < frameValue) {
            drawValue = drawArg;
        }
        SubmitCourseModel((void *)0x1F800000, drawValue);
    }
}
