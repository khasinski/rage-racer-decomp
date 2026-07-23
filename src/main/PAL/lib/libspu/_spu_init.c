#include "common.h"

typedef struct SpuInitVolume {
    s16 left;
    s16 right;
} SpuInitVolume;

typedef struct SpuInitVoiceRegister {
    SpuInitVolume volume;
    u16 pitch;
    u16 addr;
    u16 adsr[2];
    u16 volumex;
    u16 loopAddr;
} SpuInitVoiceRegister;

typedef struct SpuInitControl {
    SpuInitVoiceRegister voice[24];
    SpuInitVolume mainVol;
    SpuInitVolume revVol;
    u16 keyOn[2];
    u16 keyOff[2];
    u16 chanFm[2];
    u16 noiseMode[2];
    u16 revMode[2];
    u32 chanOn;
    u16 unk;
    u16 revWorkAddr;
    u16 irqAddr;
    u16 transAddr;
    u16 transFifo;
    u16 spuCnt;
    u16 dataTrans;
    u16 spuStat;
    SpuInitVolume cdVol;
    SpuInitVolume extVol;
    SpuInitVolume mainVolCurrent;
    SpuInitVolume unkVol;
} SpuInitControl;

typedef union SpuInitRegisterMap {
    SpuInitControl regs;
    volatile u16 raw[0x100];
} SpuInitRegisterMap;

extern s32 func_8001674C(const char *format, ...);
extern s32 func_80078F4C(s32 addr, s32 size);

extern char D_80013EC0[];
extern char D_80013ED0[];
extern s32 D_8009AB74;
extern u16 D_8009AB78;
extern volatile SpuInitRegisterMap *D_8009AB7C;
extern volatile s32 *D_8009AB8C;
extern s32 D_8009AB94;
extern s32 D_8009AB98;
extern s32 D_8009AB9C;
extern s32 D_8009ABA0;
extern s32 D_8009ABA4;
extern s32 D_8009ABA8;
extern s32 D_8009ABAC;
extern void (*volatile D_8009ABB0)(void);
extern void (*volatile D_8009ABB4)(void);
extern s32 D_8009ABB8;

#define SPU_INIT_DELAY()                    \
    delayValue = 0xD;                       \
    for (delay = 0; delay < 0xF0; delay++) { \
        delayValue *= 3;                    \
    }

s32 func_80078A30(s32 resetMode) {
    volatile s32 delay;
    volatile s32 delayValue;
    s32 waitCount;
    s32 channel;
    s16 temp;

    *D_8009AB8C |= 0xB0000;
    D_8009AB7C->regs.mainVol.left = 0;
    D_8009AB7C->regs.mainVol.right = 0;
    D_8009AB7C->regs.spuCnt = 0;
    D_8009AB94 = 0;
    D_8009AB98 = 0;
    D_8009AB78 = 0;
    SPU_INIT_DELAY();
    D_8009AB7C->regs.mainVol.left = 0;
    D_8009AB7C->regs.mainVol.right = 0;
    D_8009AB74 = 0;
    if (D_8009AB7C->regs.spuStat & 0x7FF) {
        do {
            waitCount = D_8009AB74 + 1;
            D_8009AB74 = waitCount;
            if (waitCount > 5000) {
                func_8001674C(D_80013EC0, D_80013ED0);
                break;
            }
        } while (D_8009AB7C->regs.spuStat & 0x7FF);
    }

    D_8009AB9C = 2;
    D_8009ABA0 = 3;
    D_8009ABA4 = 8;
    D_8009ABA8 = 7;
    D_8009AB7C->regs.dataTrans = 4;
    D_8009AB7C->regs.revVol.left = 0;
    D_8009AB7C->regs.revVol.right = 0;
    D_8009AB7C->regs.keyOff[0] = 0xFFFF;
    D_8009AB7C->regs.keyOff[1] = 0xFFFF;
    D_8009AB7C->regs.revMode[0] = 0;
    D_8009AB7C->regs.revMode[1] = 0;

    if (resetMode == 0) {
        D_8009AB7C->regs.chanFm[0] = 0;
        D_8009AB7C->regs.chanFm[1] = 0;
        D_8009AB7C->regs.noiseMode[0] = 0;
        D_8009AB7C->regs.noiseMode[1] = 0;
        D_8009AB7C->regs.cdVol.left = 0;
        D_8009AB7C->regs.cdVol.right = 0;
        D_8009AB7C->regs.extVol.left = 0;
        D_8009AB7C->regs.extVol.right = 0;
        D_8009AB78 = 0x200;
        func_80078F4C((s32)&D_8009ABB8, 0x10);

        for (channel = 0; channel < 24; channel++) {
            D_8009AB7C->raw[channel * 8 + 0] = 0;
            D_8009AB7C->raw[channel * 8 + 1] = 0;
            D_8009AB7C->raw[channel * 8 + 2] = 0x3FFF;
            D_8009AB7C->raw[channel * 8 + 3] = 0x200;
            D_8009AB7C->raw[channel * 8 + 4] = 0;
            D_8009AB7C->raw[channel * 8 + 5] = 0;
        }

        temp = D_8009AB7C->regs.keyOn[0];
        D_8009AB7C->regs.keyOn[0] = 0xFFFF;
        D_8009AB7C->regs.keyOn[1] |= 0xFF;
        SPU_INIT_DELAY();
        SPU_INIT_DELAY();
        SPU_INIT_DELAY();
        SPU_INIT_DELAY();
        temp = D_8009AB7C->regs.keyOff[0];
        D_8009AB7C->regs.keyOff[0] = 0xFFFF;
        D_8009AB7C->regs.keyOff[1] |= 0xFF;
        SPU_INIT_DELAY();
        SPU_INIT_DELAY();
        SPU_INIT_DELAY();
        SPU_INIT_DELAY();
    }

    D_8009ABAC = 1;
    D_8009AB7C->regs.spuCnt = 0xC000;
    D_8009ABB0 = 0;
    D_8009ABB4 = 0;
    return 0;
}
