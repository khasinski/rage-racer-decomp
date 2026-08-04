#include "common.h"
#include "psyq/kernel.h"

typedef void (*Callback)(void);

extern long g_SndTickMode asm("D_8009A558");
extern long g_SndNoTickFlag asm("D_8009A55C");
extern Callback g_SndTickCallback asm("D_8009A560");
extern Callback g_SndPrevVSyncCallback asm("D_8009A564");
extern u_char g_SndTickUsesVSync asm("D_8009A568");
extern u_char D_8009A569;
extern u_char g_SndTickIrq asm("D_8009A56A");

long func_8006DF64(long arg0, Callback arg1);
void func_8006DFC4(Callback arg0);
void SsSoundTickCallback(void);
void SsSoundTickVSyncCallback(void);

void SsStartSoundTick(long arg0);

void SsStartSoundTick(long arg0) {
    long size;
    long channel;
    register u_char *flag asm("$5");
    long state;
    long wait;

    wait = 0x3E8;
    while (--wait >= 0) {
    }

    channel = 0xF2000002;
    flag = &g_SndTickUsesVSync;
    *flag = 0;
    state = g_SndTickMode;
    g_SndTickIrq = 6;
    D_8009A569 = 0;
    g_SndPrevVSyncCallback = 0;

    switch (state) {
    case 0:
    g_SndTickIrq = 0xFF;
    return;

    case 5:
    g_SndTickIrq = 0;
    if (arg0 == 0) {
        *flag = 1;
        break;
    }
    channel = 0xF2000003;
    size = 1;
    break;

    case 3:
    size = 0x89D0;
    break;

    case 2:
    size = 0x44E8;
    break;

    default:
    {
        long *active;
        long dividend;
        register long quotient asm("$2");

        active = &g_SndNoTickFlag;
        asm("" : "=r"(active) : "0"(active));
        if (*active != 0) {
            return;
        }
        state = active[-1];
        if (state < 0x46) {
            dividend = 0x204CC0;
            quotient = dividend / state;
            asm("" : "=r"(active), "=r"(quotient) : "0"(active), "1"(quotient));
            ((u_char *)active)[0xD] = ((u_char *)active)[0xD] + 1;
        } else {
            dividend = 0x409980;
            quotient = dividend / state;
        }
        size = quotient;
    }

    }
    if (g_SndTickUsesVSync != 0) {
        EnterCriticalSection();
        func_8006DFC4(g_SndTickCallback);
    } else {

    EnterCriticalSection();
    ResetRCnt(channel);
    SetRCnt(channel, size & 0xFFFF, 0x1000);

    {
        long mode;
        Callback callback;

        mode = g_SndTickIrq;
        if (mode == 0) {
            g_SndPrevVSyncCallback = (Callback)func_8006DF64(0, 0);
            mode = g_SndTickIrq;
            callback = SsSoundTickCallback;
        } else {
            callback = SsSoundTickVSyncCallback;
            if (D_8009A569 == 0) {
                callback = g_SndTickCallback;
            }
        }
        func_8006DF64(mode, callback);
    }

    }
    ExitCriticalSection();

    return;
}
