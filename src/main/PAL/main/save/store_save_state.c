#include "common.h"
#include "game/car.h"
#include "game/race.h"
#include "game/memcard.h"
#include "game/menu.h"
#define GAME_INPUT_MAPPING_TYPE s16
#include "game/input_internal.h"
#define GAME_SAVE_BGM_TYPE u16
#include "game/save_internal.h"

void StoreSaveStateBlock(u8 *block) {
    register long saveValue asm("$4");
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
            u16 advancedUnlocked = g_AdvancedSeriesUnlocked;
            saveValue = g_TimeAttackSave.money;
            *(u16 *)(block + 0x4C) = bgmSelection;
            *(u16 *)(block + 0x4E) = advancedUnlocked;
            *(s32 *)(block + 0x48) = saveValue;
        }
    }
    *(s32 *)(block + 0x50) = g_MaxClassReached[0];
    *(s32 *)(block + 0x54) = g_MaxClassReached[1];

    {
        {
            s32 i;

            saveValue = (long)block;
            for (i = 0; i < 13; i++) {
                SavedCarSetup *grandPrixCar =
                    &((GameSaveBlock *)saveValue)->carSetup[0][0];
                SavedCarSetup *extraGrandPrixCar =
                    &((GameSaveBlock *)saveValue)->carSetup[1][0];
                SavedCarSetup *timeAttackCar =
                    &((GameSaveBlock *)saveValue)->carSetup[2][0];

                grandPrixCar->modelVariant = g_GrandPrixCars[i].modelVariant;
                grandPrixCar->tireCompound = g_GrandPrixCars[i].tireCompound;
                grandPrixCar->transmission = g_GrandPrixCars[i].transmission;
                grandPrixCar->paintColor1 = g_GrandPrixCars[i].paintColor1;
                grandPrixCar->paintColor2 = g_GrandPrixCars[i].paintColor2;
                grandPrixCar->enabled = g_GrandPrixCars[i].enabled;

                extraGrandPrixCar->modelVariant = g_ExtraGrandPrixCars[i].modelVariant;
                extraGrandPrixCar->tireCompound = g_ExtraGrandPrixCars[i].tireCompound;
                extraGrandPrixCar->transmission = g_ExtraGrandPrixCars[i].transmission;
                extraGrandPrixCar->paintColor1 = g_ExtraGrandPrixCars[i].paintColor1;
                extraGrandPrixCar->paintColor2 = g_ExtraGrandPrixCars[i].paintColor2;
                extraGrandPrixCar->enabled = g_ExtraGrandPrixCars[i].enabled;

                timeAttackCar->modelVariant = g_TimeAttackCars[i].modelVariant;
                timeAttackCar->tireCompound = g_TimeAttackCars[i].tireCompound;
                timeAttackCar->transmission = g_TimeAttackCars[i].transmission;
                timeAttackCar->paintColor1 = g_TimeAttackCars[i].paintColor1;
                timeAttackCar->paintColor2 = g_TimeAttackCars[i].paintColor2;
                timeAttackCar->enabled = g_TimeAttackCars[i].enabled;

                saveValue += sizeof(SavedCarSetup);
            }
        }

        {
            s32 index;

            saveValue = (long)block;
            index = 0;
            for (; index < 11; index++) {
                SavedClassRecord *dst =
                    &((GameSaveBlock *)saveValue)->classRecords[0];
                dst->grade = g_ClassRecords[index].place;
                dst->clears = g_ClassRecords[index].clears;
                saveValue += sizeof(SavedClassRecord);
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
            src = g_TeamLogoCanvas.halfwords;
            dst = block;
            for (; count < 0x400; count++) {
                *(u16 *)(dst + 0x1DC) = *src++;
                dst += 2;
            }
        }
    }

    {
        s32 inner;
        register s32 middle asm("$12");

        {
            /* The remaining register hints in these loops are load-bearing. */
            s32 outer = 0;
            s32 *lapBase = &g_BestLapTimes[0][0][0];
            s32 *totalBase = &g_BestTotalTimes[0][0][0];
            u8 *outerDst = block;

            for (; outer < 2; outer++) {
                register s32 outerOffset asm("$9") = (middle = 0, outer * 32);
                register u8 *middleDst asm("$11") = outerDst;
                u8 *lapDst = outerDst + 0x9DC;

            for (; middle < 4; middle++) {
                s32 middleOffset = (inner = 0, middle * 8);
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
                s32 *timeDstBase = (inner = 0, (s32 *)(middleDst + 0xCDC));
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
        register s32 *sectorBase asm("$11") = &g_BestSectorTimes[0][0][0];
        u8 *outerDst = block;
        s32 outerOffset = 0;

        for (; outer < 2; outer++) {
            s32 currentOuterOffset = (middle = 0, outerOffset);
            u8 *sectorDst = outerDst + 0xF5C;
            s32 middleOffset = 0;

            for (; middle < 4; middle++) {
                s32 *sectorOut = (inner = 0, (s32 *)sectorDst);
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
    }

    {
        /* These two accumulator hints are load-bearing. */
        register u32 count asm("$13") = 0;
        register u32 checksum asm("$6") = 0;
        s32 bgmVolume = g_BgmVolumeSetting;
        s32 sfxVolume = g_SfxVolumeSetting;
        s32 monoOutput = g_MonoOutput;
        u16 *checksumSrc = (u16 *)block;

        ((GameSaveBlock *)block)->bgmVolume = bgmVolume;
        ((GameSaveBlock *)block)->sfxVolume = sfxVolume;
        ((GameSaveBlock *)block)->monoOutput = monoOutput;
        memcpy(((GameSaveBlock *)block)->grandPrixCourseProgress, &g_GrandPrixCourseProgress, 8);
        memcpy(((GameSaveBlock *)block)->extraGrandPrixCourseProgress, &g_ExtraGrandPrixCourseProgress, 8);

        for (; count < 0x7FE; count++) {
            checksum += *checksumSrc++;
        }
        ((GameSaveBlock *)block)->checksum = ~checksum;
    }
}
