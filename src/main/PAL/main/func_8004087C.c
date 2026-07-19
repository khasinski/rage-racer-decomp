#include "common.h"
#include "game/track.h"

extern u8 *D_801E4150;
extern s32 D_8009E710;
extern s32 D_8009E778;
extern s32 D_8009E704;
extern GameTrackPoint *D_8009E688;
extern s32 D_8019CACC;

s32 func_80068634(s32 arg0);
void func_8005BEA8(s32 arg0, s32 arg1);

void func_8004087C(s16 arg) {
    s32 base;
    s16 *p;
    s16 *cur;
    s16 *end;
    s32 data;
    s16 lo;
    s32 s0, s1, s2, s3;
    s32 val;
    s32 t;
    s32 a0v, a1v;

    data = 0;
    base = (s32)D_801E4150;
    p = (s16 *)(base + 0x1B7C);
    end = (s16 *)(base + 0x1C6C);
    cur = p;
    do {
        lo = cur[0];
        if (arg < lo) {
            goto endchk;
        }
        if (cur[1] >= arg) {
            goto found;
        }
    endchk:
        if (lo == -1) {
            break;
        }
        p = cur + 4;
        cur = p;
    } while ((s32)p < (s32)end);

after:
    if (data == 0) {
        goto zero;
    }
    s0 = D_8009E710;
    if (s0 >= 0) {
        goto sub;
    }
    s0 += 0x100;
    if (s0 <= 0) {
        goto chk;
    }
    s0 = 0;
    goto chk;
found:
    data = p[2];
    goto after;
sub:
    s0 -= 0x100;
    if (s0 >= 0) {
        goto chk;
    }
    s0 = 0;
chk:
    if (s0 != 0) {
        s0 = (s0 * D_8009E778) / 12775;
        t = *(s32 *)0x1F80001C - 0xC00;
        s3 = (t + D_8009E688[D_8009E704].angle) & 0xFFF;
        if (s0 < 0 && (data & 2) > 0) {
            val = s0 * func_80068634(s3);
            if (val < 0) {
                val += 0xFFF;
            }
            s1 = -(s0 + (val >> 12));
            val = (-s0) * func_80068634(s3);
            if (val < 0) {
                val += 0xFFF;
            }
            s2 = -(s0 + (val >> 12));
        } else if (s0 > 0 && (data & 1) > 0) {
            val = s0 * func_80068634(s3);
            if (val < 0) {
                val += 0xFFF;
            }
            s2 = s0 + (val >> 12);
            val = (-s0) * func_80068634(s3);
            if (val < 0) {
                val += 0xFFF;
            }
            s1 = s0 + (val >> 12);
        } else {
            s2 = 0;
            s1 = 0;
        }
    } else {
        s2 = 0;
        s1 = 0;
    }
    if (D_8019CACC) {
        a0v = s2;
        a1v = s1;
    } else {
        a0v = s1;
        a1v = s2;
    }
    goto call;
zero:
    a0v = 0;
    a1v = 0;
call:
    func_8005BEA8(a0v, a1v);
}
