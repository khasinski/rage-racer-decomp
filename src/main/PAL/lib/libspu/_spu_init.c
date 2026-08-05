#include <sys/types.h>

#include "common.h"
#include "psyq/spu.h"

extern long _spu_writeByIO(long addr, long size);

extern char g_SpuTimeoutFmt[];
extern volatile SpuRegisterMap *g_SpuRegBase;
extern long g_SpuMemModeUnit;
extern long _spu_mem_mode_plus;
extern void (*volatile g_SpuTransferCallback)(void);
/* Deliberately raw: cleared beside g_SpuTransferCallback in _spu_init and
 * SpuQuit and never read. Shape says libspu's IRQ callback slot
 * (SpuSetIRQCallback), but this build has no call site to prove it. */
extern void (*volatile D_8009ABB4)(void);

#define SPU_INIT_DELAY()                    \
    delayValue = 0xD;                       \
    for (delay = 0; delay < 0xF0; delay++) { \
        delayValue *= 3;                    \
    }

long _spu_init(long resetMode) {
    volatile long delay;
    volatile long delayValue;
    long waitCount;
    long channel;
    short temp;

    *g_SpuDpcr |= 0xB0000;
    g_SpuRegBase->regs.mainVol.left = 0;
    g_SpuRegBase->regs.mainVol.right = 0;
    g_SpuRegBase->regs.spuCnt = 0;
    g_SpuTransferByIo = 0;
    D_8009AB98 = 0;
    g_SpuTransferStartAddr = 0;
    SPU_INIT_DELAY();
    g_SpuRegBase->regs.mainVol.left = 0;
    g_SpuRegBase->regs.mainVol.right = 0;
    g_SpuWaitCount = 0;
    if (g_SpuRegBase->regs.spuStat & 0x7FF) {
        do {
            waitCount = g_SpuWaitCount + 1;
            g_SpuWaitCount = waitCount;
            if (waitCount > 5000) {
                DebugPrintf(g_SpuTimeoutFmt, g_SpuTimeoutMsgReset);
                break;
            }
        } while (g_SpuRegBase->regs.spuStat & 0x7FF);
    }

    g_SpuMemMode = 2;
    _spu_mem_mode_unitM = 3;
    g_SpuMemModeUnit = 8;
    _spu_mem_mode_plus = 7;
    g_SpuRegBase->regs.dataTrans = 4;
    g_SpuRegBase->regs.revVol.left = 0;
    g_SpuRegBase->regs.revVol.right = 0;
    g_SpuRegBase->regs.keyOff[0] = 0xFFFF;
    g_SpuRegBase->regs.keyOff[1] = 0xFFFF;
    g_SpuRegBase->regs.revMode[0] = 0;
    g_SpuRegBase->regs.revMode[1] = 0;

    if (resetMode == 0) {
        g_SpuRegBase->regs.chanFm[0] = 0;
        g_SpuRegBase->regs.chanFm[1] = 0;
        g_SpuRegBase->regs.noiseMode[0] = 0;
        g_SpuRegBase->regs.noiseMode[1] = 0;
        g_SpuRegBase->regs.cdVol.left = 0;
        g_SpuRegBase->regs.cdVol.right = 0;
        g_SpuRegBase->regs.extVol.left = 0;
        g_SpuRegBase->regs.extVol.right = 0;
        g_SpuTransferStartAddr = 0x200;
        _spu_writeByIO((long)&g_SpuDummyAdpcmBlock, 0x10);

        for (channel = 0; channel < 24; channel++) {
            g_SpuRegBase->raw[channel * 8 + 0] = 0;
            g_SpuRegBase->raw[channel * 8 + 1] = 0;
            g_SpuRegBase->raw[channel * 8 + 2] = 0x3FFF;
            g_SpuRegBase->raw[channel * 8 + 3] = 0x200;
            g_SpuRegBase->raw[channel * 8 + 4] = 0;
            g_SpuRegBase->raw[channel * 8 + 5] = 0;
        }

        temp = g_SpuRegBase->regs.keyOn[0];
        g_SpuRegBase->regs.keyOn[0] = 0xFFFF;
        g_SpuRegBase->regs.keyOn[1] |= 0xFF;
        SPU_INIT_DELAY();
        SPU_INIT_DELAY();
        SPU_INIT_DELAY();
        SPU_INIT_DELAY();
        temp = g_SpuRegBase->regs.keyOff[0];
        g_SpuRegBase->regs.keyOff[0] = 0xFFFF;
        g_SpuRegBase->regs.keyOff[1] |= 0xFF;
        SPU_INIT_DELAY();
        SPU_INIT_DELAY();
        SPU_INIT_DELAY();
        SPU_INIT_DELAY();
    }

    g_SpuTransferCompleted = 1;
    g_SpuRegBase->regs.spuCnt = 0xC000;
    g_SpuTransferCallback = 0;
    D_8009ABB4 = 0;
    return 0;
}
