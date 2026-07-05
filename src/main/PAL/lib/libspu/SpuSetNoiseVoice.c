#include "psyq/spu.h"

u32 SpuSetNoiseVoice(s32 on_off, u32 voice_bit) {
    return _SpuSetAnyVoice(on_off, voice_bit, 0xCA, 0xCB);
}
