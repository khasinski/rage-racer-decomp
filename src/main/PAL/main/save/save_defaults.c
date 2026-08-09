#include "common.h"
#include "game/audio.h"
#include "game/car.h"
#include "game/menu.h"
#include "game/race.h"
#include "game/state.h"
#include "game/save_internal.h"

void ResetProgressSlot(void *slot, s32 *progress) {
    CarEntry *dst;
    CarEntry *src;
    s32 i;

    i = 0;
    dst = slot;
    src = g_SaveDefaults;
    do {
        *dst = *src;
        dst++;
        i++;
    } while (src++, i < 13);

    progress[1] = 3;
    progress[0] = 0;
    progress[2] = 0;
    progress[3] = -1;
    progress[4] = 0;
}

void ResetCourseProgress(s32 mode) {
    u8 *ptr = g_CourseProgress->bestPlace;

    g_CourseProgress->retriesRemaining = 5;
    ptr[3] = 0;
    ptr[2] = 0;
    ptr[1] = 0;
    ptr[0] = 0;

    if (mode < 2) {
        g_CourseProgress->bestPlace[3] = 0xFF;
    }

    g_CourseProgress->unlockPending = 0;
}

void InitSaveDefaults(void) {
    CarEntry *src;
    CarEntry *dst;
    s32 i;
    s32 emptySlot;

    i = 0;
    dst = g_TimeAttackCars;
    src = g_SaveDefaults;
    do {
        __builtin_memcpy(dst, src, sizeof(*dst));
        dst++;
        i++;
        src++;
    } while (i < 13);

    g_ClassRecords[0].place = 0;
    g_ClassRecords[0].clears = 0;
    g_ClassWinCount = 0;

    emptySlot = -1;
    for (i = 1; i < 11; i++) {
        g_ClassRecords[i].place = emptySlot;
        g_ClassRecords[i].clears = 0;
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
