#include "common.h"
#include "psyq/kernel.h"

extern s32 g_McLastCardStatus asm("D_80082F4C");
extern s32 g_McStatusState asm("D_8009E668");
extern s32 g_McPollTicks asm("D_8019C864");
extern s32 g_McStatusResult asm("D_8019C8F0");
/* The poller's own working status word. Distinct from menu.h's
 * g_McPollStatus (D_8009B720), which is the code the menu reads. */
extern s32 g_McPollStatus asm("D_801E825C");
extern char g_FmtCardDevice[] asm("D_800127CC");

void func_8005F2AC(void);
void LibcSprintf(void *dst, void *fmt, s32 arg0, s32 arg1) asm("func_800632F0");
void func_8005F304(void);
s32 func_8005F35C(void);
s32 func_8005F55C(void);
void _card_info(s32 arg0) asm("func_80063DAC");
void _card_load(s32 arg0) asm("func_80063DBC");
s32 _card_clear(s32 arg0) asm("func_80063DEC");

s32 GamePollMemoryCardStatus(s32 arg0, s32 arg1) asm("func_8005ECE0");
s32 GamePollMemoryCardStatus(s32 arg0, s32 arg1) {
    s32 handle;
    s32 two;
    s32 status;
    s32 state;

    handle = (arg0 << 4) + arg1;

    switch (g_McStatusState) {
    case 0:
        _card_info(handle);
        g_McStatusState = 1;
        g_McPollTicks = 0;
        g_McStatusResult = 0;
        break;

    case 1:
        status = func_8005F35C();
        if (status == 0) {
            break;
        }

        two = 2;
        if (!(status == two)) {
        if (status < 3) {
            if (status == 1) {
                goto case1_ready;
            }
            goto fail_neg3_case1;
        }
        if (!(status == 3)) {
        if (status == 4) {
        } else {
        goto fail_neg3_case1;

case1_ready:
        g_McPollStatus = status;
        if (g_McLastCardStatus == status) {
            g_McStatusState = 4;
        } else {
            g_McStatusState = two;
        }
        break;

        }
        } else {
        state = -1;
        goto fail_case1;

        }
        g_McPollStatus = two;
        func_8005F304();
        _card_clear(handle);
        func_8005F55C();
        g_McStatusState = two;
        g_McLastCardStatus = 0;
        break;

        }
fail_neg3_case1:
        state = -3;

fail_case1:
        g_McPollStatus = state;
        g_McStatusState = 4;
        g_McLastCardStatus = 0;
        break;

    case 2:
        func_8005F2AC();
        _card_load(handle);
        g_McStatusState = 3;
        g_McPollTicks = 0;
        break;

    case 3:
        status = func_8005F35C();
        if (status == 0) {
            break;
        }

        g_McStatusState = 4;
        if (!(status == 2)) {
        if (status < 3) {
            if (status == 1) {
                goto case3_ready;
            }
            goto fail_neg3_case3;
        }
        if (!(status == 3)) {
        if (status == 4) {
        } else {
        goto fail_neg3_case3;

case3_ready:
        g_McLastCardStatus = status;
        break;

        }
        } else {
        state = -1;
        goto fail_case3;

        }
        state = -2;

        } else {
fail_neg3_case3:
        state = -3;

        }
fail_case3:
        g_McPollStatus = state;
        g_McLastCardStatus = 0;
        break;

    case 4:
        g_McStatusState = 0;
        g_McStatusResult = g_McPollStatus;
        break;

    default:
        g_McStatusState = 0;
        g_McStatusResult = 0;
    }

    return g_McStatusResult;
}

s32 GameFormatMemoryCard(s32 arg0, s32 arg1) asm("func_8005EF44");
s32 GameFormatMemoryCard(s32 arg0, s32 arg1) {
    char device[8];
    s32 status;

    LibcSprintf(device, g_FmtCardDevice, arg0, arg1);
    func_8005F304();
    BiosFormatDevice(device);
    status = func_8005F55C();

    if (status != 1) {
        if (status == 3) {
            status = -1;
        } else {
            status = -3;
        }
    }

    return status;
}

