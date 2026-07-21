#include "common.h"

typedef struct KE {
    u16 f0;
    u16 f2;
    u16 f4;
    u16 f6;
    u16 f8;
    u16 fA;
    s32 fC;
    s16 f10;
    s16 f12;
} KE;

typedef struct B16 {
    s32 a;
    s32 b;
    s32 c;
    s32 d;
} B16;

typedef struct Obj {
    s32 x;
    s32 y;
    s32 z;
    s32 f0C;
    s32 f10;
    s32 f14;
    s32 f18;
    s32 f1C;
    s32 f20;
    s32 f24;
    s32 f28;
    s32 f2C;
} Obj;

extern u8 *volatile D_8019CAF8;
extern s32 D_801E408C;
extern KE *D_801E4038;
extern s32 D_8009AFB4;
extern s16 D_8009AFBC;
extern s16 D_8009AFBE;
extern s16 D_8009AFC0;

s32 func_80068634(s32 angle);
s32 func_80068568(s32 angle);
s32 func_8006888C(s32 arg0);
s32 func_8001A6AC(s32 arg0, s32 arg1);
void func_8001A610(void);
void func_80017A10(s32 arg0);
void func_8001DAB0(void *arg0);
void func_80033AA0(s32 arg0, s32 arg1);
void func_80043BCC(void *arg0, s32 arg1);

void func_8003C508(Obj *obj, s32 mode) {
    s32 *spad = (s32 *) 0x1F800000;
    register s32 s0v asm("$16");
    s32 delta[3];

    __asm__("" : "=r"(s0v) : "0"(28), "r"(spad));
    if (mode < 90) {
        if (mode < 2) {
            u8 *base = D_8019CAF8;
            s16 n = *(s16 *) (base + 2 * D_801E408C);
            s32 off = n * 20 + 4;
            KE *p = (KE *) (off + (s32) base);
            KE *q;
            D_801E4038 = p;
            *(B16 *) 0x1F800008 = *(B16 *) p;
            q = D_801E4038;
            D_8009AFBC = -q[0].f0 + q[1].f0;
            __asm__ volatile("");
            D_8009AFBE = -q[0].f4 + q[1].f4;
            __asm__ volatile("");
            D_8009AFC0 = -q[0].f8 + q[1].f8;
            __asm__ volatile("");
            D_8009AFB4 = q[0].f12;
        } else {
            register KE *a asm("$4") = D_801E4038;
            if (mode == a->f10) {
                D_801E4038 = &a[1];
                D_8009AFB4 = a[1].f12;
                if (a[1].fC == 1) {
                    D_8009AFBC = -a[1].f0 + ((u16 *) obj)[0];
                    D_8009AFBE = -a[1].f4 - 28 + ((u16 *) obj)[2];
                    D_8009AFC0 = -a[1].f8 + ((u16 *) obj)[4];
                } else {
                    D_8009AFBC = -a[1].f0 + a[2].f0;
                    D_8009AFBE = -a[1].f4 + a[2].f4;
                    D_8009AFC0 = -a[1].f8 + a[2].f8;
                }
            }
        }

        D_8009AFB4 = D_8009AFB4 - 1;
        if (D_8009AFB4 <= 0) {
            D_8009AFB4 = 0;
        }

        if (D_801E4038->fC == 0) {
            spad[2] = ((s32 *) D_801E4038)[0]
                      + ((s32) D_8009AFBC * func_80068634((D_8009AFB4 << 10) / D_801E4038->f12)) / 4096;
            spad[3] = ((s32 *) D_801E4038)[1]
                      + ((s32) D_8009AFBE * func_80068634((D_8009AFB4 << 10) / D_801E4038->f12)) / 4096;
            spad[4] = ((s32 *) D_801E4038)[2]
                      + ((s32) D_8009AFC0 * func_80068634((D_8009AFB4 << 10) / D_801E4038->f12)) / 4096;

            delta[0] = func_80068568(obj->f24) / 128 + obj->x - spad[2];
            delta[1] = obj->y - s0v - spad[3];
            delta[2] = func_80068634(obj->f24) / 128 + obj->z - spad[4];
            s0v = 0x400;
            spad[7] = s0v - func_8001A6AC(delta[0], delta[2]);
            s0v = s0v - func_8001A6AC(delta[1], func_8006888C(delta[0] * delta[0] + delta[2] * delta[2]) >> 6);
            spad[6] = s0v;
            spad[8] = 0;
            func_8001A610();
            func_80017A10(0);
            func_8001DAB0(obj);
        } else {
            func_80033AA0(D_8009AFB4 * 26, 0x29);
            {
                s32 c0 = obj->x;
                s32 c1 = obj->y;
                s32 c2 = obj->z;
                s32 c3 = obj->f0C;
                spad[2] = c0;
                spad[3] = c1;
                spad[4] = c2;
                spad[5] = c3;
            }
            __asm__ volatile("");
            spad[3] -= s0v;
            {
                s32 c0 = obj->f20;
                s32 c1 = obj->f24;
                s32 c2 = obj->f28;
                s32 c3 = obj->f2C;
                spad[6] = c0;
                spad[7] = c1;
                spad[8] = c2;
                spad[9] = c3;
            }
            __asm__ volatile("");
            func_8001A610();
        }
    } else {
        func_80043BCC(obj, 0);
    }
}
