#include "common.h"
#include "game/asset.h"
#include "game/vector.h"
#include "game/track.h"
#include "game/race.h"
#include "game/car.h"
#include "game/render.h"
#include "game/render_internal.h"

void SeedFinishCameraAlt(void *car) {
    register u32 word0 asm("$2");
    u32 word1;
    u32 word2;
    Block16 *src;
    Block16 *dst;
    Block16 *end;
    u32 *base;
    GameTrackPoint *track;
    AssetAddress pointAddress;
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
    end = src + sizeof(GameCarRuntime) / sizeof(*src);
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
    pointAddress.offset = (index * 3) << 3;
    pointAddress.offset += (s32)track;
    point = pointAddress.pointer;
    g_CameraCar.x = point->x;

    index = ((GameCarRuntime *)base)->trackPointIndex;
    pointAddress.offset = (index * 3) << 3;
    pointAddress.offset += (s32)track;
    point = pointAddress.pointer;
    g_CameraCar.z = point->z;

    index = ((GameCarRuntime *)base)->trackPointIndex;
    pointAddress.offset = (index * 3) << 3;
    pointAddress.offset += (s32)track;
    point = pointAddress.pointer;
    word0 = point->y;
    index = g_GrandPrixSeries;
    g_CameraCar.speed = 0;
    g_CameraCar.y = word0 - 0x30;

    lastIndex = ((GameCarRuntime *)base)->trackPointIndex;
    index <<= 11;
    pointAddress.offset = (lastIndex * 3) << 3;
    pointAddress.offset += (s32)track;
    point = pointAddress.pointer;
    index += 0xC00;
    index -= point->angle;
    g_CameraCar.headingAngle = index;
    g_CameraCarSeedYaw = index;
    g_CameraCar.angleY = index;
}
