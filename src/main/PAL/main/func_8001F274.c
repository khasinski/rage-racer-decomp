#include "common.h"
#include "game/render.h"

extern s16 D_8009E782;
extern s32 D_801E4D8C;
extern u8 *D_8019C7A4;

void func_8001F274(s32 arg0, u8 *arg1) {
    GameRenderSinglePoint *dst;
    u8 *base;
    GameRenderSourcePoint *src;
    u32 first;

    D_801E4D8C = D_8009E782;
    if (arg0 & 1) {
        return;
    }

    arg0 >>= 1;
    dst = (GameRenderSinglePoint *)(((arg0 << 3) - arg0) << 2);
    base = D_8019C7A4;
    src = (GameRenderSourcePoint *)arg1;
    first = src->field_0;
    dst = (GameRenderSinglePoint *)((s32)dst + (s32)base);
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
