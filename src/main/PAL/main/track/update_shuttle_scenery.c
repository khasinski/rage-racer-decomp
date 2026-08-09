#include "common.h"
#include "game/asset.h"
#include "game/race.h"
#include "game/render.h"
#include "game/render_internal.h"
#include "game/scratchpad.h"
#include "game/track.h"
#include "psyq/gte.h"

void UpdateShuttleScenery(s32 instance) {
    GameShuttleScenery *entry;
    s32 phase;
    s32 side;
    s32 step;
    s32 baseIndex;
    s32 altIndex;
    s32 phaseShift;
    s16 *limitPtr;
    s16 *tailLimitPtr;
    s16 denom;
    AssetAddress basePointAddress;
    AssetAddress altPointAddress;

    entry = &g_ShuttleScenery[instance];
    asm("" : "=r"(entry) : "0"(entry));
    limitPtr = g_ShuttlePathTravelMax;
    side = entry->startEndpoint;
    phase = entry->pathIndex;
    step = entry->travelStep;
    baseIndex = side << 4;
    phaseShift = phase * 32;
    baseIndex += phaseShift;
    limitPtr = &limitPtr[phase];
    denom = *limitPtr;
    altIndex = (1 - side) << 4;
    altIndex += phaseShift;
    basePointAddress.pointer = g_ShuttlePathPoints;
    basePointAddress.offset += baseIndex;
    altPointAddress.pointer = g_ShuttlePathPoints;
    altPointAddress.offset += altIndex;
    entry->position.x = ((denom - step) * ((Vec4 *)basePointAddress.pointer)->x +
                         step * ((Vec4 *)altPointAddress.pointer)->x) / denom;

    denom = *limitPtr;
    basePointAddress.pointer = g_ShuttlePathPoints;
    basePointAddress.offset += baseIndex;
    altPointAddress.pointer = g_ShuttlePathPoints;
    altPointAddress.offset += altIndex;
    entry->position.y = ((denom - step) * ((Vec4 *)basePointAddress.pointer)->y +
                         step * ((Vec4 *)altPointAddress.pointer)->y) / denom;

    denom = *limitPtr;
    basePointAddress.pointer = g_ShuttlePathPoints;
    basePointAddress.offset += baseIndex;
    altPointAddress.pointer = g_ShuttlePathPoints;
    altPointAddress.offset += altIndex;
    entry->position.z = ((denom - step) * ((Vec4 *)basePointAddress.pointer)->z +
                         step * ((Vec4 *)altPointAddress.pointer)->z) / denom;

    if (entry->travelStep >= *limitPtr) {
        entry->travelStep = 0;
        entry->dwellCounter = 0;
        entry->startEndpoint ^= 1;
        return;
    }

    tailLimitPtr = &g_ShuttlePathDwellMax[phase];
    if (entry->dwellCounter >= *tailLimitPtr) {
        entry->travelStep = entry->travelStep + 1;
        entry->dwellCounter = *tailLimitPtr;
        return;
    }
    entry->dwellCounter = entry->dwellCounter + 1;
}


void DrawShuttleScenery(s32 instance) {
    s32 drawArg;
    Matrix mtx0;
    Matrix mtx1;
    GameShuttleScenery *state;
    Matrix *mtx1Ptr;
    s32 drawValue;
    s32 wordIndex;
    s32 bitIndex;
    s32 bit;
    s32 firstValue;
    s32 value;
    u32 *visibility;
    u32 *wordPtr;
    s32 visible;
    s32 frameValue;

    state = &g_ShuttleScenery[instance];
    firstValue = state->position.z;
    wordIndex = firstValue + 0x400;
    if (wordIndex < 0) {
        wordIndex = firstValue + 0xBFF;
    }
    wordIndex >>= 11;
    value = state->position.x;
    visibility = g_VisibleCellMask;
    bit = value + 0x400;
    wordPtr = (u32 *)((wordIndex << 2) + (s32)visibility);
    if (bit < 0) {
        bit = value + 0xBFF;
    }
    bitIndex = bit >> 11;
    visible = 1 << bitIndex;
    visible &= *wordPtr;

    if ((visible != 0) || (g_CourseIndex == 2)) {
        drawArg = 0x3F;
        BuildRotMatrixY(&mtx0, state->angleY);
        mtx1Ptr = &mtx1;
        BuildRotMatrixZ(mtx1Ptr, state->angleZ);
        MulMatrix2(&mtx0, mtx1Ptr);
        MulMatrix2(SCRATCH_VIEW_MATRIX_GTE, mtx1Ptr);
        if ((g_CourseIndex & 3) >= 2) {
            drawArg = 0x3C;
        }
        SetGteObjectMatrix((void *)0x1F80011C, &state->position, mtx1Ptr);
        frameValue = g_CourseModelCount;
        SCRATCH_ENV_MODE4 = 0;
        drawValue = 1;
        if (drawArg < frameValue) {
            drawValue = drawArg;
        }
        SubmitCourseModel((void *)SCRATCHPAD_ADDR, drawValue);
    }
}
