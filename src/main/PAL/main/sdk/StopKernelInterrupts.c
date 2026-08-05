#include "psyq/kernel.h"

extern u_short g_IntrState[];
extern u_short g_IntrSavedIrqMask;
extern u_long g_IntrSavedDpcr;
extern volatile u_short *g_IrqStatus;
extern volatile u_short *g_IrqMask;
extern volatile u_long *g_KernelDpcr;

void *StopKernelInterrupts(void) {
    u_short *state;
    volatile u_short *initialMask;
    volatile u_short *mask;
    volatile u_long *newStatus;
    volatile u_long *status;
    volatile u_short *clearMask;
    u_short maskValue;
    u_long statusValue;

    state = g_IntrState;
    if (state[0] == 0) {
        return 0;
    }

    EnterCriticalSection();

    initialMask = g_IrqMask;
    mask = initialMask;
    status = g_KernelDpcr;
    maskValue = *mask;
    g_IntrSavedIrqMask = maskValue;
    statusValue = *status;
    clearMask = g_IrqStatus;
    g_IntrSavedDpcr = statusValue;
    *mask = 0;
    *clearMask = *mask;
    newStatus = g_KernelDpcr;
    status = newStatus;
    *status &= 0x77777777;
    ResetEntryInt();

    state[0] = 0;
    return state;
}
