#include "psyq/kernel.h"

extern u_short D_80099430[];
extern volatile u_short *D_8009A4BC;
extern volatile u_short *D_8009A4C0;
extern volatile u_long *D_8009A4C4;
extern u_long *D_8009A4B8;
extern u_short D_8009946C[];

void intrDispatch(void);

void *InitKernelInterrupts(void) {
    register u_short *state asm("$16");
    volatile u_short *clearMask;
    volatile u_short *mask;
    volatile u_long *status;

    state = D_80099430;
    if (state[0] != 0) {
        return 0;
    }

    clearMask = D_8009A4BC;
    mask = D_8009A4C0;
    *mask = 0;
    *clearMask = *mask;
    status = D_8009A4C4;
    *status = 0x33333333;

    clearKernelInterruptState((u_long *)state, 0x41A);
    if (SaveKernelRegisters(&state[0x1C]) != 0) {
        intrDispatch();
    }

    state = D_8009946C;
    asm("" : "=r"(state) : "0"(state));
    *(u_long *)state = (long)state + 0xFDC;
    HookEntryInt(state - 2);
    *(state - 0x1E) = 1;

    D_8009A4B8[5] = (u_long)startIntrVSync();
    D_8009A4B8[1] = (u_long)startIntrDMA();
    SysEnqIntRP(D_8009A4B8);
    state -= 0x1E;
    ExitCriticalSection();

    return state;
}
