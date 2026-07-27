#include "psyq/kernel.h"

extern u_short D_80099430[];
extern u_short D_80099462;
extern u_long D_80099464;
extern volatile u_short *D_8009A4C0;
extern volatile u_long *D_8009A4C4;

void *StartKernelInterrupts(void) {
    u_short *state;

    state = D_80099430;
    if (state[0] != 0) {
        return 0;
    }

    HookEntryInt(&state[0x1C]);
    {
        volatile u_short *mask = D_8009A4C0;
        u_short pending = D_80099462;

        state[0] = 1;
        *mask = pending;
    }
    *D_8009A4C4 = D_80099464;
    ExitCriticalSection();

    return state;
}
