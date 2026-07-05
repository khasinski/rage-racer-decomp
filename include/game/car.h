#ifndef GAME_CAR_H
#define GAME_CAR_H

#include "common.h"

typedef struct GameCarEntry {
    u8 modelVariant;
    u8 pad1[4];
    u8 enabled;
    u8 pad6[2];
} GameCarEntry;

#endif
