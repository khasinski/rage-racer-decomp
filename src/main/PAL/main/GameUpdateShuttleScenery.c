#include "common.h"
#include "game/track.h"

extern u8 D_8007E360[];
extern s16 D_8007E3D8[];
extern s16 D_8007E3E0[];

void GameUpdateShuttleScenery(s32 arg0) asm("func_8003F2A4");

void GameUpdateShuttleScenery(s32 arg0) {
    GameShuttleScenery *entry;
    register s32 phase asm("$2");
    register s32 side asm("$10");
    register s32 step asm("$6");
    register s32 baseIndex asm("$7");
    register s32 altIndex asm("$4");
    register s32 phaseOffset asm("$12");
    register s32 phaseShift asm("$9");
    register s16 *limitPtr asm("$8");
    register s16 *tailLimitPtr asm("$8");
    register s32 denom asm("$5");
    register s32 temp asm("$3");
    register s32 value asm("$2");

    entry = &g_ShuttleScenery[arg0];
    asm("" : "=r"(entry) : "0"(entry));
    limitPtr = D_8007E3D8;
    side = entry->startEndpoint;
    phase = entry->pathIndex;
    step = entry->travelStep;
    baseIndex = side << 4;
    phaseShift = phase << 5;
    baseIndex += phaseShift;
    phaseOffset = phase << 1;
    limitPtr = (s16 *)((s32)limitPtr + phaseOffset);
    denom = *limitPtr;
    temp = *(s32 *)(D_8007E360 + baseIndex);
    value = (denom - step) * temp;
    altIndex = (1 - side) << 4;
    altIndex += phaseShift;
    value = (value + (step * *(s32 *)(D_8007E360 + altIndex))) / denom;
    entry->x = value;

    denom = *limitPtr;
    temp = *(s32 *)(D_8007E360 + baseIndex + 4);
    value = (denom - step) * temp;
    value = (value + (step * *(s32 *)(D_8007E360 + altIndex + 4))) / denom;
    entry->y = value;

    denom = *limitPtr;
    temp = *(s32 *)(D_8007E360 + baseIndex + 8);
    value = (denom - step) * temp;
    value = (value + (step * *(s32 *)(D_8007E360 + altIndex + 8))) / denom;
    entry->z = value;

    if (entry->travelStep >= *limitPtr) {
        entry->travelStep = 0;
        entry->dwellCounter = 0;
        entry->startEndpoint ^= 1;
        return;
    }

    phase = (s32)D_8007E3E0;
    tailLimitPtr = (s16 *)(phaseOffset + phase);
    asm("" : "=r"(tailLimitPtr) : "0"(tailLimitPtr));
    if (entry->dwellCounter >= *tailLimitPtr) {
        entry->travelStep = entry->travelStep + 1;
        entry->dwellCounter = *tailLimitPtr;
        return;
    }
    entry->dwellCounter = entry->dwellCounter + 1;
}
