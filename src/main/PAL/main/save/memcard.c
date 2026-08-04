#include "common.h"
#include "game/car.h"
#include "psyq/kernel.h"

extern s32 g_McLastCardStatus asm("D_80082F4C");
extern s32 g_McStatusState asm("D_8009E668");
extern s32 g_McPollTicks asm("D_8019C864");
extern s32 g_McStatusResult asm("D_8019C8F0");
/* The poller's own working status word. Distinct from menu.h's
 * g_McPollStatus (D_8009B720), which is the code the menu reads. */
extern s32 g_McPollStatus asm("D_801E825C");
extern char g_FmtCardDevice[] asm("D_800127CC");

void ClearMemoryCardHwEvents(void) asm("func_8005F2AC");
/* sprintf: every caller declares its own arity; keep it prototypeless. */
void LibcSprintf() asm("func_800632F0");
void ClearMemoryCardSwEvents(void) asm("func_8005F304");
s32 PollMemoryCardHwEvent(void) asm("func_8005F35C");
s32 WaitMemoryCardSwEvent(void) asm("func_8005F55C");
void _card_info(s32 arg0) asm("func_80063DAC");
void _card_load(s32 arg0) asm("func_80063DBC");
s32 _card_clear(s32 arg0) asm("func_80063DEC");

