#include "common.h"

extern u32 g_FmvRingBuffer;
extern u32 g_FmvVlcBuffer0;
extern u32 g_FmvVlcBuffer1;
extern u32 g_FmvStripBuffer0;
extern u32 g_FmvStripBuffer1;

/* Carves the five FMV working buffers out of one block: two 0x28000 VLC
 * buffers, two 0x2D00 strip buffers and whatever is left as the sector ring. */
void SetupFmvBuffers(u32 base) {
    u32 step;
    u32 temp;

    step = 0x28000;
    g_FmvVlcBuffer0 = base;
    base += step;
    g_FmvVlcBuffer1 = base;
    base += step;
    temp = base + 0x2D00;
    g_FmvStripBuffer0 = base;
    base += 0x5A00;
    g_FmvStripBuffer1 = temp;
    g_FmvRingBuffer = base;
}
