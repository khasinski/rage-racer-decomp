#include "common.h"
#include "game/render.h"

extern s16 D_8009E782;
extern s32 D_801E4D8C;
extern u8 *D_8019C7A4;

/*
 * Single-point variant of func_8001F134: packs a GameRenderSinglePoint from a
 * GameRenderSourcePoint into the single-point output buffer D_8019C7A4, keyed
 * by pointIndex>>1 (odd indices skipped). Stride ((n<<3)-n)<<2 == n*0x1C
 * (sizeof GameRenderSinglePoint).
 */
void func_8001F274(s32 pointIndex, u8 *srcPtr) {
    GameRenderSinglePoint *dst;
    u8 *base;
    GameRenderSourcePoint *src;
    u32 first;

    D_801E4D8C = D_8009E782;
    if (pointIndex & 1) {
        return;
    }

    pointIndex >>= 1;
    dst = (GameRenderSinglePoint *)(((pointIndex << 3) - pointIndex) << 2);
    base = D_8019C7A4;
    src = (GameRenderSourcePoint *)srcPtr;
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
