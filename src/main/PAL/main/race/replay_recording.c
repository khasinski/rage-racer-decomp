#include "common.h"
#include "game/race.h"
#include "game/player_car_internal.h"
#include "game/render.h"
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

void StoreReplayCarFrame(s32 pairIndex, const GameRenderSourcePoint *srcA,
                         const GameRenderSourcePoint *srcB) {
    ReplayGrandPrixFrame *dst;
    const GameRenderSourcePoint *src1;
    const GameRenderSourcePoint *src2;
    s32 sourceModelIndex;
    s32 current;
    s32 odd;
    u32 first;

    current = g_PlayerCar.modelIndex;
    src2 = srcB;
    sourceModelIndex = src2->modelIndex;
    g_ReplayPlayerModel.word = current;
    odd = pairIndex & 1;
    g_ReplayRivalModel.word = sourceModelIndex;
    if (odd) {
        return;
    }

    pairIndex >>= 1;
    dst = &g_ReplayFramesGp[pairIndex];
    src1 = srcA;
    first = src1->x;
    dst->x0 = first;
    dst->y0 = src1->y;
    dst->z0 = src1->z;
    dst->z20 = src1->modelY;
    dst->rotX0 = src1->bodyPitch;
    dst->rotY0 = src1->bodyYaw;
    dst->rotZ0 = src1->bodyRoll;
    dst->flags0 = src1->field_48;
    dst->field44_0 = src1->field_44;
    dst->x1 = src2->x;
    dst->y1 = src2->y;
    dst->z1 = src2->z;
    dst->z21 = src2->modelY;
    dst->rotX1 = src2->bodyPitch;
    dst->rotY1 = src2->bodyYaw;
    dst->rotZ1 = src2->bodyRoll;
    dst->flags1 = src2->field_48;
    dst->field44_1 = src2->field_44;
    dst->field30_0 = src1->field_30;
    dst->field30_1 = src2->field_30;
    dst->field20 = src1->field_8C;
}

void StoreReplayTimeAttackFrame(s32 pointIndex, const GameRenderSourcePoint *srcPtr) {
    ReplayTimeAttackFrame *dst;
    const GameRenderSourcePoint *src;
    u32 first;

    g_ReplayPlayerModel.word = g_PlayerCar.modelIndex;
    if (pointIndex % 2) {
        return;
    }

    pointIndex >>= 1;
    dst = &g_ReplayFramesTimeAttack[pointIndex];
    src = srcPtr;
    first = src->x;
    dst->x = first;
    dst->y = src->y;
    dst->z = src->z;
    dst->z2 = src->modelY;
    dst->rotX = src->bodyPitch;
    dst->rotY = src->bodyYaw;
    dst->rotZ = src->bodyRoll;
    dst->flags = src->field_48;
    dst->field44 = src->field_44;
    dst->field30 = src->field_30;
    dst->field10 = src->field_8C;
}
