#include "common.h"
#include "psyq/gpu.h"

extern s32 g_FmvRingBuffer;
extern s32 g_StreamLoc;
void DecDCTReset(s32 arg0);
void DecDCToutCallback(s32 arg0);
void StSetRing(s32 arg0, s32 arg1);
void StSetStream(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 StartStreamRead(s32 arg0);
void OpenFmvStream(s32 arg0) {
    DecDCTReset(0);
    DecDCToutCallback(arg0);
    StSetRing(g_FmvRingBuffer, 0x20);
    StSetStream(1, 1, -1, 0, 0);
    StartStreamRead(g_StreamLoc);
}

extern volatile u32 *g_FmvStripBuffers[];
extern Rect g_FmvStripRects[];
extern volatile s32 g_FmvStripIndex;
extern volatile s32 g_FmvStripRectIndex;
/* One 4-halfword Rect at 0x8009AF4C. g_FmvUploadRectX/Y are its x/y and
 * g_FmvStripWidth/Height are its w/h: an MDEC strip and the VRAM rect it is
 * uploaded to are the same object, which is why the four halfwords are
 * declared as scalars, the x volatile because it is bumped one strip at a
 * time; the LoadImage copy reads the same four halfwords back as a Rect. */
extern volatile s16 g_FmvUploadRectX;
extern volatile s16 g_FmvUploadRectY;
extern s16 g_FmvStripWidth;
extern s16 g_FmvStripHeight;
extern volatile s32 g_FmvStripDone;
extern s32 g_StInterruptPending;

void DecDCTout(volatile u32 *arg0, s32 arg1);
void LoadImage(Rect *rect, void *data);
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
        /* RAW() holds the X store ahead of the Y load -- see common.h. The
         * remaining barrier is load-bearing: the pair cannot both go, and
         * widening RAW() over the branch condition does not help either. */
        g_FmvUploadRectX = RAW(g_FmvStripRects[next].x);
        asm("" : : : "memory");
        g_FmvUploadRectY = g_FmvStripRects[next].y;
    }

    oldOffset = oldBuffer * 4;
    LoadImage(&rect, (void *)*(volatile u32 **)((s32)g_FmvStripBuffers + oldOffset));
}
