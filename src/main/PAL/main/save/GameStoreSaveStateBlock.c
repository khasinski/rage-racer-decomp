#include "common.h"
#include "game/race.h"
#include "game/memcard.h"

extern u16 g_PadMappingIndex asm("D_8019CE08");
extern u16 g_NegconMappingIndex asm("D_8019CB08");
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
extern s32 g_RankingRecords[] asm("D_801E7744");
extern s32 g_TimeRecords[] asm("D_8019CB78");
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
        s32 *rankingBase = g_RankingRecords;
        s32 *timeBase = g_TimeRecords;
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
