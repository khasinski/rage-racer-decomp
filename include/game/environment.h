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
    u16 reserved2A;
    u16 mode;
    u16 spareTarget;
};

typedef char GameEnvironmentCueSizeCheck[
    sizeof(struct GameEnvironmentCue) == 0x30 ? 1 : -1];

typedef union GameEnvironmentScriptAddress {
    u32 *words;
    struct GameEnvironmentCue *cues;
} GameEnvironmentScriptAddress;

typedef struct GameEnvColorSlot {
    GameEnvColor cur;
    GameEnvColor from;
    GameEnvColor to;
} GameEnvColorSlot;

typedef char GameEnvColorSlotSizeCheck[
    sizeof(GameEnvColorSlot) == 0xC ? 1 : -1];

typedef union GameEnvironmentColors {
    struct {
        s16 fogEnabled;
        GameEnvColorSlot slots[9];
    } fields;
    u32 fogColorWord;
} GameEnvironmentColors;

/* The fields occupy the retail symbol's exact 0x6E bytes. The word overlay
 * raises the C union's alignment and rounds sizeof to 0x70; this type is never
 * used as an array or to allocate the assembly-owned BSS object. */
typedef char GameEnvironmentColorsPayloadSizeCheck[
    sizeof(((GameEnvironmentColors *)0)->fields) == 0x6E ? 1 : -1];
typedef char GameEnvironmentColorsOverlaySizeCheck[
    sizeof(GameEnvironmentColors) == 0x70 ? 1 : -1];

extern GameEnvironmentColors g_EnvironmentColors;

#endif
