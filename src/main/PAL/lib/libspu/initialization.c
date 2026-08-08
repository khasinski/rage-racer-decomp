#include "psyq/spu.h"
#include "psyq/kernel.h"
#include "psyq/spu_internal.h"

/* Bit per voice; SpuGetKeyStatus turns it into the LibRef Table 15-1 values
 * (SPU_ON / SPU_ON_ENV_OFF / SPU_OFF / SPU_OFF_ENV_ON). */

void SpuInit(void) {
    _SpuInit(0);
}

void _SpuInit(long reset_voice_center_note) {
    long temp;

    KernelCallbackSlot3();
    _spu_init(reset_voice_center_note);

    if (reset_voice_center_note == 0) {
        u_short *ptr;
        long i;
        u_short value;

        value = 0xC000;
        i = 0x17;
        ptr = &g_SpuVoiceCenterNoteLast;
        do {
            *ptr-- = value;
            i--;
        } while (i >= 0);
    }

    SpuStart();

    temp = g_SpuRevWorkAreaStartAddr[0];
    g_SpuRevState = 0;
    g_SpuRevReserveWa = 0;
    g_SpuRevAttr.mode = 0;
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
