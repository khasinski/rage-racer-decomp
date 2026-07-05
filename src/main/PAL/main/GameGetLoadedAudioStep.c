#include "common.h"
#include "game/audio.h"

extern u32 D_801E6CC4;

u32 GameGetLoadedAudioStep(void) {
    return D_801E6CC4;
}
