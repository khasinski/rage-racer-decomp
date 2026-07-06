#include "common.h"
#include "game/audio.h"
#include "psyq/snd.h"

extern u8 D_8009B3B8[];

s32 func_8005B6E4(void) {
    SsSetTableSize(D_8009B3B8, 2, 1);
    SsSetTickMode(0x1000);
    SsStartSoundTickMode1();
    SsSetVoiceCount(0xA);
    SsUtReverbOff();
    GameSetReverbPreset(2, 0, 0);
    GameResetSoundState();
    SsSetMVol(0x3FFF, 0x3FFF);
    SsSetReservedVoice(0);
    GameInitSequenceAudio();
    return 0;
}
