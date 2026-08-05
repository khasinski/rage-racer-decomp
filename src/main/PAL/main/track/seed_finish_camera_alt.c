#include "common.h"
#include "game/vector.h"
#include "game/track.h"
#include "game/race.h"
#include "game/car.h"

extern u32 g_CameraCar[];

void SeedFinishCameraAlt(void *arg0) {
    register u32 word0 asm("$2");
    u32 word1;
    u32 word2;
    Block16 *src;
    Block16 *dst;
    Block16 *end;
    u32 *base;
    GameTrackPoint *track;
    GameTrackPoint *point;
    register s32 index asm("$3");
    s32 lastIndex;
    register s32 course asm("$3");

    /* arg0 is a car runtime block: the copy below moves 0x19C bytes of it into
     * g_CameraCar, whose x/y/z and +0x24/+0xA0/+0xA4 externs line up with
     * GameCarRuntime. RAW() is required on the index reads -- as a plain member
     * access they stop aliasing the unqualified g_CameraCar* stores between
     * them and one of the four reloads is folded away; see common.h. */
    base = arg0;
    asm("" : "=r"(base) : "0"(base));
    dst = (Block16 *)g_CameraCar;
    src = (Block16 *)base;
    end = (Block16 *)((u8 *)base + 0x190);
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

    index = RAW(((GameCarRuntime *)base)->trackPointIndex);
    track = g_TrackPoints;
    point = (GameTrackPoint *)((index * 3) << 3);
    point = (GameTrackPoint *)((u8 *)point + (s32)track);
    g_CameraCar[0] = point->x;

    index = RAW(((GameCarRuntime *)base)->trackPointIndex);
    point = (GameTrackPoint *)((index * 3) << 3);
    point = (GameTrackPoint *)((u8 *)point + (s32)track);
    g_CameraCarZ = point->z;

    index = RAW(((GameCarRuntime *)base)->trackPointIndex);
    point = (GameTrackPoint *)((index * 3) << 3);
    point = (GameTrackPoint *)((u8 *)point + (s32)track);
    word0 = point->y;
    course = g_GrandPrixSeries;
    asm volatile("" ::: "memory");
    g_CameraCarSpeed = 0;
    g_CameraCarY = word0 - 0x30;

    lastIndex = RAW(((GameCarRuntime *)base)->trackPointIndex);
    course <<= 11;
    point = (GameTrackPoint *)((lastIndex * 3) << 3);
    point = (GameTrackPoint *)((u8 *)point + (s32)track);
    course += 0xC00;
    course -= point->angle;
    g_CameraCarHeading = course;
    D_801E3F60 = course;
    g_CameraCarAngleY = course;
}
