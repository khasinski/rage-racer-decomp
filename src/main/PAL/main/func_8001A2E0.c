#include "common.h"
#include "psyq/gpu.h"
#include "game/race.h"
#include "game/asset.h"

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

void func_8001A2E0(void *arg0);

void func_8001A3C0(void *arg0) {
    union {
        s32 offset;
        u8 *next;
    } state;
    u8 *ptr;

    ptr = (u8 *)arg0 + 4;
    goto test;

loop:
    state.offset = (u32)state.offset >> 2;
    state.offset <<= 2;
    state.next = ptr + state.offset;
    func_8001A2E0(ptr);
    ptr = state.next;

test:
    state.offset = *(s32 *)ptr;
    if (state.offset > 0) {
        ptr += 4;
        goto loop;
    }
}

extern Rect D_8007C710;
extern Rect D_8007C718;
extern GpuRectPacked D_8007C720;
extern u16 D_801E444C[];

void func_80065B24(Rect *rect, void *data);
void func_80065B88(Rect *rect, void *data);
s32 func_80065BEC(GpuRectPacked *rect, u32 x, u32 y);
void func_800658FC(s32 mode);

void func_8001A40C(void *dst) {
    D_801E444C[0] = 0x8000;
    func_80065B24(&D_8007C718, D_801E444C);

    if (g_GrandPrixSeries != 0) {
        func_80065BEC(&D_8007C720, 0x3F0, 0xE2);
    }

    func_80065B88(&D_8007C710, dst);
    func_800658FC(0);
    D_801E444C[0] = 0;
}


void func_8001A3C0(void *arg0);

void func_8001A498(void) {
    func_8001A3C0(g_LoadBuffer);
}
