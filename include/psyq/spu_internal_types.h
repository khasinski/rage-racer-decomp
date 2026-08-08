#ifndef RAGE_PC_PSYQ_SPU_INTERNAL_TYPES_H
#define RAGE_PC_PSYQ_SPU_INTERNAL_TYPES_H

#include "common.h"

typedef struct SpuVolume {
    short left;
    short right;
} SpuVolume;

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

typedef struct SpuVoiceRegs {
    SpuVolume volume;
    u_short pitch;
    u_short addr;
    u_short adsr[2];
    u_short volumex;
    u_short loopAddr;
} SpuVoiceRegs;

typedef struct SpuCommonRegs {
    SpuVoiceRegs voice[24];
    SpuVolume mainVol;
    SpuVolume revVol;
    u_short keyOn[2];
    u_short keyOff[2];
    u_short chanFm[2];
    u_short noiseMode[2];
    u_short revMode[2];
    u_long chanOn;
    u_short unknown;
    u_short revWorkAddr;
    u_short irqAddr;
    u_short transAddr;
    u_short transFifo;
    volatile u_short spuCnt;
    u_short dataTrans;
    u_short spuStat;
    SpuVolume cdVol;
    SpuVolume extVol;
    SpuVolume mainVolCurrent;
    SpuVolume unknownVol;
} SpuCommonRegs;

typedef union SpuRegisterMap {
    SpuCommonRegs regs;
    volatile u_short raw[0x100];
} SpuRegisterMap;

#endif
