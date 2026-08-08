#include "common.h"
#include "game/vector.h"
#include "game/track.h"
#include "game/race.h"
#include "game/car.h"
#include "game/render.h"

extern GameRenderObject g_CameraCar;

void SeedFinishCameraAlt(void *car) {
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

    /* car is a car runtime block: the copy below moves 0x19C bytes of it into
     * g_CameraCar, which is a GameRenderObject -- every one of the eleven
     * g_CameraCar* split symbols lands on one of its fields. Storing through
     * the object is what lets the index reads below stay plain: both sides
     * carry the aggregate mark now, so 44a's exemption never fires. */
    base = car;
    asm("" : "=r"(base) : "0"(base));
    dst = (Block16 *)&g_CameraCar;
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

    index = ((GameCarRuntime *)base)->trackPointIndex;
    track = g_TrackPoints;
    point = (GameTrackPoint *)((index * 3) << 3);
    point = (GameTrackPoint *)((u8 *)point + (s32)track);
    g_CameraCar.x = point->x;

    index = ((GameCarRuntime *)base)->trackPointIndex;
    point = (GameTrackPoint *)((index * 3) << 3);
    point = (GameTrackPoint *)((u8 *)point + (s32)track);
    g_CameraCar.z = point->z;

    index = ((GameCarRuntime *)base)->trackPointIndex;
    point = (GameTrackPoint *)((index * 3) << 3);
    point = (GameTrackPoint *)((u8 *)point + (s32)track);
    word0 = point->y;
    index = g_GrandPrixSeries;
    g_CameraCar.speed = 0;
    g_CameraCar.y = word0 - 0x30;

    lastIndex = ((GameCarRuntime *)base)->trackPointIndex;
    index <<= 11;
    point = (GameTrackPoint *)((lastIndex * 3) << 3);
    point = (GameTrackPoint *)((u8 *)point + (s32)track);
    index += 0xC00;
    index -= point->angle;
    g_CameraCar.headingAngle = index;
    g_CameraCarSeedYaw = index;
    g_CameraCar.angleY = index;
}