extern s32 g_McEvents[] asm("D_8009B538");


void GameOpenMemoryCardEvents(void) asm("func_8005EFAC");
void GameOpenMemoryCardEvents(void) {
    EnterCriticalSection();
    g_McEvents[0] = OpenEvent(0xF4000001, 0x0004, 0x2000, 0);
    g_McEvents[1] = OpenEvent(0xF4000001, 0x8000, 0x2000, 0);
    g_McEvents[2] = OpenEvent(0xF4000001, 0x0100, 0x2000, 0);
    g_McEvents[3] = OpenEvent(0xF4000001, 0x2000, 0x2000, 0);
    g_McEvents[4] = OpenEvent(0xF0000011, 0x0004, 0x2000, 0);
    g_McEvents[5] = OpenEvent(0xF0000011, 0x8000, 0x2000, 0);
    g_McEvents[6] = OpenEvent(0xF0000011, 0x0100, 0x2000, 0);
    g_McEvents[7] = OpenEvent(0xF0000011, 0x2000, 0x2000, 0);
    ExitCriticalSection();
}

void GameEnableMemoryCardEvents(void) asm("func_8005F0D4");
void GameEnableMemoryCardEvents(void) {
    EnableEvent(g_McEvents[0]);
    EnableEvent(g_McEvents[1]);
    EnableEvent(g_McEvents[2]);
    EnableEvent(g_McEvents[3]);
    EnableEvent(g_McEvents[4]);
    EnableEvent(g_McEvents[5]);
    EnableEvent(g_McEvents[6]);
    EnableEvent(g_McEvents[7]);
}

void GameDisableMemoryCardEvents(void) asm("func_8005F16C");
void GameDisableMemoryCardEvents(void) {
    DisableEvent(g_McEvents[0]);
    DisableEvent(g_McEvents[1]);
    DisableEvent(g_McEvents[2]);
    DisableEvent(g_McEvents[3]);
    DisableEvent(g_McEvents[4]);
    DisableEvent(g_McEvents[5]);
    DisableEvent(g_McEvents[6]);
    DisableEvent(g_McEvents[7]);
}

void GameCloseMemoryCardEvents(void) asm("func_8005F204");
void GameCloseMemoryCardEvents(void) {
    EnterCriticalSection();
    CloseEvent(g_McEvents[0]);
    CloseEvent(g_McEvents[1]);
    CloseEvent(g_McEvents[2]);
    CloseEvent(g_McEvents[3]);
    CloseEvent(g_McEvents[4]);
    CloseEvent(g_McEvents[5]);
    CloseEvent(g_McEvents[6]);
    CloseEvent(g_McEvents[7]);
    ExitCriticalSection();
}

/* ---- was GameClearMemoryCardHwEvents.c ---- */

#include "common.h"
#include "psyq/kernel.h"
#include "game/memcard.h"

/* Elements 0..3 and 4..7 of the eight-descriptor libcard event table
 * g_McEvents (D_8009B538), which save/GamePollMemoryCardStatus.c opens as an
 * array: [0..3] are the hardware class 0xF4000001 and [4..7] the software
 * class 0xF0000011, each in the order IOE, Error, Timeout, NewCard -- which is
 * why every poller below returns index + 1. They cannot be spelled
 * g_McEvents[k] in this file: with one array symbol GCC 2.6.3 keeps the base
 * address live in a callee-saved register across the TestEvent calls, which
 * grows GamePollMemoryCardHwEvent's frame from 24 to 32 bytes. */
extern s32 g_McHwEventIoe asm("D_8009B538");
extern s32 g_McHwEventError asm("D_8009B53C");
extern s32 g_McHwEventTimeout asm("D_8009B540");
extern s32 g_McHwEventNew asm("D_8009B544");
void func_800631C0(s32 arg0);
void GameClearMemoryCardHwEvents(void) asm("func_8005F2AC");
void GameClearMemoryCardHwEvents(void) {
    func_800631C0(g_McHwEventIoe);
    func_800631C0(g_McHwEventError);
    func_800631C0(g_McHwEventTimeout);
    func_800631C0(g_McHwEventNew);
}

