#include "common.h"
#include "game/race.h"
#include "game/menu.h"
#include "game/car.h"
#include "game/state.h"
#include "game/audio.h"
#include "game/sound.h"

void DrawProportionalText(s32 id, void *dst, void *src, s32 arg3);
void LibcSprintf(void *dst, void *fmt, s32 val);
extern s32 g_CourseProgress;
extern GameScoreRecord g_ClassRecords[];
extern GameScoreRecord g_ClassClears[];
s32 GetCarUnlockLevel(s32 model);
void ResetProgressSlot(s32 arg0, s32 arg1);
void ResetCourseProgress(s32 arg0);
void BeginEndingFmv(s32 arg0);
void BeginClassFmv(s32 arg0);

void DrawPrizeMoneyPanel(u8 *s0) {
    u8 sp[16];
    if (g_RaceProgress->money > 0x3B9AC9FF) {
        g_RaceProgress->money = 0x3B9AC9FF;
    }
    DrawProportionalText(0x10, s0 + 128, g_CaptionPrizeMoney, 0x7812);
    LibcSprintf(sp, g_FmtMoney, g_PrizeAmount);
    DrawProportionalText(0x12, s0 + 140, sp, 0x7812);
    DrawProportionalText(0x10, s0 + 160, g_CaptionTotalMoney, 0x7812);
    LibcSprintf(sp, g_FmtMoney, g_RaceProgress->money);
    DrawProportionalText(0x12, s0 + 172, sp, 0x7812);
    if (g_ClassPromoted != 0) {
        DrawProportionalText(0x10, s0 + 192, g_CaptionPromotionBonus, 0x7812);
        LibcSprintf(sp, g_FmtMoney, g_PromotionBonus);
        DrawProportionalText(0x12, s0 + 204, sp, 0x7812);
    }
}

void CommitClassProgress(void) {
    s32 score_index;
    u8 *slots;
    s32 slot_count;
    s32 filled;
    s32 i;
    s32 done;
    s32 value;
    s32 *state;

    slots = (u8 *)(g_CourseProgress + g_CourseIndex);
    g_ClassClearFanfareTimer = 0;

    if (*slots == 0 || g_RacePosition < *slots) {
        *slots = g_RacePosition;
    }

    value = GetCarUnlockLevel(g_PlayerCarIndex);
    slot_count = 4;
    if (g_GrandPrixClass < value) {
        *(s16 *)(g_CourseProgress + 4) = 1;
    }

    if (g_GrandPrixClass < 2) {
        slot_count = 3;
    }

    filled = 0;
    for (i = 0; i < slot_count; i++) {
        if (((u8 *)g_CourseProgress)[i] != 0) {
            filled++;
        }
    }

    done = ((slot_count ^ filled) == 0);
    g_ClassCompleted = done;

    if (done != 0) {
        s16 *record;

        score_index = (g_GrandPrixSeries * 6) + g_GrandPrixClass;

        if (score_index == 4) {
            record = &g_ClassRecord6;
            if (*record == -1) {
                *record = 0;
            }
        } else if (score_index == 10) {
            record = &g_ClassRecord5;
            if (*record == -1) {
                *record = 0;
            }
        } else {
            if (score_index != 5) {
                if (g_ClassRecords[score_index + 1].place == -1) {
                    g_ClassRecords[score_index + 1].place = 0;
                }
            }
        }
        value = ComputeClassGrade();
        g_ClassResultPlace = value;
        if (value != 0) {
            s32 offset;

            offset = score_index * 4;
            if (g_ClassRecords[score_index].place == 0 || value < g_ClassRecords[score_index].place) {
                g_ClassRecords[score_index].place = (u16)g_ClassResultPlace;
            }
            g_ClassClearFanfareTimer = 0xD2;
        }

        UpdateBgmTrackCount();
        if (g_ClassResultPlace == 1) {
            s32 offset;

            offset = score_index * 4;
            if (*(s16 *)((char *)g_ClassClears + offset) < 99) {
                (*(s16 *)((char *)g_ClassClears + offset))++;
            }
        }
    } else {
        g_ClassResultPlace = 0;
    }

    g_SeriesCleared = 0;
    if (g_ClassCompleted != 0) {
        if ((g_SeriesSelection == 0 && g_GrandPrixClass == 4) || (g_SeriesSelection == 1 && g_GrandPrixClass == 5)) {
            g_SeriesCleared = 1;
            g_AdvancedSeriesUnlocked = 1;
        }
    }

    g_ClassPromoted = 0;
    if (g_ClassCompleted != 0 && g_SeriesCleared == 0) {
        state = (s32 *)g_RaceProgress;
        if (state[3] < g_GrandPrixClass + 1) {
            g_ClassPromoted = 1;
        }
    }
}

void AdvanceGrandPrixClass(void) {
    s32 oldValue;
    GameRaceProgress *ptr;
    s32 *entry;

    if (g_ClassCompleted != 0) {
        if (g_SeriesCleared != 0) {
            s32 magic;
            GameRaceProgress *afterPtr;

            ptr = g_RaceProgress;
            oldValue = ptr->maxClassReached;
            ResetProgressSlot((s32)g_CarTable, (s32)ptr);
            magic = 0x3B9AC9FF;
            afterPtr = g_RaceProgress;
            afterPtr->money = magic;
            afterPtr->maxClassReached = oldValue;
            ResetCourseProgress(0);
            BeginEndingFmv(0x21);
        } else {
            s32 current;
            s32 next;
            GameRaceProgress *menuPtr;
            s32 enabled;

            BeginClassFmv(7);
            current = g_GrandPrixClass;
            menuPtr = g_RaceProgress;
            enabled = g_ClassPromoted;
            next = current + 1;
            g_GrandPrixClass = next;
            menuPtr->classIndex = next;
            menuPtr->course = 0;

            if (enabled != 0) {
                menuPtr->maxClassReached = next;
                entry = &g_MaxClassReached[g_SeriesSelection];
                if (*entry < next) {
                    *entry = next;
                }
            }

            ResetCourseProgress(g_GrandPrixClass);
        }
    } else {
        g_SceneId = 6;
    }
}

void EnterPrizeScreen(void) {
    s32 mode;
    s32 car;
    s32 value;

    g_SceneTimer = 0x100;
    g_FrameSyncThreshold = 0x80;

    mode = g_CourseIndex;
    car = g_GrandPrixClass;
    g_PrizeScreenState = 0;
    g_PrizeAmount = g_PrizeMoney[mode][car][g_RacePosition - 1];
    g_SceneId = 0x13;

    if (g_ClassPromoted != 0) {
        g_PromotionBonus = g_PromotionBonusTable[car];
    } else {
        g_PromotionBonus = 0;
    }

    value = g_PrizeMoney3rd[g_CourseIndex][g_GrandPrixClass][0] / 80;
    g_PrizeCountStep = value;
    if (value <= 0) {
        g_PrizeCountStep = 1;
    }

    value = g_PromotionBonusTable[g_GrandPrixClass] / 250;
    g_BonusCountStep = value;
    if (value <= 0) {
        g_BonusCountStep = 1;
    }
}

void TickClassClearFanfare(void) {
    if (g_ClassClearFanfareTimer > 0) {
        g_ClassClearFanfareTimer--;
    }
    if (g_ClassClearFanfareTimer == 0xB4) {
        PlaySoundCue(0x42);
    }
}
