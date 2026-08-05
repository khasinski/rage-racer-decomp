#include "common.h"

s32 GameQueueSpriteTrans(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 GameQueueDrawModePrimWide(s32 arg0, s32 arg1, s32 arg2) asm("QueueDrawModePrim");
extern u8 D_8007D588[];
extern u8 D_8007D589[];
extern u8 D_8007D58A[];
extern u8 D_8007D58B[];
extern u8 * volatile g_DrawBuffer;
extern volatile u8 g_PadType;
extern u8 g_LastValidPadType;

/* The 0xC x 0x18 selection arrow every setup-menu list draws at x = 0x14. */
void DrawMenuCursorArrow(s32 arg0, s32 arg1);
void DrawMenuCursorArrow(s32 arg0, s32 arg1) {
    u8 *base = g_DrawBuffer;
    s32 *scratch = (s32 *)0x1F800000;
    s32 value;
    s32 scratchValue;

    base += 0xCC;
    scratchValue = *scratch;
    value = GameQueueSpriteTrans((s32)base, scratchValue, arg0, arg1, 0xC, 0x18, 0xE0, 0x48, 0x7F40);
    *scratch = GameQueueDrawModePrimWide((s32)base, value, 0x3F);
}

/* The bottom hint bar: a left arrow, a caption picked from D_8007D588 by `variant`, and a right arrow. */
void DrawOptionHintBar(s32 arg0);
void DrawOptionHintBar(s32 arg0) {
    s32 index;
    s32 y;
    s32 which;
    s32 base;
    s32 next;

    base = (s32)g_DrawBuffer + 0xCC;
    next = *(s32 *)0x1F800000;
    which = arg0;

    if (which == 4) {
        y = 0x5A;
    } else {
        y = (0x120 - D_8007D58A[which * 4]) / 2;
    }

    next = GameQueueSpriteTrans(base, next, y, 0x180, 0xC, 0x18, 0xE0, 0x78, 0x7F40);

    y += 0x10;
    index = which * 4;
    next = GameQueueSpriteTrans(base, next, y, 0x180, D_8007D58A[index], 0x18, D_8007D588[index], D_8007D589[index], 0x7F40);

    y += D_8007D58B[index];
    if (which == 4) {
        next = GameQueueSpriteTrans(base, next, y, 0x180, 0x30, 0x18, 0, 0x78, 0x7F40);
        y += 0x34;
    }

    next = GameQueueSpriteTrans(base, next, y, 0x180, 0xC, 0x18, 0xEC, 0x78, 0x7F40);
    *(s32 *)0x1F800000 = GameQueueDrawModePrimWide(base, next, 0x3F);
}

/* Two glyphs plus a label naming the connected pad; caches the last valid g_PadType. */
void DrawPadTypeHint(void);
void DrawPadTypeHint(void) {
    s32 color;
    s32 y2;
    s32 h;
    s32 w;
    s32 y0;
    s32 base;
    register s32 raw_base asm("$2");
    register s32 value asm("$3");
    s32 scratch;
    s32 next;

    raw_base = (s32)g_DrawBuffer;
    value = g_PadType;
    scratch = *(s32 *)0x1F800000;
    next = scratch;
    base = raw_base + 0xCC;

    if (value != 0x41 && value != 0x23) {
        value = g_LastValidPadType;
    } else {
        raw_base = g_PadType;
        asm("" : "=r"(raw_base) : "0"(raw_base));
        value = (u8)raw_base;
        g_LastValidPadType = raw_base;
    }


    y0 = value == 0x23 ? 0xA0 : 0x90;
    w = 8;
    h = 0x10;
    y2 = 0xB8;
    color = 0x7F40;

    next = GameQueueSpriteTrans(base, next, 0x7A, 0x1A0, w, h, y0, y2, color);
    next = GameQueueSpriteTrans(base, next, 0x92, 0x1A0, w, h, y0 + 8, y2, color);
    next = GameQueueSpriteTrans(base, next, 0x58, 0x1A0, 0x90, h, 0, y2, color);
    *(s32 *)0x1F800000 = GameQueueDrawModePrimWide(base, next, 0x3F);
}
