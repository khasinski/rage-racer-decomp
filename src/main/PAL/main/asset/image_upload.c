#include "common.h"
#include "psyq/gpu.h"
#include "game/race.h"
#include "game/asset.h"

void LoadImage(Rect *rect, void *data);
void DrawSync(long mode);

void UploadImageBlock(void *arg0);
void UploadImageBlock(void *arg0) {
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
        DrawSync(0);
        arg0 = (u8 *)block + (((u32)block->size >> 2) << 2);
    }

    block = arg0;
    rect[0] = block->x;
    rect[1] = block->y;
    width = rect[2] = block->w;
    rect[3] = height = block->h;
    if (((s32)(width << 16) > 0) && ((s32)(height << 16) > 0)) {
        LoadImage((Rect *)rect, block->pixels);
        DrawSync(0);
    }
}

void UploadImageBlock(void *arg0);

void UploadImageAsset(void *arg0);
void UploadImageAsset(void *arg0) {
    union {
        s32 offset;
        u8 *next;
    } state;
    u8 *ptr;

    ptr = (u8 *)arg0 + 4;
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

extern Rect g_TrackTextureRect;
extern Rect g_TeamLogoClutLoadRect;
extern GpuRectPacked g_TeamLogoClutMoveRect;
extern u16 g_TeamLogoClut[];

void LoadImage(Rect *rect, void *data);
void StoreImage(Rect *rect, void *data);
void DrawSync(long mode);

void StoreTeamLogoImage(void *dst);
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


void UploadImageAsset(void *arg0);

void UploadLoadBufferImage(void);
void UploadLoadBufferImage(void) {
    UploadImageAsset(g_LoadBuffer);
}
