#include "common.h"
#include "game/race.h"
#include "game/player_car_internal.h"
#include "game/render.h"
#define GAME_REPLAY_COUNTER_TYPE u32
#include "game/replay_internal.h"

void ResetReplayFrameCounts(void) {
    g_ReplayFramesGp = (ReplayGrandPrixFrame *)&g_ReplayFrameBuffer;
    g_ReplayFramesTimeAttack = (ReplayTimeAttackFrame *)&g_ReplayFrameBuffer;
}

void ResetReplayWriteCursor(void) {
    u32 value;

    value = g_GrandPrixMode;
    g_ReplayWriteCursor = 0;
    if (value != 0) {
        value = 0x5DC;
    } else {
        value = 0xA0A;
    }
    g_ReplayFrameCount = value;
    g_ReplayBufferWrapped = 0;
}

void StoreReplayCarFrame(s32 pairIndex, u8 *srcA, u8 *srcB) {
    ReplayGrandPrixFrame *dst;
    GameRenderSourcePoint *src1;
    GameRenderSourcePoint *src2;
    s32 sourceField_AE;
    s32 current;
    s32 odd;
    u32 first;

    current = g_PlayerCar.field_AE;
    src2 = (GameRenderSourcePoint *)srcB;
    sourceField_AE = src2->field_AE;
    g_ReplayPlayerModel = current;
    odd = pairIndex & 1;
    g_ReplayRivalModel = sourceField_AE;
    if (odd) {
        return;
    }

    pairIndex >>= 1;
    dst = &g_ReplayFramesGp[pairIndex];
    src1 = (GameRenderSourcePoint *)srcA;
    first = src1->field_0;
    dst->x0 = first;
    dst->y0 = src1->field_4;
    dst->z0 = src1->field_8;
    dst->z20 = src1->field_60;
    dst->rotX0 = src1->field_20;
    dst->rotY0 = src1->field_24;
    dst->rotZ0 = src1->field_28;
    dst->flags0 = src1->field_48;
    dst->field44_0 = src1->field_44;
    dst->x1 = src2->field_0;
    dst->y1 = src2->field_4;
    dst->z1 = src2->field_8;
    dst->z21 = src2->field_60;
    dst->rotX1 = src2->field_20;
    dst->rotY1 = src2->field_24;
    dst->rotZ1 = src2->field_28;
    dst->flags1 = src2->field_48;
    dst->field44_1 = src2->field_44;
    dst->field30_0 = src1->field_30;
    dst->field30_1 = src2->field_30;
    dst->field20 = src1->field_8C;
}

void StoreReplayTimeAttackFrame(s32 pointIndex, u8 *srcPtr) {
    ReplayTimeAttackFrame *dst;
    GameRenderSourcePoint *src;
    u32 first;

    g_ReplayPlayerModel = g_PlayerCar.field_AE;
    if (pointIndex % 2) {
        return;
    }

    pointIndex >>= 1;
    dst = &g_ReplayFramesTimeAttack[pointIndex];
    src = (GameRenderSourcePoint *)srcPtr;
    first = src->field_0;
    dst->x = first;
    dst->y = src->field_4;
    dst->z = src->field_8;
    dst->z2 = src->field_60;
    dst->rotX = src->field_20;
    dst->rotY = src->field_24;
    dst->rotZ = src->field_28;
    dst->flags = src->field_48;
    dst->field44 = src->field_44;
    dst->field30 = src->field_30;
    dst->field10 = src->field_8C;
}
