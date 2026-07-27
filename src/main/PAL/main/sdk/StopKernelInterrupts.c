#include "psyq/kernel.h"

extern u_short D_80099430[];
extern u_short D_80099462;
extern u_long D_80099464;
extern volatile u_short *D_8009A4BC;
extern volatile u_short *D_8009A4C0;
extern volatile u_long *D_8009A4C4;

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

    state = D_80099430;
    if (state[0] == 0) {
        return 0;
    }

    EnterCriticalSection();

    initialMask = D_8009A4C0;
    mask = initialMask;
    status = D_8009A4C4;
    maskValue = *mask;
    D_80099462 = maskValue;
    statusValue = *status;
    clearMask = D_8009A4BC;
    D_80099464 = statusValue;
    *mask = 0;
    *clearMask = *mask;
    newStatus = D_8009A4C4;
    status = newStatus;
    *status &= 0x77777777;
    ResetEntryInt();

    state[0] = 0;
    return state;
}
