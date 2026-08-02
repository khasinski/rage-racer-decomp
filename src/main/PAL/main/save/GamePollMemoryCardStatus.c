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
/* sprintf: every caller declares its own arity; keep it prototypeless. */
void LibcSprintf() asm("func_800632F0");
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

/* ---- was GameClearSaveHeaderRows.c ---- */

#include "common.h"
#include "game/memcard.h"
#include "psyq/gpu.h"
#include "game/menu.h"

void GameClearSaveHeaderRows(GameSaveHeaderRow *rows) {
    u8 *arg0 = (u8 *)rows;
    s32 i = 0;
    u8 *ptr1 = arg0;
    s32 j;
    u8 *ptr2;
    u8 *ptr3;

    do {
        arg0[0] = 0;
        j = 5;
        ptr2 = arg0 + 5;
        do {
            ptr2[1] = 0;
            ptr2--;
            j--;
        } while (j >= 0);

        *(volatile u32 *)&arg0[8] = 0;

        j = 0;
        ptr3 = ptr1;
        do {
            j++;
        } while ((*(u16 *)&ptr3[0xC] = 0, j < 0x38));

        *(volatile u32 *)&arg0[0x7C] = 0;
        arg0 += 0x80;
        i++;
        ptr1 += 0x82;
    } while (i < 3);
}

extern char g_FmtString[] asm("D_80012F8C");
extern Rect g_SaveIconRect asm("D_8009B55C");

/* sprintf: every caller declares its own arity; keep it prototypeless. */
void LibcSprintf() asm("func_800632F0");
void StoreImage(Rect *rect, void *data) asm("func_80065B88");
void DrawSync(long mode) asm("func_800658FC");

void GameBuildSaveIconBlock(u8 *block, char *title, s32 iconTile, s32 imageX, s32 imageY) {
    u8 *blockReg;
    /* These pins are load-bearing: removing any one changes .text. */
    register char *titleReg asm("$3");
    register s32 iconTileReg asm("$16");
    register s32 imageXReg asm("$17");
    register Rect *rectArg asm("$4");
    register u8 *imageData asm("$5");
    register Rect *rect asm("$19");
    register s32 dataOffset asm("$20");
    s32 i;
    /* This pin is load-bearing: removing it changes .text. */
    register s32 rectW asm("$23");
    s32 rectH;
    s32 tileRow;
    s32 tileX;

    blockReg = block;
    /* These barriers are load-bearing: removing any one changes .text. */
    asm("" : "=r"(blockReg) : "0"(blockReg));
    titleReg = title;
    asm("" : "=r"(titleReg) : "0"(titleReg));
    iconTileReg = iconTile;
    asm("" : "=r"(iconTileReg) : "0"(iconTileReg));
    imageXReg = imageX;

    blockReg[0] = 'S';
    blockReg[1] = 'C';
    blockReg[2] = 0x11;
    blockReg[3] = 1;
    LibcSprintf(blockReg + 4, g_FmtString, (s32)titleReg);

    tileRow = iconTileReg / 20;
    rectArg = &g_SaveIconRect;
    imageData = blockReg + 0x60;
    g_SaveIconRect.w = 0x10;
    g_SaveIconRect.h = 1;
    tileX = iconTileReg % 20;
    rectArg->x = tileX << 4;
    g_SaveIconRect.y = tileRow + 0x1E0;
    i = 0;
    rect = rectArg;
    rectW = 4;
    rectH = 0x10;
    StoreImage(rectArg, imageData);
    /* These barriers are load-bearing: removing any one changes .text. */
    asm("" : "=r"(rect) : "0"(rect));
    asm("" : "=r"(rectW) : "0"(rectW));
    asm("" : "=r"(rectH) : "0"(rectH));
    dataOffset = 0x80;
    DrawSync(0);

    do {
        rect->x = imageXReg;
        rect->y = imageY;
        rect->w = rectW;
        rect->h = rectH;
        StoreImage(rect, blockReg + dataOffset);
        DrawSync(0);
        dataOffset += 0x80;
        i++;
        imageXReg += 4;
    } while (i <= 0);
}

extern s32 g_SaveElapsedTicks asm("D_801E7A54");

void GameWriteSaveHeaderRow(GameSaveHeaderRow *row) {
    u8 *arg0 = (u8 *)row;
    s32 i;
    /* This pin is load-bearing: removing it changes .text. */
    register u32 checksum asm("$3");
    u16 *scan;

    arg0[0] = g_TeamNameLength;

    for (i = 0; i < 7; i++) {
        *((arg0 + i) + 1) = g_TeamNameChars[i];
    }

    i = 0;
    checksum = 0;
    *(s32 *)(arg0 + 8) = g_SaveElapsedTicks;
    scan = (u16 *)arg0;

    do {
        checksum += *scan++;
        i++;
    } while ((u32)i < 0x3E);

    *(u32 *)(arg0 + 0x7C) = ~checksum;
}
