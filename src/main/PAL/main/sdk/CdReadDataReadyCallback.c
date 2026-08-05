#include <sys/types.h>

#include "common.h"
#include "game/state.h"

extern long g_PrologueStep;
extern void (*D_8007D778[])(void);

void TickPrologueStep(void) {
    void (*func)(void);

    func = D_8007D778[g_PrologueStep];
    g_SceneTimer++;
    func();
}

extern u_char D_800111C4;
extern u_char D_800111DC;
extern u_char D_800111F4;
extern volatile long g_CdReadSectorCount;
extern volatile long g_CdReadBuffer;
extern volatile long g_CdReadMode;
extern void (*g_CdReadCallback)(long, long);
extern volatile long g_CdReadPtr;
extern volatile long g_CdReadSectorWords;
extern volatile long g_CdReadRemaining;
extern volatile long g_CdReadLastVSync;
extern volatile long g_CdReadStartVSync;
extern volatile long g_CdReadExpectedSector;
extern volatile long g_CdReadSavedSyncCallback;
extern volatile long g_CdReadSavedReadyCallback;
void CdGetSector2(void *arg0, long arg1);
long CdPosToInt_Local(void *arg0);
void LibcPutString(void *arg0);
long VSync(long mode);
long CdControl(long com, long param, long result);
long CdStatus(void);
long CdLastPos(void);
long CdMode(void);
void CdFlush(void);
void CdControlF(long arg0, long arg1);
long CdReadRetry(long arg0);
void CdSyncCallback(long arg0);
void CdReadyCallback(long arg0);

void CdReadDataReadyCallback(u_char arg0, long arg1) {
    volatile long *p;
    long dv;
    long buf[4];

    if (arg0 == 1) {
        p = &g_CdReadRemaining;
        if (*p > 0) {
            if (g_CdReadSectorWords == 0x200) {
                CdGetSector2(buf, 3);
                if (CdPosToInt_Local(buf) != g_CdReadExpectedSector) {
                    LibcPutString(&D_800111C4);
                    *p = -1;
                }
            }
            p = &g_CdReadPtr;
            dv = *p;
            CdGetSector2((void *)dv, g_CdReadSectorWords);
            *p = (g_CdReadSectorWords * 4) + *p;
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
        CdSyncCallback(g_CdReadSavedSyncCallback);
        CdReadyCallback(g_CdReadSavedReadyCallback);
        CdControl(9, 0, 0);
        if (g_CdReadCallback != 0) {
            g_CdReadCallback((g_CdReadRemaining == 0) ? 2 : 5, arg1);
        }
    }
}
long CdReadRetry(long arg0) {
    u_char buf[8];
    long t;

    CdSyncCallback(0);
    CdReadyCallback(0);
    if (CdStatus() & 0x10) {
        if ((VSync(-1) & 0x3F) == 0) {
            LibcPutString(&D_800111DC);
        }
        CdControlF(1, 0);
        {
            volatile long *q = &g_CdReadStartVSync;
            *q = VSync(-1);
        }
        g_CdReadRemaining = -1;
        return g_CdReadRemaining;
    }
    if (arg0 != 0) {
        LibcPutString(&D_800111F4);
        CdControl(9, 0, 0);
        if (CdControl(2, CdLastPos(), 0) == 0) {
            long value = -1;
            volatile long *q = &g_CdReadRemaining;

            *q = value;
            return *q;
        }
    }
    CdFlush();
    {
        volatile long *q = &g_CdReadMode;
        t = *q;
    }
    buf[0] = t;
    if (((u8)t) != CdMode() || arg0 != 0) {
        if (CdControl(0xE, (long)buf, 0) == 0) {
            g_CdReadRemaining = -1;
            return g_CdReadRemaining;
        }
    }
    {
        volatile long *q = &g_CdReadExpectedSector;
        *q = CdPosToInt_Local((void *)CdLastPos());
    }
    CdReadyCallback((long)CdReadDataReadyCallback);
    g_CdReadPtr = g_CdReadBuffer;
    CdControlF(6, 0);
    g_CdReadRemaining = g_CdReadSectorCount;
    g_CdReadLastVSync = VSync(-1);
    return g_CdReadRemaining;
}

void CdReadBreak(void) {
    volatile long *ptr;

    ptr = &g_CdReadRemaining;
    *ptr = 0;
    CdSyncCallback(g_CdReadSavedSyncCallback);
    CdReadyCallback(g_CdReadSavedReadyCallback);
    CdControl(9, 0, 0);
}
