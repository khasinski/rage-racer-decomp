#include "common.h"

extern u32 g_FmvRingBuffer;
extern u32 g_FmvVlcBuffer0;
extern u32 g_FmvVlcBuffer1;
extern u32 g_FmvStripBuffer0;
extern u32 g_FmvStripBuffer1;

void SetupFmvBuffers(u32 arg0) {
    u32 step;
    u32 temp;

    step = 0x28000;
    g_FmvVlcBuffer0 = arg0;
    arg0 += step;
    g_FmvVlcBuffer1 = arg0;
    arg0 += step;
    temp = arg0 + 0x2D00;
    g_FmvStripBuffer0 = arg0;
    arg0 += 0x5A00;
    g_FmvStripBuffer1 = temp;
    g_FmvRingBuffer = arg0;
}
