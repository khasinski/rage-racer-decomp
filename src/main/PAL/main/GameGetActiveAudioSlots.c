#include "common.h"
#include "game/audio.h"

extern s32 g_AudioSlotMask asm("D_801E6C9C");

s32 GameGetActiveAudioSlots(void) {
    return g_AudioSlotMask;
}
