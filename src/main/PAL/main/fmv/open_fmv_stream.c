#include "common.h"
#include "game/fmv.h"
#include "game/asset.h"
#include "psyq/gpu.h"
#include "game/render.h"
#include "game/fmv_decode_internal.h"
#include "game/fmv_internal.h"
#include "psyq/press_internal.h"

void OpenFmvStream(s32 callback) {
    DecDCTReset(0);
    DecDCToutCallback(callback);
    StSetRing(g_FmvRingBuffer, 0x20);
    StSetStream(1, 1, -1, 0, 0);
    StartStreamRead(g_StreamLoc);
}

void UploadFmvSlice(void) {
    Rect rect;
    register volatile s32 *bufferPtr asm("$6");
    s32 oldBuffer;
    s32 state;
    s32 bufferIndex;
    s32 bufferAddr;
    volatile u32 **bufferAddress;
    s32 pixelCount;
    s32 next;
    s32 index;
    register u16 x asm("$2");
    register u16 step asm("$7");
    register s32 signedStep asm("$2");

    if (g_StInterruptPending != 0) {
        StCdInterrupt();
        g_StInterruptPending = 0;
    }

    rect = *(Rect *)&g_FmvUploadRectX;

    bufferPtr = &g_FmvStripIndex;
    oldBuffer = *bufferPtr;
    state = *bufferPtr;
    x = g_FmvUploadRectX;
    step = g_FmvStripWidth;
    *bufferPtr = state == 0;

    index = g_FmvStripRectIndex;
    x += step;
    g_FmvUploadRectX = x;

    if ((s16)x < (g_FmvStripRects.rects[index].x +
                  g_FmvStripRects.rects[index].w)) {
        signedStep = (s16)step;
        pixelCount = signedStep * g_FmvStripHeight;
        bufferIndex = g_FmvStripIndex;
        bufferAddr = bufferIndex << 2;
        asm("" : "=r"(bufferPtr) : "0"(bufferPtr));
        {
            s32 rel = bufferAddr;
            bufferAddr = (s32)bufferPtr + rel;
        }
        bufferAddress = (volatile u32 **)bufferAddr;
        DecDCTout(bufferAddress[-2], pixelCount / 2);
    } else {
        g_FmvStripDone = 1;
        next = index == 0;
        g_FmvStripRectIndex = next;
        g_FmvUploadRectX = g_FmvStripRects.components[next][0];
        g_FmvUploadRectY = g_FmvStripRects.components[next][1];
    }

    LoadImage(&rect, (void *)g_FmvStripBuffers[oldBuffer]);
}
