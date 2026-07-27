#include "common.h"
#include "game/state.h"
#include "game/menu.h"
#include "game/race.h"

typedef struct {
    u8 raw[8];
} UnkCopyChunk;

extern s32 g_PrizeScreenState asm("D_8019CB74");
extern s32 g_PrizeTickRate asm("D_801E6DA0");
extern s32 g_BonusTickRate asm("D_801E6C78");
extern s32 g_PendingPrizeMoney asm("D_801F17B0");
extern s32 g_PendingClassBonus asm("D_8019CE0C");
extern s32 D_801E4D0C;
extern s32 D_801E4B94;
extern s32 g_SeriesCleared asm("D_8019C8EC");
void GameDrawFullscreenFadeTile(s32, s32) asm("func_80033AA0");
void func_800204F4(s32);
void func_800206B8(s32);
void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");
void func_80020D90(void);
void GameRequestSelectBgmAssets(void) asm("func_80018410");
void func_80020B08(void);
void func_800201D4(void);
extern s32 g_BgmVolumeSetting asm("D_8019C704");
extern s32 g_SfxVolumeSetting asm("D_801E8A50");
extern s32 g_MonoOutput asm("D_801E6C70");
void func_8005BD84(s32 arg0);
void GameSetEffectVolumeSetting(s32 arg0) asm("func_8005BDD4");
void func_8005BE24(void);
void func_8005BE58(void);
extern UnkCopyChunk D_8007BE68[];
extern u8 g_TimeAttackCars asm("D_801E4388");
extern s16 g_ClassRecords asm("D_8019CB40");
extern s16 D_8019CB42;
extern s32 D_801E4DA8;
extern s32 D_8019C984;
extern s32 D_8019C988;
extern s32 D_8019C98C;
extern s32 D_8019C990;
extern u8 g_GrandPrixCars asm("D_801E4F44");
extern u8 g_ExtraGrandPrixCars asm("D_8019C914");
extern u8 g_ExtraGrandPrixCourseProgress asm("D_8009E874");
extern u8 g_GrandPrixCourseProgress asm("D_801E42EC");
extern u8 *g_CourseProgress asm("D_8009E67C");
extern s32 g_BgmTrackCount asm("D_801E40A8");
extern s32 g_BgmSelection asm("D_801E42CC");
void GameApplyAudioSettings(void) asm("func_80021224");
void GameResetProgressSlot(void *arg0, s32 *arg1) asm("func_80021288");
void GameResetCourseProgress(s32 arg0) asm("func_800212F0");
void func_8001B488(void);

/* Scene 19: counts the prize money and then the class-clear bonus into the save block. */
void GameUpdatePrizeMoneyScreen(void) asm("func_80020DDC");
void GameUpdatePrizeMoneyScreen(void) {
    s32 lim1 = g_PrizeTickRate;
    s32 lim0 = g_BonusTickRate;
    s32 st;
    s32 t;

    if (g_PadHeld & 0x860) {
        lim1 <<= 2;
        lim0 <<= 2;
    }

    switch (g_PrizeScreenState) {
    case 0:
        g_SceneTimer -= 8;
        GameDrawFullscreenFadeTile(g_SceneTimer, 0x49);
        if (g_SceneTimer == 0) g_PrizeScreenState = 1;
        func_800204F4(0);
        goto L428;
    case 1:
        func_800204F4(0);
        if (g_PadEdge2 & 0x860) {
            g_PrizeScreenState = 2;
            g_SceneTimer = 0;
        }
        goto L428;
    case 2:
        g_SceneTimer += 8;
        func_800204F4(g_SceneTimer);
        if ((u32)g_SceneTimer >= 129) g_PrizeScreenState = 3;
        goto L428;
    case 3:
        g_SceneTimer -= 8;
        func_800206B8(g_SceneTimer);
        if (g_SceneTimer == 0) g_PrizeScreenState = 4;
        goto L428;
    case 4:
        g_SceneTimer += 1;
        if ((u32)g_SceneTimer < 121) goto L234;
        if (g_PendingPrizeMoney == 0) goto L248;
        GamePlaySoundCue((g_PadHeld & 0x860) ? 0x10 : 0xf);
        t = g_PendingPrizeMoney;
        if (t >= lim1) {
            g_PendingPrizeMoney = t - lim1;
            g_RaceProgress->unk10 += lim1;
        } else {
            s32 e = g_RaceProgress->unk10;
            g_PendingPrizeMoney = 0;
            g_RaceProgress->unk10 = e + t;
        }
    L234:
        if (g_PendingPrizeMoney != 0) goto L420;
    L248:
        g_SceneTimer = 0;
        if (g_PendingClassBonus == 0) goto Lstore7;
        st = 5;
        goto Lstore;
    case 5:
        GamePlaySoundCue(0x11);
        if (!(g_PadEdge2 & 0x860)) goto L420;
        st = 6;
    Lstore:
        g_PrizeScreenState = st;
        goto L420;
    case 6:
        func_80020D90();
        if (g_PendingClassBonus == 0) { st = 7; goto Lstore; }
        GamePlaySoundCue((g_PadHeld & 0x860) ? 0x10 : 0xf);
        t = g_PendingClassBonus;
        if (t >= lim0) {
            g_PendingClassBonus = t - lim0;
            g_RaceProgress->unk10 += lim0;
        } else {
            s32 e = g_RaceProgress->unk10;
            g_PendingClassBonus = 0;
            g_RaceProgress->unk10 = e + t;
        }
        if (g_PendingClassBonus != 0) goto L420;
    Lstore7:
        st = 7;
        goto Lstore;
    case 7:
        func_80020D90();
        GamePlaySoundCue(0x11);
        if (!(g_PadEdge2 & 0x860)) goto L420;
        if (D_801E4D0C != 0) goto L420;
        if (D_801E4B94 != 0) { st = 8; goto Lstore; }
        GameRequestSelectBgmAssets();
        st = 8;
        goto Lstore;
    case 8:
        if (g_SeriesCleared != 0)
            g_SceneTimer += 1;
        else
            g_SceneTimer += 2;
        GameDrawFullscreenFadeTile(g_SceneTimer, 0x49);
        if ((u32)g_SceneTimer < 0x100) goto L420;
        break;
    default:
        goto L428;
    }
    func_80020B08();
L420:
    func_800206B8(0);
L428:
    func_800201D4();
}

