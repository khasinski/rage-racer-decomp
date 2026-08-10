#ifndef RAGE_PC_PSYQ_GPU_CACHE_TYPES_H
#define RAGE_PC_PSYQ_GPU_CACHE_TYPES_H

#include "common.h"

typedef struct GpuDisplayEnvironmentCache {
    volatile u_short displayX;
    volatile u_short displayY;
    volatile u_short displayWidth;
    volatile u_short displayHeight;
    volatile u_short screenX;
    volatile u_short screenY;
    volatile u_short screenWidth;
    volatile u_short screenHeight;
    u_long flags;
} GpuDisplayEnvironmentCache;

#endif
