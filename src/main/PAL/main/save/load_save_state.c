#include "common.h"
#include "game/car.h"
#include "game/memcard.h"
#include "game/race.h"
#include "game/menu.h"
#include "psyq/gpu.h"
#define GAME_INPUT_MAPPING_TYPE s16
#include "game/input_internal.h"
#define GAME_SAVE_BGM_TYPE u16
#include "game/save_internal.h"

/* The loader stores a whole word here; the saver reads only the low half
 * as g_BgmSelection. Same address, two widths, so two names. */



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
                *dst++ = *(u16 *)(src + 0x1BC);
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
