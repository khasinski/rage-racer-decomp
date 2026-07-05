#include "psyq/spu.h"
#include "psyq/kernel.h"

extern s32 D_8009A768;
extern s32 D_8009AB70;
extern s32 D_8009ABB0;
extern s32 D_8009ABB4;

void SpuQuit(void) {
    if (D_8009AB70 == 1) {
        D_8009AB70 = 0;
        EnterCriticalSection();
        __asm__ volatile("addu $4,$0,$0");
        D_8009ABB0 = 0;
        D_8009ABB4 = 0;
        __asm__ volatile("jal func_80079AC0" ::: "$31");
        CloseEvent(D_8009A768);
        DisableEvent(D_8009A768);
        ExitCriticalSection();
    }
}
