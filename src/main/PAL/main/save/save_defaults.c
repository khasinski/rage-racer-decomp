#include "common.h"
#include "game/audio.h"
#include "game/car.h"
#include "game/menu.h"
#include "game/race.h"
#include "game/state.h"

extern CarEntry g_SaveDefaults[];
extern s16 g_ClassRecords;
extern s16 g_ClassClears;
extern s32 g_ClassWinCount;
extern u8 g_ExtraGrandPrixCourseProgress;
extern u8 g_GrandPrixCourseProgress;
extern u8 *g_CourseProgress;
extern s32 g_BgmSelection;

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
    u8 *ptr = g_CourseProgress;

    *(s16 *)(ptr + 6) = 5;
    ptr[3] = 0;
    ptr[2] = 0;
    ptr[1] = 0;
    ptr[0] = 0;

    if (mode < 2) {
        g_CourseProgress[3] = 0xFF;
    }

    *(s16 *)(g_CourseProgress + 4) = 0;
}

void InitSaveDefaults(void) {
    u8 *src;
    u8 *dst;
    s32 i;
    s32 offset;
    s32 emptySlot;

    i = 0;
    dst = (u8 *)g_TimeAttackCars;
    src = (u8 *)g_SaveDefaults;
    do {
        __builtin_memcpy(dst, src, sizeof(CarEntry));
        dst += sizeof(CarEntry);
        i++;
        src += sizeof(CarEntry);
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
