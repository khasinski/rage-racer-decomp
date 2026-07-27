#include <sys/types.h>

#include "common.h"
#include "game/state.h"

extern long D_801E4178;
extern void (*D_8007D778[])(void);

void func_800271EC(void) {
    void (*func)(void);

    func = D_8007D778[D_801E4178];
    g_SceneTimer++;
    func();
}

extern u_char D_800111C4;
extern u_char D_800111DC;
extern u_char D_800111F4;
extern volatile long D_8007D790;
extern volatile long D_8007D794;
extern volatile long D_8007D79C;
extern void (*D_8007D78C)(long, long);
extern volatile long D_8007D798;
extern volatile long D_8007D7A0;
extern volatile long D_8007D7A4;
extern volatile long D_8007D7A8;
extern volatile long D_8007D7AC;
extern volatile long D_8007D7B0;
extern volatile long D_8007D7B4;
extern volatile long D_8007D7B8;
void CdGetSector2(void *arg0, long arg1) asm("func_8006A970");
long func_8006AADC(void *arg0);
void func_80063C38(void *arg0);
long VSync(long mode) asm("func_8006DD30");
long CdControl(long com, long param, long result) asm("func_8006A5A4");
long func_8006A3E8(void);
long func_8006A418(void);
long func_8006A3F8(void);
void func_8006A494(void);
void func_8006A6DC(long arg0, long arg1);
long CdReadRetry(long arg0) asm("func_8002745C");
void func_8006A574(long arg0);
void func_8006A58C(long arg0);

void CdReadDataReadyCallback(u_char arg0, long arg1) asm("func_80027238");
void CdReadDataReadyCallback(u_char arg0, long arg1) {
    volatile long *p;
    long dv;
    long buf[4];

    if (arg0 == 1) {
        p = &D_8007D7A4;
        if (*p > 0) {
            if (D_8007D7A0 == 0x200) {
                CdGetSector2(buf, 3);
                if (func_8006AADC(buf) != D_8007D7B0) {
                    func_80063C38(&D_800111C4);
                    *p = -1;
                }
            }
            p = &D_8007D798;
            dv = *p;
            CdGetSector2((void *)dv, D_8007D7A0);
            *p = (D_8007D7A0 << 2) + *p;
            D_8007D7A4 = D_8007D7A4 - 1;
            D_8007D7A4;
            D_8007D7B0 = D_8007D7B0 + 1;
            D_8007D7B0;
        }
    } else {
        volatile long *q = &D_8007D7A4;
        *q = -1;
    }

    {
        volatile long *r = &D_8007D7A8;
        *r = VSync(-1);
    }

    if (D_8007D7A4 < 0) {
        CdReadRetry(1);
    }

    if (VSync(-1) > D_8007D7AC + 0x4B0) {
        D_8007D7A4 = -1;
    }

    if (D_8007D7A4 == 0 || VSync(-1) > D_8007D7AC + 0x4B0) {
        func_8006A574(D_8007D7B4);
        func_8006A58C(D_8007D7B8);
        CdControl(9, 0, 0);
        if (D_8007D78C != 0) {
            D_8007D78C((D_8007D7A4 == 0) ? 2 : 5, arg1);
        }
    }
}
long CdReadRetry(long arg0) {
    u_char buf[8];
    long t;

    func_8006A574(0);
    func_8006A58C(0);
    if (func_8006A3E8() & 0x10) {
        if ((VSync(-1) & 0x3F) == 0) {
            func_80063C38(&D_800111DC);
        }
        func_8006A6DC(1, 0);
        {
            volatile long *q = &D_8007D7AC;
            *q = VSync(-1);
        }
        D_8007D7A4 = -1;
        return D_8007D7A4;
    }
    if (arg0 != 0) {
        func_80063C38(&D_800111F4);
        CdControl(9, 0, 0);
        if (CdControl(2, func_8006A418(), 0) == 0) {
            long value = -1;
            volatile long *q = &D_8007D7A4;

            *q = value;
            return *q;
        }
    }
    func_8006A494();
    {
        volatile long *q = &D_8007D79C;
        t = *q;
    }
    buf[0] = t;
    if ((t & 0xFF) != func_8006A3F8() || arg0 != 0) {
        if (CdControl(0xE, (long)buf, 0) == 0) {
            D_8007D7A4 = -1;
            return D_8007D7A4;
        }
    }
    {
        volatile long *q = &D_8007D7B0;
        *q = func_8006AADC((void *)func_8006A418());
    }
    func_8006A58C((long)CdReadDataReadyCallback);
    D_8007D798 = D_8007D794;
    func_8006A6DC(6, 0);
    D_8007D7A4 = D_8007D790;
    D_8007D7A8 = VSync(-1);
    return D_8007D7A4;
}

void CdReadBreak(void) asm("func_80027634");
void CdReadBreak(void) {
    volatile long *ptr;

    ptr = &D_8007D7A4;
    *ptr = 0;
    func_8006A574(D_8007D7B4);
    func_8006A58C(D_8007D7B8);
    CdControl(9, 0, 0);
}
