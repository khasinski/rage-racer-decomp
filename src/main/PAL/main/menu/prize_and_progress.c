#include "common.h"
#include "game/state.h"
#include "game/menu.h"
#include "game/race.h"
void SetMonoOutput(void);
void SetStereoOutput(void);

typedef struct UnkCopyChunk {
    u8 raw[8];
} UnkCopyChunk;

extern s32 g_PrizeScreenState asm("D_8019CB74");
extern s32 g_PrizeCountStep asm("D_801E6DA0");
extern s32 g_BonusCountStep asm("D_801E6C78");
extern s32 g_PrizeAmount asm("D_801F17B0");
extern s32 g_PromotionBonus asm("D_8019CE0C");
extern s32 g_ClassClearFanfareTimer asm("D_801E4D0C");
extern s32 g_ClassCompleted asm("D_801E4B94");
extern s32 g_SeriesCleared asm("D_8019C8EC");
void DrawFullscreenFadeTile(s32, s32) asm("func_80033AA0");
void DrawRaceTimePanel(s32);
void DrawPrizeMoneyPanel(s32);
void PlaySoundCue(s32 cue) asm("func_8005D6EC");
void TickClassClearFanfare(void);
void RequestSelectBgmAssets(void);
void AdvanceGrandPrixClass(void);
void func_800201D4(void);
extern s32 g_BgmVolumeSetting asm("D_8019C704");
extern s32 g_SfxVolumeSetting asm("D_801E8A50");
extern s32 g_MonoOutput asm("D_801E6C70");
void SetSequenceVolumeSetting(s32 arg0) asm("func_8005BD84");
void SetEffectVolumeSetting(s32 arg0);
extern UnkCopyChunk g_SaveDefaults[] asm("D_8007BE68");
extern u8 g_TimeAttackCars asm("D_801E4388");
extern s16 g_ClassRecords asm("D_8019CB40");
extern s16 g_ClassClears asm("D_8019CB42");
extern s32 g_ClassWinCount asm("D_801E4DA8");
extern u8 g_GrandPrixCars asm("D_801E4F44");
extern u8 g_ExtraGrandPrixCars asm("D_8019C914");
extern u8 g_ExtraGrandPrixCourseProgress asm("D_8009E874");
extern u8 g_GrandPrixCourseProgress asm("D_801E42EC");
extern u8 *g_CourseProgress asm("D_8009E67C");
extern s32 g_BgmTrackCount asm("D_801E40A8");
extern s32 g_BgmSelection asm("D_801E42CC");
void ApplyAudioSettings(void);
void ResetProgressSlot(void *arg0, s32 *arg1);
void ResetCourseProgress(s32 arg0);
void ShuffleBgmOrder(void) asm("func_8001B488");

/* Scene 19: counts the prize money and then the class-clear bonus into the save block. */
void UpdatePrizeMoneyScreen(void) asm("func_80020DDC");
void UpdatePrizeMoneyScreen(void) {
    s32 lim1 = g_PrizeCountStep;
    s32 lim0 = g_BonusCountStep;
    s32 st;
    s32 t;

    if (g_PadHeld & 0x860) {
        lim1 <<= 2;
        lim0 <<= 2;
    }

    switch (g_PrizeScreenState) {
    case 0:
        g_SceneTimer -= 8;
        DrawFullscreenFadeTile(g_SceneTimer, 0x49);
        if (g_SceneTimer == 0) g_PrizeScreenState = 1;
        DrawRaceTimePanel(0);
        func_800201D4();
        return;
    case 1:
        DrawRaceTimePanel(0);
        if (g_PadEdge2 & 0x860) {
            g_PrizeScreenState = 2;
            g_SceneTimer = 0;
        }
        func_800201D4();
        return;
    case 2:
        g_SceneTimer += 8;
        DrawRaceTimePanel(g_SceneTimer);
        if ((u32)g_SceneTimer >= 129) g_PrizeScreenState = 3;
        func_800201D4();
        return;
    case 3:
        g_SceneTimer -= 8;
        DrawPrizeMoneyPanel(g_SceneTimer);
        if (g_SceneTimer == 0) g_PrizeScreenState = 4;
        func_800201D4();
        return;
    case 4:
        g_SceneTimer += 1;
        if (!((u32)g_SceneTimer < 121)) {
        if (g_PrizeAmount == 0) {
        g_SceneTimer = 0;
        if (g_PromotionBonus == 0) goto Lstore7;
        st = 5;
        goto Lstore;
        }
        PlaySoundCue((g_PadHeld & 0x860) ? 0x10 : 0xf);
        t = g_PrizeAmount;
        if (t >= lim1) {
            g_PrizeAmount = t - lim1;
            g_RaceProgress->money += lim1;
        } else {
            s32 e = g_RaceProgress->money;
            g_PrizeAmount = 0;
            g_RaceProgress->money = e + t;
        }
        }
        if (g_PrizeAmount != 0) break;
        g_SceneTimer = 0;
        if (g_PromotionBonus == 0) goto Lstore7;
        st = 5;
        goto Lstore;
    case 5:
        PlaySoundCue(0x11);
        if (!(g_PadEdge2 & 0x860)) break;
        st = 6;
    Lstore:
        g_PrizeScreenState = st;
        break;
    case 6:
        TickClassClearFanfare();
        if (g_PromotionBonus == 0) { st = 7; goto Lstore; }
        PlaySoundCue((g_PadHeld & 0x860) ? 0x10 : 0xf);
        t = g_PromotionBonus;
        if (t >= lim0) {
            g_PromotionBonus = t - lim0;
            g_RaceProgress->money += lim0;
        } else {
            s32 e = g_RaceProgress->money;
            g_PromotionBonus = 0;
            g_RaceProgress->money = e + t;
        }
        if (g_PromotionBonus != 0) break;
    Lstore7:
        st = 7;
        goto Lstore;
    case 7:
        TickClassClearFanfare();
        PlaySoundCue(0x11);
        if (!(g_PadEdge2 & 0x860)) break;
        if (g_ClassClearFanfareTimer != 0) break;
        if (g_ClassCompleted == 0) { RequestSelectBgmAssets(); }
        st = 8;
        goto Lstore;
    case 8:
        if (g_SeriesCleared != 0)
            g_SceneTimer += 1;
        else
            g_SceneTimer += 2;
        DrawFullscreenFadeTile(g_SceneTimer, 0x49);
        if ((u32)g_SceneTimer < 0x100) break;
        AdvanceGrandPrixClass();
        break;
    default:
        func_800201D4();
        return;
    }
    DrawPrizeMoneyPanel(0);
    func_800201D4();
}

