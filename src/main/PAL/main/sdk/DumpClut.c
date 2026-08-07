#include "common.h"
#include "psyq/gpu.h"

extern char D_8001338C[];

void DumpClut(long clut) {
    GPU_printf(D_8001338C, (clut & 0x3F) << 4, (clut & 0xFFFF) >> 6);
}
