#include <sys/types.h>

#include "common.h"
#include "game/state.h"

extern long g_PrologueStep asm("D_801E4178");
extern void (*D_8007D778[])(void);

void func_800271EC(void) {
    void (*func)(void);

    func = D_8007D778[g_PrologueStep];
    g_SceneTimer++;
    func();
}

extern u_char D_800111C4;
extern u_char D_800111DC;
extern u_char D_800111F4;
extern volatile long g_CdReadSectorCount asm("D_8007D790");
extern volatile long g_CdReadBuffer asm("D_8007D794");
extern volatile long g_CdReadMode asm("D_8007D79C");
extern void (*g_CdReadCallback)(long, long) asm("D_8007D78C");
extern volatile long g_CdReadPtr asm("D_8007D798");
extern volatile long g_CdReadSectorWords asm("D_8007D7A0");
extern volatile long g_CdReadRemaining asm("D_8007D7A4");
extern volatile long g_CdReadLastVSync asm("D_8007D7A8");
extern volatile long g_CdReadStartVSync asm("D_8007D7AC");
extern volatile long g_CdReadExpectedSector asm("D_8007D7B0");
extern volatile long g_CdReadSavedSyncCallback asm("D_8007D7B4");
extern volatile long g_CdReadSavedReadyCallback asm("D_8007D7B8");
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
        p = &g_CdReadRemaining;
        if (*p > 0) {
            if (g_CdReadSectorWords == 0x200) {
                CdGetSector2(buf, 3);
                if (func_8006AADC(buf) != g_CdReadExpectedSector) {
                    func_80063C38(&D_800111C4);
                    *p = -1;
                }
            }
            p = &g_CdReadPtr;
            dv = *p;
            CdGetSector2((void *)dv, g_CdReadSectorWords);
            *p = (g_CdReadSectorWords << 2) + *p;
            g_CdReadRemaining = g_CdReadRemaining - 1;
            g_CdReadRemaining;
            g_CdReadExpectedSector = g_CdReadExpectedSector + 1;
            g_CdReadExpectedSector;
        }
    } else {
        volatile long *q = &g_CdReadRemaining;
        *q = -1;
    }

    {
        volatile long *r = &g_CdReadLastVSync;
        *r = VSync(-1);
    }

    if (g_CdReadRemaining < 0) {
        CdReadRetry(1);
    }

    if (VSync(-1) > g_CdReadStartVSync + 0x4B0) {
        g_CdReadRemaining = -1;
    }

    if (g_CdReadRemaining == 0 || VSync(-1) > g_CdReadStartVSync + 0x4B0) {
        func_8006A574(g_CdReadSavedSyncCallback);
        func_8006A58C(g_CdReadSavedReadyCallback);
        CdControl(9, 0, 0);
        if (g_CdReadCallback != 0) {
            g_CdReadCallback((g_CdReadRemaining == 0) ? 2 : 5, arg1);
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
            volatile long *q = &g_CdReadStartVSync;
            *q = VSync(-1);
        }
        g_CdReadRemaining = -1;
        return g_CdReadRemaining;
    }
    if (arg0 != 0) {
        func_80063C38(&D_800111F4);
        CdControl(9, 0, 0);
        if (CdControl(2, func_8006A418(), 0) == 0) {
            long value = -1;
            volatile long *q = &g_CdReadRemaining;

            *q = value;
            return *q;
        }
    }
    func_8006A494();
    {
        volatile long *q = &g_CdReadMode;
        t = *q;
    }
    buf[0] = t;
    if ((t & 0xFF) != func_8006A3F8() || arg0 != 0) {
        if (CdControl(0xE, (long)buf, 0) == 0) {
            g_CdReadRemaining = -1;
            return g_CdReadRemaining;
        }
    }
    {
        volatile long *q = &g_CdReadExpectedSector;
        *q = func_8006AADC((void *)func_8006A418());
    }
    func_8006A58C((long)CdReadDataReadyCallback);
    g_CdReadPtr = g_CdReadBuffer;
    func_8006A6DC(6, 0);
    g_CdReadRemaining = g_CdReadSectorCount;
    g_CdReadLastVSync = VSync(-1);
    return g_CdReadRemaining;
}

void CdReadBreak(void) asm("func_80027634");
void CdReadBreak(void) {
    volatile long *ptr;

    ptr = &g_CdReadRemaining;
    *ptr = 0;
    func_8006A574(g_CdReadSavedSyncCallback);
    func_8006A58C(g_CdReadSavedReadyCallback);
    CdControl(9, 0, 0);
}
