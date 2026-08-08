#include "common.h"
#include "psyq/snd.h"

#include "psyq/snd_internal.h"


void SsSoundTickCallback(void) {
    if (g_SndPrevVSyncCallback != 0) {
        g_SndPrevVSyncCallback();
    }

    g_SndTickCallback();
}
