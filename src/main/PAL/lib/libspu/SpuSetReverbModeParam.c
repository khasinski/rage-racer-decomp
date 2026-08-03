#include "psyq/spu.h"
#include "psyq/kernel.h"

extern long g_SpuRevWorkAreaAddr asm("D_8009A720");
extern SpuRevAttrState g_SpuRevAttr asm("D_8009A728");
extern long g_SpuTransferEvent asm("D_8009A768");
extern long g_SpuZeroBuf[] asm("D_8009A770");
extern SpuCommonRegs *g_SpuRegBase asm("D_8009AB7C");
extern long g_SpuTransferByIo asm("D_8009AB94");
extern long _spu_mem_mode_unitM asm("D_8009ABA0");
extern volatile long g_SpuTransferCallback asm("D_8009ABB0");
extern long g_SpuRevWorkAreaStartAddr[] asm("D_8009ABE0");
extern SpuReverbRegAttr g_SpuRevAttrTable[] asm("D_8009AC30");

#define SPU_REG16 ((volatile u_short *)g_SpuRegBase)

static inline void copy_reverb_attr(SpuReverbRegAttr *dst, SpuReverbRegAttr *src) {
    u_char *d = (u_char *)dst;
    u_char *s = (u_char *)src;
    long size = sizeof(SpuReverbRegAttr) - 1;

    while (size != -1) {
        *d++ = *s++;
        size--;
    }
}

long SpuSetReverbModeParam(SpuReverbAttr *attr) {
    SpuReverbRegAttr entry;
    long reenable;
    long mode_changed;
    long delay_changed;
    long feedback_changed;
    long clear_wa;
    long depth_changed;
    u_long mask;
    long set_all;
    u_long mode;
    long delay;
    u_short cnt;

    reenable = 0;
    mode_changed = 0;
    delay_changed = 0;
    clear_wa = 0;
    feedback_changed = 0;
    depth_changed = 0;

    mask = attr->mask;
    set_all = attr->mask == 0;
    entry.flags = 0;

    if (set_all || (mask & 0x1)) {
        mode = attr->mode;
        if (mode & 0x100) {
            mode &= ~0x100;
            clear_wa = 1;
        }
        if ((mode >= 0xA) || (_SpuIsInAllocateArea_(g_SpuRevWorkAreaStartAddr[mode]) != 0)) {
            return -1;
        }
        mode_changed = 1;
        g_SpuRevAttr.mode = mode;
        g_SpuRevWorkAreaAddr = g_SpuRevWorkAreaStartAddr[g_SpuRevAttr.mode];
        copy_reverb_attr(&entry, &g_SpuRevAttrTable[g_SpuRevAttr.mode]);

        switch (g_SpuRevAttr.mode) {
        case 7:
            g_SpuRevAttr.feedback = 0x7F;
            g_SpuRevAttr.delay = 0x7F;
            break;
        case 8:
            g_SpuRevAttr.feedback = 0;
            g_SpuRevAttr.delay = 0x7F;
            break;
        default:
            g_SpuRevAttr.feedback = 0;
            g_SpuRevAttr.delay = 0;
            break;
        }
    }

    if (set_all || (mask & 0x8)) {
        if (g_SpuRevAttr.mode < 9) {
            if (g_SpuRevAttr.mode >= 7) {
                delay_changed = 1;
                if (!mode_changed) {
                    copy_reverb_attr(&entry, &g_SpuRevAttrTable[g_SpuRevAttr.mode]);
                    entry.flags = 0x0C011C00;
                }
                delay = attr->delay;
                g_SpuRevAttr.delay = delay;
                entry.mLSAME = ((delay << 13) / 0x7F) - entry.dAPF1;
                entry.mRSAME = ((delay << 12) / 0x7F) - entry.dAPF2;
                entry.mLCOMB1 = ((delay << 12) / 0x7F) + entry.mRCOMB1;
                entry.dLSAME = ((delay << 12) / 0x7F) + entry.dRSAME;
                entry.mLAPF1 = ((delay << 12) / 0x7F) + entry.mLAPF2;
                entry.mRAPF1 = ((delay << 12) / 0x7F) + entry.mRAPF2;
            } else {
                g_SpuRevAttr.delay = 0;
            }
        } else {
            g_SpuRevAttr.delay = 0;
        }
    }

    if (set_all || (mask & 0x10)) {
        if (g_SpuRevAttr.mode < 9) {
            if (g_SpuRevAttr.mode >= 7) {
                feedback_changed = 1;
                if (!mode_changed) {
                    if (!delay_changed) {
                        copy_reverb_attr(&entry, &g_SpuRevAttrTable[g_SpuRevAttr.mode]);
                        entry.flags = 0x80;
                    } else {
                        entry.flags |= 0x80;
                    }
                }
                g_SpuRevAttr.feedback = attr->feedback;
                entry.vWALL = (g_SpuRevAttr.feedback * 0x8100) / 0x7F;
            } else {
                g_SpuRevAttr.feedback = 0;
            }
        } else {
            g_SpuRevAttr.feedback = 0;
        }
    }

    if (mode_changed) {
        reenable = (g_SpuRegBase->spuCnt >> 7) & 1;
        if (reenable) {
            cnt = g_SpuRegBase->spuCnt;
            cnt &= ~0x80;
            g_SpuRegBase->spuCnt = cnt;
        }
    }

    if (!mode_changed) {
        if (set_all || (mask & 0x2)) {
            g_SpuRegBase->revVol.left = attr->depth.left;
            g_SpuRevAttr.depth_left = attr->depth.left;
        }
        if (set_all || (mask & 0x4)) {
            g_SpuRegBase->revVol.right = attr->depth.right;
            g_SpuRevAttr.depth_right = attr->depth.right;
        }
    } else {
        g_SpuRegBase->revVol.left = 0;
        g_SpuRegBase->revVol.right = 0;
        g_SpuRevAttr.depth_left = 0;
        g_SpuRevAttr.depth_right = 0;
    }

    if (mode_changed || delay_changed || feedback_changed) {
        _spu_setReverbAttr(&entry);
    }
    if (clear_wa) {
        SpuClearReverbWorkArea(g_SpuRevAttr.mode);
    }
    if (mode_changed) {
        _spu_FsetRXX(0xD1, g_SpuRevWorkAreaAddr, 0);
        if (reenable) {
            cnt = g_SpuRegBase->spuCnt;
            cnt |= 0x80;
            g_SpuRegBase->spuCnt = cnt;
        }
    }
    return 0;
}

