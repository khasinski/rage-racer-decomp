#include "common.h"
#include "game/render.h"

extern s16 D_8009E782;
extern s32 D_801E4D8C;
extern s32 D_801E4BC0;
extern u8 *D_8009F0A4;

void func_8001F134(s32 arg0, u8 *arg1, u8 *arg2) {
    GameRenderPairPoint *dst;
    u8 *base;
    GameRenderSourcePoint *src1;
    GameRenderSourcePoint *src2;
    s32 saved_ae;
    s32 current;
    s32 odd;
    u32 first;

    current = D_8009E782;
    src2 = (GameRenderSourcePoint *)arg2;
    saved_ae = src2->field_AE;
    D_801E4D8C = current;
    odd = arg0 & 1;
    D_801E4BC0 = saved_ae;
    if (odd) {
        return;
    }

    arg0 >>= 1;
    dst = (GameRenderPairPoint *)(((arg0 << 1) + arg0) << 4);
    base = D_8009F0A4;
    src1 = (GameRenderSourcePoint *)arg1;
    first = src1->field_0;
    dst = (GameRenderPairPoint *)((s32)dst + (s32)base);
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
