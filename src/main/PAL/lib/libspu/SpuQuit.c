#include "psyq/spu.h"
#include "psyq/kernel.h"

extern long g_SpuTransferEvent asm("D_8009A768");
extern long g_SpuIsStarted asm("D_8009AB70");
extern long g_SpuTransferCallback asm("D_8009ABB0");
extern long D_8009ABB4;

void SpuQuit(void) {
    if (g_SpuIsStarted == 1) {
        g_SpuIsStarted = 0;
        EnterCriticalSection();
        __asm__ volatile("addu $4,$0,$0");
        g_SpuTransferCallback = 0;
        D_8009ABB4 = 0;
        __asm__ volatile("jal func_80079AC0" ::: "$31");
        CloseEvent(g_SpuTransferEvent);
        DisableEvent(g_SpuTransferEvent);
        ExitCriticalSection();
    }
}
