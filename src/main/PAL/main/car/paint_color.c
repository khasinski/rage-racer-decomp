#include "common.h"

/*
 * Members declare these three halfwords as u16, s16 and volatile u16;
 * reconciled to the most specific form.  Only ApplyBodyColor1 and
 * ApplyBodyColor2 read them, the others just store.
 */
extern volatile u16 g_PaintBlendShade0 asm("D_8019CB38");
extern volatile u16 g_PaintBlendShade1 asm("D_8019CB3A");
extern volatile u16 g_PaintBlendShade2 asm("D_8019CB3C");


/*
 * D_8019CB38 / D_8019CB3A (the two blended shade words this routine emits)
 * MUST keep the raw D_ spelling: they are referenced from the %hi/%lo pairs in
 * the inline asm below, which does not follow asm() labels. See names.md 12c.
 */
void BlendPaintColor(u32 arg0, u32 arg1) asm("func_8001D5C8");
void BlendPaintColor(u32 arg0, u32 arg1) {
    u32 a;
    u32 b;

    a = arg0 >> 1;
    b = arg1 >> 1;
    a &= 0x3DEF;
    b &= 0x3DEF;
    g_PaintBlendShade0 = (a + b) | 0x8000;
}

void BlendPaintColorThirds(u32 arg0, u32 arg1) asm("func_8001D5F0");
void BlendPaintColorThirds(u32 arg0, u32 arg1) {
    u16 color0;
    u16 color1;
    s32 r0;
    s32 r1;
    s32 g0;
    s32 g1;
    s32 b0;
    s32 b1;
    s32 r;
    s32 g;
    s32 b;

    r0 = arg0 & 0x1F;
    r1 = arg1 & 0x1F;
    r = r0 + r1;
    color0 = arg0;
    g0 = (color0 >> 5) & 0x1F;
    color1 = arg1;
    g1 = (color1 >> 5) & 0x1F;
    g = g0 + g1;
    b0 = (color0 >> 10) & 0x1F;
    b1 = (color1 >> 10) & 0x1F;
    b = b0 + b1;

    r0 = r * 2 / 3;
    g0 = g * 2 / 3;
    b0 = b * 2 / 3;
    r1 = r / 3;
    g1 = g / 3;
    b1 = b / 3;

    g_PaintBlendShade0 = (b0 << 10) + (g0 << 5) + r0 + 0x8000;
    g_PaintBlendShade1 = (b1 << 10) + (g1 << 5) + r1 + 0x8000;
}


void BlendPaintColorQuarters(u32 arg0, u32 arg1) asm("func_8001D6F4");
void BlendPaintColorQuarters(u32 arg0, u32 arg1) {
    u32 a;
    u32 b;
    u32 c;
    u32 high;
    s32 bias;

    a = arg0 >> 1;
    b = arg1 >> 1;
    a &= 0x3DEF;
    b &= 0x3DEF;
    c = a + b;
    high = c;
    bias = 0x8000;
    high += bias;
    c >>= 1;
    c &= 0x3DEF;
    a += c;
    /* Preserve the bias while keeping its two GCC live ranges separate. */
    a -= bias++;
    c += b;
    c -= --bias;

    g_PaintBlendShade1 = high;
    g_PaintBlendShade0 = a;
    g_PaintBlendShade2 = c;
}

/*
 * ApplyBodyColor1.
 *
 * The store to raw+0x7162 written just before the call is scheduled by the
 * compiler into the jal delay slot; no hand-written asm is needed.
 */

extern u16 g_BodyColorPrimary[] asm("D_8007D30C");
extern u16 g_BodyColorSecondary[] asm("D_8007D330");
/* Clut word offsets of each paint ramp inside the car texture's palette:
 * nine 3-stop ramps for body colour 1, eight for body colour 2, and four
 * 4-stop ramps shared by both. */
extern volatile u16 g_PaintSlots3StopA[] asm("D_8007D354");
extern volatile u16 g_PaintSlots4Stop[] asm("D_8007D378");

void BlendPaintColor(u32 arg0, u32 arg1) asm("func_8001D5C8");
void BlendPaintColorThirds(u32 arg0, u32 arg1) asm("func_8001D5F0");
void BlendPaintColorQuarters(u32 arg0, u32 arg1) asm("func_8001D6F4");

