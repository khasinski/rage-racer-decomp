#include "psyq/spu.h"
#include "psyq/kernel.h"

extern long D_8009A718;
extern long g_SpuRevAttr;
/* Bit per voice; SpuGetKeyStatus turns it into the LibRef Table 15-1 values
 * (SPU_ON / SPU_ON_ENV_OFF / SPU_OFF / SPU_OFF_ENV_ON). */
extern long g_SpuKeyStatus;
extern long g_SpuRevWorkAreaStartAddr;

void SpuInit(void) {
    _SpuInit(0);
}

void _SpuInit(long arg0) {
    long temp;

    KernelCallbackSlot3();
    _spu_init(arg0);

    if (arg0 == 0) {
        u_short *ptr;
        long i;
        register long value asm("$4");

        value = 0xC000;
        i = 0x17;
        ptr = &D_8009A766;
        do {
            *ptr-- = value;
            i--;
        } while (i >= 0);
    }

    SpuStart();

    temp = g_SpuRevWorkAreaStartAddr;
    D_8009A718 = 0;
    g_SpuRevReserveWa = 0;
    g_SpuRevAttr = 0;
    g_SpuRevAttrDepthLeft = 0;
    g_SpuRevAttrDepthRight = 0;
    g_SpuRevAttrDelay = 0;
    g_SpuRevAttrFeedback = 0;
    g_SpuRevWorkAreaAddr = temp;

    _spu_FsetRXX(0xD1, temp, 0);

    g_SpuTransferMode = 0;
    g_SpuTransferByIo = 0;
    g_SpuKeyStatus = 0;
}

void SpuStart(void) {
    long temp;

    if (g_SpuIsStarted == 0) {
        g_SpuIsStarted = 1;
        EnterCriticalSection();
        D_8009A710 = 0;
        _SpuDataCallback(_spu_transferCallback);
        temp = OpenEvent(0xF0000009, 0x20, 0x2000, 0);
        g_SpuTransferEvent = temp;
        EnableEvent(temp);
        ExitCriticalSection();
    }
}
