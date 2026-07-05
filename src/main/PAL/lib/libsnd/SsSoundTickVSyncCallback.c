#include "common.h"

typedef void (*Callback)(void);

extern Callback D_8009A560;
extern s32 D_8009A56C;

void SsSoundTickVSyncCallback(void) asm("func_80071F2C");

void SsSoundTickVSyncCallback(void) {
    if (D_8009A56C == 0) {
        D_8009A56C = 1;
    } else {
        D_8009A56C = 0;
        D_8009A560();
    }
}
