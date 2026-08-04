#include "common.h"

typedef void (*Callback)(void);

extern Callback g_SndTickCallback asm("D_8009A560");
extern Callback g_SndPrevVSyncCallback asm("D_8009A564");

void SsSoundTickCallback(void);

void SsSoundTickCallback(void) {
    if (g_SndPrevVSyncCallback != 0) {
        g_SndPrevVSyncCallback();
    }

    g_SndTickCallback();
}
