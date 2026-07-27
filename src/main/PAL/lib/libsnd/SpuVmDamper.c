#include "psyq/snd.h"

extern s16 D_8019CA68;
extern volatile s32 D_801E40AC;

void SpuVmDamperOff(void) {
    D_8019CA68 = 0;
}

void SpuVmDamperOn(void) {
    D_8019CA68 = 2;
}

/* Re-entrancy-guarded wrapper around the internal flush; see psyq/snd.h. */
void func_800731CC(void) {
    if (D_801E40AC != 1) {
        D_801E40AC = 1;
        SsUtFlush();
        D_801E40AC = 0;
    }
}
