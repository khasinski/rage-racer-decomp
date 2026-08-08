#include "common.h"
#include "game/race.h"
#include "game/render.h"

extern u8 *g_ReplayFramesGp;
extern u32 g_ReplayWriteCursor;
extern u32 g_ReplayFrameCount;
extern u32 g_ReplayBufferWrapped;
extern s32 g_ReplayPlayerModel;
extern s32 g_ReplayRivalModel;

void ResetReplayFrameCounts(void) {
    g_ReplayFramesGp = &g_ReplayFrameBuffer;
    g_ReplayFramesTimeAttack = &g_ReplayFrameBuffer;
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
    GameRenderPairPoint *dst;
    u8 *base;
    GameRenderSourcePoint *src1;
    GameRenderSourcePoint *src2;
    s32 sourceField_AE;
    s32 current;
    s32 odd;
    u32 first;

    current = g_PlayerCarAssetIndex;
    src2 = (GameRenderSourcePoint *)srcB;
    sourceField_AE = src2->field_AE;
    g_ReplayPlayerModel = current;
    odd = pairIndex & 1;
    g_ReplayRivalModel = sourceField_AE;
    if (odd) {
        return;
    }

    pairIndex >>= 1;
    dst = (GameRenderPairPoint *)((pairIndex * 3) << 4);
    base = g_ReplayFramesGp;
    src1 = (GameRenderSourcePoint *)srcA;
    first = src1->field_0;
    dst = (GameRenderPairPoint *)((u8 *)dst + (s32)base);
    dst->first_0 = first;
    dst->first_2 = src1->field_4;
    dst->first_4 = src1->field_8;
    dst->first_6 = src1->field_60;
    dst->first_8 = src1->field_20;
    dst->first_A = src1->field_24;
    dst->first_C = src1->field_28;
    dst->first_E = src1->field_48;
    dst->first_2C = src1->field_44;
    dst->second_10 = src2->field_0;
    dst->second_12 = src2->field_4;
    dst->second_14 = src2->field_8;
    dst->second_16 = src2->field_60;
    dst->second_18 = src2->field_20;
    dst->second_1A = src2->field_24;
    dst->second_1C = src2->field_28;
    dst->second_1E = src2->field_48;
    dst->second_2E = src2->field_44;
    dst->first_24 = src1->field_30;
    dst->second_28 = src2->field_30;
    dst->field_20 = src1->field_8C;
}

void StoreReplayTimeAttackFrame(s32 pointIndex, u8 *srcPtr) {
    GameRenderSinglePoint *dst;
    u8 *base;
    GameRenderSourcePoint *src;
    u32 first;

    g_ReplayPlayerModel = g_PlayerCarAssetIndex;
    if (pointIndex % 2) {
        return;
    }

    pointIndex >>= 1;
    dst = (GameRenderSinglePoint *)((pointIndex * 7) << 2);
    base = g_ReplayFramesTimeAttack;
    src = (GameRenderSourcePoint *)srcPtr;
    first = src->field_0;
    dst = (GameRenderSinglePoint *)((u8 *)dst + (s32)base);
    dst->field_0 = first;
    dst->field_2 = src->field_4;
    dst->field_4 = src->field_8;
    dst->field_6 = src->field_60;
    dst->field_8 = src->field_20;
    dst->field_A = src->field_24;
    dst->field_C = src->field_28;
    dst->field_E = src->field_48;
    dst->field_18 = src->field_44;
    dst->field_14 = src->field_30;
    dst->field_10 = src->field_8C;
}
