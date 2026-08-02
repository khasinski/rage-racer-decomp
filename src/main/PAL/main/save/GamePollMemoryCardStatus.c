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

/* ---- was GameStoreSaveStateBlock.c ---- */

#include "common.h"
#include "game/race.h"
#include "game/memcard.h"
#include "game/menu.h"

extern s16 g_PadMappingIndex asm("D_8019CE08");
extern s16 g_NegconMappingIndex asm("D_8019CB08");
extern u16 g_NegconSteerNeutral asm("D_801E4BF0");
extern u16 g_NegconSteerPlay asm("D_8019CAD0");
extern u16 g_NegconNeutralI asm("D_8019CA08");
extern u16 g_NegconNeutralII asm("D_8019CA0A");
extern u16 g_NegconMaxTwist asm("D_801E418C");
extern u16 g_NegconNeutralL asm("D_8019CA0C");
extern u16 g_BgmSelection asm("D_801E42CC");

extern u8 g_GrandPrixCars[] asm("D_801E4F44");
extern u8 g_ExtraGrandPrixCars[] asm("D_8019C914");
extern u8 g_TimeAttackCars[] asm("D_801E4388");
extern u16 g_ClassRecords[] asm("D_8019CB40");
extern u16 g_TeamLogoClut[] asm("D_801E444C");
extern u16 g_TeamLogoCanvas[] asm("D_801E6F2C");
extern s32 g_BestLapTimes[] asm("D_801E4408");
extern s32 g_BestTotalTimes[] asm("D_8019C70C");
extern s32 g_BestSectorTimes[] asm("D_801E41E8");

extern s32 g_BgmVolumeSetting asm("D_8019C704");
extern s32 g_SfxVolumeSetting asm("D_801E8A50");
extern s32 g_MonoOutput asm("D_801E6C70");
extern u8 g_GrandPrixCourseProgress[] asm("D_801E42EC");
extern u8 g_ExtraGrandPrixCourseProgress[] asm("D_8009E874");

/*
 * Serialises every live global into the 0x1000-byte memory-card payload. The
 * field layout is GameSaveBlock (game/memcard.h), but the block is addressed
 * with raw offsets on purpose: as GameSaveBlock member stores, gcc 2.6.3 stops
 * treating them as aliasing the plain global loads that feed them and hoists
 * every load to the top of the function, which retail does not do.
 */
