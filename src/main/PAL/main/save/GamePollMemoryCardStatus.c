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
void _card_clear(s32 arg0) asm("func_80063DEC");

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
        goto clear_result;

    case 1:
        status = func_8005F35C();
        if (status == 0) {
            goto done;
        }

        two = 2;
        if (status == two) {
            goto fail_neg3_case1;
        }
        if (status < 3) {
            if (status == 1) {
                goto case1_ready;
            }
            goto fail_neg3_case1;
        }
        if (status == 3) {
            goto fail_neg1_case1;
        }
        if (status == 4) {
            goto case1_status4;
        }
        goto fail_neg3_case1;

case1_ready:
        g_McPollStatus = status;
        if (g_McLastCardStatus == status) {
            g_McStatusState = 4;
        } else {
            g_McStatusState = two;
        }
        goto done;

fail_neg1_case1:
        state = -1;
        goto fail_case1;

case1_status4:
        g_McPollStatus = two;
        func_8005F304();
        _card_clear(handle);
        func_8005F55C();
        g_McStatusState = two;
        g_McLastCardStatus = 0;
        goto done;

fail_neg3_case1:
        state = -3;

fail_case1:
        g_McPollStatus = state;
        g_McStatusState = 4;
        g_McLastCardStatus = 0;
        goto done;

    case 2:
        func_8005F2AC();
        _card_load(handle);
        g_McStatusState = 3;
        g_McPollTicks = 0;
        goto done;

    case 3:
        status = func_8005F35C();
        if (status == 0) {
            goto done;
        }

        g_McStatusState = 4;
        if (status == 2) {
            goto fail_neg3_case3;
        }
        if (status < 3) {
            if (status == 1) {
                goto case3_ready;
            }
            goto fail_neg3_case3;
        }
        if (status == 3) {
            goto fail_neg1_case3;
        }
        if (status == 4) {
            goto fail_neg2_case3;
        }
        goto fail_neg3_case3;

case3_ready:
        g_McLastCardStatus = status;
        goto done;

fail_neg1_case3:
        state = -1;
        goto fail_case3;

fail_neg2_case3:
        state = -2;
        goto fail_case3;

fail_neg3_case3:
        state = -3;

fail_case3:
        g_McPollStatus = state;
        g_McLastCardStatus = 0;
        goto done;

    case 4:
        g_McStatusState = 0;
        g_McStatusResult = g_McPollStatus;
        goto done;

    default:
        g_McStatusState = 0;
clear_result:
        g_McStatusResult = 0;
    }

done:
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
