#include "common.h"
#include "psyq/gpu.h"

void LoadImage(Rect *rect, void *data) asm("func_80065B24");
void DrawSync(s32 mode) asm("func_800658FC");

void func_8001A2E0(void *arg0) {
    u8 *block;
    u8 *cursor;
    u16 rect[4];
    u32 width;
    u32 height;
    s32 flags;

    cursor = (u8 *)arg0 + 4;
    __asm__ volatile("" : "=r"(cursor) : "0"(cursor));
    flags = *(s32 *)cursor;
    cursor += 4;

    if (flags & 8) {
        block = cursor;
        rect[0] = *(u16 *)(block + 4);
        rect[1] = *(u16 *)(block + 6);
        rect[2] = *(u16 *)(block + 8);
        rect[3] = *(u16 *)(block + 0xA);
        LoadImage((Rect *)rect, block + 0xC);
        DrawSync(0);
        cursor = block + (((u32)*(s32 *)block >> 2) << 2);
    }

    block = cursor;
    rect[0] = *(u16 *)(block + 4);
    rect[1] = *(u16 *)(block + 6);
    width = rect[2] = *(u16 *)(block + 8);
    rect[3] = height = *(u16 *)(block + 0xA);
    if (((s32)(width << 16) > 0) && ((s32)(height << 16) > 0)) {
        LoadImage((Rect *)rect, block + 0xC);
        DrawSync(0);
    }
}
