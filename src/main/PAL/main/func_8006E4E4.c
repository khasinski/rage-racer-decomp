#include "psyq/kernel.h"

extern u16 D_80099430[];
extern u16 D_80099462;
extern u32 D_80099464;
extern volatile u16 *D_8009A4BC;
extern volatile u16 *D_8009A4C0;
extern volatile u32 *D_8009A4C4;

void *func_8006E4E4(void) {
    u16 *state;
    volatile u16 *initialMask;
    volatile u16 *mask;
    volatile u32 *newStatus;
    volatile u32 *status;
    volatile u16 *clearMask;
    u16 maskValue;
    u32 statusValue;

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
