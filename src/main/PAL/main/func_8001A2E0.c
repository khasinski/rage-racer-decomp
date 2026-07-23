#include "common.h"
#include "psyq/gpu.h"

void func_80065B24(Rect *rect, void *data);
void func_800658FC(s32 mode);

void func_8001A2E0(void *arg0) {
    u8 *block;
    u16 rect[4];
    u32 width;
    u32 height;
    s32 flags;

    arg0 = (u8 *)arg0 + 4;
    flags = *(s32 *)arg0;
    arg0 = (u8 *)arg0 + 4;

    if (flags & 8) {
        block = arg0;
        rect[0] = *(u16 *)(block + 4);
        rect[1] = *(u16 *)(block + 6);
        rect[2] = *(u16 *)(block + 8);
        rect[3] = *(u16 *)(block + 0xA);
        func_80065B24((Rect *)rect, block + 0xC);
        func_800658FC(0);
        arg0 = block + (((u32)*(s32 *)block >> 2) << 2);
    }

    block = arg0;
    rect[0] = *(u16 *)(block + 4);
    rect[1] = *(u16 *)(block + 6);
    width = rect[2] = *(u16 *)(block + 8);
    rect[3] = height = *(u16 *)(block + 0xA);
    if (((s32)(width << 16) > 0) && ((s32)(height << 16) > 0)) {
        func_80065B24((Rect *)rect, block + 0xC);
        func_800658FC(0);
    }
}
