#include "psyq/spu.h"
#include "psyq/kernel.h"

extern s32 D_8009A720;
extern SpuRevAttrState D_8009A728;
extern s32 D_8009A768;
extern s32 D_8009A770[];
extern SpuRxx *D_8009AB7C;
extern s32 D_8009AB94;
extern s32 _spu_mem_mode_unitM asm("D_8009ABA0");
extern volatile s32 D_8009ABB0;
extern volatile u16 *D_8009AB7C_half asm("D_8009AB7C");
extern s32 D_8009ABE0[];
extern SpuReverbRegAttr D_8009AC30[];

static inline void copy_reverb_attr(SpuReverbRegAttr *dst, SpuReverbRegAttr *src) {
    u8 *d = (u8 *)dst;
    u8 *s = (u8 *)src;
    s32 size = sizeof(SpuReverbRegAttr) - 1;

    while (size != -1) {
        *d++ = *s++;
        size--;
    }
}

s32 SpuSetReverbModeParam(SpuReverbAttr *attr) {
    SpuReverbRegAttr entry;
    s32 reenable;
    s32 mode_changed;
    s32 delay_changed;
    s32 feedback_changed;
    s32 clear_wa;
    s32 depth_changed;
    u32 mask;
    s32 set_all;
    u32 mode;
    register s32 delay asm("a0");
    u16 cnt;

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
        if ((mode >= 0xA) || (_SpuIsInAllocateArea_(D_8009ABE0[mode]) != 0)) {
            return -1;
        }
        mode_changed = 1;
        D_8009A728.mode = mode;
        D_8009A720 = D_8009ABE0[D_8009A728.mode];
        copy_reverb_attr(&entry, &D_8009AC30[D_8009A728.mode]);

        switch (D_8009A728.mode) {
        case 7:
            D_8009A728.feedback = 0x7F;
            D_8009A728.delay = 0x7F;
            break;
        case 8:
            D_8009A728.feedback = 0;
            D_8009A728.delay = 0x7F;
            break;
        default:
            D_8009A728.feedback = 0;
            D_8009A728.delay = 0;
            break;
        }
    }

    if (set_all || (mask & 0x8)) {
        if (D_8009A728.mode < 9) {
            if (D_8009A728.mode >= 7) {
                delay_changed = 1;
                if (!mode_changed) {
                    copy_reverb_attr(&entry, &D_8009AC30[D_8009A728.mode]);
                    entry.flags = 0x0C011C00;
                }
                delay = attr->delay;
                D_8009A728.delay = delay;
                entry.mLSAME = ((delay << 13) / 0x7F) - entry.dAPF1;
                entry.mRSAME = ((delay << 12) / 0x7F) - entry.dAPF2;
                entry.mLCOMB1 = ((delay << 12) / 0x7F) + entry.mRCOMB1;
                entry.dLSAME = ((delay << 12) / 0x7F) + entry.dRSAME;
                entry.mLAPF1 = ((delay << 12) / 0x7F) + entry.mLAPF2;
                entry.mRAPF1 = ((delay << 12) / 0x7F) + entry.mRAPF2;
            } else {
                D_8009A728.delay = 0;
            }
        } else {
            D_8009A728.delay = 0;
        }
    }

    if (set_all || (mask & 0x10)) {
        if (D_8009A728.mode < 9) {
            if (D_8009A728.mode >= 7) {
                feedback_changed = 1;
                if (!mode_changed) {
                    if (!delay_changed) {
                        copy_reverb_attr(&entry, &D_8009AC30[D_8009A728.mode]);
                        entry.flags = 0x80;
                    } else {
                        entry.flags |= 0x80;
                    }
                }
                D_8009A728.feedback = attr->feedback;
                entry.vWALL = (D_8009A728.feedback * 0x8100) / 0x7F;
            } else {
                D_8009A728.feedback = 0;
            }
        } else {
            D_8009A728.feedback = 0;
        }
    }

    if (mode_changed) {
        reenable = (D_8009AB7C->spucnt >> 7) & 1;
        if (reenable) {
            cnt = D_8009AB7C->spucnt;
            cnt &= ~0x80;
            D_8009AB7C->spucnt = cnt;
        }
    }

    if (!mode_changed) {
        if (set_all || (mask & 0x2)) {
            D_8009AB7C->rev_vol_left = attr->depth.left;
            D_8009A728.depth_left = attr->depth.left;
        }
        if (set_all || (mask & 0x4)) {
            D_8009AB7C->rev_vol_right = attr->depth.right;
            D_8009A728.depth_right = attr->depth.right;
        }
    } else {
        D_8009AB7C->rev_vol_left = 0;
        D_8009AB7C->rev_vol_right = 0;
        D_8009A728.depth_left = 0;
        D_8009A728.depth_right = 0;
    }

    if (mode_changed || delay_changed || feedback_changed) {
        _spu_setReverbAttr(&entry);
    }
    if (clear_wa) {
        SpuClearReverbWorkArea(D_8009A728.mode);
    }
    if (mode_changed) {
        _spu_FsetRXX(0xD1, D_8009A720, 0);
        if (reenable) {
            cnt = D_8009AB7C->spucnt;
            cnt |= 0x80;
            D_8009AB7C->spucnt = cnt;
        }
    }
    return 0;
}

