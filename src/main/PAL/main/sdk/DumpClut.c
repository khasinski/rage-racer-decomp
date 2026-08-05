#include "common.h"
#include "psyq/gpu.h"

extern char D_8001338C[];

void DumpClut(long arg0) {
    GPU_printf(D_8001338C, (arg0 & 0x3F) << 4, (arg0 & 0xFFFF) >> 6);
}
