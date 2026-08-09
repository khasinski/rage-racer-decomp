#include "common.h"
#include "game/car.h"
#include "game/memcard.h"
#include "game/race.h"
#include "game/menu.h"
#include "psyq/gpu.h"
#include "game/input_internal.h"
#include "game/save_internal.h"

s32 LoadSaveStateBlock(u8 *block) {
    register u8 *base asm("$17") = block;
    s32 i;
    __asm__("" : "=r"(base) : "0"(base));
    {
        u32 sum;
        u16 *p;

        i = 0;
        __asm__("" : "=r"(i) : "0"(i));
        sum = i;
        p = (u16 *)base;
        do {
            sum += *p++;
            i++;
        } while ((u32)i < 0x7FE);
        printf(g_MsgSaveChecksumOk);
        sum = ~sum;
        printf(g_FmtSaveChecksum, ((GameSaveBlock *)base)->checksum, sum);
        if (((GameSaveBlock *)base)->checksum != sum) {
            return 0;
        }
    }

    {
        GameSaveBlockAddress padMappingAddress;
        GameSaveBlockAddress negconMappingAddress;
        GameSaveBlockAddress steerNeutralAddress;
        GameSaveBlockAddress steerPlayAddress;
        GameSaveBlockAddress neutralIIAddress;
        GameSaveBlockAddress neutralLAddress;
        u16 padMappingIndex;
        u16 negconMappingIndex;
        u16 negconSteerNeutral;
        u16 negconSteerPlay;

        padMappingAddress.halfwordPointer = &((GameSaveBlock *)base)->padMappingIndex;
        padMappingIndex = *padMappingAddress.halfwordPointer;
        negconMappingAddress.halfwordPointer = &((GameSaveBlock *)base)->negconMappingIndex;
        negconMappingIndex = *negconMappingAddress.halfwordPointer;
        steerNeutralAddress.halfwordPointer = &((GameSaveBlock *)base)->negconSteerNeutral;
        negconSteerNeutral = *steerNeutralAddress.halfwordPointer;
        steerPlayAddress.halfwordPointer = &((GameSaveBlock *)base)->negconSteerPlay;
        negconSteerPlay = *steerPlayAddress.halfwordPointer;
        g_NegconNeutralI = ((GameSaveBlock *)base)->negconNeutralI;
        neutralIIAddress.halfwordPointer = &((GameSaveBlock *)base)->negconNeutralII;
        g_NegconNeutralII = *neutralIIAddress.halfwordPointer;
        neutralLAddress.halfwordPointer = &((GameSaveBlock *)base)->negconNeutralL;
        g_NegconNeutralL = *neutralLAddress.halfwordPointer;
        {
            GameSaveBlockAddress maxTwistAddress;
            u16 hE;
            s32 extraMaxClass;

            maxTwistAddress.halfwordPointer = &((GameSaveBlock *)base)->negconMaxTwist;
            hE = *maxTwistAddress.halfwordPointer;
            g_GrandPrixSave.course = ((GameSaveBlock *)base)->grandPrixProgress.course;
            g_GrandPrixSave.carIndex = ((GameSaveBlock *)base)->grandPrixProgress.carIndex;
            g_GrandPrixSave.classIndex = ((GameSaveBlock *)base)->grandPrixProgress.classIndex;
            g_GrandPrixSave.maxClassReached = ((GameSaveBlock *)base)->grandPrixProgress.maxClassReached;
            g_GrandPrixSave.money = ((GameSaveBlock *)base)->grandPrixProgress.money;
            g_ExtraGrandPrixSave.course = ((GameSaveBlock *)base)->extraGrandPrixProgress.course;
            g_ExtraGrandPrixSave.carIndex = ((GameSaveBlock *)base)->extraGrandPrixProgress.carIndex;
            g_ExtraGrandPrixSave.classIndex = ((GameSaveBlock *)base)->extraGrandPrixProgress.classIndex;
            extraMaxClass = ((GameSaveBlock *)base)->extraGrandPrixProgress.maxClassReached;
            g_PadMappingIndex = padMappingIndex;
            g_NegconMappingIndex = negconMappingIndex;
            g_NegconSteerNeutral = negconSteerNeutral;
            g_NegconSteerPlay = negconSteerPlay;
            g_NegconMaxTwist = hE;
            g_ExtraGrandPrixSave.maxClassReached = extraMaxClass;
        }
        g_ExtraGrandPrixSave.money = ((GameSaveBlock *)base)->extraGrandPrixProgress.money;
        g_TimeAttackSave.course = ((GameSaveBlock *)base)->timeAttackProgress.course;
        g_TimeAttackSave.carIndex = ((GameSaveBlock *)base)->timeAttackProgress.carIndex;
        g_TimeAttackSave.classIndex = ((GameSaveBlock *)base)->timeAttackProgress.classIndex;
        g_TimeAttackSave.maxClassReached = ((GameSaveBlock *)base)->timeAttackProgress.maxClassReached;
        g_TimeAttackSave.money = ((GameSaveBlock *)base)->timeAttackProgress.money;
        {
            s32 bgmSelection = ((GameSaveBlock *)base)->bgmSelection;
            u16 advancedUnlocked = ((GameSaveBlock *)base)->advancedUnlocked;
            s32 maxClassReached1;
            g_MaxClassReached[0] = ((GameSaveBlock *)base)->maxClassReached[0];
            maxClassReached1 = ((GameSaveBlock *)base)->maxClassReached[1];
            g_BgmSelection = bgmSelection;
            g_AdvancedSeriesUnlocked = advancedUnlocked;
            g_MaxClassReached[1] = maxClassReached1;
        }
    }

    {
        register u8 *src asm("$6") = base;
        s32 i;
        for (i = 0; i < 13; i++) {
            SavedCarSetup *grandPrixCar =
                &((GameSaveBlock *)src)->carSetup[0][0];
            SavedCarSetup *extraGrandPrixCar =
                &((GameSaveBlock *)src)->carSetup[1][0];
            SavedCarSetup *timeAttackCar =
                &((GameSaveBlock *)src)->carSetup[2][0];

            g_GrandPrixCars[i].modelVariant = grandPrixCar->modelVariant;
            g_GrandPrixCars[i].tireCompound = grandPrixCar->tireCompound;
            g_GrandPrixCars[i].transmission = grandPrixCar->transmission;
            g_GrandPrixCars[i].paintColor1 = grandPrixCar->paintColor1;
            g_GrandPrixCars[i].paintColor2 = grandPrixCar->paintColor2;
            g_GrandPrixCars[i].enabled = grandPrixCar->enabled;
            g_ExtraGrandPrixCars[i].modelVariant = extraGrandPrixCar->modelVariant;
            g_ExtraGrandPrixCars[i].tireCompound = extraGrandPrixCar->tireCompound;
            g_ExtraGrandPrixCars[i].transmission = extraGrandPrixCar->transmission;
            g_ExtraGrandPrixCars[i].paintColor1 = extraGrandPrixCar->paintColor1;
            g_ExtraGrandPrixCars[i].paintColor2 = extraGrandPrixCar->paintColor2;
            g_ExtraGrandPrixCars[i].enabled = extraGrandPrixCar->enabled;
            g_TimeAttackCars[i].modelVariant = timeAttackCar->modelVariant;
            g_TimeAttackCars[i].tireCompound = timeAttackCar->tireCompound;
            g_TimeAttackCars[i].transmission = timeAttackCar->transmission;
            g_TimeAttackCars[i].paintColor1 = timeAttackCar->paintColor1;
            g_TimeAttackCars[i].paintColor2 = timeAttackCar->paintColor2;
            g_TimeAttackCars[i].enabled = timeAttackCar->enabled;
            src += sizeof(SavedCarSetup);
        }
    }

    {
        register u8 *src asm("$4") = base;
        s32 index = 0;
        for (; index < 11; index++) {
            SavedClassRecord *saved =
                &((GameSaveBlock *)src)->classRecords[0];

            g_ClassRecords[index].place = saved->grade;
            g_ClassRecords[index].clears = saved->clears;
            src += sizeof(SavedClassRecord);
        }
    }

    {
        s32 j;
        register s32 k asm("$7");

        {
            u16 *dst;
            u8 *src;

            i = 0;
            dst = g_TeamLogoClut;
            src = base;
            for (; i < 0x10; i++) {
                *dst++ = ((GameSaveBlock *)src)->teamLogoClut[0];
                src += 2;
            }
        }

        {
            u16 *dst;
            u8 *src;

            i = 0;
            dst = g_TeamLogoCanvas.halfwords;
            src = base;
            for (; i < 0x400; i++) {
                *dst++ = ((GameSaveBlock *)src)->teamLogoCanvas[0];
                src += 2;
            }
        }

        /* g_BestLapTimes / g_BestTotalTimes */
        {
            i = 0;
            for (; i < 2; i++) {
                j = 0;
                for (; j < 4; j++) {
                    g_BestLapTimes[i][j][0] =
                        ((GameSaveBlock *)base)->bestLapTimes[i][j][0];
                    g_BestTotalTimes[i][j][0] =
                        ((GameSaveBlock *)base)->bestTotalTimes[i][j][0];
                }
            }
        }

        /* g_RankingRecords / g_TimeRecords */
        {
            s32 *cb78;
            s32 *d1base;
            s32 ioff;
            i = 0;
            cb78 = (s32 *)g_TimeRecords;
            d1base = (s32 *)g_RankingRecords;
            ioff = 0;
            for (; i < 2; i++) {
                register s32 iofc asm("$15");
                s32 *d1;
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
                    d1 += sizeof(g_RankingRecords[0][0]) / sizeof(*d1);
                    joff += 0x50;
                }
                d1base += sizeof(g_RankingRecords[0]) / sizeof(*d1base);
                ioff += 0x140;
            }
        }

        /* g_BestSectorTimes */
        {
            register s32 *e41e8 asm("$10");
            s32 ioff;
            i = 0;
            e41e8 = &g_BestSectorTimes[0][0][0];
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
                    SectorTimeTableAddress sourceAddress;
                    s32 *src;
                    k = 0;
                    dst = dbase;
                    sourceAddress.pointer = (s32 *)base;
                    sourceAddress.byteOffset =
                        iofc + sourceAddress.byteOffset + 0xF5C;
                    sourceAddress.byteOffset = joff + sourceAddress.byteOffset;
                    src = sourceAddress.pointer;
                    for (; k < 3; k++) {
                        *dst++ = *src++;
                    }
                    dbase += sizeof(g_BestSectorTimes[0][0]) / sizeof(*dbase);
                    joff += 0xC;
                }
                e41e8 += sizeof(g_BestSectorTimes[0]) / sizeof(*e41e8);
                ioff += 0x30;
            }
        }
    }

    /* g_BgmVolumeSetting / g_SfxVolumeSetting / g_MonoOutput clamps */
    {
        s32 v = ((GameSaveBlock *)base)->bgmVolume;
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
        v = ((GameSaveBlock *)base)->sfxVolume;
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
        v = ((GameSaveBlock *)base)->monoOutput;
        g_SfxVolumeSetting = c;
        g_MonoOutput = v;
        if (v != 0) {
            g_MonoOutput = 1;
        }
    }

    /* g_GrandPrixCourseProgress / g_ExtraGrandPrixCourseProgress unaligned copies */
    memcpy(&g_GrandPrixCourseProgress, ((GameSaveBlock *)base)->grandPrixCourseProgress, 8);
    memcpy(&g_ExtraGrandPrixCourseProgress, ((GameSaveBlock *)base)->extraGrandPrixCourseProgress, 8);

    LoadPadButtonMapping(g_PadMappingIndex, g_NegconMappingIndex);
    ApplyAudioSettings();
    LoadImage(&g_TeamLogoRect, &g_TeamLogoCanvas);
    LoadImage(&g_TeamLogoClutRect, g_TeamLogoClut);
    return 1;
}
