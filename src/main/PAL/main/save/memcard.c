#include "common.h"
#include "game/car.h"
#include "psyq/kernel.h"

extern s32 g_McLastCardStatus;
extern s32 g_McStatusState;
extern s32 g_McPollTicks;
extern s32 g_McStatusResult;
/* The poller's own working status word. Distinct from menu.h's
 * g_McPollStatus (g_McCardStatus), which is the code the menu reads. */
extern s32 g_McPollStatus;
extern char g_FmtCardDevice[];

/* sprintf: every caller declares its own arity; keep it prototypeless. */
void LibcSprintf();
void _card_info(s32 chan);
void _card_load(s32 chan);

s32 PollMemoryCardStatus(s32 port, s32 slot) {
    s32 handle;
    s32 two;
    s32 status;
    s32 state;

    handle = (port * 16) + slot;

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

s32 FormatMemoryCard(s32 port, s32 slot) {
    char device[8];
    s32 status;

    LibcSprintf(device, g_FmtCardDevice, port, slot);
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

/* The eight libcard event descriptors: [0..3] are the hardware class
 * 0xF4000001 and [4..7] the software class 0xF0000011, each in the order IOE,
 * Error, Timeout, NewCard -- which is why every poller below returns
 * index + 1. They are eight scalars rather than one array because with an
 * array symbol gcc 2.6.3 keeps the base address live in a callee-saved
 * register across the TestEvent calls, which grows PollMemoryCardHwEvent's
 * frame from 24 to 32 bytes. */
extern s32 g_McHwEventIoe;
extern s32 g_McHwEventError;
extern s32 g_McHwEventTimeout;
extern s32 g_McHwEventNew;
extern s32 g_McSwEventIoe;
extern s32 g_McSwEventError;
extern s32 g_McSwEventTimeout;
extern s32 g_McSwEventNew;

void OpenMemoryCardEvents(void) {
    EnterCriticalSection();
    g_McHwEventIoe = OpenEvent(0xF4000001, 0x0004, 0x2000, 0);
    g_McHwEventError = OpenEvent(0xF4000001, 0x8000, 0x2000, 0);
    g_McHwEventTimeout = OpenEvent(0xF4000001, 0x0100, 0x2000, 0);
    g_McHwEventNew = OpenEvent(0xF4000001, 0x2000, 0x2000, 0);
    g_McSwEventIoe = OpenEvent(0xF0000011, 0x0004, 0x2000, 0);
    g_McSwEventError = OpenEvent(0xF0000011, 0x8000, 0x2000, 0);
    g_McSwEventTimeout = OpenEvent(0xF0000011, 0x0100, 0x2000, 0);
    g_McSwEventNew = OpenEvent(0xF0000011, 0x2000, 0x2000, 0);
    ExitCriticalSection();
}

void EnableMemoryCardEvents(void) {
    EnableEvent(g_McHwEventIoe);
    EnableEvent(g_McHwEventError);
    EnableEvent(g_McHwEventTimeout);
    EnableEvent(g_McHwEventNew);
    EnableEvent(g_McSwEventIoe);
    EnableEvent(g_McSwEventError);
    EnableEvent(g_McSwEventTimeout);
    EnableEvent(g_McSwEventNew);
}

void DisableMemoryCardEvents(void) {
    DisableEvent(g_McHwEventIoe);
    DisableEvent(g_McHwEventError);
    DisableEvent(g_McHwEventTimeout);
    DisableEvent(g_McHwEventNew);
    DisableEvent(g_McSwEventIoe);
    DisableEvent(g_McSwEventError);
    DisableEvent(g_McSwEventTimeout);
    DisableEvent(g_McSwEventNew);
}

void CloseMemoryCardEvents(void) {
    EnterCriticalSection();
    CloseEvent(g_McHwEventIoe);
    CloseEvent(g_McHwEventError);
    CloseEvent(g_McHwEventTimeout);
    CloseEvent(g_McHwEventNew);
    CloseEvent(g_McSwEventIoe);
    CloseEvent(g_McSwEventError);
    CloseEvent(g_McSwEventTimeout);
    CloseEvent(g_McSwEventNew);
    ExitCriticalSection();
}

#include "common.h"
#include "psyq/kernel.h"
#include "game/memcard.h"

void ClearMemoryCardHwEvents(void) {
    TestEvent(g_McHwEventIoe);
    TestEvent(g_McHwEventError);
    TestEvent(g_McHwEventTimeout);
    TestEvent(g_McHwEventNew);
}

void ClearMemoryCardSwEvents(void) {
    TestEvent(g_McSwEventIoe);
    TestEvent(g_McSwEventError);
    TestEvent(g_McSwEventTimeout);
    TestEvent(g_McSwEventNew);
}

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

void RestartMemoryCard(void);
void RestartMemoryCard(void) { InitCARD(1); StartCARD(); BiosBuInit(); g_SaveElapsedTicks = 0; }

extern s32 g_FrameSyncThreshold;

void AdvanceSaveHeaderCounter(void) {
    if (g_FrameSyncThreshold == 0x80) {
        g_SaveElapsedTicks++;
    } else {
        g_SaveElapsedTicks += 2;
    }
}

#include "common.h"
#include "game/memcard.h"
#include "psyq/gpu.h"
#include "game/menu.h"

void ClearSaveHeaderRows(GameSaveHeaderRow *rows) {
    u8 *rowBytes = (u8 *)rows;
    s32 i = 0;
    u8 *ptr1 = rowBytes;
    s32 j;
    u8 *ptr2;
    u8 *ptr3;

    do {
        rowBytes[0] = 0;
        j = 5;
        ptr2 = rowBytes + 5;
        do {
            ptr2[1] = 0;
            ptr2--;
            j--;
        } while (j >= 0);

        *(volatile u32 *)&rowBytes[8] = 0;

        j = 0;
        ptr3 = ptr1;
        do {
            j++;
        } while ((*(u16 *)&ptr3[0xC] = 0, j < 0x38));

        *(volatile u32 *)&rowBytes[0x7C] = 0;
        rowBytes += 0x80;
        i++;
        ptr1 += 0x82;
    } while (i < 3);
}

/* sprintf: every caller declares its own arity; keep it prototypeless. */
void LibcSprintf();

/* The icon strip is copied to VRAM in two passes: one 16x1 run of palette
 * entries at block+0x60, then the 4x16 frame tiles from block+0x80 on.  The
 * frame loop is written against the counter rather than against running
 * offsets because that is what the retail code's induction variables are:
 * `i * 0x80` and `i * 4` are strength-reduced back into the `addiu s4,s4,128`
 * / `addiu s1,s1,4` pair, and doing it this way puts the two increments'
 * initialisers after the loop-invariant `4` and `0x10` in the preheader,
 * which is the order retail schedules them in. */
void BuildSaveIconBlock(u8 *block, char *title, s32 iconTile, s32 imageX, s32 imageY) {
    Rect *iconRect;
    Rect *frameRect;
    s32 i;
    s32 tileRow;
    s32 tileX;

    block[0] = 'S';
    block[1] = 'C';
    block[2] = 0x11;
    block[3] = 1;
    LibcSprintf(block + 4, g_FmtString, title);

    tileRow = iconTile / 20;
    iconRect = &g_SaveIconRect;
    g_SaveIconRect.w = 0x10;
    g_SaveIconRect.h = 1;
    tileX = iconTile % 20;
    iconRect->x = tileX * 16;
    g_SaveIconRect.y = tileRow + 0x1E0;
    StoreImage(iconRect, block + 0x60);
    DrawSync(0);

    i = 0;
    frameRect = iconRect;
    do {
        frameRect->x = imageX + i * 4;
        frameRect->y = imageY;
        frameRect->w = 4;
        frameRect->h = 0x10;
        StoreImage(frameRect, block + 0x80 + i * 0x80);
        DrawSync(0);
        i++;
    } while (i <= 0);
}

void WriteSaveHeaderRow(GameSaveHeaderRow *row) {
    u8 *rowBytes = (u8 *)row;
    s32 i;
    u32 checksum;
    u16 *scan;

    rowBytes[0] = g_TeamNameLength;

    for (i = 0; i < 7; i++) {
        *((rowBytes + i) + 1) = g_TeamNameChars[i];
    }

    i = 0;
    checksum = 0;
    *(s32 *)(rowBytes + 8) = g_SaveElapsedTicks;
    scan = (u16 *)rowBytes;

    do {
        checksum += *scan++;
        i++;
    } while ((u32)i < 0x3E);

    checksum = ~checksum;
    *(u32 *)(rowBytes + 0x7C) = checksum;
}

#include "common.h"
#include "game/race.h"
#include "game/memcard.h"
#include "game/menu.h"

extern s16 g_PadMappingIndex;
extern s16 g_NegconMappingIndex;
extern u16 g_NegconSteerNeutral;
extern u16 g_NegconSteerPlay;
extern u16 g_NegconNeutralI;
extern u16 g_NegconNeutralII;
extern u16 g_NegconMaxTwist;
extern u16 g_NegconNeutralL;
extern u16 g_BgmSelection;

extern u16 g_ClassRecords[];
extern u16 g_TeamLogoCanvas[];
extern s32 g_BestLapTimes[];
extern s32 g_BestTotalTimes[];
extern s32 g_BestSectorTimes[];

extern u8 g_GrandPrixCourseProgress[];
extern u8 g_ExtraGrandPrixCourseProgress[];

void StoreSaveStateBlock(u8 *block) {
    {
        u16 padMappingIndex = g_PadMappingIndex;
        u16 negconMappingIndex = g_NegconMappingIndex;
        u16 negconSteerNeutral = g_NegconSteerNeutral;
        u16 negconSteerPlay = g_NegconSteerPlay;
        *(u16 *)(block + 0x0) = padMappingIndex;
        *(u16 *)(block + 0x2) = negconMappingIndex;
        *(u16 *)(block + 0x4) = negconSteerNeutral;
        *(u16 *)(block + 0x6) = negconSteerPlay;
    }
    *(u16 *)(block + 0x8) = g_NegconNeutralI;
    *(u16 *)(block + 0xA) = g_NegconNeutralII;
    {
        u16 negconMaxTwist = g_NegconMaxTwist;
        u16 negconNeutralL = g_NegconNeutralL;
        *(u16 *)(block + 0xE) = negconMaxTwist;
        *(u16 *)(block + 0xC) = negconNeutralL;
    }

    *(s32 *)(block + 0x10) = g_GrandPrixSave.course;
    *(s32 *)(block + 0x14) = g_GrandPrixSave.carIndex;
    *(s32 *)(block + 0x18) = g_GrandPrixSave.classIndex;
    *(s32 *)(block + 0x1C) = g_GrandPrixSave.maxClassReached;
    *(s32 *)(block + 0x20) = g_GrandPrixSave.money;
    *(s32 *)(block + 0x24) = g_ExtraGrandPrixSave.course;
    *(s32 *)(block + 0x28) = g_ExtraGrandPrixSave.carIndex;
    *(s32 *)(block + 0x2C) = g_ExtraGrandPrixSave.classIndex;
    *(s32 *)(block + 0x30) = g_ExtraGrandPrixSave.maxClassReached;
    {
        s32 extraMoney = g_ExtraGrandPrixSave.money;
        u16 bgmSelection = g_BgmSelection;
        *(s32 *)(block + 0x34) = extraMoney;
        *(s32 *)(block + 0x38) = g_TimeAttackSave.course;
        *(s32 *)(block + 0x3C) = g_TimeAttackSave.carIndex;
        *(s32 *)(block + 0x40) = g_TimeAttackSave.classIndex;
        *(s32 *)(block + 0x44) = g_TimeAttackSave.maxClassReached;
        {
            register s32 timeAttackMoney asm("$4") = g_TimeAttackSave.money;
            u16 advancedUnlocked = g_AdvancedSeriesUnlocked;
            *(u16 *)(block + 0x4C) = bgmSelection;
            *(u16 *)(block + 0x4E) = advancedUnlocked;
            *(s32 *)(block + 0x48) = timeAttackMoney;
        }
    }
    *(s32 *)(block + 0x50) = g_MaxClassReached[0];
    *(s32 *)(block + 0x54) = g_MaxClassReached[1];

    {
        register u8 *dst asm("$4");

        {
            s32 i;

            dst = block;
            for (i = 0; i < 13; i++) {
                dst[MC_GP_CARS_OFS + 0] = g_GrandPrixCars[i].modelVariant;
                dst[MC_GP_CARS_OFS + 1] = g_GrandPrixCars[i].tireCompound;
                dst[MC_GP_CARS_OFS + 2] = g_GrandPrixCars[i].transmission;
                dst[MC_GP_CARS_OFS + 3] = g_GrandPrixCars[i].paintColor1;
                dst[MC_GP_CARS_OFS + 4] = g_GrandPrixCars[i].paintColor2;
                dst[MC_GP_CARS_OFS + 5] = g_GrandPrixCars[i].enabled;

                dst[MC_EXTRA_CARS_OFS + 0] = g_ExtraGrandPrixCars[i].modelVariant;
                dst[MC_EXTRA_CARS_OFS + 1] = g_ExtraGrandPrixCars[i].tireCompound;
                dst[MC_EXTRA_CARS_OFS + 2] = g_ExtraGrandPrixCars[i].transmission;
                dst[MC_EXTRA_CARS_OFS + 3] = g_ExtraGrandPrixCars[i].paintColor1;
                dst[MC_EXTRA_CARS_OFS + 4] = g_ExtraGrandPrixCars[i].paintColor2;
                dst[MC_EXTRA_CARS_OFS + 5] = g_ExtraGrandPrixCars[i].enabled;

                dst[MC_TIME_CARS_OFS + 0] = g_TimeAttackCars[i].modelVariant;
                dst[MC_TIME_CARS_OFS + 1] = g_TimeAttackCars[i].tireCompound;
                dst[MC_TIME_CARS_OFS + 2] = g_TimeAttackCars[i].transmission;
                dst[MC_TIME_CARS_OFS + 3] = g_TimeAttackCars[i].paintColor1;
                dst[MC_TIME_CARS_OFS + 4] = g_TimeAttackCars[i].paintColor2;
                dst[MC_TIME_CARS_OFS + 5] = g_TimeAttackCars[i].enabled;

                dst += 8;
            }
        }

        {
            s32 offset;

            dst = block;
            offset = 0;
            for (; offset < 0x2C; offset += 4) {
                *(u16 *)(dst + 0x190) = *(u16 *)((u8 *)g_ClassRecords + offset);
                *(u16 *)(dst + 0x192) = *(u16 *)((u8 *)g_ClassRecords + offset + 2);
                dst += 4;
            }
        }
    }

    {
        register s32 count asm("$13");

        {
            u16 *src;
            u8 *dst;

            count = 0;
            src = g_TeamLogoClut;
            dst = block;
            for (; count < 0x10; count++) {
                *(u16 *)(dst + 0x1BC) = *src++;
                dst += 2;
            }
        }

        {
            u16 *src;
            u8 *dst;

            count = 0;
            src = g_TeamLogoCanvas;
            dst = block;
            for (; count < 0x400; count++) {
                *(u16 *)(dst + 0x1DC) = *src++;
                dst += 2;
            }
        }
    }

    {
        /* The remaining register hints in these loops are load-bearing. */
        s32 outer = 0;
        s32 *lapBase = g_BestLapTimes;
        s32 *totalBase = g_BestTotalTimes;
        u8 *outerDst = block;

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
        register u8 *outerDst asm("$25") = block;
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
        u8 *outerDst = block;
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
        u16 *checksumSrc = (u16 *)block;

        *(s32 *)(block + 0xFBC) = bgmVolume;
        *(s32 *)(block + 0xFC0) = sfxVolume;
        *(s32 *)(block + 0xFC4) = monoOutput;
        memcpy(block + 0xFC8, g_GrandPrixCourseProgress, 8);
        memcpy(block + 0xFD0, g_ExtraGrandPrixCourseProgress, 8);

        for (; count < 0x7FE; count++) {
            checksum += *checksumSrc++;
        }
        *(u32 *)(block + MC_BLOCK_CHECKSUM_OFS) = ~checksum;
    }
}

#include "common.h"
#include "game/race.h"
#include "game/menu.h"
#include "psyq/gpu.h"

extern s16 g_PadMappingIndex;
extern s16 g_NegconMappingIndex;
extern u16 g_NegconSteerNeutral;
extern u16 g_NegconSteerPlay;
extern u16 g_NegconNeutralI;
extern u16 g_NegconNeutralII;
extern u16 g_NegconMaxTwist;
extern u16 g_NegconNeutralL;
/* The loader stores a whole word here; the saver reads only the low half
 * as g_BgmSelection. Same address, two widths, so two names. */

extern u16 g_ClassRecords[];
extern u16 g_TeamLogoCanvas[];
extern s32 g_BestLapTimes[];
extern s32 g_BestTotalTimes[];
extern s32 g_BestSectorTimes[];

extern u8 g_GrandPrixCourseProgress[];
extern u8 g_ExtraGrandPrixCourseProgress[];

extern u8 g_TeamLogoRect[];
extern u8 g_TeamLogoClutRect[];

s32 LoadSaveStateBlock(u8 *block) {
    register u8 *base asm("$17") = block;
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
        u16 padMappingIndex = *(u16 *)(base + 0x0);
        u16 negconMappingIndex = *(u16 *)(base + 0x2);
        u16 negconSteerNeutral = *(u16 *)(base + 0x4);
        u16 negconSteerPlay = *(u16 *)(base + 0x6);
        g_NegconNeutralI = *(u16 *)(base + 0x8);
        g_NegconNeutralII = *(u16 *)(base + 0xA);
        g_NegconNeutralL = *(u16 *)(base + 0xC);
        {
            u16 hE = *(u16 *)(base + 0xE);
            s32 extraMaxClass;
            g_GrandPrixSave.course = *(s32 *)(base + 0x10);
            g_GrandPrixSave.carIndex = *(s32 *)(base + 0x14);
            g_GrandPrixSave.classIndex = *(s32 *)(base + 0x18);
            g_GrandPrixSave.maxClassReached = *(s32 *)(base + 0x1C);
            g_GrandPrixSave.money = *(s32 *)(base + 0x20);
            g_ExtraGrandPrixSave.course = *(s32 *)(base + 0x24);
            g_ExtraGrandPrixSave.carIndex = *(s32 *)(base + 0x28);
            g_ExtraGrandPrixSave.classIndex = *(s32 *)(base + 0x2C);
            extraMaxClass = *(s32 *)(base + 0x30);
            g_PadMappingIndex = padMappingIndex;
            g_NegconMappingIndex = negconMappingIndex;
            g_NegconSteerNeutral = negconSteerNeutral;
            g_NegconSteerPlay = negconSteerPlay;
            g_NegconMaxTwist = hE;
            g_ExtraGrandPrixSave.maxClassReached = extraMaxClass;
        }
        g_ExtraGrandPrixSave.money = *(s32 *)(base + 0x34);
        g_TimeAttackSave.course = *(s32 *)(base + 0x38);
        g_TimeAttackSave.carIndex = *(s32 *)(base + 0x3C);
        g_TimeAttackSave.classIndex = *(s32 *)(base + 0x40);
        g_TimeAttackSave.maxClassReached = *(s32 *)(base + 0x44);
        g_TimeAttackSave.money = *(s32 *)(base + 0x48);
        {
            s32 bgmSelection = *(s16 *)(base + 0x4C);
            u16 advancedUnlocked = *(u16 *)(base + 0x4E);
            s32 maxClassReached1;
            g_MaxClassReached[0] = *(s32 *)(base + 0x50);
            maxClassReached1 = *(s32 *)(base + 0x54);
            *(s32 *)&g_BgmSelection = bgmSelection;
            g_AdvancedSeriesUnlocked = advancedUnlocked;
            g_MaxClassReached[1] = maxClassReached1;
        }
    }

    {
        register u8 *src asm("$6") = base;
        s32 i;
        for (i = 0; i < 13; i++) {
            g_GrandPrixCars[i].modelVariant = src[MC_GP_CARS_OFS + 0];
            g_GrandPrixCars[i].tireCompound = src[MC_GP_CARS_OFS + 1];
            g_GrandPrixCars[i].transmission = src[MC_GP_CARS_OFS + 2];
            g_GrandPrixCars[i].paintColor1 = src[MC_GP_CARS_OFS + 3];
            g_GrandPrixCars[i].paintColor2 = src[MC_GP_CARS_OFS + 4];
            g_GrandPrixCars[i].enabled = src[MC_GP_CARS_OFS + 5];
            g_ExtraGrandPrixCars[i].modelVariant = src[MC_EXTRA_CARS_OFS + 0];
            g_ExtraGrandPrixCars[i].tireCompound = src[MC_EXTRA_CARS_OFS + 1];
            g_ExtraGrandPrixCars[i].transmission = src[MC_EXTRA_CARS_OFS + 2];
            g_ExtraGrandPrixCars[i].paintColor1 = src[MC_EXTRA_CARS_OFS + 3];
            g_ExtraGrandPrixCars[i].paintColor2 = src[MC_EXTRA_CARS_OFS + 4];
            g_ExtraGrandPrixCars[i].enabled = src[MC_EXTRA_CARS_OFS + 5];
            g_TimeAttackCars[i].modelVariant = src[MC_TIME_CARS_OFS + 0];
            g_TimeAttackCars[i].tireCompound = src[MC_TIME_CARS_OFS + 1];
            g_TimeAttackCars[i].transmission = src[MC_TIME_CARS_OFS + 2];
            g_TimeAttackCars[i].paintColor1 = src[MC_TIME_CARS_OFS + 3];
            g_TimeAttackCars[i].paintColor2 = src[MC_TIME_CARS_OFS + 4];
            g_TimeAttackCars[i].enabled = src[MC_TIME_CARS_OFS + 5];
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
        register s32 i asm("$13");
        register s32 j asm("$12");
        register s32 k asm("$7");

        {
            u16 *dst;
            u8 *src;

            i = 0;
            dst = g_TeamLogoClut;
            src = base;
            for (; i < 0x10; i++) {
                *dst++ = *(u16 *)(src + 0x1BC);
                src += 2;
            }
        }

        {
            u16 *dst;
            u8 *src;

            i = 0;
            dst = g_TeamLogoCanvas;
            src = base;
            for (; i < 0x400; i++) {
                *dst++ = *(u16 *)(src + 0x1DC);
                src += 2;
            }
        }

        /* g_BestLapTimes / g_BestTotalTimes */
        {
            s32 off;
            i = 0;
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
            s32 *cb78;
            register s32 *d1base asm("$24");
            register s32 ioff asm("$16");
            i = 0;
            cb78 = (s32 *)g_TimeRecords;
            d1base = (s32 *)g_RankingRecords;
            ioff = 0;
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
            register s32 *e41e8 asm("$10");
            s32 ioff;
            i = 0;
            e41e8 = g_BestSectorTimes;
            ioff = 0;
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

s32 WriteMemoryCardSaveSlot(s32 slot, GameSaveHeaderRow *header) {
    u8 block0[0x200];
    u8 block1[MC_BLOCK_SIZE];
    s32 i;

    for (i = 0x1FF; i >= 0; i--) {
        block0[i] = 0;
    }

    GameMenuLoadPhase = 0x1000;
    return WriteMemoryCardSaveFile(
        g_SaveFilePath + slot * 0x1A,
        g_SaveTitleSjis + slot * 0x46,
        block0,
        header,
        block1);
}

/* The header is stored twice, at 0x1280 and at 0x200; the first copy whose
 * 16-bit sum matches the complement stored in its last word wins.
 *
 * The scan is written as ptr[i] rather than *ptr++ so that gcc derives the
 * walking pointer itself: a strength-reduced giv is initialised after the
 * counter, which is the order retail emits the two moves in. */
s32 ReadVerifiedSaveHeader(s32 slot, GameSaveHeaderRow *header) {
    void *buffer;
    s32 sum;
    s32 i;
    u16 *ptr;

    buffer = header;
    sum = 0;

    GameMenuLoadPhase = 0x120;
    if (BiosFileSeek(slot, 0x1280, 0) < 0) {
        return 0;
    }

    GameMenuLoadPhase = 0x130;
    if (BiosFileRead(slot, buffer, 0x80) != 0x80) {
        return 0;
    }

    GameMenuLoadPhase = 0x140;
    ptr = buffer;
    for (i = 0; (u32)i < 0x3E; i++) {
        sum += ptr[i];
    }
    /* Folding the complement back into sum is what puts it in sum's own
     * register; a `== ~sum` in the test needs a second one. */
    sum = ~sum;

    if (*(s32 *)((u8 *)buffer + 0x7C) == sum) {
        return 1;
    }

    GameMenuLoadPhase = 0x150;
    if (BiosFileSeek(slot, 0x200, 0) < 0) {
        return 0;
    }

    GameMenuLoadPhase = 0x160;
    if (BiosFileRead(slot, buffer, 0x80) != 0x80) {
        return 0;
    }

    GameMenuLoadPhase = 0x170;
    sum = 0;
    ptr = buffer;
    for (i = 0; (u32)i < 0x3E; i++) {
        sum += ptr[i];
    }
    /* Folding the complement back into sum is what puts it in sum's own
     * register; a `== ~sum` in the test needs a second one. */
    sum = ~sum;

    if (*(s32 *)((u8 *)buffer + 0x7C) == sum) {
        return 1;
    }

    GameMenuLoadPhase = 0x180;
    return 0;
}

s32 ScanMemoryCardSaveHeaders(GameSaveHeaderRow *headers) {
    s32 fd;
    s32 i;
    s32 mask;
    s32 nameOffset;
    void *buffer;

    mask = 0;
    GameMenuLoadPhase = 0x110;
    i = 0;
    buffer = headers;
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

s32 LoadMemoryCardSaveSlot(s32 slot, GameSaveHeaderRow *outHeader) {
    u8 block[MC_BLOCK_SIZE];
    void *header;
    s32 tries;
    s32 fd;
    s32 temp;
    s32 i;

    header = outHeader;
    GameMenuLoadPhase = 0x3000;
    tries = 0;
    temp = slot * 2;
    temp += slot;
    temp <<= 2;
    temp += slot;

    /* Written as a goto rather than a do/while on purpose: retail recomputes
     * g_SaveFilePath + nameOffset on both attempts, and any spelling the front
     * end marks as a loop lets loop.c hoist that pair of insns into the
     * preheader.  A backward goto never gets a NOTE_INSN_LOOP_BEG, so there is
     * nowhere to hoist to and the address is rebuilt each time round. */
    {
        s32 nameOffset = temp * 2;
        char *name;

    retry:
        name = g_SaveFilePath;
        name = (char *)(nameOffset + (s32)name);
        fd = BiosFileOpen(name, 1);
        if (fd < 0) {
            tries++;
            if (tries < 2) {
                goto retry;
            }
        }
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

void LibcSprintf();

s32 CountMemoryCardFiles(s32 port, s32 slot) {
    char path[0x20];
    void *entry;
    void *ret;
    s32 count;

    count = 0;
    LibcSprintf(path, g_FmtCardWildcard, port, slot);
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

#include "common.h"
#include "game/memcard.h"
#include "game/menu.h"

s32 CalculateMemoryCardFreeBlocks(s32 port) {
    u8 scratch[8];
    s32 i;
    s32 sum;
    u8 *ptr;
    s32 value;

    i = 0;
    sum = 0;

    if (port > 0) {
        ptr = (u8 *)g_McDirEntries;
        do {
            value = *(s32 *)(ptr + 0x18);
            sum += value;
            ptr += 0x28;
        } while (++i < port);
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

s32 RefreshMemoryCardSaveStatus(s32 slot, GameSaveHeaderRow *header) {
    s32 ret;

    GameMenuLoadPhase = 0x100;
    ClearSaveHeaderRows(header);
    g_McCardFileCount = CountMemoryCardFiles(0, 0);
    g_McFreeBlocks = CalculateMemoryCardFreeBlocks(g_McCardFileCount);
    ret = ScanMemoryCardSaveHeaders(header);
    GameMenuLoadPhase = 0x200;

    return ret;
}

/* sprintf: every caller declares its own arity; keep it prototypeless. */
void LibcSprintf();

void *FormatSaveElapsedTime(void *dst, u32 seconds) {
    u32 hours = seconds / 216000;
    u32 totalMinutes = seconds / 3600;
    u32 totalSeconds = seconds / 60;

    LibcSprintf(dst, g_FmtPlayTime, hours, totalMinutes - (hours * 60), totalSeconds - (totalMinutes * 60));
    return (u8 *)dst + 2;
}

void DrawMemoryCardMessageLine(s32 unused, s32 messageIndex) {
    DrawText8x8(0x28, 0xB8, &g_McMessageText[messageIndex * 30], 0x78CC);
}

void DrawMemoryCardHelpPrompt(s32 page) {
    s32 i;

    i = page * 0x3C;
    DrawText8x8(0x50, 0x28, &g_McHelpText[i], 0x78CC);
    DrawText8x8(0x50, 0x40, &g_McHelpText[i + 0x1E], 0x78CC);
}

#include "common.h"
#include "game/memcard.h"
#include "game/audio.h"
#include "game/menu.h"
#include "game/state.h"
#include "psyq/gpu.h"
#include "game/render.h"

/* DrawLargeText with word-wide parameters; the header spelling does not
 * match here. See DrawText8x8 above. */
void DrawLargeTextWide(s32, s32, void *, s32, s32, s32, s32, s32) asm("DrawLargeText");
void LibcSprintf();

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

void DrawFullscreenFadeTile(s32 level, s32 tpage) asm("DrawFullscreenFadeTile480");

void DrawMenuFadeOverlay(s32 level) {
    DrawFullscreenFadeTile(level, 0x40);
}

void StartMenuExitFade(void) {
    StopMemoryCardEvents();
    g_McFadeStep = 8;
}

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
