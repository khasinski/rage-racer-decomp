#include "common.h"
#include "game/fmv.h"
#include "game/fmv_internal.h"

/* Carves the five FMV working buffers out of one block: two 0x28000 VLC
 * buffers, two 0x2D00 strip buffers and whatever is left as the sector ring. */
void SetupFmvBuffers(u32 base) {
    u32 step;
    u32 temp;

    step = 0x28000;
    g_FmvVlcBuffer0 = (volatile u32 *)base;
    base += step;
    g_FmvVlcBuffer1 = (volatile u32 *)base;
    base += step;
    temp = base + 0x2D00;
    g_FmvStripBuffer0 = (volatile u32 *)base;
    base += 0x5A00;
    g_FmvStripBuffer1 = (volatile u32 *)temp;
    g_FmvRingBuffer = base;
}
