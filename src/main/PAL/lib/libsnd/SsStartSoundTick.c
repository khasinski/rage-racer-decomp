#include "common.h"
#include "psyq/kernel.h"

typedef void (*Callback)(void);

extern s32 D_8009A558;
extern s32 D_8009A55C;
extern Callback D_8009A560;
extern Callback D_8009A564;
extern u8 D_8009A568;
extern u8 D_8009A569;
extern u8 D_8009A56A;

s32 func_8006DF64(s32 arg0, Callback arg1);
void func_8006DFC4(Callback arg0);
void SsSoundTickCallback(void) asm("func_80071EEC");
void SsSoundTickVSyncCallback(void) asm("func_80071F2C");

void SsStartSoundTick(s32 arg0) asm("func_80071C44");

void SsStartSoundTick(s32 arg0) {
    register s32 size asm("$16");
    register s32 channel asm("$17");
    register u8 *flag asm("$5");
    register s32 state asm("$3");
    s32 wait;

    wait = 0x3E8;
    while (--wait >= 0) {
    }

    channel = 0xF2000002;
    flag = &D_8009A568;
    *flag = 0;
    state = D_8009A558;
    D_8009A56A = 6;
    D_8009A569 = 0;
    D_8009A564 = 0;

    if (state == 2) {
        goto state_2;
    }
    if (state < 3) {
        if (state == 0) {
            goto state_0;
        }
        goto derive_size;
    }
    if (state == 3) {
        goto state_3;
    }
    if (state == 5) {
        goto state_5;
    }
    goto derive_size;

state_0:
    D_8009A56A = 0xFF;
    goto done;

state_5:
    D_8009A56A = 0;
    if (arg0 == 0) {
        *flag = 1;
        goto setup;
    }
    channel = 0xF2000003;
    size = 1;
    goto setup;

state_3:
    size = 0x89D0;
    goto setup;

state_2:
    size = 0x44E8;
    goto setup;

derive_size:
    {
        register s32 *active asm("$4");
        register s32 dividend asm("$2");
        register s32 quotient asm("$2");

        active = &D_8009A55C;
        asm("" : "=r"(active) : "0"(active));
        if (*active != 0) {
            goto done;
        }
        state = active[-1];
        if (state < 0x46) {
            dividend = 0x204CC0;
            quotient = dividend / state;
            asm("" : "=r"(active), "=r"(quotient) : "0"(active), "1"(quotient));
            ((u8 *)active)[0xD] = ((u8 *)active)[0xD] + 1;
        } else {
            dividend = 0x409980;
            quotient = dividend / state;
        }
        size = quotient;
    }

setup:
    if (D_8009A568 != 0) {
        EnterCriticalSection();
        func_8006DFC4(D_8009A560);
        goto unlock;
    }

    EnterCriticalSection();
    ResetRCnt(channel);
    SetRCnt(channel, size & 0xFFFF, 0x1000);

    {
        register s32 mode asm("$4");
        Callback callback;

        mode = D_8009A56A;
        if (mode == 0) {
            D_8009A564 = (Callback)func_8006DF64(0, 0);
            mode = D_8009A56A;
            callback = SsSoundTickCallback;
        } else {
            callback = SsSoundTickVSyncCallback;
            if (D_8009A569 == 0) {
                callback = D_8009A560;
            }
        }
        func_8006DF64(mode, callback);
    }

unlock:
    ExitCriticalSection();

done:
    return;
}
