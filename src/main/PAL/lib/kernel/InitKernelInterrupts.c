#include "psyq/kernel.h"

extern u_short g_IntrState[] asm("D_80099430");
/* Interrupt hardware, from the data segment: I_STAT 0x1F801070, I_MASK
 * 0x1F801074, DPCR 0x1F8010F0 (the kernel's own pointer to it; libgpu and
 * libspu keep separate pointers to the same register). */
extern volatile u_short *g_IrqStatus asm("D_8009A4BC");
extern volatile u_short *g_IrqMask asm("D_8009A4C0");
extern volatile u_long *g_KernelDpcr asm("D_8009A4C4");
extern u_long *g_IntrRpNode asm("D_8009A4B8");
extern u_short D_8009946C[];

void intrDispatch(void);

void *InitKernelInterrupts(void) {
    /* This pin is load-bearing: removing it changes .text. */
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
    /* This barrier is load-bearing: removing it changes .text. */
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
