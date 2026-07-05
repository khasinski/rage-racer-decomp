#include "psyq/spu.h"

extern s32 D_8009A720;
extern SpuRevAttrState D_8009A728;
extern SpuRxx *D_8009AB7C;
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
