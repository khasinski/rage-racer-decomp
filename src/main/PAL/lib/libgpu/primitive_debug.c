#include "common.h"
#include "psyq/gpu.h"

extern char g_FmtGpuClut[];

void DumpClut(long clut) {
    GPU_printf(g_FmtGpuClut, (clut & 0x3F) << 4, (clut & 0xFFFF) >> 6);
}
