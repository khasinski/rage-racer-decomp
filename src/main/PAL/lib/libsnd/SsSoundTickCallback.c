#include "common.h"

typedef void (*Callback)(void);

extern Callback D_8009A560;
extern Callback D_8009A564;

void SsSoundTickCallback(void) asm("func_80071EEC");

void SsSoundTickCallback(void) {
    if (D_8009A564 != 0) {
        D_8009A564();
    }

    D_8009A560();
}
