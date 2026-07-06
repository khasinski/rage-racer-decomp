#include "psyq/kernel.h"

extern u16 D_80099430[];
extern u16 D_80099462;
extern u32 D_80099464;
extern volatile u16 *D_8009A4BC;
extern volatile u16 *D_8009A4C0;
extern volatile u32 *D_8009A4C4;

void *func_8006E4E4(void) {
    u16 *state;
    register volatile u16 *mask asm("$2");
    register volatile u32 *status asm("$4");
    volatile u16 *clearMask;
    u32 statusValue;

    state = D_80099430;
    if (state[0] == 0) {
        return 0;
    }

    EnterCriticalSection();

    mask = D_8009A4C0;
    status = D_8009A4C4;
    D_80099462 = *mask;
    statusValue = *status;
    clearMask = D_8009A4BC;
    D_80099464 = statusValue;
    *mask = 0;
    *clearMask = *mask;
    status = D_8009A4C4;
    *status &= 0x77777777;
    ResetEntryInt();

    state[0] = 0;
    return state;
}