void _spu_setReverbAttr(SpuReverbRegAttr *attr) {
    u_long mask = attr->flags;
    long set_all = attr->flags == 0;

    if (set_all || (mask & 0x1)) {
        SPU_REG16[0xE0] = attr->dAPF1;
    }
    if (set_all || (mask & 0x2)) {
        SPU_REG16[0xE1] = attr->dAPF2;
    }
    if (set_all || (mask & 0x4)) {
        SPU_REG16[0xE2] = attr->vIIR;
    }
    if (set_all || (mask & 0x8)) {
        SPU_REG16[0xE3] = attr->vCOMB1;
    }
    if (set_all || (mask & 0x10)) {
        SPU_REG16[0xE4] = attr->vCOMB2;
    }
    if (set_all || (mask & 0x20)) {
        SPU_REG16[0xE5] = attr->vCOMB3;
    }
    if (set_all || (mask & 0x40)) {
        SPU_REG16[0xE6] = attr->vCOMB4;
    }
    if (set_all || (mask & 0x80)) {
        SPU_REG16[0xE7] = attr->vWALL;
    }
    if (set_all || (mask & 0x100)) {
        SPU_REG16[0xE8] = attr->vAPF1;
    }
    if (set_all || (mask & 0x200)) {
        SPU_REG16[0xE9] = attr->vAPF2;
    }
    if (set_all || (mask & 0x400)) {
        SPU_REG16[0xEA] = attr->mLSAME;
    }
    if (set_all || (mask & 0x800)) {
        SPU_REG16[0xEB] = attr->mRSAME;
    }
    if (set_all || (mask & 0x1000)) {
        SPU_REG16[0xEC] = attr->mLCOMB1;
    }
    if (set_all || (mask & 0x2000)) {
        SPU_REG16[0xED] = attr->mRCOMB1;
    }
    if (set_all || (mask & 0x4000)) {
        SPU_REG16[0xEE] = attr->mLCOMB2;
    }
    if (set_all || (mask & 0x8000)) {
        SPU_REG16[0xEF] = attr->mRCOMB2;
    }
    if (set_all || (mask & 0x10000)) {
        SPU_REG16[0xF0] = attr->dLSAME;
    }
    if (set_all || (mask & 0x20000)) {
        SPU_REG16[0xF1] = attr->dRSAME;
    }
    if (set_all || (mask & 0x40000)) {
        SPU_REG16[0xF2] = attr->mLDIFF;
    }
    if (set_all || (mask & 0x80000)) {
        SPU_REG16[0xF3] = attr->mRDIFF;
    }
    if (set_all || (mask & 0x100000)) {
        SPU_REG16[0xF4] = attr->mLCOMB3;
    }
    if (set_all || (mask & 0x200000)) {
        SPU_REG16[0xF5] = attr->mRCOMB3;
    }
    if (set_all || (mask & 0x400000)) {
        SPU_REG16[0xF6] = attr->mLCOMB4;
    }
    if (set_all || (mask & 0x800000)) {
        SPU_REG16[0xF7] = attr->mRCOMB4;
    }
    if (set_all || (mask & 0x1000000)) {
        SPU_REG16[0xF8] = attr->dLDIFF;
    }
    if (set_all || (mask & 0x2000000)) {
        SPU_REG16[0xF9] = attr->dRDIFF;
    }
    if (set_all || (mask & 0x4000000)) {
        SPU_REG16[0xFA] = attr->mLAPF1;
    }
    if (set_all || (mask & 0x8000000)) {
        SPU_REG16[0xFB] = attr->mRAPF1;
    }
    if (set_all || (mask & 0x10000000)) {
        SPU_REG16[0xFC] = attr->mLAPF2;
    }
    if (set_all || (mask & 0x20000000)) {
        SPU_REG16[0xFD] = attr->mRAPF2;
    }
    if (set_all || (mask & 0x40000000)) {
        SPU_REG16[0xFE] = attr->vLIN;
    }
    if (set_all || (mask & 0x80000000)) {
        SPU_REG16[0xFF] = attr->vRIN;
    }
}

