#include "psyq/kernel.h"

extern u_short g_IntrState[] asm("D_80099430");
extern u_short g_IntrSavedIrqMask asm("D_80099462");
extern u_long g_IntrSavedDpcr asm("D_80099464");
extern volatile u_short *g_IrqMask asm("D_8009A4C0");
extern volatile u_long *g_KernelDpcr asm("D_8009A4C4");

void *StartKernelInterrupts(void) {
    u_short *state;

    state = g_IntrState;
    if (state[0] != 0) {
        return 0;
    }

    HookEntryInt(&state[0x1C]);
    {
        volatile u_short *mask = g_IrqMask;
        u_short pending = g_IntrSavedIrqMask;

        state[0] = 1;
        *mask = pending;
    }
    *g_KernelDpcr = g_IntrSavedDpcr;
    ExitCriticalSection();

    return state;
}
