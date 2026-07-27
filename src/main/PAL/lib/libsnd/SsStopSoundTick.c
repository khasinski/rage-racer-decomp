#include "common.h"
#include "psyq/kernel.h"

typedef void (*Callback)(void);

extern long D_8009A55C;
extern Callback D_8009A564;
extern u_char D_8009A568;
extern u_char D_8009A569;
extern u_char D_8009A56A;

long func_8006DF64(long arg0, Callback arg1);
void func_8006DFC4(long arg0);

void SsStopSoundTick(void) asm("func_80072260");

void SsStopSoundTick(void) {
    if (D_8009A55C == 0) {
        D_8009A569 = 0;
        EnterCriticalSection();

        if (D_8009A568 != 0) {
            func_8006DFC4(0);
            D_8009A568 = 0;
        } else if (D_8009A56A == 0) {
            func_8006DF64(0, D_8009A564);
            D_8009A564 = 0;
        } else {
            func_8006DF64(6, 0);
        }

        ExitCriticalSection();
        D_8009A56A = 0xFF;
    }
}
