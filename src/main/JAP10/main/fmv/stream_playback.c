#include "common.h"
#include <stdio.h>
#include "game/render.h"
#include "game/fmv.h"
#include "game/asset.h"
#include "psyq/cd.h"
#include "game/race.h"
#include "game/state.h"
#include "psyq/gpu.h"
#include "psyq/kernel.h"
#include "game/fmv_internal.h"




s32 PresentFmvFrame(FmvDecodeContext *ctx) {
    void *p;
    register FmvDecodeContext *state asm("$18") = ctx;
    s32 attempts = 0x800000;

retry_frame:
    p = GetFmvFrame(state);
    attempts--;
    if (p != 0) {
        goto present_frame;
    }
    if (attempts == 0) {
        goto frame_failed;
    }
    goto retry_frame;

present_frame:
    state->vlcIndex = (state->vlcIndex == 0);
    MdecUnpackStatus(p, state->vlcBuffers[state->vlcIndex]);
    return StFreeRing(p);

frame_failed:
    ;
}

void *GetFmvFrame(FmvDecodeContext *ctx) {
    StRingEventRecord *slot[2];
    Rect rect;
    s32 count;
    StRingEventRecord *entry;
    s32 w;
    s32 h;
    s32 h32;
    s32 w32;
    register u16 hgt16 asm("$4");
    u16 wid16;
    u16 wdraw;
    s32 half;
    s32 c067e;
    s32 frameStatus;
    register void *ret asm("$2");

    count = 0x800000;
    do {
        frameStatus = StGetNext(&slot[0], &slot[1]);
        count--;
        if (frameStatus == 0) {
            break;
        }
    } while (count != 0);
    if (frameStatus != 0) {
        return 0;
    }

    entry = slot[1];
    if (entry->frame >= g_StreamSectorCount) {
        g_FmvStreamEnded = 1;
    }
    if (g_GameClock >= g_StreamSectorLimit) {
        g_FmvStreamEnded = 1;
    }
    w = entry->width;
    if ((g_FmvFrameWidth != w) || (g_FmvFrameHeight != entry->height)) {
        h = entry->height;
        rect.x = 0;
        rect.y = 0;
        rect.w = w * 3 / 2;
        rect.h = 0x1E0;
        g_FmvFrameWidth = w;
        g_FmvFrameHeight = h;
        ClearImage(&rect, 0, 0, 0);
    }

    ret = slot[0];
    h32 = g_FmvFrameHeight;
    w32 = g_FmvFrameWidth;
    half = (0xF0 - h32) / 2;
    ctx->displayRects[0].y = ReadStableFmvDisplayCoordinate(&g_DispEnv0Y) + half;
    __asm__ __volatile__("" ::);
    {
        u32 wm = w32 * 3;
        hgt16 = wm >> 31;
        wdraw = (u16)((wm + hgt16) >> 1);
    }
    ctx->displayRects[1].w = wdraw;
    ctx->displayRects[0].w = wdraw;
    wid16 = g_FmvFrameWidth;
    c067e = ReadStableFmvDisplayCoordinate(&g_DispEnv1Y);
    hgt16 = g_FmvFrameHeight;
    ctx->displayRects[1].h = hgt16;
    ctx->displayRects[0].h = hgt16;
    ctx->decodedHeight = hgt16;
    c067e += half;
    ctx->displayRects[1].y = c067e;
    g_DispEnv1W = wid16;
    g_DispEnv0W = wid16;
    g_DispEnv1H = hgt16;
    g_DispEnv0H = hgt16;
    return ret;
}

void WaitFmvDecode(FmvDecodeContext *state, s32 mode) {
    volatile s32 timeout = 0x800000;
    s32 one;
    u16 x;

    if (state->decodeComplete == 0) {
        one = 1;
        do {
            timeout = timeout - 1;
            if (timeout == 0) {
                printf(g_MsgFmvDecodeTimeout);
                state->decodeComplete = one;
                state->frameParity = state->frameParity < 1U;
                x = state->displayRects[state->frameParity].x;
                state->stripWidth = x;
                state->stripHeight = state->displayRects[state->frameParity].y;
            }
        } while (state->decodeComplete == 0);
    }

    state->decodeComplete = 0;
}

void StartStreamRead(void *loc) {
outer:
    while (CdSync(1, 0) == 0) {
    }

send:
    while (CdControl(0x15, g_StreamLoc, 0) == 0) {
    }

poll:
    switch (CdSync(1, 0)) {
    case 0:
        goto poll;
    case 2:
        break;
    default:
        goto send;
    }

    if (CdRead2(0x1C0) == 0) {
        goto outer;
    }
}
