#include <sys/types.h>

#include "common.h"
#include "psyq/snd.h"

#include "psyq/snd_internal.h"
void _SsInitTables(void) {
    short *ptr;
    long i;
    long j;

    ptr = (short *)0x1F801C00;
    for (i = 0; i < 0x18; i++) {
        for (j = 0; j < 8; j++) {
            *ptr++ = g_SndVoiceRegDefaults[j];
        }
    }

    ptr = (short *)0x1F801D80;
    for (i = 0; i < 0x10; i++) {
        *ptr++ = g_SndSpuCtrlDefaults[i];
    }

    SpuVmInit(0x18);

    for (j = 0; j < 0x20; j++) {
        for (i = 0; i < 0x10; i++) {
            g_SndMarkCallbacks[j][i] = 0;
        }
    }

    g_SndTickResolution = 0x3C;
    g_SndSeqOpenMask = 0;
    g_SndUpdateLock = 0;
}
