#include "common.h"
#include "psyq/gpu.h"
#include "game/race.h"
#include "game/asset.h"

void LoadImage(Rect *rect, void *data) asm("func_80065B24");
void func_800658FC(s32 mode);

void GameUploadImageBlock(void *arg0) asm("func_8001A2E0");
void GameUploadImageBlock(void *arg0) {
    GameImageBlock *block;
    u16 rect[4];
    u32 width;
    u32 height;
    s32 flags;

    arg0 = (u8 *)arg0 + 4;
    flags = *(s32 *)arg0;
    arg0 = (u8 *)arg0 + 4;

    if (flags & 8) {
        block = arg0;
        rect[0] = block->x;
        rect[1] = block->y;
        rect[2] = block->w;
        rect[3] = block->h;
        LoadImage((Rect *)rect, block->pixels);
        func_800658FC(0);
        arg0 = (u8 *)block + (((u32)block->size >> 2) << 2);
    }

    block = arg0;
    rect[0] = block->x;
    rect[1] = block->y;
    width = rect[2] = block->w;
    rect[3] = height = block->h;
    if (((s32)(width << 16) > 0) && ((s32)(height << 16) > 0)) {
        LoadImage((Rect *)rect, block->pixels);
        func_800658FC(0);
    }
}

void GameUploadImageBlock(void *arg0) asm("func_8001A2E0");

void GameUploadImageAsset(void *arg0) asm("func_8001A3C0");
void GameUploadImageAsset(void *arg0) {
    union {
        s32 offset;
        u8 *next;
    } state;
    u8 *ptr;

    ptr = (u8 *)arg0 + 4;
    goto test;

for (;;) {
    state.offset = (u32)state.offset >> 2;
    state.offset <<= 2;
    state.next = ptr + state.offset;
    GameUploadImageBlock(ptr);
    ptr = state.next;

test:
    state.offset = *(s32 *)ptr;
    if (state.offset > 0) {
        ptr += 4;
        continue;
    }
break;
}
}

extern Rect g_TrackTextureRect asm("D_8007C710");
extern Rect g_TeamLogoClutLoadRect asm("D_8007C718");
extern GpuRectPacked g_TeamLogoClutMoveRect asm("D_8007C720");
extern u16 g_TeamLogoClut[] asm("D_801E444C");

void LoadImage(Rect *rect, void *data) asm("func_80065B24");
void func_80065B88(Rect *rect, void *data);
s32 func_80065BEC(GpuRectPacked *rect, u32 x, u32 y);
void func_800658FC(s32 mode);

void func_8001A40C(void *dst) {
    g_TeamLogoClut[0] = 0x8000;
    LoadImage(&g_TeamLogoClutLoadRect, g_TeamLogoClut);

    if (g_GrandPrixSeries != 0) {
        func_80065BEC(&g_TeamLogoClutMoveRect, 0x3F0, 0xE2);
    }

    func_80065B88(&g_TrackTextureRect, dst);
    func_800658FC(0);
    g_TeamLogoClut[0] = 0;
}


void GameUploadImageAsset(void *arg0);

void GameUploadLoadBufferImage(void) asm("func_8001A498");
void GameUploadLoadBufferImage(void) {
    GameUploadImageAsset(g_LoadBuffer);
}
