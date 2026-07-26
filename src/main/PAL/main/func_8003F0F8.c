#include "common.h"
#include "game/race.h"
#include "game/track.h"

/*
 * g_ShuttleScenery[1]'s fields, which retail reaches through their own split
 * symbols rather than by indexing the array:
 *   D_801E4FEC = dwellCounter   D_801E4FF4 = travelStep
 *   D_801E4FF8 = startEndpoint  D_801E4FFA = pathIndex
 *   D_801E4FFC..D_801E5008 = x/y/z/unk1C
 *   D_801E500C = angleX  D_801E5010 = angleY  D_801E5014 = angleZ
 */
extern s32 D_801E4FEC;
extern s32 D_801E4FF4;
extern s16 D_801E4FF8;
extern s16 D_801E4FFA;
extern s32 D_801E4FFC;
extern s32 D_801E5000;
extern s32 D_801E5004;
extern s32 D_801E5008;
extern s32 D_801E500C;
extern s32 D_801E5010;
extern s32 D_801E5014;
extern s32 D_8007E3A0[];
extern s32 D_8007E360[];
extern s16 D_8007E3C0[];
extern s16 D_8007E3E0[];

void GameInitShuttleScenery(void) asm("func_8003F0F8");

void GameInitShuttleScenery(void) {
    register GameShuttleScenery *state asm("$7");
    register s32 *src asm("$5");
    register s32 *dst asm("$4");
    register s32 index asm("$4");
    register s32 value asm("$2");
    register s32 v1 asm("$3");
    register s32 a4 asm("$4");
    register s32 a5 asm("$5");
    register s32 a6 asm("$6");

    state = &g_ShuttleScenery[0];
    if ((g_CourseIndex & 3) == 2) {
        D_801E4FFA = 2;
        asm("" ::: "memory");
        src = D_8007E3A0;
        dst = &D_801E4FFC;
        asm("" : "=r"(src), "=r"(dst) : "0"(src), "1"(dst));
        value = src[0];
        v1 = src[1];
        dst[0] = value;
        dst[1] = v1;
        value = src[2];
        v1 = src[3];
        dst[2] = value;
        dst[3] = v1;

        asm("" ::: "memory");
        index = D_801E4FFA;
        v1 = index << 3;
        D_801E500C = *(s16 *)((s32)D_8007E3C0 + v1);
        D_801E5010 = *(s16 *)((s32)D_8007E3C0 + v1 + 2);
        value = *(s16 *)((s32)D_8007E3C0 + v1 + 4);
        index <<= 1;
        D_801E4FF8 = 0;
        D_801E4FF4 = 0;
        D_801E5014 = value;
        v1 = *(s16 *)((s32)D_8007E3E0 + index);
        state->pathIndex = 1;
        D_801E4FEC = v1;
        asm(".globl func_8003F1D0\nfunc_8003F1D0 = func_8003F0F8 + 0xD8");
        goto updateState;
    }

    state->pathIndex = 0;
updateState:
    asm("" ::: "memory");
    asm("" : "=r"(state) : "0"(state));
    value = state->pathIndex;
    value <<= 5;
    v1 = *(s32 *)((s32)D_8007E360 + value);
    a4 = *(s32 *)((s32)D_8007E360 + value + 4);
    a5 = *(s32 *)((s32)D_8007E360 + value + 8);
    a6 = *(s32 *)((s32)D_8007E360 + value + 12);
    state->x = v1;
    state->y = a4;
    state->z = a5;
    state->unk1C = a6;
    asm("" ::: "memory");
    value = state->pathIndex;
    value <<= 3;
    v1 = *(s16 *)((s32)D_8007E3C0 + value);
    asm("" ::: "memory");
    value = state->pathIndex;
    value <<= 3;
    state->angleX = v1;
    v1 = *(s16 *)((s32)D_8007E3C0 + value + 2);
    asm("" ::: "memory");
    value = state->pathIndex;
    value <<= 3;
    state->angleY = v1;
    v1 = *(s16 *)((s32)D_8007E3C0 + value + 4);
    asm("" ::: "memory");
    value = state->pathIndex;
    state->startEndpoint = 0;
    state->travelStep = 0;
    value <<= 1;
    state->angleZ = v1;
    value = *(s16 *)((s32)D_8007E3E0 + value);
    state->dwellCounter = value;
}
