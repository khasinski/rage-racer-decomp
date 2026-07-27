#include "common.h"
#include "game/track.h"
#include "game/race.h"


extern u32 g_CameraCar[] asm("D_801E3E14");
extern s32 g_CameraCarY asm("D_801E3E18");
extern s32 g_CameraCarZ asm("D_801E3E1C");
extern s32 g_CameraCarAngleY asm("D_801E3E38");
extern s32 g_CameraCarHeading asm("D_801E3EB4");
extern s32 g_CameraCarSpeed asm("D_801E3EB8");
extern s32 D_801E3F60;

typedef struct CopyBlock8003CDF4 {
    u32 word0;
    u32 word1;
    u32 word2;
    u32 word3;
} CopyBlock8003CDF4;

void GameSeedFinishCameraAlt(void *arg0) asm("func_8003CDF4");
void GameSeedFinishCameraAlt(void *arg0) {
    register u32 word0 asm("$2");
    register u32 word1 asm("$3");
    register u32 word2 asm("$4");
    register CopyBlock8003CDF4 *src asm("$6");
    register CopyBlock8003CDF4 *dst asm("$7");
    register CopyBlock8003CDF4 *end asm("$9");
    register u32 *base asm("$8");
    register GameTrackPoint *track asm("$5");
    register GameTrackPoint *point asm("$2");
    register s32 index asm("$3");
    register s32 lastIndex asm("$4");
    register s32 course asm("$3");

    base = arg0;
    asm("" : "=r"(base) : "0"(base));
    dst = (CopyBlock8003CDF4 *)g_CameraCar;
    src = (CopyBlock8003CDF4 *)base;
    end = (CopyBlock8003CDF4 *)((s32)base + 0x190);
    do {
        *dst = *src;
        src++;
        dst++;
    } while (src != end);

    word0 = ((u32 *)src)[0];
    word1 = ((u32 *)src)[1];
    word2 = ((u32 *)src)[2];
    ((u32 *)dst)[0] = word0;
    ((u32 *)dst)[1] = word1;
    ((u32 *)dst)[2] = word2;

    index = *(s32 *)((s32)base + 0x30);
    track = g_TrackPoints;
    point = (GameTrackPoint *)((index * 3) << 3);
    point = (GameTrackPoint *)((s32)point + (s32)track);
    g_CameraCar[0] = point->x;

    index = *(s32 *)((s32)base + 0x30);
    point = (GameTrackPoint *)((index * 3) << 3);
    point = (GameTrackPoint *)((s32)point + (s32)track);
    g_CameraCarZ = point->z;

    index = *(s32 *)((s32)base + 0x30);
    point = (GameTrackPoint *)((index * 3) << 3);
    point = (GameTrackPoint *)((s32)point + (s32)track);
    word0 = point->y;
    course = g_GrandPrixSeries;
    asm volatile("" : "=r"(word0), "=r"(course) : "0"(word0), "1"(course) : "memory");
    g_CameraCarSpeed = 0;
    asm volatile("" ::: "memory");
    g_CameraCarY = word0 - 0x30;

    lastIndex = *(s32 *)((s32)base + 0x30);
    course <<= 11;
    point = (GameTrackPoint *)((lastIndex * 3) << 3);
    point = (GameTrackPoint *)((s32)point + (s32)track);
    course += 0xC00;
    course -= point->angle;
    g_CameraCarHeading = course;
    D_801E3F60 = course;
    g_CameraCarAngleY = course;
}
