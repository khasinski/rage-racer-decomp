#include <sys/types.h>

#include "common.h"

typedef struct SpuInitVolume {
    short left;
    short right;
} SpuInitVolume;

typedef struct SpuInitVoiceRegister {
    SpuInitVolume volume;
    u_short pitch;
    u_short addr;
    u_short adsr[2];
    u_short volumex;
    u_short loopAddr;
} SpuInitVoiceRegister;

typedef struct SpuInitControl {
    SpuInitVoiceRegister voice[24];
    SpuInitVolume mainVol;
    SpuInitVolume revVol;
    u_short keyOn[2];
    u_short keyOff[2];
    u_short chanFm[2];
    u_short noiseMode[2];
    u_short revMode[2];
    u_long chanOn;
    u_short unk;
    u_short revWorkAddr;
    u_short irqAddr;
    u_short transAddr;
    u_short transFifo;
    u_short spuCnt;
    u_short dataTrans;
    u_short spuStat;
    SpuInitVolume cdVol;
    SpuInitVolume extVol;
    SpuInitVolume mainVolCurrent;
    SpuInitVolume unkVol;
} SpuInitControl;

typedef union SpuInitRegisterMap {
    SpuInitControl regs;
    volatile u_short raw[0x100];
} SpuInitRegisterMap;

extern long func_80078F4C(long addr, long size);

extern char g_SpuTimeoutFmt[] asm("D_80013EC0");
extern char g_SpuTimeoutMsgReset[] asm("D_80013ED0");
extern long g_SpuWaitCount asm("D_8009AB74");
extern u_short g_SpuTransferStartAddr asm("D_8009AB78");
extern volatile SpuInitRegisterMap *g_SpuRegBase asm("D_8009AB7C");
extern volatile long *g_SpuDpcr asm("D_8009AB8C");
extern long g_SpuTransferByIo asm("D_8009AB94");
/* Deliberately raw: written zero here and read nowhere in the image. */
extern long D_8009AB98;
/* psyq's _spu_mem_mode group, all four set together: the enable flag, the
 * unit size 8, its shift 3 and its mask 7. _spu_FsetRXXa rounds a byte
 * address up to a multiple of the unit before shifting it into a register. */
extern long g_SpuMemMode asm("D_8009AB9C");
extern long _spu_mem_mode_unitM asm("D_8009ABA0");
extern long g_SpuMemModeUnit asm("D_8009ABA4");
extern long _spu_mem_mode_plus asm("D_8009ABA8");
extern long g_SpuTransferCompleted asm("D_8009ABAC");
extern void (*volatile g_SpuTransferCallback)(void) asm("D_8009ABB0");
/* Deliberately raw: cleared beside g_SpuTransferCallback in _spu_init and
 * SpuQuit and never read. Shape says libspu's IRQ callback slot
 * (SpuSetIRQCallback), but this build has no call site to prove it. */
extern void (*volatile D_8009ABB4)(void);
extern long g_SpuDummyAdpcmBlock asm("D_8009ABB8");

#define SPU_INIT_DELAY()                    \
    delayValue = 0xD;                       \
    for (delay = 0; delay < 0xF0; delay++) { \
        delayValue *= 3;                    \
    }

long _spu_init(long resetMode) asm("func_80078A30");
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
                GameDebugPrintf(g_SpuTimeoutFmt, g_SpuTimeoutMsgReset);
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
        func_80078F4C((long)&g_SpuDummyAdpcmBlock, 0x10);

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
