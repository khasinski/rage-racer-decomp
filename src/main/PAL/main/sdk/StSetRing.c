#include <sys/types.h>

#include "common.h"

void StSetRing(long base, long size);
long CdGetToc(long toc);

u_long func_8006A018[16] __attribute__((section(".text"))) = {
    0x241A0100,
    0x8F5A0008,
    0x00000000,
    0x8F5A0000,
    0x00000000,
    0x235A0008,
    0xAF410004,
    0xAF420008,
    0xAF43000C,
    0xAF5F007C,
    0x40026800,
    0x00000000,
    0x240A00A0,
    0x01400008,
    0x24090044,
    0x00000000,
};

extern long g_StRingBase asm("D_801E8AAC");
extern long g_StRingSize;
void StClearRing(void);

/* StSetRing: installs the stream ring buffer (`base`, `size`) then clears it. */
void StSetRing(long base, long size);
void StSetRing(long base, long size) { g_StRingBase = base; g_StRingSize = size; StClearRing(); }

long CdGetToc2(long arg0, long arg1);

/* CdGetToc: reads the disc table of contents into `toc` (thin wrapper over
 * CdGetToc2 / CdGetToc2 with track count 1). */
long CdGetToc(long toc);
long CdGetToc(long toc) {
    return CdGetToc2(1, toc);
}