void ApplyBodyColor1(u32 arg0, u32 arg1) asm("func_8001D748");
void ApplyBodyColor1(u32 arg0, u32 arg1) {
    u32 raw;
    u16 *base;
    u16 s1;
    u16 s2;
    volatile u16 *idx;
    volatile u16 *color;
    u16 c;
    s32 i;

    raw = arg1;
    arg0 <<= 1;
    base = (u16 *)(raw + 0x7060);
    s1 = *(u16 *)((u8 *)g_BodyColorPrimary + arg0);
    s2 = *(u16 *)((u8 *)g_BodyColorSecondary + arg0);

    *(u16 *)(raw + 0x7162) = s1;
    BlendPaintColor(s1, s2);

    i = 0;
    color = &g_PaintBlendShade0;
    idx = g_PaintSlots3StopA;
    for (; i < 9; idx++) {
        i++;
        base[idx[0] + 0] = s1;
        base[idx[0] + 1] = color[0];
        base[idx[0] + 2] = s2;
    }

    BlendPaintColorThirds(s1, s2);

    i = 0;
    color = &g_PaintBlendShade0;
    idx = g_PaintSlots4Stop;
    for (; i < 4; idx++) {
        i++;
        base[idx[0] + 0] = s1;
        base[idx[0] + 1] = color[0];
        base[idx[0] + 2] = color[1];
        base[idx[0] + 3] = s2;
    }

    BlendPaintColorQuarters(s1, s2);

    ((volatile u16 *)base)[0x2C1] = s1;
    ((volatile u16 *)base)[0x2C2] = g_PaintBlendShade0;
    ((volatile u16 *)base)[0x2C3] = g_PaintBlendShade1;
    c = g_PaintBlendShade2;
    ((volatile u16 *)base)[0x2C5] = s2;
    ((volatile u16 *)base)[0x2C4] = c;
}

extern u32 g_CarModelAsset asm("D_8009E698");
extern u32 g_CarModelSlot asm("D_8009E87C");

void ApplyBodyColor1(u32 arg0, u32 arg1) asm("func_8001D748");
void UploadCarImage(u32 arg0) asm("func_80017B5C");

void SetBodyColor1(u32 arg0) asm("func_8001D8C4");
void SetBodyColor1(u32 arg0) {
    ApplyBodyColor1(arg0, *(u32 *)(g_CarModelAsset + 0x24));
    UploadCarImage(g_CarModelSlot);
}

extern u16 g_BodyColorPrimary[] asm("D_8007D30C");
extern u16 g_BodyColorSecondary[] asm("D_8007D330");
extern volatile u16 g_PaintSlots3StopB[] asm("D_8007D368");
extern volatile u16 g_PaintSlots4Stop[] asm("D_8007D378");

void BlendPaintColor(u32 arg0, u32 arg1) asm("func_8001D5C8");
void BlendPaintColorThirds(u32 arg0, u32 arg1) asm("func_8001D5F0");
void BlendPaintColorQuarters(u32 arg0, u32 arg1) asm("func_8001D6F4");

void ApplyBodyColor2(u32 arg0, u32 arg1) asm("func_8001D900");
void ApplyBodyColor2(u32 arg0, u32 arg1) {
    u16 *base;
    u16 s1;
    u16 s2;
    volatile u16 *idx;
    volatile u16 *color;
    u16 c;
    s32 i;

    base = (u16 *)(arg1 + 0x7060);
    s1 = g_BodyColorPrimary[arg0];
    s2 = g_BodyColorSecondary[arg0];

    BlendPaintColor(s1, s2);

    i = 0;
    color = &g_PaintBlendShade0;
    idx = g_PaintSlots3StopB;
    for (; i < 8; idx++) {
        i++;
        base[idx[0] + 3] = s1;
        base[idx[0] + 4] = color[0];
        base[idx[0] + 5] = s2;
    }

    BlendPaintColorThirds(s1, s2);

    i = 0;
    color = &g_PaintBlendShade0;
    idx = g_PaintSlots4Stop;
    for (; i < 4; idx++) {
        i++;
        base[idx[0] + 4] = s1;
        base[idx[0] + 5] = color[0];
        base[idx[0] + 6] = color[1];
        base[idx[0] + 7] = s2;
    }

    BlendPaintColorQuarters(s1, s2);

    ((volatile u16 *)base)[0x2C6] = s1;
    ((volatile u16 *)base)[0x2C7] = g_PaintBlendShade0;
    ((volatile u16 *)base)[0x2C8] = g_PaintBlendShade1;
    c = g_PaintBlendShade2;
    ((volatile u16 *)base)[0x2CA] = s2;
    ((volatile u16 *)base)[0x2C9] = c;
}

extern u32 g_CarModelAsset asm("D_8009E698");
extern u32 g_CarModelSlot asm("D_8009E87C");

void ApplyBodyColor2(u32 arg0, u32 arg1) asm("func_8001D900");
void UploadCarImage(u32 arg0) asm("func_80017B5C");

void SetBodyColor2(u32 arg0) asm("func_8001DA74");
void SetBodyColor2(u32 arg0) {
    ApplyBodyColor2(arg0, *(u32 *)(g_CarModelAsset + 0x24));
    UploadCarImage(g_CarModelSlot);
}
