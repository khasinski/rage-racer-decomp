#include "psyq/kernel.h"

extern u16 D_80099430[];
extern volatile u16 *D_8009A4BC;
extern volatile u16 *D_8009A4C0;
extern volatile u32 *D_8009A4C4;
extern u32 *D_8009A4B8;
extern u16 D_8009946C[];

void intrDispatch(void);

void *InitKernelInterrupts(void) {
    register u16 *state asm("$16");
    volatile u16 *clearMask;
    volatile u16 *mask;
    volatile u32 *status;

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

    clearKernelInterruptState((u32 *)state, 0x41A);
    if (SaveKernelRegisters(&state[0x1C]) != 0) {
        intrDispatch();
    }

    state = D_8009946C;
    asm("" : "=r"(state) : "0"(state));
    *(u32 *)state = (s32)state + 0xFDC;
    HookEntryInt(state - 2);
    *(state - 0x1E) = 1;

    D_8009A4B8[5] = (u32)startIntrVSync();
    D_8009A4B8[1] = (u32)startIntrDMA();
    SysEnqIntRP(D_8009A4B8);
    state -= 0x1E;
    ExitCriticalSection();

    return state;
}
