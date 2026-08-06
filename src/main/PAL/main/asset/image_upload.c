#include "common.h"
#include "psyq/gpu.h"
#include "game/race.h"
#include "game/asset.h"

extern u16 g_TeamLogoClut[];

void UploadImageBlock(void *asset) {
    GameImageBlock *block;
    u16 rect[4];
    u32 width;
    u32 height;
    s32 flags;

    asset = (u8 *)asset + 4;
    flags = *(s32 *)asset;
    asset = (u8 *)asset + 4;

    if (flags & 8) {
        block = asset;
        rect[0] = block->x;
        rect[1] = block->y;
        rect[2] = block->w;
        rect[3] = block->h;
        LoadImage((Rect *)rect, block->pixels);
        DrawSync(0);
        asset = (u8 *)block + (((u32)block->size >> 2) << 2);
    }

    block = asset;
    rect[0] = block->x;
    rect[1] = block->y;
    width = rect[2] = block->w;
    rect[3] = height = block->h;
    if (((s32)(width << 16) > 0) && ((s32)(height << 16) > 0)) {
        LoadImage((Rect *)rect, block->pixels);
        DrawSync(0);
    }
}

void UploadImageAsset(void *asset) {
    union {
        s32 offset;
        u8 *next;
    } state;
    u8 *ptr;

    ptr = (u8 *)asset + 4;
    goto test;

for (;;) {
    state.offset = (u32)state.offset / 4;
    state.offset <<= 2;
    state.next = ptr + state.offset;
    UploadImageBlock(ptr);
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

void StoreTeamLogoImage(void *dst) {
    g_TeamLogoClut[0] = 0x8000;
    LoadImage(&g_TeamLogoClutLoadRect, g_TeamLogoClut);

    if (g_GrandPrixSeries != 0) {
        MoveImage(&g_TeamLogoClutMoveRect, 0x3F0, 0xE2);
    }

    StoreImage(&g_TrackTextureRect, dst);
    DrawSync(0);
    g_TeamLogoClut[0] = 0;
}

void UploadLoadBufferImage(void) {
    UploadImageAsset(g_LoadBuffer);
}
