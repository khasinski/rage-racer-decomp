#include "common.h"
#include "psyq/gpu.h"
#include "psyq/gpu_internal.h"


void DumpClut(long clut) {
    GPU_printf(g_FmtGpuClut, (clut & 0x3F) << 4, (clut & 0xFFFF) >> 6);
}
