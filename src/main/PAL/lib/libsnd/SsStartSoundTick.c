#include "common.h"
#include "psyq/kernel.h"
#include "psyq/snd.h"

typedef void (*Callback)(void);

extern Callback g_SndTickCallback;
extern Callback g_SndPrevVSyncCallback;

long KernelCallbackSlot2Wide(long arg0, Callback arg1) asm("KernelCallbackSlot2");

void SsStartSoundTick(long mode) {
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
    if (mode == 0) {
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
        VSyncCallback(g_SndTickCallback);
    } else {

    EnterCriticalSection();
    ResetRCnt(channel);
    SetRCnt(channel, size & 0xFFFF, 0x1000);

    {
        long mode;
        Callback callback;

        mode = g_SndTickIrq;
        if (mode == 0) {
            g_SndPrevVSyncCallback = (Callback)KernelCallbackSlot2Wide(0, 0);
            mode = g_SndTickIrq;
            callback = SsSoundTickCallback;
        } else {
            callback = SsSoundTickVSyncCallback;
            if (D_8009A569 == 0) {
                callback = g_SndTickCallback;
            }
        }
        KernelCallbackSlot2Wide(mode, callback);
    }

    }
    ExitCriticalSection();

    return;
}
