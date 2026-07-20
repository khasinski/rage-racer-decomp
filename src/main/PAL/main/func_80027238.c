#include "common.h"

extern u8 D_800111C4;
extern u8 D_800111DC;
extern u8 D_800111F4;

extern volatile s32 D_8007D790;
extern volatile s32 D_8007D794;
extern volatile s32 D_8007D79C;

extern void (*D_8007D78C)(s32, s32);
extern volatile s32 D_8007D798;
extern volatile s32 D_8007D7A0;
extern volatile s32 D_8007D7A4;
extern volatile s32 D_8007D7A8;
extern volatile s32 D_8007D7AC;
extern volatile s32 D_8007D7B0;
extern volatile s32 D_8007D7B4;
extern volatile s32 D_8007D7B8;

void func_8006A970(void *arg0, s32 arg1);
s32 func_8006AADC(void *arg0);
void func_80063C38(void *arg0);
s32 func_8006DD30(s32 arg0);
s32 func_8006A574(s32 arg0);
s32 func_8006A58C(s32 arg0);
s32 func_8006A5A4(s32 arg0, s32 arg1, s32 arg2);
s32 func_8006A3E8(void);
s32 func_8006A418(void);
s32 func_8006A3F8(void);
void func_8006A494(void);
void func_8006A6DC(s32 arg0, s32 arg1);
s32 func_8002745C(s32 arg0);

void func_80027238(u8 arg0, s32 arg1) {
    volatile s32 *p;
    s32 dv;
    s32 buf[4];

    if (arg0 == 1) {
        p = &D_8007D7A4;
        if (*p > 0) {
            if (D_8007D7A0 == 0x200) {
                func_8006A970(buf, 3);
                if (func_8006AADC(buf) != D_8007D7B0) {
                    func_80063C38(&D_800111C4);
                    *p = -1;
                }
            }
            p = &D_8007D798;
            dv = *p;
            func_8006A970((void *)dv, D_8007D7A0);
            *p = (D_8007D7A0 << 2) + *p;
            D_8007D7A4 = D_8007D7A4 - 1;
            D_8007D7A4;
            D_8007D7B0 = D_8007D7B0 + 1;
            D_8007D7B0;
        }
    } else {
        volatile s32 *q = &D_8007D7A4;
        *q = -1;
    }

    {
        volatile s32 *r = &D_8007D7A8;
        *r = func_8006DD30(-1);
    }

    if (D_8007D7A4 < 0) {
        func_8002745C(1);
    }

    if (func_8006DD30(-1) > D_8007D7AC + 0x4B0) {
        D_8007D7A4 = -1;
    }

    if (D_8007D7A4 == 0 || func_8006DD30(-1) > D_8007D7AC + 0x4B0) {
        func_8006A574(D_8007D7B4);
        func_8006A58C(D_8007D7B8);
        func_8006A5A4(9, 0, 0);
        if (D_8007D78C != 0) {
            D_8007D78C((D_8007D7A4 == 0) ? 2 : 5, arg1);
        }
    }
}

s32 func_8002745C(s32 arg0) {
    u8 buf[8];
    s32 t;

    func_8006A574(0);
    func_8006A58C(0);
    if (func_8006A3E8() & 0x10) {
        if ((func_8006DD30(-1) & 0x3F) == 0) {
            func_80063C38(&D_800111DC);
        }
        func_8006A6DC(1, 0);
        {
            volatile s32 *q = &D_8007D7AC;
            *q = func_8006DD30(-1);
        }
        D_8007D7A4 = -1;
        return D_8007D7A4;
    }
    if (arg0 != 0) {
        func_80063C38(&D_800111F4);
        func_8006A5A4(9, 0, 0);
        if (func_8006A5A4(2, func_8006A418(), 0) == 0) {
            register volatile s32 *q asm("$2") = &D_8007D7A4;
            *q = -1;
            return *q;
        }
    }
    func_8006A494();
    {
        volatile s32 *q = &D_8007D79C;
        t = *q;
    }
    buf[0] = t;
    if ((t & 0xFF) != func_8006A3F8() || arg0 != 0) {
        if (func_8006A5A4(0xE, (s32)buf, 0) == 0) {
            D_8007D7A4 = -1;
            return D_8007D7A4;
        }
    }
    {
        volatile s32 *q = &D_8007D7B0;
        *q = func_8006AADC((void *)func_8006A418());
    }
    func_8006A58C((s32)func_80027238);
    D_8007D798 = D_8007D794;
    func_8006A6DC(6, 0);
    D_8007D7A4 = D_8007D790;
    D_8007D7A8 = func_8006DD30(-1);
    return D_8007D7A4;
}
