#include "common.h"
#include <stdio.h>
#include "game/render.h"
#include "psyq/cd.h"
#include "game/race.h"
#include "game/state.h"
#include "psyq/gpu.h"
#include "psyq/kernel.h"
#include "game/fmv_internal.h"

typedef struct FmvDisplayState {
    u8 pad0[0x18];
    u16 displayRects[2][4];
    s32 field_28;
    u16 field_2C;
    u16 field_2E;
    u8 pad2[0x4];
    s32 field_34;
} FmvDisplayState;

s32 PresentFmvFrame(s32 *ctx) {
    void *p;
    s32 retry;
    for (retry = 1; retry != 0; retry--) {
        p = GetFmvFrame(ctx);
        if (p != 0) {
            ctx[2] = (ctx[2] == 0);
            MdecUnpackStatus(p, ctx[ctx[2]]);
            return StFreeRing(p);
        }
    }
    return -1;
}

void *GetFmvFrame(s32 *ctx) {
    StRingEventRecord *slot[2];
    u16 rect[4];
    s32 count;
    u16 *dst;
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
    if (entry->frame >= (u32)g_StreamSectorCount) {
        g_FmvStreamEnded = 1;
    }
    w = entry->width;
    if ((g_FmvFrameWidth != w) || (g_FmvFrameHeight != entry->height)) {
        h = entry->height;
        rect[0] = 0;
        rect[1] = 0;
        rect[2] = w * 3 / 2;
        rect[3] = 0x1E0;
        g_FmvFrameWidth = w;
        g_FmvFrameHeight = h;
        ClearImage(rect, 0, 0, 0);
    }

    dst = (u16 *)ctx;
    ret = slot[0];
    h32 = g_FmvFrameHeight;
    w32 = g_FmvFrameWidth;
    half = (0xF0 - h32) / 2;
    dst[0xD] = g_DispEnv0Y + half;
    __asm__ __volatile__("" ::);
    {
        u32 wm = (u32)w32 * 3;
        hgt16 = wm >> 31;
        wdraw = (u16)((wm + hgt16) >> 1);
    }
    dst[0x12] = wdraw;
    dst[0xE] = wdraw;
    wid16 = g_FmvFrameWidth;
    c067e = g_DispEnv1Y;
    hgt16 = g_FmvFrameHeight;
    dst[0x13] = hgt16;
    dst[0xF] = hgt16;
    dst[0x19] = hgt16;
    c067e += half;
    dst[0x11] = c067e;
    g_DispEnv1W = wid16;
    g_DispEnv0W = wid16;
    g_DispEnv1H = hgt16;
    g_DispEnv0H = hgt16;
    return ret;
}

void WaitFmvDecode(FmvDisplayState *state) {
    volatile s32 timeout = 0x800000;
    s32 one;
    u16 x;

    if (state->field_34 == 0) {
        one = 1;
        do {
            timeout = timeout - 1;
            if (timeout == 0) {
                printf(g_MsgFmvDecodeTimeout);
                state->field_34 = one;
                state->field_28 = state->field_28 < 1U;
                x = state->displayRects[state->field_28][0];
                state->field_2C = x;
                state->field_2E = state->displayRects[state->field_28][1];
            }
        } while (state->field_34 == 0);
    }

    state->field_34 = 0;
}

void StartStreamRead(void *loc) {
    u8 byte;

outer:
    while (CdSync(1, 0) == 0) {
    }

    byte = 0x80;
    while (CdControl(0xE, &byte, 0) == 0) {
    }

pollFirst:
    switch (CdSync(1, 0)) {
    case 0:
        goto pollFirst;
    case 2:
        VSync(3);
        break;
    }

send:
    while (CdControl(0x15, loc, 0) == 0) {
    }

pollNext:
    switch (CdSync(1, 0)) {
    case 0:
        goto pollNext;
    case 2:
        break;
    default:
        goto send;
    }

    if (CdRead2(0x1E0) == 0) {
        goto outer;
    }
}
