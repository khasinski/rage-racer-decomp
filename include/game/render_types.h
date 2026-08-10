#ifndef GAME_RENDER_TYPES_H
#define GAME_RENDER_TYPES_H

#include "common.h"
#include "game/vector.h"
#include "psyq/gpu.h"

typedef union RenderBufferAddress {
    s32 byteOffset;
    u8 *bytes;
    void *pointer;
    SPRT *sprite;
    SPRT_8 *sprite8;
    volatile SPRT_8 *volatileSprite8;
    TILE *tile;
    POLY_F4 *polyF4;
    POLY_G4 *polyG4;
    CVec *color;
} RenderBufferAddress;

#endif