s32 PollMemoryCardStatus(s32 arg0, s32 arg1) asm("func_8005ECE0");
s32 PollMemoryCardStatus(s32 arg0, s32 arg1) {
    s32 handle;
    s32 two;
    s32 status;
    s32 state;

    handle = (arg0 * 16) + arg1;

    switch (g_McStatusState) {
    case 0:
        _card_info(handle);
        g_McStatusState = 1;
        g_McPollTicks = 0;
        g_McStatusResult = 0;
        break;

    case 1:
        status = PollMemoryCardHwEvent();
        if (status == 0) {
            break;
        }

        two = 2;
        if (!(status == two)) {
        if (status < 3) {
            if (status == 1) {
                goto case1_ready;
            }
            state = -3;
        g_McPollStatus = state;
        g_McStatusState = 4;
        g_McLastCardStatus = 0;
        break;
        }
        if (!(status == 3)) {
        if (status == 4) {
        } else {
        state = -3;
        goto fail_case1;

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
        g_McPollStatus = state;
        g_McStatusState = 4;
        g_McLastCardStatus = 0;
        break;

        }
        g_McPollStatus = two;
        ClearMemoryCardSwEvents();
        _card_clear(handle);
        WaitMemoryCardSwEvent();
        g_McStatusState = two;
        g_McLastCardStatus = 0;
        break;

        }
        state = -3;

fail_case1:
        g_McPollStatus = state;
        g_McStatusState = 4;
        g_McLastCardStatus = 0;
        break;

    case 2:
        ClearMemoryCardHwEvents();
        _card_load(handle);
        g_McStatusState = 3;
        g_McPollTicks = 0;
        break;

    case 3:
        status = PollMemoryCardHwEvent();
        if (status == 0) {
            break;
        }

        g_McStatusState = 4;
        if (!(status == 2)) {
        if (status < 3) {
            if (status == 1) {
                goto case3_ready;
            }
            state = -3;
        g_McPollStatus = state;
        g_McLastCardStatus = 0;
        break;
        }
        if (!(status == 3)) {
        if (status == 4) {
        } else {
        state = -3;
        g_McPollStatus = state;
        g_McLastCardStatus = 0;
        break;

case3_ready:
        g_McLastCardStatus = status;
        break;

        }
        } else {
        state = -1;
        g_McPollStatus = state;
        g_McLastCardStatus = 0;
        break;

        }
        state = -2;

        } else {
        state = -3;

        }
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

s32 FormatMemoryCard(s32 arg0, s32 arg1) asm("func_8005EF44");
s32 FormatMemoryCard(s32 arg0, s32 arg1) {
    char device[8];
    s32 status;

    LibcSprintf(device, g_FmtCardDevice, arg0, arg1);
    ClearMemoryCardSwEvents();
    BiosFormatDevice(device);
    status = WaitMemoryCardSwEvent();

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


void OpenMemoryCardEvents(void) asm("func_8005EFAC");
void OpenMemoryCardEvents(void) {
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

void EnableMemoryCardEvents(void) asm("func_8005F0D4");
void EnableMemoryCardEvents(void) {
    EnableEvent(g_McEvents[0]);
    EnableEvent(g_McEvents[1]);
    EnableEvent(g_McEvents[2]);
    EnableEvent(g_McEvents[3]);
    EnableEvent(g_McEvents[4]);
    EnableEvent(g_McEvents[5]);
    EnableEvent(g_McEvents[6]);
    EnableEvent(g_McEvents[7]);
}

void DisableMemoryCardEvents(void) asm("func_8005F16C");
void DisableMemoryCardEvents(void) {
    DisableEvent(g_McEvents[0]);
    DisableEvent(g_McEvents[1]);
    DisableEvent(g_McEvents[2]);
    DisableEvent(g_McEvents[3]);
    DisableEvent(g_McEvents[4]);
    DisableEvent(g_McEvents[5]);
    DisableEvent(g_McEvents[6]);
    DisableEvent(g_McEvents[7]);
}

void CloseMemoryCardEvents(void) asm("func_8005F204");
void CloseMemoryCardEvents(void) {
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

/* ---- was ClearMemoryCardHwEvents.c ---- */

#include "common.h"
#include "psyq/kernel.h"
#include "game/memcard.h"

/* Elements 0..3 and 4..7 of the eight-descriptor libcard event table
 * g_McEvents (D_8009B538), which save/PollMemoryCardStatus.c opens as an
 * array: [0..3] are the hardware class 0xF4000001 and [4..7] the software
 * class 0xF0000011, each in the order IOE, Error, Timeout, NewCard -- which is
 * why every poller below returns index + 1. They cannot be spelled
 * g_McEvents[k] in this file: with one array symbol GCC 2.6.3 keeps the base
 * address live in a callee-saved register across the TestEvent calls, which
 * grows PollMemoryCardHwEvent's frame from 24 to 32 bytes. */
extern s32 g_McHwEventIoe asm("D_8009B538");
extern s32 g_McHwEventError asm("D_8009B53C");
extern s32 g_McHwEventTimeout asm("D_8009B540");
extern s32 g_McHwEventNew asm("D_8009B544");
void ClearMemoryCardHwEvents(void) asm("func_8005F2AC");
void ClearMemoryCardHwEvents(void) {
    TestEvent(g_McHwEventIoe);
    TestEvent(g_McHwEventError);
    TestEvent(g_McHwEventTimeout);
    TestEvent(g_McHwEventNew);
}

extern s32 g_McSwEventIoe asm("D_8009B548");
extern s32 g_McSwEventError asm("D_8009B54C");
extern s32 g_McSwEventTimeout asm("D_8009B550");
extern s32 g_McSwEventNew asm("D_8009B554");
void ClearMemoryCardSwEvents(void) asm("func_8005F304");
void ClearMemoryCardSwEvents(void) {
    TestEvent(g_McSwEventIoe);
    TestEvent(g_McSwEventError);
    TestEvent(g_McSwEventTimeout);
    TestEvent(g_McSwEventNew);
}

extern s32 g_McPollTicks asm("D_8019C864");

s32 PollMemoryCardHwEvent(void) asm("func_8005F35C");
s32 PollMemoryCardHwEvent(void) {
    s32 result;
    s32 ready;
    s32 count;

    ready = 1;
    if (TestEvent(g_McHwEventIoe) == 1) {
        result = 1;
    }
    if (TestEvent(g_McHwEventError) == 1) {
        result = 2;
    }
    if (TestEvent(g_McHwEventTimeout) == 1) {
        result = 3;
    }
    if (TestEvent(g_McHwEventNew) == 1) {
        result = 4;
    }

    count = g_McPollTicks;
    g_McPollTicks = count + 1;
    if (count >= 90) {
        result = 2;
    }

    return result;
}

s32 PollMemoryCardHwEventLimit(s32 limit) asm("func_8005F420");
s32 PollMemoryCardHwEventLimit(s32 limit) {
    s32 i;
    i = 0;
    while (i < limit) {
        if (TestEvent(g_McHwEventIoe) == 1) {
            return 1;
        }
        if (TestEvent(g_McHwEventError) == 1) {
            return 2;
        }
        if (TestEvent(g_McHwEventTimeout) == 1) {
            return 3;
        }
        if (TestEvent(g_McHwEventNew) == 1) {
            return 4;
        }
        i++;
    }

    return 0;
}

s32 WaitMemoryCardHwEvent(void) asm("func_8005F4D8");
s32 WaitMemoryCardHwEvent(void) {
    s32 ready;

    ready = 1;
    while (1) {
        if (TestEvent(g_McHwEventIoe) == 1) {
            return 1;
        }
        if (TestEvent(g_McHwEventError) == 1) {
            return 2;
        }
        if (TestEvent(g_McHwEventTimeout) == 1) {
            return 3;
        }
        if (TestEvent(g_McHwEventNew) == 1) {
            return 4;
        }
    }
}

s32 WaitMemoryCardSwEvent(void) asm("func_8005F55C");
s32 WaitMemoryCardSwEvent(void) {
    s32 ready;

    ready = 1;
    while (1) {
        if (TestEvent(g_McSwEventIoe) == 1) {
            return 1;
        }
        if (TestEvent(g_McSwEventError) == 1) {
            return 2;
        }
        if (TestEvent(g_McSwEventTimeout) == 1) {
            return 3;
        }
        if (TestEvent(g_McSwEventNew) == 1) {
            return 4;
        }
    }
}

extern s32 g_SaveElapsedTicks asm("D_801E7A54");
void InitCARD(s32 arg0) asm("func_80063DCC");
void StartCARD(void) asm("func_80063DDC");
void BiosBuInit(void) asm("func_80063180");
void RestartMemoryCard(void) asm("func_8005F5E0");
void RestartMemoryCard(void) { InitCARD(1); StartCARD(); BiosBuInit(); g_SaveElapsedTicks = 0; }

extern s32 g_FrameSyncThreshold asm("D_8019C768");

void AdvanceSaveHeaderCounter(void) {
    if (g_FrameSyncThreshold == 0x80) {
        g_SaveElapsedTicks++;
    } else {
        g_SaveElapsedTicks += 2;
    }
}

/* ---- was ClearSaveHeaderRows.c ---- */

#include "common.h"
#include "game/memcard.h"
#include "psyq/gpu.h"
#include "game/menu.h"

void ClearSaveHeaderRows(GameSaveHeaderRow *rows) {
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

void BuildSaveIconBlock(u8 *block, char *title, s32 iconTile, s32 imageX, s32 imageY) {
    u8 *blockReg;
    register char *titleReg asm("$3");
    register s32 iconTileReg asm("$16");
    register s32 imageXReg asm("$17");
    register Rect *rectArg asm("$4");
    register u8 *imageData asm("$5");
    register Rect *rect asm("$19");
    register s32 dataOffset asm("$20");
    s32 i;
    register s32 rectW asm("$23");
    s32 rectH;
    s32 tileRow;
    s32 tileX;

    blockReg = block;
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
    rectArg->x = tileX * 16;
    g_SaveIconRect.y = tileRow + 0x1E0;
    i = 0;
    rect = rectArg;
    rectW = 4;
    rectH = 0x10;
    StoreImage(rectArg, imageData);
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

void WriteSaveHeaderRow(GameSaveHeaderRow *row) {
    u8 *arg0 = (u8 *)row;
    s32 i;
    u32 checksum;
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

    checksum = ~checksum;
    *(u32 *)(arg0 + 0x7C) = checksum;
}

/* ---- was StoreSaveStateBlock.c ---- */

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
void StoreSaveStateBlock(u8 *arg0) asm("func_8005F88C");
void StoreSaveStateBlock(u8 *arg0) {
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
    *(s32 *)(arg0 + 0x20) = g_GrandPrixSave.money;
    *(s32 *)(arg0 + 0x24) = g_ExtraGrandPrixSave.course;
    *(s32 *)(arg0 + 0x28) = g_ExtraGrandPrixSave.carIndex;
    *(s32 *)(arg0 + 0x2C) = g_ExtraGrandPrixSave.classIndex;
    *(s32 *)(arg0 + 0x30) = g_ExtraGrandPrixSave.maxClassReached;
    {
        s32 w34 = g_ExtraGrandPrixSave.money;
        u16 h4C = g_BgmSelection;
        *(s32 *)(arg0 + 0x34) = w34;
        *(s32 *)(arg0 + 0x38) = g_TimeAttackSave.course;
        *(s32 *)(arg0 + 0x3C) = g_TimeAttackSave.carIndex;
        *(s32 *)(arg0 + 0x40) = g_TimeAttackSave.classIndex;
        *(s32 *)(arg0 + 0x44) = g_TimeAttackSave.maxClassReached;
        {
            register s32 w48 asm("$4") = g_TimeAttackSave.money;
            u16 h4E = g_AdvancedSeriesUnlocked;
            *(u16 *)(arg0 + 0x4C) = h4C;
            *(u16 *)(arg0 + 0x4E) = h4E;
            *(s32 *)(arg0 + 0x48) = w48;
        }
    }
    *(s32 *)(arg0 + 0x50) = g_MaxClassReached[0];
    *(s32 *)(arg0 + 0x54) = g_MaxClassReached[1];

    {
        s32 i;
        register u8 *dst asm("$4") = arg0;

        for (i = 0; i < 13; i++) {
            dst[0x58 + 0] = g_GrandPrixCars[i].modelVariant;
            dst[0x58 + 1] = g_GrandPrixCars[i].tireCompound;
            dst[0x58 + 2] = g_GrandPrixCars[i].transmission;
            dst[0x58 + 3] = g_GrandPrixCars[i].paintColor1;
            dst[0x58 + 4] = g_GrandPrixCars[i].paintColor2;
            dst[0x58 + 5] = g_GrandPrixCars[i].enabled;

            dst[0xC0 + 0] = g_ExtraGrandPrixCars[i].modelVariant;
            dst[0xC0 + 1] = g_ExtraGrandPrixCars[i].tireCompound;
            dst[0xC0 + 2] = g_ExtraGrandPrixCars[i].transmission;
            dst[0xC0 + 3] = g_ExtraGrandPrixCars[i].paintColor1;
            dst[0xC0 + 4] = g_ExtraGrandPrixCars[i].paintColor2;
            dst[0xC0 + 5] = g_ExtraGrandPrixCars[i].enabled;

            dst[0x128 + 0] = g_TimeAttackCars[i].modelVariant;
            dst[0x128 + 1] = g_TimeAttackCars[i].tireCompound;
            dst[0x128 + 2] = g_TimeAttackCars[i].transmission;
            dst[0x128 + 3] = g_TimeAttackCars[i].paintColor1;
            dst[0x128 + 4] = g_TimeAttackCars[i].paintColor2;
            dst[0x128 + 5] = g_TimeAttackCars[i].enabled;

            dst += 8;
        }
    }

    {
        register u8 *dst asm("$4") = arg0;
        s32 offset = 0;

        for (; offset < 0x2C; offset += 4) {
            *(u16 *)(dst + 0x190) = *(u16 *)((u8 *)g_ClassRecords + offset);
            *(u16 *)(dst + 0x192) = *(u16 *)((u8 *)g_ClassRecords + offset + 2);
            dst += 4;
        }
    }

    {
        register s32 count asm("$13") = 0;
        u16 *src = g_TeamLogoClut;
        u8 *dst = arg0;

        for (; count < 0x10; count++) {
            *(u16 *)(dst + 0x1BC) = *src++;
            dst += 2;
        }
    }

    {
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
            register s32 outerOffset asm("$9") = outer * 32;
            register u8 *middleDst asm("$11") = outerDst;
            u8 *lapDst = outerDst + 0x9DC;

            for (; middle < 4; middle++) {
                register s32 inner asm("$6") = 0;
                s32 middleOffset = middle * 8;
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
        *(u32 *)(arg0 + MC_BLOCK_CHECKSUM_OFS) = ~checksum;
    }
}

/* ---- was LoadSaveStateBlock.c ---- */

#include "common.h"
#include "game/race.h"
#include "game/menu.h"
#include "psyq/gpu.h"

extern char g_MsgSaveChecksumOk[] asm("D_80012F90");
extern char g_FmtSaveChecksum[] asm("D_80012F98");

extern s16 g_PadMappingIndex asm("D_8019CE08");
extern s16 g_NegconMappingIndex asm("D_8019CB08");
extern u16 g_NegconSteerNeutral asm("D_801E4BF0");
extern u16 g_NegconSteerPlay asm("D_8019CAD0");
extern u16 g_NegconNeutralI asm("D_8019CA08");
extern u16 g_NegconNeutralII asm("D_8019CA0A");
extern u16 g_NegconMaxTwist asm("D_801E418C");
extern u16 g_NegconNeutralL asm("D_8019CA0C");
/* The loader stores a whole word here; the saver reads only the low half
 * as g_BgmSelection. Same address, two widths, so two names. */
extern s32 g_BgmSelectionW asm("D_801E42CC");

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

extern u8 g_TeamLogoRect[] asm("D_8007BEE4");
extern u8 g_TeamLogoClutRect[] asm("D_8007BEDC");

void LoadPadButtonMapping(s32 a, s32 b) asm("func_80013F80");
void ApplyAudioSettings(void) asm("func_80021224");

/*
 * Verifies the memory-card payload's checksum and scatters it back into the
 * live globals. The layout is GameSaveBlock in game/memcard.h - the offsets
 * below are its fields - but the block is read with raw offsets on purpose:
 * as GameSaveBlock member reads, gcc 2.6.3 stops treating them as aliasing the
 * plain global stores they feed and hoists every load ahead of the stores,
 * which costs two extra callee-saved registers and does not match retail.
 * (That is also why the header is not included here: its prototype takes a
 * void *, and gcc 2.6.3 rejects the u8 * signature this body needs.)
 */
s32 LoadSaveStateBlock(u8 *arg0) asm("func_8005FED4");
s32 LoadSaveStateBlock(u8 *arg0) {
    register u8 *base asm("$17") = arg0;
    __asm__("" : "=r"(base) : "0"(base));
    {
        u32 sum;
        register u32 i asm("$13");
        u16 *p;

        i = 0;
        __asm__("" : "=r"(i) : "0"(i));
        sum = i;
        p = (u16 *)base;
        do {
            sum += *p++;
            i++;
        } while (i < 0x7FE);
        DebugPrintf(g_MsgSaveChecksumOk);
        sum = ~sum;
        DebugPrintf(g_FmtSaveChecksum, *(s32 *)(base + MC_BLOCK_CHECKSUM_OFS), sum);
        if (*(s32 *)(base + MC_BLOCK_CHECKSUM_OFS) != sum) {
            return 0;
        }
    }

    {
        u16 h0 = *(u16 *)(base + 0x0);
        u16 h1 = *(u16 *)(base + 0x2);
        u16 h2 = *(u16 *)(base + 0x4);
        u16 h3 = *(u16 *)(base + 0x6);
        g_NegconNeutralI = *(u16 *)(base + 0x8);
        g_NegconNeutralII = *(u16 *)(base + 0xA);
        g_NegconNeutralL = *(u16 *)(base + 0xC);
        {
            u16 hE = *(u16 *)(base + 0xE);
            s32 w30;
            g_GrandPrixSave.course = *(s32 *)(base + 0x10);
            g_GrandPrixSave.carIndex = *(s32 *)(base + 0x14);
            g_GrandPrixSave.classIndex = *(s32 *)(base + 0x18);
            g_GrandPrixSave.maxClassReached = *(s32 *)(base + 0x1C);
            g_GrandPrixSave.money = *(s32 *)(base + 0x20);
            g_ExtraGrandPrixSave.course = *(s32 *)(base + 0x24);
            g_ExtraGrandPrixSave.carIndex = *(s32 *)(base + 0x28);
            g_ExtraGrandPrixSave.classIndex = *(s32 *)(base + 0x2C);
            w30 = *(s32 *)(base + 0x30);
            g_PadMappingIndex = h0;
            g_NegconMappingIndex = h1;
            g_NegconSteerNeutral = h2;
            g_NegconSteerPlay = h3;
            g_NegconMaxTwist = hE;
            g_ExtraGrandPrixSave.maxClassReached = w30;
        }
        g_ExtraGrandPrixSave.money = *(s32 *)(base + 0x34);
        g_TimeAttackSave.course = *(s32 *)(base + 0x38);
        g_TimeAttackSave.carIndex = *(s32 *)(base + 0x3C);
        g_TimeAttackSave.classIndex = *(s32 *)(base + 0x40);
        g_TimeAttackSave.maxClassReached = *(s32 *)(base + 0x44);
        g_TimeAttackSave.money = *(s32 *)(base + 0x48);
        {
            s32 h4C = *(s16 *)(base + 0x4C);
            u16 h4E = *(u16 *)(base + 0x4E);
            s32 w54;
            g_MaxClassReached[0] = *(s32 *)(base + 0x50);
            w54 = *(s32 *)(base + 0x54);
            g_BgmSelectionW = h4C;
            g_AdvancedSeriesUnlocked = h4E;
            g_MaxClassReached[1] = w54;
        }
    }

    {
        register u8 *src asm("$6") = base;
        s32 i;
        for (i = 0; i < 13; i++) {
            g_GrandPrixCars[i].modelVariant = src[0x58 + 0];
            g_GrandPrixCars[i].tireCompound = src[0x58 + 1];
            g_GrandPrixCars[i].transmission = src[0x58 + 2];
            g_GrandPrixCars[i].paintColor1 = src[0x58 + 3];
            g_GrandPrixCars[i].paintColor2 = src[0x58 + 4];
            g_GrandPrixCars[i].enabled = src[0x58 + 5];
            g_ExtraGrandPrixCars[i].modelVariant = src[0xC0 + 0];
            g_ExtraGrandPrixCars[i].tireCompound = src[0xC0 + 1];
            g_ExtraGrandPrixCars[i].transmission = src[0xC0 + 2];
            g_ExtraGrandPrixCars[i].paintColor1 = src[0xC0 + 3];
            g_ExtraGrandPrixCars[i].paintColor2 = src[0xC0 + 4];
            g_ExtraGrandPrixCars[i].enabled = src[0xC0 + 5];
            g_TimeAttackCars[i].modelVariant = src[0x128 + 0];
            g_TimeAttackCars[i].tireCompound = src[0x128 + 1];
            g_TimeAttackCars[i].transmission = src[0x128 + 2];
            g_TimeAttackCars[i].paintColor1 = src[0x128 + 3];
            g_TimeAttackCars[i].paintColor2 = src[0x128 + 4];
            g_TimeAttackCars[i].enabled = src[0x128 + 5];
            src += 8;
        }
    }

    {
        register u8 *src asm("$4") = base;
        s32 offset = 0;
        for (; offset < 0x2C; offset += 4) {
            *(u16 *)((u8 *)g_ClassRecords + offset) = *(u16 *)(src + 0x190);
            *(u16 *)((u8 *)g_ClassRecords + offset + 2) = *(u16 *)(src + 0x192);
            src += 4;
        }
    }

    {
        register s32 count asm("$13") = 0;
        u16 *dst = g_TeamLogoClut;
        u8 *src = base;
        for (; count < 0x10; count++) {
            *dst++ = *(u16 *)(src + 0x1BC);
            src += 2;
        }
    }

    {
        register s32 count asm("$13") = 0;
        u16 *dst = g_TeamLogoCanvas;
        u8 *src = base;
        for (; count < 0x400; count++) {
            *dst++ = *(u16 *)(src + 0x1DC);
            src += 2;
        }
    }

    /* g_BestLapTimes / g_BestTotalTimes */
    {
        register s32 i asm("$13") = 0;
        register s32 j asm("$12");
        s32 off;
        for (; i < 2; i++) {
            j = 0;
            off = i * 32;
            for (; j < 4; j++) {
                *(s32 *)((u8 *)g_BestLapTimes + off) = *(s32 *)(base + off + 0x9DC);
                *(s32 *)((u8 *)g_BestTotalTimes + off) = *(s32 *)(base + off + 0xA1C);
                off += 8;
            }
        }
    }

    /* g_RankingRecords / g_TimeRecords */
    {
        s32 i = 0;
        s32 j;
        register s32 k asm("$7");
        s32 *cb78 = (s32 *)g_TimeRecords;
        register s32 *d1base asm("$24") = (s32 *)g_RankingRecords;
        register s32 ioff asm("$16") = 0;
        for (; i < 2; i++) {
            register s32 iofc asm("$15");
            register s32 *d1 asm("$14");
            s32 joff;
            j = 0;
            iofc = ioff;
            d1 = d1base;
            joff = 0;
            for (; j < 4; j++) {
                register s32 dc asm("$2");
                register s32 *dst2 asm("$11");
                register s32 sb asm("$3");
                s32 s2;
                s32 *src2;
                s32 *dst1;
                register s32 *src1 asm("$6");
                s32 s1;
                k = 0;
                dc = iofc + (s32)cb78;
                dst2 = (s32 *)(joff + dc);
                sb = iofc + (s32)base;
                s2 = sb + 0xCDC;
                src2 = (s32 *)(joff + s2);
                dst1 = d1;
                s1 = sb + 0xA5C;
                src1 = (s32 *)(joff + s1);
                do {
                    s32 a0 = src1[0], a1 = src1[1], a2 = src1[2], a3 = src1[3];
                    dst1[0] = a0;
                    dst1[1] = a1;
                    dst1[2] = a2;
                    dst1[3] = a3;
                    {
                        s32 b0 = src2[0], b1 = src2[1], b2 = src2[2], b3 = src2[3];
                        dst2[0] = b0;
                        dst2[1] = b1;
                        dst2[2] = b2;
                        dst2[3] = b3;
                    }
                    dst2 += 4;
                    __asm__("" : "=r"(src2) : "0"(src2), "r"(dst2));
                    src2 += 4;
                    __asm__("" : "=r"(dst1) : "0"(dst1), "r"(src2));
                    dst1 += 4;
                    __asm__("" : "=r"(k) : "0"(k), "r"(dst1));
                    k++;
                    __asm__("" : "=r"(src1) : "0"(src1), "r"(k));
                    src1 += 4;
                } while (k < 5);
                d1 = (s32 *)((u8 *)d1 + 0x50);
                joff += 0x50;
            }
            d1base = (s32 *)((u8 *)d1base + 0x140);
            ioff += 0x140;
        }
    }

    /* g_BestSectorTimes */
    {
        register s32 i asm("$13") = 0;
        register s32 j asm("$12");
        register s32 k asm("$7");
        register s32 *e41e8 asm("$10") = g_BestSectorTimes;
        s32 ioff = 0;
        for (; i < 2; i++) {
            s32 iofc;
            s32 *dbase;
            s32 joff;
            j = 0;
            iofc = ioff;
            dbase = e41e8;
            joff = 0;
            for (; j < 4; j++) {
                s32 *dst;
                s32 sb;
                s32 *src;
                k = 0;
                dst = dbase;
                sb = iofc + (s32)base + 0xF5C;
                src = (s32 *)(joff + sb);
                for (; k < 3; k++) {
                    *dst++ = *src++;
                }
                dbase = (s32 *)((u8 *)dbase + 0xC);
                joff += 0xC;
            }
            e41e8 = (s32 *)((u8 *)e41e8 + 0x30);
            ioff += 0x30;
        }
    }

    /* g_BgmVolumeSetting / g_SfxVolumeSetting / g_MonoOutput clamps */
    {
        s32 v = *(s32 *)(base + 0xFBC);
        s32 c;
        g_BgmVolumeSetting = v;
        if (v >= 0) {
            c = v;
            if (c >= 0x10) {
                c = 0xF;
            }
        } else {
            c = 0;
        }
        v = *(s32 *)(base + 0xFC0);
        g_BgmVolumeSetting = c;
        g_SfxVolumeSetting = v;
        if (v >= 0) {
            c = v;
            if (c >= 0x10) {
                c = 0xF;
            }
        } else {
            c = 0;
        }
        v = *(s32 *)(base + 0xFC4);
        g_SfxVolumeSetting = c;
        g_MonoOutput = v;
        if (v != 0) {
            g_MonoOutput = 1;
        }
    }

    /* g_GrandPrixCourseProgress / g_ExtraGrandPrixCourseProgress unaligned copies */
    memcpy(g_GrandPrixCourseProgress, base + 0xFC8, 8);
    memcpy(g_ExtraGrandPrixCourseProgress, base + 0xFD0, 8);

    LoadPadButtonMapping(g_PadMappingIndex, g_NegconMappingIndex);
    ApplyAudioSettings();
    LoadImage((Rect *)g_TeamLogoRect, g_TeamLogoCanvas);
    LoadImage((Rect *)g_TeamLogoClutRect, g_TeamLogoClut);
    return 1;
}

/* ---- was WriteMemoryCardSaveFile.c ---- */

#include "common.h"
#include "game/memcard.h"
#include "psyq/kernel.h"
#include "game/menu.h"


s32 WriteMemoryCardSaveFile(
    char *path,
    char *title,
    void *iconBlock,
    GameSaveHeaderRow *header,
    void *saveBlock) {
    s32 fd;
    s32 prevFd;
    s32 written;
    s32 attempt;
    s32 ok;

    GameMenuLoadPhase = 0x1100;
    BuildSaveIconBlock(iconBlock, title, 0x222, 0x3C0, 0x1F0);
    GameMenuLoadPhase = 0x1200;
    WriteSaveHeaderRow(header);
    attempt = 0;
    GameMenuLoadPhase = 0x1300;
    StoreSaveStateBlock(saveBlock);
    GameMenuLoadPhase = 0x1500;

    do {
        fd = BiosFileOpen(path, 2);
        prevFd = fd;
        if (fd == -1) {
            fd = BiosFileOpen(path, 0x10200);
            if (fd == prevFd) {
                GameMenuLoadPhase = attempt | 0x1520;
            } else {
                BiosFileClose(fd);
                fd = BiosFileOpen(path, 2);
                if (fd == prevFd) {
                    GameMenuLoadPhase = attempt | 0x1510;
                }
            }
            ok = 0;
        } else {
            ok = 1;
        }

        if (ok) {
            break;
        }
        attempt++;
    } while (attempt < 2);

    if (!ok) {
        return 0;
    }

    GameMenuLoadPhase = attempt | 0x1530;
    if (BiosFileWrite(fd, iconBlock, 0x200) != 0x200) {
        return 0;
    }
    GameMenuLoadPhase = attempt | 0x1540;
    written = BiosFileWrite(fd, header, 0x80);
    if (written != 0x80) {
        return 0;
    }
    GameMenuLoadPhase = attempt | 0x1550;
    if (BiosFileWrite(fd, saveBlock, MC_BLOCK_SIZE) != MC_BLOCK_SIZE) {
        return 0;
    }
    GameMenuLoadPhase = attempt | 0x1560;
    if (BiosFileWrite(fd, header, 0x80) != written) {
        return 0;
    }
    GameMenuLoadPhase = attempt | 0x1570;
    BiosFileClose(fd);
    return 1;
}

extern char g_SaveFilePath[] asm("D_800128AC");
extern char g_SaveTitleSjis[] asm("D_800127D8");

s32 WriteMemoryCardSaveSlot(s32 arg0, GameSaveHeaderRow *arg1) {
    u8 block0[0x200];
    u8 block1[MC_BLOCK_SIZE];
    s32 i;

    for (i = 0x1FF; i >= 0; i--) {
        block0[i] = 0;
    }

    GameMenuLoadPhase = 0x1000;
    return WriteMemoryCardSaveFile(
        g_SaveFilePath + arg0 * 0x1A,
        g_SaveTitleSjis + arg0 * 0x46,
        block0,
        arg1,
        block1);
}

s32 ReadVerifiedSaveHeader(s32 arg0, GameSaveHeaderRow *arg1) {
    s32 fd;
    register void *buffer asm("$18");
    register s32 sum asm("$16");
    s32 i;
    register u16 *ptr asm("$4");

    fd = arg0;
    buffer = arg1;
    asm("" : "=r"(sum) : "r"(fd), "r"(buffer), "0"(0));

    GameMenuLoadPhase = 0x120;
    if (BiosFileSeek(fd, 0x1280, 0) < 0) {
        return 0;
    }

    GameMenuLoadPhase = 0x130;
    if (BiosFileRead(fd, buffer, 0x80) != 0x80) {
        return 0;
    }

    GameMenuLoadPhase = 0x140;
    i = 0;
    ptr = buffer;
    do {
        sum += *ptr++;
        i++;
    } while ((u32)i < 0x3E);

    if (*(s32 *)((u8 *)buffer + 0x7C) == ~sum) {
        return 1;
    }

    GameMenuLoadPhase = 0x150;
    if (BiosFileSeek(fd, 0x200, 0) < 0) {
        return 0;
    }

    GameMenuLoadPhase = 0x160;
    if (BiosFileRead(fd, buffer, 0x80) != 0x80) {
        return 0;
    }

    GameMenuLoadPhase = 0x170;
    sum = 0;
    i = 0;
    ptr = buffer;
    do {
        sum += *ptr++;
        i++;
    } while ((u32)i < 0x3E);

    if (*(s32 *)((u8 *)buffer + 0x7C) == ~sum) {
        return 1;
    }

    GameMenuLoadPhase = 0x180;
    return 0;
}

s32 ScanMemoryCardSaveHeaders(GameSaveHeaderRow *arg0) {
    s32 fd;
    s32 i;
    s32 mask;
    s32 nameOffset;
    void *buffer;

    mask = 0;
    GameMenuLoadPhase = 0x110;
    i = 0;
    buffer = arg0;
    nameOffset = 0;

    do {
        fd = BiosFileOpen(g_SaveFilePath + nameOffset, 1);
        if (fd >= 0) {
            if (ReadVerifiedSaveHeader(fd, buffer) == 0) {
                BiosFileClose(fd);
                mask |= 0x10000 << i;
            } else {
                BiosFileClose(fd);
                mask |= 1 << i;
            }
        }

        buffer = (void *)((u8 *)buffer + 0x80);
        i++;
        nameOffset += 0x1A;
    } while (i < 3);

    GameMenuLoadPhase = 0x190;
    return mask;
}

extern volatile s32 GameMenuLoadPhase asm("D_8009B740");
extern s32 g_SaveElapsedTicks asm("D_801E7A54");

s32 LoadMemoryCardSaveSlot(s32 arg0, GameSaveHeaderRow *arg1) {
    u8 block[MC_BLOCK_SIZE];
    void *header;
    s32 tries;
    s32 fd;
    s32 temp;
    s32 i;

    header = arg1;
    GameMenuLoadPhase = 0x3000;
    tries = 0;
    temp = arg0 * 2;
    temp += arg0;
    temp <<= 2;
    temp += arg0;

    {
        s32 nameOffset = temp * 2;
        register char *name asm("$4");

        do {
            name = g_SaveFilePath;
            name = (char *)(nameOffset + (s32)name);
            fd = BiosFileOpen(name, 1);
            if (fd >= 0) {
                break;
            }
            tries++;
        } while (tries < 2);
    }

    GameMenuLoadPhase = tries | 0x3100;

    if (fd < 0) {
        return 0;
    }

    GameMenuLoadPhase = 0x3300;
    if (ReadVerifiedSaveHeader(fd, header) == 0) {
        return 0;
    }

    GameMenuLoadPhase = 0x3500;
    if (BiosFileSeek(fd, 0x280, 0) < 0) {
        return 0;
    }

    GameMenuLoadPhase = 0x3600;
    if (BiosFileRead(fd, block, MC_BLOCK_SIZE) != MC_BLOCK_SIZE) {
        return 0;
    }

    BiosFileClose(fd);
    GameMenuLoadPhase = 0x3700;
    if (LoadSaveStateBlock(block) == 0) {
        return 0;
    }

    GameMenuLoadPhase = 0x3800;
    g_TeamNameLength = *(u8 *)header;
    i = 0;
    do {
        u8 *copy_src = (u8 *)header + i;
        g_TeamNameChars[i] = copy_src[1];
        i++;
    } while (i < 7);

    {
        s32 one = 1;
        s32 word;
        s32 status;

        word = *(s32 *)((u8 *)header + 8);
        status = tries | 0x3900;
        GameMenuLoadPhase = status;
        g_SaveElapsedTicks = word;
        return one;
    }
}

extern char g_FmtCardWildcard[] asm("D_80012FAC");
extern char g_McDirEntries[] asm("D_8009B748");

void LibcSprintf() asm("func_800632F0");

s32 CountMemoryCardFiles(s32 arg0, s32 arg1) {
    char path[0x20];
    void *entry;
    void *ret;
    s32 count;

    count = 0;
    LibcSprintf(path, g_FmtCardWildcard, arg0, arg1);
    entry = g_McDirEntries;

    if (BiosFirstFile(path, entry) == entry) {
        do {
            count++;
            entry = (char *)entry + 0x28;
            ret = BiosNextFile(entry);
        } while (ret == entry);
    }

    return count;
}

/* ---- was CalculateMemoryCardFreeBlocks.c ---- */

#include "common.h"
#include "game/memcard.h"
#include "game/menu.h"

extern char g_McDirEntries[] asm("D_8009B748");

s32 CalculateMemoryCardFreeBlocks(s32 arg0) {
    u8 scratch[8];
    s32 i;
    s32 sum;
    u8 *ptr;
    s32 value;

    i = 0;
    sum = 0;

    if (arg0 > 0) {
        ptr = (u8 *)g_McDirEntries;
        do {
            value = *(s32 *)(ptr + 0x18);
            sum += value;
            ptr += 0x28;
        } while (++i < arg0);
    }

    {
        s32 biased;

        biased = sum;
        if (sum < 0) {
            biased = sum + 0x1FFF;
        }
        sum = biased >> 13;

        return 0xF - sum;
    }
}

extern s32 g_McCardFileCount asm("D_8009B738");
extern s32 g_McFreeBlocks asm("D_8009B73C");

s32 RefreshMemoryCardSaveStatus(s32 arg0, GameSaveHeaderRow *arg1) {
    s32 ret;

    GameMenuLoadPhase = 0x100;
    ClearSaveHeaderRows(arg1);
    g_McCardFileCount = CountMemoryCardFiles(0, 0);
    g_McFreeBlocks = CalculateMemoryCardFreeBlocks(g_McCardFileCount);
    ret = ScanMemoryCardSaveHeaders(arg1);
    GameMenuLoadPhase = 0x200;

    return ret;
}

extern char g_FmtPlayTime[] asm("D_80012FB8");

/* sprintf: every caller declares its own arity; keep it prototypeless. */
void LibcSprintf() asm("func_800632F0");

void *FormatSaveElapsedTime(void *arg0, u32 arg1) {
    u32 hours = arg1 / 216000;
    u32 totalMinutes = arg1 / 3600;
    u32 totalSeconds = arg1 / 60;

    LibcSprintf(arg0, g_FmtPlayTime, hours, totalMinutes - (hours * 60), totalSeconds - (totalMinutes * 60));
    return (u8 *)arg0 + 2;
}

extern u8 g_McMessageText[] asm("D_800128FC");

/* DrawText8x8 again, declared with word-wide parameters: the header
 * spelling with s16/u16 does not match here. Same convention as
 * GameQueueSprite vs GameQueueSpriteWide in game/render.h. */
void DrawText8x8Wide(s32 arg0, s32 arg1, void *arg2, s32 arg3) asm("func_80016754");

void DrawMemoryCardMessageLine(s32 arg0, s32 arg1) {
    DrawText8x8Wide(0x28, 0xB8, &g_McMessageText[arg1 * 30], 0x78CC);
}

extern u8 g_McHelpText[] asm("D_80012ADC");


void DrawMemoryCardHelpPrompt(s32 arg0) {
    s32 i;

    i = arg0 * 0x3C;
    DrawText8x8Wide(0x50, 0x28, &g_McHelpText[i], 0x78CC);
    DrawText8x8Wide(0x50, 0x40, &g_McHelpText[i + 0x1E], 0x78CC);
}

/* ---- was DrawMemoryCardSaveRows.c ---- */

#include "common.h"
#include "game/memcard.h"
#include "game/audio.h"
#include "game/menu.h"
#include "game/state.h"
#include "psyq/gpu.h"
#include "game/render.h"

extern char g_FmtSaveRow[] asm("D_80012FC8");
extern u8 g_SaveNameCharset[] asm("D_80012FD0");
extern char g_FmtSaveRowTail[] asm("D_80012FFC");
extern char g_FmtSaveRowEmpty[] asm("D_80013000");
extern char g_McSlotLabels[] asm("D_80082F7C");
extern char g_McSlotLabelNoFile[] asm("D_80082F86");
extern char g_McSlotLabelError[] asm("D_80082F9A");
extern s32 g_McMenuPage asm("D_80082F50");
extern s32 g_McMenuRowCursor asm("D_80082F54");
extern s32 g_McFreeBlocks asm("D_8009B73C");

/* DrawLargeText with word-wide parameters; the header spelling does not
 * match here. See DrawText8x8Wide above. */
void DrawLargeTextWide(s32, s32, void *, s32, s32, s32, s32, s32) asm("func_80047958");
void LibcSprintf() asm("func_800632F0");

void DrawMemoryCardSaveRows(s32 flags, GameSaveHeaderRow *rows) {
    char text[16];
    u8 *rows_base = (u8 *)rows;
    s32 flags_reg = flags;
    s32 color = 0x7F;
    s32 width = 0x244;
    s32 height = 0xA0;
    char *text_ptr = text;
    s32 y = 0xD8;
    s32 row_bit = 1;
    u8 *row = rows_base;

    do {
        if (flags_reg % 2) {
            s32 i;

            LibcSprintf(text, g_FmtSaveRow, row_bit);
            DrawLargeTextWide(0x48, y, text, 0x7F, color, color, width, height);

            for (i = 0; i < row[0]; i++) {
                text_ptr[i] = g_SaveNameCharset[*((row + i) + 1)];
            }
            while (i < 7) {
                text_ptr[i++] = ' ';
            }
            LibcSprintf(text + 6, g_FmtSaveRowTail);
            DrawLargeTextWide(0x68, y, text, 0x7F, color, color, width, height);
            DrawLargeTextWide(0xB0, y, FormatSaveElapsedTime(text, *(s32 *)(row + 8)), 0x7F, color, color, width, height);
        } else if (flags_reg & 0x10000) {
            LibcSprintf(text, g_FmtSaveRow, row_bit);
            DrawLargeTextWide(0x48, y, text, 0x7F, color, color, width, height);
            DrawLargeTextWide(0x88, y, g_McSlotLabelError, 0x7F, color, color, width, height);
        } else if (g_McFreeBlocks == 0) {
            if (g_McMenuPage == 0) {
                LibcSprintf(text, g_FmtSaveRowEmpty, row_bit);
                DrawLargeTextWide(0x48, y, text, 0x7F, color, color, width, height);
            } else if (g_McMenuRowCursor == 0) {
                LibcSprintf(text, g_FmtSaveRowEmpty, row_bit);
                DrawLargeTextWide(0x48, y, text, 0x7F, color, color, width, height);
            } else {
                LibcSprintf(text, g_FmtSaveRow, row_bit);
                DrawLargeTextWide(0x48, y, text, 0x7F, color, color, width, height);
                DrawLargeTextWide(0x90, y, g_McSlotLabelNoFile, 0x7F, color, color, width, height);
            }
        } else if (g_McMenuPage == 0) {
            LibcSprintf(text, g_FmtSaveRowEmpty, row_bit);
            DrawLargeTextWide(0x48, y, text, 0x7F, color, color, width, height);
        } else {
            LibcSprintf(text, g_FmtSaveRow, row_bit);
            DrawLargeTextWide(0x48, y, text, 0x7F, color, color, width, height);
            DrawLargeTextWide(0x90, y, g_McSlotLabels + (g_McMenuRowCursor * 10), 0x7F, color, color, width, height);
        }

        row_bit++;
        row += 0x80;
        y += 0x30;
        flags_reg >>= 1;
    } while ((s32)row < (s32)(rows_base + 0x180));
}

void AdjustMenuSelectionHorizontal(s32 *value, s32 min, s32 max) {
    u16 input = g_PadEdge;
    s32 next;

    if (input & 0x4000) {
        next = *value + 1;
        *value = next;
        if (max < next) {
            *value = max;
            return;
        }
    } else if (input & 0x1000) {
        next = *value - 1;
        *value = next;
        if (next < min) {
            *value = min;
            return;
        }
    } else {
        return;
    }

    PlaySoundCue(1);
}

void SetMenuBinaryChoiceVertical(s32 *value) {
    u16 input = g_PadEdge;

    if (input & 0x8000) {
        if (*value == 0) {
            PlaySoundCue(1);
            *value = 1;
        } else {
            *value = 1;
        }
    } else if (input & 0x2000) {
        if (*value == 1) {
            PlaySoundCue(1);
        }
        *value = 0;
    }
}

u16 PollMenuConfirmInput(void) {
    u16 *state = &g_PadEdge2;
    u16 value;

    value = *state & 0x860;
    if (value != 0) {
        PlaySoundCue(2);
    }

    return *state & 0x860;
}

u16 PollMenuBackInput(void) {
    u16 *state = &g_PadEdge2;
    u16 value;

    value = *state & 0x90;
    if (value != 0) {
        PlaySoundCue(3);
    }

    return *state & 0x90;
}

void DrawFullscreenFadeTile(s32 arg0, s32 arg1) asm("func_80023A60");

void DrawMenuFadeOverlay(s32 arg0) {
    DrawFullscreenFadeTile(arg0, 0x40);
}

extern s32 g_McFadeStep asm("D_8009B9A0");

void StartMenuExitFade(void) {
    StopMemoryCardEvents();
    g_McFadeStep = 8;
}

extern s32 g_McMenuRowCount asm("D_8009B744");
extern s32 g_McMenuPage;
extern s32 g_McMenuRowCursor;
extern s32 g_McFromLoadMenu asm("D_8009B730");
extern s32 g_McFadeStep;
extern s32 g_McFadeLevel asm("D_8009B9A4");
void EnterMemoryCardMenu(void) asm("func_800613B8");
void EnterMemoryCardMenu(void) {
    SetDispMask(0);
    SetupDisplay480(0, 0, 0);
    g_McMenuRowCount = 2;
    g_McMenuState = -1;
    g_SceneTimer = 0;
    g_McMenuPage = 0;
    g_McMenuRowCursor = 0;
    g_McMenuSubState = 1;
    g_McFromLoadMenu = 0;
    StartMemoryCardEvents();
    g_McFadeStep = -8;
    g_McFadeLevel = 0xFF;
    g_SceneId = 0x1A;
}
