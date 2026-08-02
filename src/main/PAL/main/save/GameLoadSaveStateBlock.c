#include "common.h"
#include "game/race.h"
#include "game/menu.h"

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

extern u8 g_TeamLogoRect[] asm("D_8007BEE4");
extern u8 g_TeamLogoClutRect[] asm("D_8007BEDC");

void func_80013F80(s32 a, s32 b);
void GameApplyAudioSettings(void) asm("func_80021224");
void LoadImage(void *rect, void *data) asm("func_80065B24");

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
s32 GameLoadSaveStateBlock(u8 *arg0) asm("func_8005FED4");
s32 GameLoadSaveStateBlock(u8 *arg0) {
    /* This pin is load-bearing: removing it changes .text. */
    register u8 *base asm("$17") = arg0;
    __asm__("" : "=r"(base) : "0"(base));
    {
        u32 sum;
        /* This pin is load-bearing: removing it changes .text. */
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
        GameDebugPrintf(g_MsgSaveChecksumOk);
        sum = ~sum;
        GameDebugPrintf(g_FmtSaveChecksum, *(s32 *)(base + 0xFFC), sum);
        if (*(s32 *)(base + 0xFFC) != sum) {
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
            g_GrandPrixSave.unk10 = *(s32 *)(base + 0x20);
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
        g_ExtraGrandPrixSave.unk10 = *(s32 *)(base + 0x34);
        g_TimeAttackSave.course = *(s32 *)(base + 0x38);
        g_TimeAttackSave.carIndex = *(s32 *)(base + 0x3C);
        g_TimeAttackSave.classIndex = *(s32 *)(base + 0x40);
        g_TimeAttackSave.maxClassReached = *(s32 *)(base + 0x44);
        g_TimeAttackSave.unk10 = *(s32 *)(base + 0x48);
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
        /* This pin is load-bearing: removing it changes .text. */
        register u8 *src asm("$6") = base;
        s32 offset = 0;
        for (; offset < 0x68; offset += 8) {
            g_GrandPrixCars[offset + 0] = src[0x58 + 0];
            g_GrandPrixCars[offset + 1] = src[0x58 + 1];
            g_GrandPrixCars[offset + 2] = src[0x58 + 2];
            g_GrandPrixCars[offset + 3] = src[0x58 + 3];
            g_GrandPrixCars[offset + 4] = src[0x58 + 4];
            g_GrandPrixCars[offset + 5] = src[0x58 + 5];
            g_ExtraGrandPrixCars[offset + 0] = src[0xC0 + 0];
            g_ExtraGrandPrixCars[offset + 1] = src[0xC0 + 1];
            g_ExtraGrandPrixCars[offset + 2] = src[0xC0 + 2];
            g_ExtraGrandPrixCars[offset + 3] = src[0xC0 + 3];
            g_ExtraGrandPrixCars[offset + 4] = src[0xC0 + 4];
            g_ExtraGrandPrixCars[offset + 5] = src[0xC0 + 5];
            g_TimeAttackCars[offset + 0] = src[0x128 + 0];
            g_TimeAttackCars[offset + 1] = src[0x128 + 1];
            g_TimeAttackCars[offset + 2] = src[0x128 + 2];
            g_TimeAttackCars[offset + 3] = src[0x128 + 3];
            g_TimeAttackCars[offset + 4] = src[0x128 + 4];
            g_TimeAttackCars[offset + 5] = src[0x128 + 5];
            src += 8;
        }
    }

    {
        /* This pin is load-bearing: removing it changes .text. */
        register u8 *src asm("$4") = base;
        s32 offset = 0;
        for (; offset < 0x2C; offset += 4) {
            *(u16 *)((u8 *)g_ClassRecords + offset) = *(u16 *)(src + 0x190);
            *(u16 *)((u8 *)g_ClassRecords + offset + 2) = *(u16 *)(src + 0x192);
            src += 4;
        }
    }

    {
        /* This pin is load-bearing: removing it changes .text. */
        register s32 count asm("$13") = 0;
        u16 *dst = g_TeamLogoClut;
        u8 *src = base;
        for (; count < 0x10; count++) {
            *dst++ = *(u16 *)(src + 0x1BC);
            src += 2;
        }
    }

    {
        /* This pin is load-bearing: removing it changes .text. */
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
        /* These pins are load-bearing: removing any one changes .text. */
        register s32 i asm("$13") = 0;
        register s32 j asm("$12");
        s32 off;
        for (; i < 2; i++) {
            j = 0;
            off = i << 5;
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
        /* These pins are load-bearing: removing any one changes .text. */
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
                /* These pins are load-bearing: removing any one changes .text. */
                register s32 dc asm("$2");
                register s32 *dst2 asm("$11");
                register s32 sb asm("$3");
                s32 s2;
                s32 *src2;
                s32 *dst1;
                /* This pin is load-bearing: removing it changes .text. */
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
        /* These pins are load-bearing: removing any one changes .text. */
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

    func_80013F80(g_PadMappingIndex, g_NegconMappingIndex);
    GameApplyAudioSettings();
    LoadImage((void *)g_TeamLogoRect, g_TeamLogoCanvas);
    LoadImage((void *)g_TeamLogoClutRect, g_TeamLogoClut);
    return 1;
}
