#include "common.h"
#include "psyq/snd.h"

void SsStartSoundTick(long arg0);

void SsStartSoundTickMode0(void) {
    SsStartSoundTick(0);
}
