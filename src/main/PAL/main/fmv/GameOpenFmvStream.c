#include "common.h"
#include "psyq/gpu.h"

extern s32 g_FmvRingBuffer asm("D_8009AF58");
extern s32 g_StreamLoc asm("D_801E8A90");
void func_80063E44(s32 arg0);
void DecDCToutCallback(s32 arg0) asm("func_800640B0");
void StSetRing(s32 arg0, s32 arg1) asm("func_8006A058");
void StSetStream(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) asm("func_8006CF68");
s32 GameStartStreamRead(s32 arg0) asm("func_8001F018");
void GameOpenFmvStream(s32 arg0) asm("func_8001EB5C");
void GameOpenFmvStream(s32 arg0) {
    func_80063E44(0);
    DecDCToutCallback(arg0);
    StSetRing(g_FmvRingBuffer, 0x20);
    StSetStream(1, 1, -1, 0, 0);
    GameStartStreamRead(g_StreamLoc);
}

extern volatile u32 *g_FmvStripBuffers[] asm("D_8009AF2C");
extern Rect g_FmvStripRects[] asm("D_8009AF38");
extern volatile s32 g_FmvStripIndex asm("D_8009AF34");
extern volatile s32 g_FmvStripRectIndex asm("D_8009AF48");
/* One 4-halfword Rect at 0x8009AF4C. g_FmvUploadRectX/Y are its x/y and
 * g_FmvStripWidth/Height are its w/h: an MDEC strip and the VRAM rect it is
 * uploaded to are the same object, which is why the four halfwords are
 * declared both as a whole Rect (for the LoadImage copy) and as scalars (the
 * x is bumped one strip at a time, so it has to be volatile). */
extern Rect g_FmvUploadRect asm("D_8009AF4C");
extern volatile s16 g_FmvUploadRectX asm("D_8009AF4C");
extern volatile s16 g_FmvUploadRectY asm("D_8009AF4E");
extern s16 g_FmvStripWidth asm("D_8009AF50");
extern s16 g_FmvStripHeight asm("D_8009AF52");
extern volatile s32 g_FmvStripDone asm("D_8009AF54");
extern s32 g_StInterruptPending asm("D_8019CA00");

void DecDCTout(volatile u32 *arg0, s32 arg1) asm("func_8006402C");
void LoadImage(Rect *rect, void *data) asm("func_80065B24");
void StCdInterrupt(void) asm("func_8006D1D0");

void GameUploadFmvSlice(void) asm("func_8001EBC8");
void GameUploadFmvSlice(void) {
    Rect rect;
    /* This pin is load-bearing: removing it changes .text. */
    register volatile s32 *bufferPtr asm("$6");
    s32 oldBuffer;
    s32 state;
    s32 bufferIndex;
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 bufferAddr asm("$2");
    s32 pixelCount;
    register s32 next asm("$2");
    s32 oldOffset;
    s32 index;
    /* These pins are load-bearing: removing any one changes .text. */
    register u16 x asm("$2");
    register u16 step asm("$7");
    register s32 signedStep asm("$2");

    if (g_StInterruptPending != 0) {
        StCdInterrupt();
        g_StInterruptPending = 0;
    }

    rect = g_FmvUploadRect;

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
        /* This barrier is load-bearing: removing it changes .text. */
        asm("" : "=r"(bufferPtr) : "0"(bufferPtr));
        bufferAddr = (s32)bufferPtr + bufferAddr;
        DecDCTout(*(volatile u32 **)(bufferAddr - 8), pixelCount / 2);
    } else {
        g_FmvStripDone = 1;
        /* This barrier is load-bearing: removing it changes .text. */
        asm("" : : : "memory");
        next = index == 0;
        g_FmvStripRectIndex = next;
        /* These barriers are load-bearing: removing any one changes .text. */
        asm("" : : : "memory");
        g_FmvUploadRectX = g_FmvStripRects[next].x;
        asm("" : : : "memory");
        g_FmvUploadRectY = g_FmvStripRects[next].y;
    }

    oldOffset = oldBuffer << 2;
    LoadImage(&rect, (void *)*(volatile u32 **)((s32)g_FmvStripBuffers + oldOffset));
}
