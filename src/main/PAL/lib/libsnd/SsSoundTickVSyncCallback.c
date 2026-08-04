#include "common.h"

typedef void (*Callback)(void);

extern Callback g_SndTickCallback asm("D_8009A560");
extern long g_SndTickVSyncToggle asm("D_8009A56C");

void SsSoundTickVSyncCallback(void);

void SsSoundTickVSyncCallback(void) {
    if (g_SndTickVSyncToggle == 0) {
        g_SndTickVSyncToggle = 1;
    } else {
        g_SndTickVSyncToggle = 0;
        g_SndTickCallback();
    }
}
