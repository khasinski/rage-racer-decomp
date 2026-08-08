#ifndef GAME_ENVIRONMENT_H
#define GAME_ENVIRONMENT_H

#include "common.h"

typedef union GameEnvColor {
    struct {
        u32 rgb __attribute__((packed));
    } word;
    struct {
        u8 r;
        u8 g;
        u8 b;
        u8 unused;
    } bytes;
} GameEnvColor;

struct GameEnvironmentCue {
    s32 time;
    GameEnvColor colors[9];
    u16 duration;
    u16 field_2A;
    u16 mode;
    u16 field_2E;
};

typedef struct GameEnvColorSlot {
    GameEnvColor cur;
    GameEnvColor from;
    GameEnvColor to;
} GameEnvColorSlot;

#endif
