#include <sys/types.h>

#include "common.h"
#include "psyq/cd.h"

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

extern long g_StRingBase;
extern long g_StRingSize;
/* StSetRing: installs the stream ring buffer (`base`, `size`) then clears it. */
void StSetRing(void *base, long size) { g_StRingBase = (long)base; g_StRingSize = size; StClearRing(); }

/* CdGetToc: reads the disc table of contents into `toc` (thin wrapper over
 * CdGetToc2 / CdGetToc2 with track count 1). */
long CdGetToc(CdlLOC *toc) {
    return CdGetToc2(1, (u_char *)toc);
}
