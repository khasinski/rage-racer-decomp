#ifndef RAGE_PC_PSYQ_SPU_H
#define RAGE_PC_PSYQ_SPU_H

#include "common.h"

typedef struct SpuMallocEntry {
    u32 addr;
    u32 size;
} SpuMallocEntry;

typedef struct SpuVolume {
    s16 left;
    s16 right;
} SpuVolume;

typedef struct SpuCommonAttr {
    u32 mask;
    struct {
        SpuVolume volume;
        SpuVolume volmode;
        SpuVolume volumex;
    } mvol;
    struct {
        SpuVolume volume;
        s32 reverb;
        s32 mix;
    } cd;
    struct {
        SpuVolume volume;
        s32 reverb;
        s32 mix;
    } ext;
} SpuCommonAttr;

typedef struct SpuRxx {
    u8 pad[0x184];
    s16 rev_vol_left;
    s16 rev_vol_right;
    u8 pad2[0x22];
    volatile u16 spucnt;
} SpuRxx;

typedef struct SpuReverbRegAttr {
    u32 flags;
    s16 dAPF1;
    s16 dAPF2;
    s16 vIIR;
    s16 vCOMB1;
    s16 vCOMB2;
    s16 vCOMB3;
    s16 vCOMB4;
    s16 vWALL;
    s16 vAPF1;
    s16 vAPF2;
    s16 mLSAME;
    s16 mRSAME;
    s16 mLCOMB1;
    s16 mRCOMB1;
    s16 mLCOMB2;
    s16 mRCOMB2;
    s16 dLSAME;
    s16 dRSAME;
    s16 mLDIFF;
    s16 mRDIFF;
    s16 mLCOMB3;
    s16 mRCOMB3;
    s16 mLCOMB4;
    s16 mRCOMB4;
    s16 dLDIFF;
    s16 dRDIFF;
    s16 mLAPF1;
    s16 mRAPF1;
    s16 mLAPF2;
    s16 mRAPF2;
    s16 vLIN;
    s16 vRIN;
} SpuReverbRegAttr;

typedef struct SpuReverbAttr {
    u32 mask;
    s32 mode;
    SpuVolume depth;
    s32 delay;
    s32 feedback;
} SpuReverbAttr;

typedef struct SpuRevAttrState {
    s32 mode;
    s16 depth_left;
    s16 depth_right;
    s32 delay;
    s32 feedback;
} SpuRevAttrState;

void SpuInit(void) asm("func_80078838");
void _SpuInit(s32 reset_voice_center_note) asm("func_80078858");
void SpuStart(void) asm("func_80078920");
s32 _spu_resetTransferControl(void) asm("func_800789A4");
void _spu_init(s32 reset_mode) asm("func_80078A30");
s32 _spu_writeByIO(s32 addr, s32 size) asm("func_80078F4C");
void _spu_transferCallback(void) asm("func_80079280");
void _spu_startDmaTransfer(u32 addr, u16 mode, u32 size) asm("func_80079420");
s32 _spu_t(s32 op, ...) asm("func_80079600");
s32 _spu_Fw(s32 addr, s32 size) asm("func_800798A8");
s32 _spu_Fr(s32 addr, s32 size) asm("func_80079930");
void _spu_FsetRXX(s32 reg, u32 value, s32 use_mem_shift) asm("func_80079998");
s32 _spu_FsetRXXa(s32 reg, u32 value) asm("func_800799E0");
s32 _spu_FgetRXXa(s32 reg, s32 raw) asm("func_80079A84");
void _SpuDataCallback(void *callback) asm("func_80079AC0");
void SpuQuit(void) asm("func_80079AE4");
void _spu_gcSPU(void) asm("func_80079E7C");
s32 _SpuIsInAllocateArea(u32 addr) asm("func_8007A3A0");
s32 _SpuIsInAllocateArea_(u32 addr) asm("func_8007A410");
s32 SpuInitMalloc(s32 num, u32 *memlist) asm("func_80079B60");
s32 SpuMalloc(s32 size) asm("func_80079BB4");
void SpuFree(u32 addr) asm("func_8007A17C");
u32 SpuSetNoiseVoice(s32 on_off, u32 voice_bit) asm("func_8007A1F8");
u32 _SpuSetAnyVoice(s32 on_off, u32 voice_bit, s32 reg_lo, s32 reg_hi) asm("func_8007A21C");
s32 SpuSetReverb(s32 on_off) asm("func_8007A2D0");
s32 SpuSetReverbModeParam(SpuReverbAttr *attr) asm("func_8007A488");
void _spu_setReverbAttr(SpuReverbRegAttr *attr) asm("func_8007A998");
s32 SpuClearReverbWorkArea(u32 mode) asm("func_8007AE68");
s32 SpuTransferStatus(s32 addr, s32 mode) asm("func_8007B014");
s32 SpuGetKeyStatus(u32 voice_bit) asm("func_8007B088");
u32 Spu_ReadFromSpu(s32 addr, u32 size) asm("func_8007B11C");
s32 SpuSetTransferStartAddr(s32 addr) asm("func_8007B17C");
void SpuSetTransferMode(s32 mode) asm("func_8007B1B8");
s32 SpuIsTransferCompleted(s32 wait) asm("func_8007B1EC");
void _spu_setTransferCompletionFlag(s32 completed) asm("func_8007B294");
u32 _spu_isTransferIdle(void) asm("func_8007B2C0");
void SpuSetCommonAttr(SpuCommonAttr *attr) asm("func_8007B2D0");

#endif
