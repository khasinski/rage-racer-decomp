#include "psyq/spu.h"

extern volatile u16 *D_8009AB7C;

void _spu_setReverbAttr(SpuReverbRegAttr *attr) {
    u32 mask = attr->flags;
    s32 set_all = attr->flags == 0;

    if (set_all || (mask & 0x1)) {
        D_8009AB7C[0xE0] = attr->dAPF1;
    }
    if (set_all || (mask & 0x2)) {
        D_8009AB7C[0xE1] = attr->dAPF2;
    }
    if (set_all || (mask & 0x4)) {
        D_8009AB7C[0xE2] = attr->vIIR;
    }
    if (set_all || (mask & 0x8)) {
        D_8009AB7C[0xE3] = attr->vCOMB1;
    }
    if (set_all || (mask & 0x10)) {
        D_8009AB7C[0xE4] = attr->vCOMB2;
    }
    if (set_all || (mask & 0x20)) {
        D_8009AB7C[0xE5] = attr->vCOMB3;
    }
    if (set_all || (mask & 0x40)) {
        D_8009AB7C[0xE6] = attr->vCOMB4;
    }
    if (set_all || (mask & 0x80)) {
        D_8009AB7C[0xE7] = attr->vWALL;
    }
    if (set_all || (mask & 0x100)) {
        D_8009AB7C[0xE8] = attr->vAPF1;
    }
    if (set_all || (mask & 0x200)) {
        D_8009AB7C[0xE9] = attr->vAPF2;
    }
    if (set_all || (mask & 0x400)) {
        D_8009AB7C[0xEA] = attr->mLSAME;
    }
    if (set_all || (mask & 0x800)) {
        D_8009AB7C[0xEB] = attr->mRSAME;
    }
    if (set_all || (mask & 0x1000)) {
        D_8009AB7C[0xEC] = attr->mLCOMB1;
    }
    if (set_all || (mask & 0x2000)) {
        D_8009AB7C[0xED] = attr->mRCOMB1;
    }
    if (set_all || (mask & 0x4000)) {
        D_8009AB7C[0xEE] = attr->mLCOMB2;
    }
    if (set_all || (mask & 0x8000)) {
        D_8009AB7C[0xEF] = attr->mRCOMB2;
    }
    if (set_all || (mask & 0x10000)) {
        D_8009AB7C[0xF0] = attr->dLSAME;
    }
    if (set_all || (mask & 0x20000)) {
        D_8009AB7C[0xF1] = attr->dRSAME;
    }
    if (set_all || (mask & 0x40000)) {
        D_8009AB7C[0xF2] = attr->mLDIFF;
    }
    if (set_all || (mask & 0x80000)) {
        D_8009AB7C[0xF3] = attr->mRDIFF;
    }
    if (set_all || (mask & 0x100000)) {
        D_8009AB7C[0xF4] = attr->mLCOMB3;
    }
    if (set_all || (mask & 0x200000)) {
        D_8009AB7C[0xF5] = attr->mRCOMB3;
    }
    if (set_all || (mask & 0x400000)) {
        D_8009AB7C[0xF6] = attr->mLCOMB4;
    }
    if (set_all || (mask & 0x800000)) {
        D_8009AB7C[0xF7] = attr->mRCOMB4;
    }
    if (set_all || (mask & 0x1000000)) {
        D_8009AB7C[0xF8] = attr->dLDIFF;
    }
    if (set_all || (mask & 0x2000000)) {
        D_8009AB7C[0xF9] = attr->dRDIFF;
    }
    if (set_all || (mask & 0x4000000)) {
        D_8009AB7C[0xFA] = attr->mLAPF1;
    }
    if (set_all || (mask & 0x8000000)) {
        D_8009AB7C[0xFB] = attr->mRAPF1;
    }
    if (set_all || (mask & 0x10000000)) {
        D_8009AB7C[0xFC] = attr->mLAPF2;
    }
    if (set_all || (mask & 0x20000000)) {
        D_8009AB7C[0xFD] = attr->mRAPF2;
    }
    if (set_all || (mask & 0x40000000)) {
        D_8009AB7C[0xFE] = attr->vLIN;
    }
    if (set_all || (mask & 0x80000000)) {
        D_8009AB7C[0xFF] = attr->vRIN;
    }
}