void GameStoreSaveStateBlock(u8 *arg0) asm("func_8005F88C");
void GameStoreSaveStateBlock(u8 *arg0) {
    {
        u16 h0 = g_PadMappingIndex;
        u16 h1 = g_NegconMappingIndex;
        u16 h2 = g_NegconSteerNeutral;
        u16 h3 = g_NegconSteerPlay;
        *(u16 *)(arg0 + 0x0) = h0;
        *(u16 *)(arg0 + 0x2) = h1;
        *(u16 *)(arg0 + 0x4) = h2;
        *(u16 *)(arg0 + 0x6) = h3;
    }
    *(u16 *)(arg0 + 0x8) = g_NegconNeutralI;
    *(u16 *)(arg0 + 0xA) = g_NegconNeutralII;
    {
        u16 h0 = g_NegconMaxTwist;
        u16 h1 = g_NegconNeutralL;
        *(u16 *)(arg0 + 0xE) = h0;
        *(u16 *)(arg0 + 0xC) = h1;
    }

    *(s32 *)(arg0 + 0x10) = g_GrandPrixSave.course;
    *(s32 *)(arg0 + 0x14) = g_GrandPrixSave.carIndex;
    *(s32 *)(arg0 + 0x18) = g_GrandPrixSave.classIndex;
    *(s32 *)(arg0 + 0x1C) = g_GrandPrixSave.maxClassReached;
    *(s32 *)(arg0 + 0x20) = g_GrandPrixSave.unk10;
    *(s32 *)(arg0 + 0x24) = g_ExtraGrandPrixSave.course;
    *(s32 *)(arg0 + 0x28) = g_ExtraGrandPrixSave.carIndex;
    *(s32 *)(arg0 + 0x2C) = g_ExtraGrandPrixSave.classIndex;
    *(s32 *)(arg0 + 0x30) = g_ExtraGrandPrixSave.maxClassReached;
    {
        s32 w34 = g_ExtraGrandPrixSave.unk10;
        u16 h4C = g_BgmSelection;
        *(s32 *)(arg0 + 0x34) = w34;
        *(s32 *)(arg0 + 0x38) = g_TimeAttackSave.course;
        *(s32 *)(arg0 + 0x3C) = g_TimeAttackSave.carIndex;
        *(s32 *)(arg0 + 0x40) = g_TimeAttackSave.classIndex;
        *(s32 *)(arg0 + 0x44) = g_TimeAttackSave.maxClassReached;
        {
            /* This pin is load-bearing: removing it changes .text. */
            register s32 w48 asm("$4") = g_TimeAttackSave.unk10;
            u16 h4E = g_AdvancedSeriesUnlocked;
            *(u16 *)(arg0 + 0x4C) = h4C;
            *(u16 *)(arg0 + 0x4E) = h4E;
            *(s32 *)(arg0 + 0x48) = w48;
        }
    }
    *(s32 *)(arg0 + 0x50) = g_MaxClassReached[0];
    *(s32 *)(arg0 + 0x54) = g_MaxClassReached[1];

    {
        s32 offset = 0;
        /* This pin is load-bearing: removing it changes .text. */
        register u8 *dst asm("$4") = arg0;

        for (; offset < 0x68; offset += 8) {
            dst[0x58 + 0] = g_GrandPrixCars[offset + 0];
            dst[0x58 + 1] = g_GrandPrixCars[offset + 1];
            dst[0x58 + 2] = g_GrandPrixCars[offset + 2];
            dst[0x58 + 3] = g_GrandPrixCars[offset + 3];
            dst[0x58 + 4] = g_GrandPrixCars[offset + 4];
            dst[0x58 + 5] = g_GrandPrixCars[offset + 5];

            dst[0xC0 + 0] = g_ExtraGrandPrixCars[offset + 0];
            dst[0xC0 + 1] = g_ExtraGrandPrixCars[offset + 1];
            dst[0xC0 + 2] = g_ExtraGrandPrixCars[offset + 2];
            dst[0xC0 + 3] = g_ExtraGrandPrixCars[offset + 3];
            dst[0xC0 + 4] = g_ExtraGrandPrixCars[offset + 4];
            dst[0xC0 + 5] = g_ExtraGrandPrixCars[offset + 5];

            dst[0x128 + 0] = g_TimeAttackCars[offset + 0];
            dst[0x128 + 1] = g_TimeAttackCars[offset + 1];
            dst[0x128 + 2] = g_TimeAttackCars[offset + 2];
            dst[0x128 + 3] = g_TimeAttackCars[offset + 3];
            dst[0x128 + 4] = g_TimeAttackCars[offset + 4];
            dst[0x128 + 5] = g_TimeAttackCars[offset + 5];
            dst += 8;
        }
    }

    {
        /* This pin is load-bearing: removing it changes .text. */
        register u8 *dst asm("$4") = arg0;
        s32 offset = 0;

        for (; offset < 0x2C; offset += 4) {
            *(u16 *)(dst + 0x190) = *(u16 *)((u8 *)g_ClassRecords + offset);
            *(u16 *)(dst + 0x192) = *(u16 *)((u8 *)g_ClassRecords + offset + 2);
            dst += 4;
        }
    }

    {
        /* This pin is load-bearing: removing it changes .text. */
        register s32 count asm("$13") = 0;
        u16 *src = g_TeamLogoClut;
        u8 *dst = arg0;

        for (; count < 0x10; count++) {
            *(u16 *)(dst + 0x1BC) = *src++;
            dst += 2;
        }
    }

    {
        /* This pin is load-bearing: removing it changes .text. */
        register s32 count asm("$13") = 0;
        u16 *src = g_TeamLogoCanvas;
        u8 *dst = arg0;

        for (; count < 0x400; count++) {
            *(u16 *)(dst + 0x1DC) = *src++;
            dst += 2;
        }
    }

    {
        /* The remaining register hints in these loops are load-bearing. */
        s32 outer = 0;
        s32 *lapBase = g_BestLapTimes;
        s32 *totalBase = g_BestTotalTimes;
        u8 *outerDst = arg0;

        for (; outer < 2; outer++) {
            register s32 middle asm("$12") = 0;
            register s32 outerOffset asm("$9") = outer << 5;
            register u8 *middleDst asm("$11") = outerDst;
            u8 *lapDst = outerDst + 0x9DC;

            for (; middle < 4; middle++) {
                register s32 inner asm("$6") = 0;
                s32 middleOffset = middle << 3;
                s32 *totalOutBase = (s32 *)(middleDst + 0xA1C);
                s32 *totalOut =
                    (s32 *)(middleOffset + (s32)totalOutBase);
                s32 *totalIn =
                    (s32 *)(middleOffset +
                            (outerOffset + (s32)totalBase));
                s32 *lapOut = (s32 *)lapDst;
                s32 *lapIn =
                    (s32 *)(middleOffset +
                            (outerOffset + (s32)lapBase));

                for (; inner < 2; inner++) {
                    *lapOut = *lapIn++;
                    *totalOut++ = *totalIn++;
                    lapOut++;
                }
                lapDst += 8;
            }
            outerDst += 0x20;
        }
    }

    {
        /* The remaining register hints in these loops are load-bearing. */
        s32 outer = 0;
        /* Serialised as a flat run of words, hence the cast. */
        s32 *rankingBase = (s32 *)g_RankingRecords;
        s32 *timeBase = (s32 *)g_TimeRecords;
        register u8 *outerDst asm("$25") = arg0;
        register s32 outerOffset asm("$16") = 0;

        for (; outer < 2; outer++) {
            s32 middle = 0;
            register s32 currentOuterOffset asm("$15") = outerOffset;
            register u8 *middleDst asm("$17") = outerDst;
            register u8 *rankingDst asm("$14") = outerDst + 0xA5C;
            s32 middleOffset = 0;

            for (; middle < 4; middle++) {
                register s32 inner asm("$6") = 0;
                s32 *timeDstBase = (s32 *)(middleDst + 0xCDC);
                s32 *timeDst =
                    (s32 *)(middleOffset + (s32)timeDstBase);
                s32 *timeIn =
                    (s32 *)(middleOffset +
                            (currentOuterOffset + (s32)timeBase));
                s32 *rankingOut = (s32 *)rankingDst;
                s32 *rankingIn =
                    (s32 *)(middleOffset +
                            (currentOuterOffset + (s32)rankingBase));

                for (; inner < 5; inner++) {
                    memcpy(rankingOut, rankingIn, 0x10);
                    memcpy(timeDst, timeIn, 0x10);
                    timeDst += 4;
                    timeIn += 4;
                    rankingOut += 4;
                    rankingIn += 4;
                }
                rankingDst += 0x50;
                middleOffset += 0x50;
            }
            outerDst += 0x140;
            outerOffset += 0x140;
        }
    }

    {
        /* The remaining register hints in these loops are load-bearing. */
        s32 outer = 0;
        register s32 *sectorBase asm("$11") = g_BestSectorTimes;
        u8 *outerDst = arg0;
        s32 outerOffset = 0;

        for (; outer < 2; outer++) {
            register s32 middle asm("$12") = 0;
            s32 currentOuterOffset = outerOffset;
            u8 *sectorDst = outerDst + 0xF5C;
            s32 middleOffset = 0;

            for (; middle < 4; middle++) {
                register s32 inner asm("$6") = 0;
                s32 *sectorOut = (s32 *)sectorDst;
                s32 *sectorIn =
                    (s32 *)(middleOffset +
                            (currentOuterOffset + (s32)sectorBase));

                for (; inner < 3; inner++) {
                    *sectorOut = *sectorIn++;
                    sectorOut++;
                }
                sectorDst += 0xC;
                middleOffset += 0xC;
            }
            outerDst += 0x30;
            outerOffset += 0x30;
        }
    }

    {
        /* These two accumulator hints are load-bearing. */
        register u32 count asm("$13") = 0;
        register u32 checksum asm("$6") = 0;
        s32 bgmVolume = g_BgmVolumeSetting;
        s32 sfxVolume = g_SfxVolumeSetting;
        s32 monoOutput = g_MonoOutput;
        u16 *checksumSrc = (u16 *)arg0;

        *(s32 *)(arg0 + 0xFBC) = bgmVolume;
        *(s32 *)(arg0 + 0xFC0) = sfxVolume;
        *(s32 *)(arg0 + 0xFC4) = monoOutput;
        memcpy(arg0 + 0xFC8, g_GrandPrixCourseProgress, 8);
        memcpy(arg0 + 0xFD0, g_ExtraGrandPrixCourseProgress, 8);

        for (; count < 0x7FE; count++) {
            checksum += *checksumSrc++;
        }
        *(u32 *)(arg0 + 0xFFC) = ~checksum;
    }
}
