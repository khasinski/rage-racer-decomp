#include "common.h"

extern char D_8001338C[];
/* libgpu's printf hook; every GPU trace string goes through it. */
extern void (*GPU_printf)(char *, ...) asm("D_800941E4");

void DumpClut(long arg0);
void DumpClut(long arg0) {
    GPU_printf(D_8001338C, (arg0 & 0x3F) << 4, (arg0 & 0xFFFF) >> 6);
}
