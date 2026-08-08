#include "psyq/spu.h"
#include "psyq/kernel.h"
#include "psyq/spu_internal.h"

/* Transfer/IRQ callback slots, shared with the SPU interrupt handler; every
 * other unit that touches them declares them volatile as well. */
void SpuQuit(void) {
    if (g_SpuIsStarted == 1) {
        g_SpuIsStarted = 0;
        EnterCriticalSection();
        g_SpuTransferCallback = 0;
        g_SpuIrqCallback = 0;
        _SpuDataCallback(0);
        CloseEvent(g_SpuTransferEvent);
        DisableEvent(g_SpuTransferEvent);
        ExitCriticalSection();
    }
}
