#ifndef GAME_RENDER_TYPES_H
#define GAME_RENDER_TYPES_H

#include "common.h"
#include "game/vector.h"
#include "psyq/gpu.h"

typedef union RenderBufferAddress {
    s32 byteOffset;
    u8 *bytes;
    void *pointer;
    DrawPacket *drawPacket;
    SPRT *sprite;
    volatile SPRT *volatileSprite;
    SPRT_8 *sprite8;
    volatile SPRT_8 *volatileSprite8;
    TILE *tile;
    LINE_F2 *lineF2;
    LINE_F3 *lineF3;
    LINE_G2 *lineG2;
    POLY_F3 *polyF3;
    POLY_F4 *polyF4;
    POLY_FT4 *polyFT4;
    POLY_G4 *polyG4;
    CVec *color;
} RenderBufferAddress;

#endif
