#include "common.h"
#include "psyq/snd.h"

#include "psyq/snd_internal.h"


void SsSoundTickVSyncCallback(void) {
    if (g_SndTickVSyncToggle == 0) {
        g_SndTickVSyncToggle = 1;
    } else {
        g_SndTickVSyncToggle = 0;
        g_SndTickCallback();
    }
}
