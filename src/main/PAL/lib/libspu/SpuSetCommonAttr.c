#include "psyq/spu.h"

typedef struct SpuVoiceRegs {
    SpuVolume volume;
    u16 pitch;
    u16 addr;
    u16 adsr[2];
    u16 volumex;
    u16 loopAddr;
} SpuVoiceRegs;

typedef struct SpuCommonRegs {
    SpuVoiceRegs voice[24];
    SpuVolume mainVol;
    SpuVolume revVol;
    u16 keyOn[2];
    u16 keyOff[2];
    u16 chanFm[2];
    u16 noiseMode[2];
    u16 revMode[2];
    u32 chanOn;
    u16 unknown;
    u16 revWorkAddr;
    u16 irqAddr;
    u16 transAddr;
    u16 transFifo;
    u16 spuCnt;
    u16 dataTrans;
    u16 spuStat;
    SpuVolume cdVol;
    SpuVolume extVol;
    SpuVolume mainVolCurrent;
    SpuVolume unknownVol;
} SpuCommonRegs;

extern SpuCommonRegs *D_8009AB7C;

void func_8007B2D0(SpuCommonAttr *attr) {
    u16 mainModeLeft;
    u16 mainModeRight;
    u16 totalLeft;
    u16 totalRight;
    u32 mask;
    s32 setAll;
    u16 control;

    totalLeft = 0;
    totalRight = 0;
    mask = attr->mask;
    setAll = attr->mask == 0;

    if (setAll != 0 || mask & 1) {
        if (setAll != 0 || mask & 4) {
            switch (attr->mvol.volmode.left) {
            case 1:
                mainModeLeft = 0x8000;
                break;
            case 2:
                mainModeLeft = 0x9000;
                break;
            case 3:
                mainModeLeft = 0xA000;
                break;
            case 4:
                mainModeLeft = 0xB000;
                break;
            case 5:
                mainModeLeft = 0xC000;
                break;
            case 6:
                mainModeLeft = 0xD000;
                break;
            case 7:
                mainModeLeft = 0xE000;
                break;
            case 0:
                totalLeft = attr->mvol.volume.left;
                mainModeLeft = 0;
                break;
            default:
                totalLeft = attr->mvol.volume.left;
                mainModeLeft = 0;
                break;
            }
        } else {
            totalLeft = attr->mvol.volume.left;
            mainModeLeft = 0;
        }

        if (mainModeLeft != 0) {
            if (attr->mvol.volume.left >= 0x80) {
                totalLeft = 0x7F;
            } else if (attr->mvol.volume.left < 0) {
                totalLeft = 0;
            } else {
                totalLeft = attr->mvol.volume.left;
            }
        }
        totalLeft &= 0x7FFF;
        D_8009AB7C->mainVol.left = totalLeft | mainModeLeft;
    }

    if (setAll != 0 || mask & 2) {
        if (setAll != 0 || mask & 8) {
            switch (attr->mvol.volmode.right) {
            case 1:
                mainModeRight = 0x8000;
                break;
            case 2:
                mainModeRight = 0x9000;
                break;
            case 3:
                mainModeRight = 0xA000;
                break;
            case 4:
                mainModeRight = 0xB000;
                break;
            case 5:
                mainModeRight = 0xC000;
                break;
            case 6:
                mainModeRight = 0xD000;
                break;
            case 7:
                mainModeRight = 0xE000;
                break;
            case 0:
                totalRight = attr->mvol.volume.right;
                mainModeRight = 0;
                break;
            default:
                totalRight = attr->mvol.volume.right;
                mainModeRight = 0;
                break;
            }
        } else {
            totalRight = attr->mvol.volume.right;
            mainModeRight = 0;
        }

        if (mainModeRight != 0) {
            if (attr->mvol.volume.right >= 0x80) {
                totalRight = 0x7F;
            } else if (attr->mvol.volume.right < 0) {
                totalRight = 0;
            } else {
                totalRight = attr->mvol.volume.right;
            }
        }
        totalRight &= 0x7FFF;
        D_8009AB7C->mainVol.right = totalRight | mainModeRight;
    }

    if (setAll != 0 || mask & 0x40) {
        D_8009AB7C->cdVol.left = attr->cd.volume.left;
    }

    if (setAll != 0 || mask & 0x80) {
        D_8009AB7C->cdVol.right = attr->cd.volume.right;
    }

    if (setAll != 0 || mask & 0x100) {
        if (attr->cd.reverb == 0) {
            control = D_8009AB7C->spuCnt;
            control &= ~4;
            D_8009AB7C->spuCnt = control;
        } else {
            control = D_8009AB7C->spuCnt;
            control |= 4;
            D_8009AB7C->spuCnt = control;
        }
    }

    if (setAll != 0 || mask & 0x200) {
        if (attr->cd.mix == 0) {
            control = D_8009AB7C->spuCnt;
            control &= ~1;
            D_8009AB7C->spuCnt = control;
        } else {
            control = D_8009AB7C->spuCnt;
            control |= 1;
            D_8009AB7C->spuCnt = control;
        }
    }

    if (setAll != 0 || mask & 0x400) {
        D_8009AB7C->extVol.left = attr->ext.volume.left;
    }

    if (setAll != 0 || mask & 0x800) {
        D_8009AB7C->extVol.right = attr->ext.volume.right;
    }

    if (setAll != 0 || mask & 0x1000) {
        if (attr->ext.reverb == 0) {
            control = D_8009AB7C->spuCnt;
            control &= ~8;
            D_8009AB7C->spuCnt = control;
        } else {
            control = D_8009AB7C->spuCnt;
            control |= 8;
            D_8009AB7C->spuCnt = control;
        }
    }

    if (setAll != 0 || mask & 0x2000) {
        if (attr->ext.mix == 0) {
            control = D_8009AB7C->spuCnt;
            control &= ~2;
            D_8009AB7C->spuCnt = control;
        } else {
            control = D_8009AB7C->spuCnt;
            control |= 2;
            D_8009AB7C->spuCnt = control;
        }
    }
}
