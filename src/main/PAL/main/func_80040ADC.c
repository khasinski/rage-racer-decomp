#include "common.h"

extern u8 *D_801E4150;
extern s32 D_801E408C;
extern s32 D_801E40D8;
extern s32 D_8019CACC;

s32 func_8006888C(s32 arg0);
s32 func_8001A6AC(s32 arg0, s32 arg1);
s32 func_80068568(s32 arg0);
void func_8005C31C(s32 arg0, s32 arg1, s32 arg2);

typedef struct {
    s32 lo;    /* 0x00 */
    s32 hi;    /* 0x04 */
    u16 f08;   /* 0x08 */
    u16 f0A;   /* 0x0A */
    s32 f0C;   /* 0x0C */
    s32 f10;   /* 0x10 */
    s32 f14;   /* 0x14 */
} TrackSeg;    /* size 0x18 */

void func_80040ADC(s32 arg) {
    register s32 base asm("v0");
    register s32 startp asm("a1");
    register TrackSeg *seg asm("a3");
    register s32 v1 asm("v1");
    register s32 t0 asm("t0");
    register u16 a1raw asm("a1");
    register u16 t1raw asm("t1");
    register s32 sentinel asm("t2");
    register s32 s0v asm("s0");
    register s32 s1 asm("s1");
    register s32 s2 asm("s2");
    register s32 s3 asm("s3");
    register s32 s4 asm("s4");
    register s32 s5 asm("s5");
    register s32 s6 asm("s6");
    register s32 a2v asm("a2");
    register s32 a1s asm("a1");
    s32 v0;
    s32 angle, sinv;

    base = (s32)D_801E4150;
    startp = base + 0x1C6C;
    if (D_801E408C != 0) {
        arg = D_801E40D8 - arg;
    }

    s5 = 0;
    a2v = 0;
    s2 = 0;
    s0v = 0;
    sentinel = -1;
    seg = (TrackSeg *)startp;
loop:
    v1 = seg->lo;
    t0 = seg->hi;
    if (v1 == sentinel) {
        goto matched;
    }
    a1raw = seg->f08;
    t1raw = seg->f0A;
    if (arg < v1) {
        goto next;
    }
    if (t0 < arg) {
        goto next;
    }
    v0 = a1raw << 16;
    __asm__("" : "=r"(v0) : "0"(v0));
    a1s = v0 >> 16;
    if (arg < v1 + a1s) {
        v0 = arg - v1;
    } else {
        v0 = t1raw << 16;
        __asm__("" : "=r"(v0) : "0"(v0));
        a1s = v0 >> 16;
        if ((t0 - a1s) < arg) {
            v0 = t0 - arg;
        } else {
            goto set30;
        }
    }
    v1 = (v0 * 48) / a1s;
    s2 = v1;
    goto load;
set30:
    s2 = 0x30;
load:
    s3 = seg->f0C;
    s4 = seg->f10;
    s6 = seg->f14;
    goto matched;
next:
    s0v++;
    seg++;
    if (s0v < 2) {
        goto loop;
    }

matched:
    v0 = s2 << 16;
    __asm__("" : "=r"(v0) : "0"(v0));
    s0v = v0 >> 16;
    if (s0v != 0) {
        s3 -= *(s32 *)0x1F800008;
        s4 -= *(s32 *)0x1F800010;
        v0 = func_8006888C((s3 * s3) / 4 + (s4 * s4) / 4);
        v0 = s2 - (v0 >> 11);
        s1 = v0;
        __asm__("" : "=r"(v0) : "0"(v0));
        v0 = s0v < (s16)v0;
        __asm__("" : "=r"(v0) : "0"(v0));
        if (v0) {
            s1 = s2;
        }
        if ((s16)s1 < 0) {
            s1 = 0;
        }
        angle = func_8001A6AC(s3, s4);
        v1 = *(s32 *)0x1F80001C;
        __asm__("" : "=r"(v1) : "0"(v1));
        v1 -= 0xC00;
        v1 += angle;
        s0v = v1 & 0xFFF;
        sinv = func_80068568(s0v);
        v1 = s1 << 16;
        __asm__("" : "=r"(v1) : "0"(v1));
        s1 = v1 >> 16;
        v0 = s2 + (s1 * sinv) / 4096;
        __asm__("" : "=r"(v0) : "0"(v0));
        s5 = v0 + 0x20;
        sinv = func_80068568(s0v);
        v1 = -s1;
        __asm__("" : "=r"(v1) : "0"(v1));
        v0 = s2 + (v1 * sinv) / 4096;
        __asm__("" : "=r"(v0) : "0"(v0));
        a2v = v0 + 0x20;
        if (s6 < 0) {
            s6 = -s6;
        }
    }

    if (s6 == 1) {
        if (D_8019CACC != 0) {
            func_8005C31C(2, (s16)s5, (s16)a2v);
        } else {
            func_8005C31C(2, (s16)a2v, (s16)s5);
        }
    } else {
        if (D_8019CACC != 0) {
            func_8005C31C(3, (s16)s5, (s16)a2v);
        } else {
            func_8005C31C(3, (s16)a2v, (s16)s5);
        }
    }
}
