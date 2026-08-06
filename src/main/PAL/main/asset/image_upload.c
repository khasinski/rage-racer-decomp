#include "common.h"
#include "game/menu.h"
#include "psyq/gpu.h"
#include "game/race.h"
#include "game/asset.h"


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

/*
 * Walk the chain of image blocks in an image asset. Each link is a word count
 * followed by that many bytes of GameImageBlock records; a word <= 0 ends it.
 *
 * The jump into the loop is load-bearing, not decompiler residue: every plain
 * `while` / `for (;;) { ...; if (x) break; }` spelling lets gcc 2.6.3's
 * duplicate_loop_exit_test copy the test above the loop, which costs 12
 * instructions retail does not have.
 */
void UploadImageAsset(void *asset) {
    u8 *ptr;
    s32 size;

    ptr = (u8 *)asset + 4;
    goto test;

    do {
        u8 *next = ptr + (((u32)size >> 2) << 2);
        UploadImageBlock(ptr);
        ptr = next;
    test:
        size = *(s32 *)ptr;
        ptr += 4;
    } while (size > 0);
}

void StoreTeamLogoImage(void *dst) {
    g_TeamLogoClut[0] = CLUT_STP_BIT;
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
