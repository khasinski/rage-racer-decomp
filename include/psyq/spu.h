#ifndef RAGE_PC_PSYQ_SPU_H
#define RAGE_PC_PSYQ_SPU_H

#include <sys/types.h>

#include "common.h"

typedef struct SpuMallocEntry {
    u_long addr;
    u_long size;
} SpuMallocEntry;

typedef struct SpuVolume {
    short left;
    short right;
} SpuVolume;

typedef struct SpuCommonAttr {
    u_long mask;
    struct {
        SpuVolume volume;
        SpuVolume volmode;
        SpuVolume volumex;
    } mvol;
    struct {
        SpuVolume volume;
        long reverb;
        long mix;
    } cd;
    struct {
        SpuVolume volume;
        long reverb;
        long mix;
    } ext;
} SpuCommonAttr;

typedef struct SpuRxx {
    u_char pad[0x184];
    short rev_vol_left;
    short rev_vol_right;
    u_char pad2[0x22];
    volatile u_short spucnt;
} SpuRxx;

typedef struct SpuReverbRegAttr {
    u_long flags;
    short dAPF1;
    short dAPF2;
    short vIIR;
    short vCOMB1;
    short vCOMB2;
    short vCOMB3;
    short vCOMB4;
    short vWALL;
    short vAPF1;
    short vAPF2;
    short mLSAME;
    short mRSAME;
    short mLCOMB1;
    short mRCOMB1;
    short mLCOMB2;
    short mRCOMB2;
    short dLSAME;
    short dRSAME;
    short mLDIFF;
    short mRDIFF;
    short mLCOMB3;
    short mRCOMB3;
    short mLCOMB4;
    short mRCOMB4;
    short dLDIFF;
    short dRDIFF;
    short mLAPF1;
    short mRAPF1;
    short mLAPF2;
    short mRAPF2;
    short vLIN;
    short vRIN;
} SpuReverbRegAttr;

typedef struct SpuReverbAttr {
    u_long mask;
    long mode;
    SpuVolume depth;
    long delay;
    long feedback;
} SpuReverbAttr;

typedef struct SpuRevAttrState {
    long mode;
    short depth_left;
    short depth_right;
    long delay;
    long feedback;
} SpuRevAttrState;

void SpuInit(void) asm("func_80078838");
void _SpuInit(long reset_voice_center_note) asm("func_80078858");
void SpuStart(void) asm("func_80078920");
long _spu_resetTransferControl(void) asm("func_800789A4");
void _spu_init(long reset_mode) asm("func_80078A30");
long _spu_writeByIO(long addr, long size) asm("func_80078F4C");
void _spu_transferCallback(void) asm("func_80079280");
void _spu_startDmaTransfer(u_long addr, u_short mode, u_long size) asm("func_80079420");
long _spu_t(long op, ...) asm("func_80079600");
long _spu_Fw(long addr, long size) asm("func_800798A8");
long _spu_Fr(long addr, long size) asm("func_80079930");
void _spu_FsetRXX(long reg, u_long value, long use_mem_shift) asm("func_80079998");
long _spu_FsetRXXa(long reg, u_long value) asm("func_800799E0");
long _spu_FgetRXXa(long reg, long raw) asm("func_80079A84");
void _SpuDataCallback(void *callback) asm("func_80079AC0");
void SpuQuit(void) asm("func_80079AE4");
void _spu_gcSPU(void) asm("func_80079E7C");
long _SpuIsInAllocateArea(u_long addr) asm("func_8007A3A0");
long _SpuIsInAllocateArea_(u_long addr) asm("func_8007A410");
long SpuInitMalloc(long num, u_long *memlist) asm("func_80079B60");
long SpuMalloc(long size) asm("func_80079BB4");
void SpuFree(u_long addr) asm("func_8007A17C");
u_long SpuSetNoiseVoice(long on_off, u_long voice_bit) asm("func_8007A1F8");
u_long _SpuSetAnyVoice(long on_off, u_long voice_bit, long reg_lo, long reg_hi) asm("func_8007A21C");
long SpuSetReverb(long on_off) asm("func_8007A2D0");
long SpuSetReverbModeParam(SpuReverbAttr *attr) asm("func_8007A488");
void _spu_setReverbAttr(SpuReverbRegAttr *attr) asm("func_8007A998");
long SpuClearReverbWorkArea(u_long mode) asm("func_8007AE68");
long SpuTransferStatus(long addr, long mode) asm("func_8007B014");
long SpuGetKeyStatus(u_long voice_bit) asm("func_8007B088");
u_long Spu_ReadFromSpu(long addr, u_long size) asm("func_8007B11C");
long SpuSetTransferStartAddr(long addr) asm("func_8007B17C");
void SpuSetTransferMode(long mode) asm("func_8007B1B8");
long SpuIsTransferCompleted(long wait) asm("func_8007B1EC");
void _spu_setTransferCompletionFlag(long completed) asm("func_8007B294");
u_long _spu_isTransferIdle(void) asm("func_8007B2C0");
void SpuSetCommonAttr(SpuCommonAttr *attr) asm("func_8007B2D0");

#endif
