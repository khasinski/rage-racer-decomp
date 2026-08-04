#include "common.h"

extern u32 g_FmvRingBuffer asm("D_8009AF58");
extern u32 g_FmvVlcBuffer0 asm("D_8009AF5C");
extern u32 g_FmvVlcBuffer1 asm("D_8009AF60");
extern u32 g_FmvStripBuffer0 asm("D_8009AF64");
extern u32 g_FmvStripBuffer1 asm("D_8009AF68");

void SetupFmvBuffers(u32 arg0);
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
