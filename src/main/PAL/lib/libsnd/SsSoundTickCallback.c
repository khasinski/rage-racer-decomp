#include "common.h"

typedef void (*Callback)(void);

extern Callback g_SndTickCallback;
extern Callback g_SndPrevVSyncCallback;

void SsSoundTickCallback(void);

void SsSoundTickCallback(void) {
    if (g_SndPrevVSyncCallback != 0) {
        g_SndPrevVSyncCallback();
    }

    g_SndTickCallback();
}
