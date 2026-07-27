#include "psyq/spu.h"
#include "psyq/kernel.h"

extern long g_SpuTransferEvent asm("D_8009A768");
extern long g_SpuIsStarted asm("D_8009AB70");
/* Transfer/IRQ callback slots, shared with the SPU interrupt handler; every
 * other unit that touches them declares them volatile as well. */
extern void (*volatile g_SpuTransferCallback)(void) asm("D_8009ABB0");
extern void (*volatile D_8009ABB4)(void);

void SpuQuit(void) {
    if (g_SpuIsStarted == 1) {
        g_SpuIsStarted = 0;
        EnterCriticalSection();
        g_SpuTransferCallback = 0;
        D_8009ABB4 = 0;
        _SpuDataCallback(0);
        CloseEvent(g_SpuTransferEvent);
        DisableEvent(g_SpuTransferEvent);
        ExitCriticalSection();
    }
}
