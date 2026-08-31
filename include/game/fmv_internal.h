#ifndef GAME_FMV_INTERNAL_H
#define GAME_FMV_INTERNAL_H

#include "common.h"
#include "psyq/gpu.h"

typedef union FmvStripRectTable {
    Rect rects[2];
    volatile s16 components[2][4];
} FmvStripRectTable;

extern FmvStripRectTable g_FmvStripRects;
extern volatile u32 *g_FmvVlcBuffer0;
extern volatile u32 *g_FmvVlcBuffer1;
extern volatile u32 *g_FmvStripBuffer0;
extern volatile u32 *g_FmvStripBuffer1;
extern volatile u16 g_DispEnv0Y;
extern volatile u16 g_DispEnv1X;
extern volatile u16 g_DispEnv1Y;
extern s32 g_FrameParity;
extern s32 g_StreamReturnScene;

typedef union FmvDisplayCoordinateAddress {
    const volatile u16 *live;
    const u16 *stable;
} FmvDisplayCoordinateAddress;

static inline u16 ReadStableFmvDisplayCoordinate(const volatile u16 *coordinate) {
    FmvDisplayCoordinateAddress address;

    address.live = coordinate;
    return *address.stable;
}
#endif