long SpuClearReverbWorkArea(u_long mode) {
    volatile long callback;
    long old_transmode;
    u_long size;
    u_long addr;
    long chunk_active;
    u_long chunk_size;
    long restored_transmode;

    callback = 0;
    restored_transmode = 0;
    if ((mode >= 0xA) || (_SpuIsInAllocateArea_(g_SpuRevWorkAreaStartAddr[mode]) != 0)) {
        return -1;
    }

    if (mode == 0) {
        size = 0x10 << _spu_mem_mode_unitM;
        addr = 0xFFF0 << _spu_mem_mode_unitM;
    } else {
        size = (0x10000 - g_SpuRevWorkAreaStartAddr[mode]) << _spu_mem_mode_unitM;
        addr = g_SpuRevWorkAreaStartAddr[mode] << _spu_mem_mode_unitM;
    }

    old_transmode = g_SpuTransferByIo;
    if (g_SpuTransferByIo == 1) {
        g_SpuTransferByIo = 0;
        restored_transmode = 1;
    }

    chunk_active = 1;
    if (g_SpuTransferCallback != 0) {
        callback = g_SpuTransferCallback;
        g_SpuTransferCallback = 0;
    }

    do {
        chunk_size = size;
        if (size >= 0x401) {
            chunk_size = 0x400;
        } else {
            chunk_active = 0;
        }

        _spu_t(2, addr);
        _spu_t(1);
        _spu_t(3, g_SpuZeroBuf, chunk_size);
        WaitEvent(g_SpuTransferEvent);
        size -= 0x400;
        addr += 0x400;
    } while (chunk_active != 0);

    if (restored_transmode != 0) {
        g_SpuTransferByIo = old_transmode;
    }
    if (callback != 0) {
        g_SpuTransferCallback = callback;
    }
    return 0;
}