void GameApplyAudioSettings(void) asm("func_80021224");
void GameApplyAudioSettings(void) {
    func_8005BD84(g_BgmVolumeSetting);
    GameSetEffectVolumeSetting(g_SfxVolumeSetting);
    if (g_MonoOutput == 0) {
        func_8005BE24();
    } else {
        func_8005BE58();
    }
}

void GameResetProgressSlot(void *arg0, s32 *arg1) asm("func_80021288");
void GameResetProgressSlot(void *arg0, s32 *arg1) {
    UnkCopyChunk *dst;
    UnkCopyChunk *src;
    s32 i;

    i = 0;
    dst = arg0;
    src = D_8007BE68;
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

void GameResetCourseProgress(s32 arg0) asm("func_800212F0");
void GameResetCourseProgress(s32 arg0) {
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

void GameInitSaveDefaults(void) asm("func_80021338");
void GameInitSaveDefaults(void) {
    u8 *src;
    u8 *dst;
    s32 i;
    s32 offset;
    s32 emptySlot;

    i = 0;
    dst = &g_TimeAttackCars;
    src = &D_8007BE68;
    do {
        __builtin_memcpy(dst, src, sizeof(UnkCopyChunk));
        dst += sizeof(UnkCopyChunk);
        i++;
        src += sizeof(UnkCopyChunk);
    } while (i < 13);

    g_ClassRecords = 0;
    D_8019CB42 = 0;
    D_801E4DA8 = 0;

    emptySlot = -1;
    for (offset = 4; offset < 0x2C; offset += 4) {
        *(s16 *)((u8 *)&g_ClassRecords + offset) = emptySlot;
        *(s16 *)((u8 *)&D_8019CB42 + offset) = 0;
    }

    g_TimeAttackSave = 0;
    D_8019C984 = 3;
    D_8019C988 = 0;
    D_8019C98C = 0;
    D_8019C990 = 0;
    GameResetProgressSlot(&g_GrandPrixCars, &g_GrandPrixSave);
    GameResetProgressSlot(&g_ExtraGrandPrixCars, &g_ExtraGrandPrixSave);

    g_CourseProgress = &g_ExtraGrandPrixCourseProgress;
    GameResetCourseProgress(0);
    g_CourseProgress = &g_GrandPrixCourseProgress;
    GameResetCourseProgress(0);

    g_MaxClassReached[1] = 0;
    g_MaxClassReached[0] = 0;
    g_BgmTrackCount = 9;
    g_BgmSelection = 0;
    func_8001B488();
    g_BgmVolumeSetting = 0xF;
    g_SfxVolumeSetting = 0xF;
    g_MonoOutput = 0;
    GameApplyAudioSettings();
}

/* Derives the 0..3 class grade from a course-progress block; 0 when an over-class car was used. */
s32 GameComputeClassGrade(void) asm("func_800214B8");
s32 GameComputeClassGrade(void) {
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
