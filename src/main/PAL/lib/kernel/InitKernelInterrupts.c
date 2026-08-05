#include "psyq/kernel.h"

extern u_long *g_IntrRpNode;

void *InitKernelInterrupts(void) {
    register u_short *state asm("$16");
    volatile u_short *clearMask;
    volatile u_short *mask;
    volatile u_long *status;

    state = g_IntrState;
    if (state[0] != 0) {
        return 0;
    }

    clearMask = g_IrqStatus;
    mask = g_IrqMask;
    *mask = 0;
    *clearMask = *mask;
    status = g_KernelDpcr;
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

    g_IntrRpNode[5] = (u_long)startIntrVSync();
    g_IntrRpNode[1] = (u_long)startIntrDMA();
    SysEnqIntRP(g_IntrRpNode);
    state -= 0x1E;
    ExitCriticalSection();

    return state;
}
