#include "psyq/kernel.h"

extern u_short g_IntrState[] asm("D_80099430");
extern u_short g_IntrSavedIrqMask asm("D_80099462");
extern u_long g_IntrSavedDpcr asm("D_80099464");
extern volatile u_short *g_IrqStatus asm("D_8009A4BC");
extern volatile u_short *g_IrqMask asm("D_8009A4C0");
extern volatile u_long *g_KernelDpcr asm("D_8009A4C4");

void *StopKernelInterrupts(void) asm("func_8006E4E4");
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
