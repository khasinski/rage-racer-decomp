#include "psyq/snd.h"

#include "psyq/snd_internal.h"

void SpuVmDamperOff(void) {
    g_SndDamper = 0;
}

void SpuVmDamperOn(void) {
    g_SndDamper = 2;
}

/* Re-entrancy-guarded wrapper around the internal flush; see psyq/snd.h. */
void SpuVmDamperStep(void) {
    if (g_SndUpdateLock != 1) {
        g_SndUpdateLock = 1;
        SsUtFlush();
        g_SndUpdateLock = 0;
    }
}
