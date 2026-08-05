#include "common.h"
#include "psyq/snd.h"

typedef void (*Callback)(void);

extern Callback g_SndTickCallback;
extern long g_SndTickVSyncToggle;

void SsSoundTickVSyncCallback(void) {
    if (g_SndTickVSyncToggle == 0) {
        g_SndTickVSyncToggle = 1;
    } else {
        g_SndTickVSyncToggle = 0;
        g_SndTickCallback();
    }
}
