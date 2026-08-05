#include "psyq/snd.h"

extern short g_SndDamper asm("D_8019CA68");
extern volatile long g_SndUpdateLock asm("D_801E40AC");

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