extern s32 g_McSwEventIoe asm("D_8009B548");
extern s32 g_McSwEventError asm("D_8009B54C");
extern s32 g_McSwEventTimeout asm("D_8009B550");
extern s32 g_McSwEventNew asm("D_8009B554");
void GameClearMemoryCardSwEvents(void) asm("func_8005F304");
void GameClearMemoryCardSwEvents(void) {
    func_800631C0(g_McSwEventIoe);
    func_800631C0(g_McSwEventError);
    func_800631C0(g_McSwEventTimeout);
    func_800631C0(g_McSwEventNew);
}

extern s32 g_McPollTicks asm("D_8019C864");

s32 GamePollMemoryCardHwEvent(void) asm("func_8005F35C");
s32 GamePollMemoryCardHwEvent(void) {
    s32 result;
    s32 ready;
    s32 count;

    ready = 1;
    if (TestEvent(g_McHwEventIoe) == ready) {
        result = 1;
    }
    if (TestEvent(g_McHwEventError) == ready) {
        result = 2;
    }
    if (TestEvent(g_McHwEventTimeout) == ready) {
        result = 3;
    }
    if (TestEvent(g_McHwEventNew) == ready) {
        result = 4;
    }

    count = g_McPollTicks;
    g_McPollTicks = count + 1;
    if (count >= 90) {
        result = 2;
    }

    return result;
}

s32 GamePollMemoryCardHwEventLimit(s32 limit) asm("func_8005F420");
s32 GamePollMemoryCardHwEventLimit(s32 limit) {
    /* This pin is load-bearing: removing it changes .text. */
    register s32 i asm("$16");
    s32 ready;
    volatile s32 stack[2];

    i = 0;
    if (limit > 0) {
        ready = 1;
        do {
            if (TestEvent(g_McHwEventIoe) == ready) {
                return 1;
            }
            if (TestEvent(g_McHwEventError) == ready) {
                return 2;
            }
            if (TestEvent(g_McHwEventTimeout) == ready) {
                return 3;
            }
            if (TestEvent(g_McHwEventNew) == ready) {
                return 4;
            }
            i++;
        } while (i < limit);
    }

    return 0;
}

s32 GameWaitMemoryCardHwEvent(void) asm("func_8005F4D8");
s32 GameWaitMemoryCardHwEvent(void) {
    s32 ready;

    ready = 1;
    while (1) {
        if (TestEvent(g_McHwEventIoe) == ready) {
            return 1;
        }
        if (TestEvent(g_McHwEventError) == ready) {
            return 2;
        }
        if (TestEvent(g_McHwEventTimeout) == ready) {
            return 3;
        }
        if (TestEvent(g_McHwEventNew) == ready) {
            return 4;
        }
    }
}

s32 GameWaitMemoryCardSwEvent(void) asm("func_8005F55C");
s32 GameWaitMemoryCardSwEvent(void) {
    s32 ready;

    ready = 1;
    while (1) {
        if (TestEvent(g_McSwEventIoe) == ready) {
            return 1;
        }
        if (TestEvent(g_McSwEventError) == ready) {
            return 2;
        }
        if (TestEvent(g_McSwEventTimeout) == ready) {
            return 3;
        }
        if (TestEvent(g_McSwEventNew) == ready) {
            return 4;
        }
    }
}

extern s32 g_SaveElapsedTicks asm("D_801E7A54");
void InitCARD(s32 arg0) asm("func_80063DCC");
void StartCARD(void) asm("func_80063DDC");
void func_80063180(void);
void func_8005F5E0(void) { InitCARD(1); StartCARD(); func_80063180(); g_SaveElapsedTicks = 0; }

extern s32 g_FrameSyncThreshold asm("D_8019C768");

void GameAdvanceSaveHeaderCounter(void) {
    if (g_FrameSyncThreshold == 0x80) {
        g_SaveElapsedTicks++;
    } else {
        g_SaveElapsedTicks += 2;
    }
}