void _spu_setReverbAttr(SpuReverbRegAttr *attr) {
    u32 mask = attr->flags;
    s32 set_all = attr->flags == 0;

    if (set_all || (mask & 0x1)) {
        D_8009AB7C_half[0xE0] = attr->dAPF1;
    }
    if (set_all || (mask & 0x2)) {
        D_8009AB7C_half[0xE1] = attr->dAPF2;
    }
    if (set_all || (mask & 0x4)) {
        D_8009AB7C_half[0xE2] = attr->vIIR;
    }
    if (set_all || (mask & 0x8)) {
        D_8009AB7C_half[0xE3] = attr->vCOMB1;
    }
    if (set_all || (mask & 0x10)) {
        D_8009AB7C_half[0xE4] = attr->vCOMB2;
    }
    if (set_all || (mask & 0x20)) {
        D_8009AB7C_half[0xE5] = attr->vCOMB3;
    }
    if (set_all || (mask & 0x40)) {
        D_8009AB7C_half[0xE6] = attr->vCOMB4;
    }
    if (set_all || (mask & 0x80)) {
        D_8009AB7C_half[0xE7] = attr->vWALL;
    }
    if (set_all || (mask & 0x100)) {
        D_8009AB7C_half[0xE8] = attr->vAPF1;
    }
    if (set_all || (mask & 0x200)) {
        D_8009AB7C_half[0xE9] = attr->vAPF2;
    }
    if (set_all || (mask & 0x400)) {
        D_8009AB7C_half[0xEA] = attr->mLSAME;
    }
    if (set_all || (mask & 0x800)) {
        D_8009AB7C_half[0xEB] = attr->mRSAME;
    }
    if (set_all || (mask & 0x1000)) {
        D_8009AB7C_half[0xEC] = attr->mLCOMB1;
    }
    if (set_all || (mask & 0x2000)) {
        D_8009AB7C_half[0xED] = attr->mRCOMB1;
    }
    if (set_all || (mask & 0x4000)) {
        D_8009AB7C_half[0xEE] = attr->mLCOMB2;
    }
    if (set_all || (mask & 0x8000)) {
        D_8009AB7C_half[0xEF] = attr->mRCOMB2;
    }
    if (set_all || (mask & 0x10000)) {
        D_8009AB7C_half[0xF0] = attr->dLSAME;
    }
    if (set_all || (mask & 0x20000)) {
        D_8009AB7C_half[0xF1] = attr->dRSAME;
    }
    if (set_all || (mask & 0x40000)) {
        D_8009AB7C_half[0xF2] = attr->mLDIFF;
    }
    if (set_all || (mask & 0x80000)) {
        D_8009AB7C_half[0xF3] = attr->mRDIFF;
    }
    if (set_all || (mask & 0x100000)) {
        D_8009AB7C_half[0xF4] = attr->mLCOMB3;
    }
    if (set_all || (mask & 0x200000)) {
        D_8009AB7C_half[0xF5] = attr->mRCOMB3;
    }
    if (set_all || (mask & 0x400000)) {
        D_8009AB7C_half[0xF6] = attr->mLCOMB4;
    }
    if (set_all || (mask & 0x800000)) {
        D_8009AB7C_half[0xF7] = attr->mRCOMB4;
    }
    if (set_all || (mask & 0x1000000)) {
        D_8009AB7C_half[0xF8] = attr->dLDIFF;
    }
    if (set_all || (mask & 0x2000000)) {
        D_8009AB7C_half[0xF9] = attr->dRDIFF;
    }
    if (set_all || (mask & 0x4000000)) {
        D_8009AB7C_half[0xFA] = attr->mLAPF1;
    }
    if (set_all || (mask & 0x8000000)) {
        D_8009AB7C_half[0xFB] = attr->mRAPF1;
    }
    if (set_all || (mask & 0x10000000)) {
        D_8009AB7C_half[0xFC] = attr->mLAPF2;
    }
    if (set_all || (mask & 0x20000000)) {
        D_8009AB7C_half[0xFD] = attr->mRAPF2;
    }
    if (set_all || (mask & 0x40000000)) {
        D_8009AB7C_half[0xFE] = attr->vLIN;
    }
    if (set_all || (mask & 0x80000000)) {
        D_8009AB7C_half[0xFF] = attr->vRIN;
    }
}

s32 SpuClearReverbWorkArea(u32 mode) {
    volatile s32 callback;
    s32 old_transmode;
    u32 size;
    u32 addr;
    s32 chunk_active;
    u32 chunk_size;
    s32 restored_transmode;

    callback = 0;
    restored_transmode = 0;
    if ((mode >= 0xA) || (_SpuIsInAllocateArea_(D_8009ABE0[mode]) != 0)) {
        return -1;
    }

    if (mode == 0) {
        size = 0x10 << _spu_mem_mode_unitM;
        addr = 0xFFF0 << _spu_mem_mode_unitM;
    } else {
        size = (0x10000 - D_8009ABE0[mode]) << _spu_mem_mode_unitM;
        addr = D_8009ABE0[mode] << _spu_mem_mode_unitM;
    }

    old_transmode = D_8009AB94;
    if (D_8009AB94 == 1) {
        D_8009AB94 = 0;
        restored_transmode = 1;
    }

    chunk_active = 1;
    if (D_8009ABB0 != 0) {
        callback = D_8009ABB0;
        D_8009ABB0 = 0;
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
        _spu_t(3, D_8009A770, chunk_size);
        WaitEvent(D_8009A768);
        size -= 0x400;
        addr += 0x400;
    } while (chunk_active != 0);

    if (restored_transmode != 0) {
        D_8009AB94 = old_transmode;
    }
    if (callback != 0) {
        D_8009ABB0 = callback;
    }
    return 0;
}
