#include "psyq/kernel.h"

extern u16 D_80099430[];
extern u16 D_80099462;
extern u32 D_80099464;
extern volatile u16 *D_8009A4C0;
extern volatile u32 *D_8009A4C4;

void *StartKernelInterrupts(void) {
    u16 *state;

    state = D_80099430;
    if (state[0] != 0) {
        return 0;
    }

    HookEntryInt(&state[0x1C]);
    {
        volatile u16 *mask = D_8009A4C0;
        u16 pending = D_80099462;

        state[0] = 1;
        *mask = pending;
    }
    *D_8009A4C4 = D_80099464;
    ExitCriticalSection();

    return state;
}
