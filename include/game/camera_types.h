#ifndef GAME_CAMERA_TYPES_H
#define GAME_CAMERA_TYPES_H

#include "common.h"

typedef struct CamRow {
    s32 textureSectionLo;
    s32 textureSectionHi;
    s32 environmentScriptOffset;
    s16 axis0;
    u16 axis1;
    u16 axis2;
    s16 horizon;
} CamRow;

#endif
