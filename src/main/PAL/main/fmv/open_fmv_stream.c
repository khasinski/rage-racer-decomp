#include "common.h"
#include "game/asset.h"
#include "psyq/gpu.h"
#include "game/render.h"

extern s32 g_FmvRingBuffer;
void DecDCToutCallback(s32 arg0);
void StSetRing(s32 arg0, s32 arg1);
void StSetStream(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 StartStreamRead(s32 arg0);
void OpenFmvStream(s32 arg0) {
    DecDCTReset(0);
    DecDCToutCallback(arg0);
    StSetRing(g_FmvRingBuffer, 0x20);
    StSetStream(1, 1, -1, 0, 0);
    StartStreamRead((s32)g_StreamLoc);
}

extern Rect g_FmvStripRects[];
extern volatile s32 g_FmvStripIndex;
extern s32 g_StInterruptPending;

void DecDCTout(volatile u32 *arg0, s32 arg1);
void StCdInterrupt(void);

void UploadFmvSlice(void) {
    Rect rect;
    register volatile s32 *bufferPtr asm("$6");
    s32 oldBuffer;
    s32 state;
    s32 bufferIndex;
s32 bufferAddr;
    s32 pixelCount;
    s32 next;
    s32 oldOffset;
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

    if ((s16)x < (g_FmvStripRects[index].x + g_FmvStripRects[index].w)) {
        signedStep = (s16)step;
        pixelCount = signedStep * g_FmvStripHeight;
        bufferIndex = g_FmvStripIndex;
        bufferAddr = bufferIndex << 2;
        asm("" : "=r"(bufferPtr) : "0"(bufferPtr));
        { s32 rel = bufferAddr; bufferAddr = (s32)bufferPtr + rel; }
        DecDCTout(*(volatile u32 **)(bufferAddr - 8), pixelCount / 2);
    } else {
        g_FmvStripDone = 1;
        next = index == 0;
        g_FmvStripRectIndex = next;
        {
            /* Both halves are read out of the rect by hand.  Written as
             * g_FmvStripRects[next].x/.y the loads carry the struct alias
             * mark, so gcc 2.6.3 lets them float above the plain-global
             * stores that surround them and the four accesses interleave
             * the wrong way; addressed as offsets they stay put. */
            s32 rectOffset = next * 8;
            g_FmvUploadRectX = *(s16 *)((s32)g_FmvStripRects + rectOffset);
            g_FmvUploadRectY = *(s16 *)((s32)g_FmvStripRects + rectOffset + 2);
        }
    }

    oldOffset = oldBuffer * 4;
    LoadImage(&rect, (void *)*(volatile u32 **)((s32)g_FmvStripBuffers + oldOffset));
}