void ApplyAudioSettings(void);
void ApplyAudioSettings(void) {
    SetSequenceVolumeSetting(g_BgmVolumeSetting);
    SetEffectVolumeSetting(g_SfxVolumeSetting);
    if (g_MonoOutput == 0) {
        SetStereoOutput();
    } else {
        SetMonoOutput();
    }
}

void ResetProgressSlot(void *arg0, s32 *arg1);
void ResetProgressSlot(void *arg0, s32 *arg1) {
    UnkCopyChunk *dst;
    UnkCopyChunk *src;
    s32 i;

    i = 0;
    dst = arg0;
    src = g_SaveDefaults;
    do {
        *dst = *src;
        dst++;
        i++;
    } while (src++, i < 13);

    arg1[1] = 3;
    arg1[0] = 0;
    arg1[2] = 0;
    arg1[3] = -1;
    arg1[4] = 0;
}

void ResetCourseProgress(s32 arg0);
void ResetCourseProgress(s32 arg0) {
    u8 *ptr = g_CourseProgress;

    *(s16 *)(ptr + 6) = 5;
    ptr[3] = 0;
    ptr[2] = 0;
    ptr[1] = 0;
    ptr[0] = 0;

    if (arg0 < 2) {
        g_CourseProgress[3] = 0xFF;
    }

    *(s16 *)(g_CourseProgress + 4) = 0;
}

void InitSaveDefaults(void);
void InitSaveDefaults(void) {
    u8 *src;
    u8 *dst;
    s32 i;
    s32 offset;
    s32 emptySlot;

    i = 0;
    dst = &g_TimeAttackCars;
    src = &g_SaveDefaults;
    do {
        __builtin_memcpy(dst, src, sizeof(UnkCopyChunk));
        dst += sizeof(UnkCopyChunk);
        i++;
        src += sizeof(UnkCopyChunk);
    } while (i < 13);

    g_ClassRecords = 0;
    g_ClassClears = 0;
    g_ClassWinCount = 0;

    emptySlot = -1;
    for (offset = 4; offset < 0x2C; offset += 4) {
        *(s16 *)((u8 *)&g_ClassRecords + offset) = emptySlot;
        *(s16 *)((u8 *)&g_ClassClears + offset) = 0;
    }

    g_TimeAttackSave.course = 0;
    g_TimeAttackSave.carIndex = 3;
    g_TimeAttackSave.classIndex = 0;
    g_TimeAttackSave.maxClassReached = 0;
    g_TimeAttackSave.money = 0;
    ResetProgressSlot(&g_GrandPrixCars, (s32 *)&g_GrandPrixSave);
    ResetProgressSlot(&g_ExtraGrandPrixCars, (s32 *)&g_ExtraGrandPrixSave);

    g_CourseProgress = &g_ExtraGrandPrixCourseProgress;
    ResetCourseProgress(0);
    g_CourseProgress = &g_GrandPrixCourseProgress;
    ResetCourseProgress(0);

    g_MaxClassReached[1] = 0;
    g_MaxClassReached[0] = 0;
    g_BgmTrackCount = 9;
    g_BgmSelection = 0;
    ShuffleBgmOrder();
    g_BgmVolumeSetting = 0xF;
    g_SfxVolumeSetting = 0xF;
    g_MonoOutput = 0;
    ApplyAudioSettings();
}

/* Derives the 0..3 class grade from a course-progress block; 0 when an over-class car was used. */
s32 ComputeClassGrade(void);
s32 ComputeClassGrade(void) {
    u8 *ptr;
    s32 value;
    u8 *end;
    u8 extra;

    ptr = g_CourseProgress;
    value = 0;
    if (*(s16 *)(ptr + 4) != 0) {
        return 0;
    }

    end = ptr + 3;
    do {
        value += *ptr++;
    } while ((s32)ptr < (s32)end);

    extra = g_CourseProgress[3];
    if (extra == 0xFF) {
        value++;
    } else {
        value += extra;
    }

    value -= 3;
    if (value >= 4) {
        value = 0;
    }
    return value;
}
