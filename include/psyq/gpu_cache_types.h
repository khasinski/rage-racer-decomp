#ifndef RAGE_PC_PSYQ_GPU_CACHE_TYPES_H
#define RAGE_PC_PSYQ_GPU_CACHE_TYPES_H

#include "common.h"

typedef struct GpuDisplayEnvironmentCache {
    volatile u_short x0;
    volatile u_short x2;
    volatile u_short x4;
    volatile u_short x6;
    volatile u_short x8;
    volatile u_short xA;
    volatile u_short xC;
    volatile u_short xE;
    u_long x10;
} GpuDisplayEnvironmentCache;

#endif
