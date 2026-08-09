#include "common.h"
#include "game/asset.h"
#include "game/vector.h"
#include "game/track.h"
#include "game/race.h"
#include "game/car.h"
#include "game/render.h"
#include "game/render_internal.h"

typedef union FinishCameraCarAddress {
    u32 *words;
    Block16 *blocks;
    GameCarRuntime *car;
} FinishCameraCarAddress;

void SeedFinishCameraAlt(void *car) {
    register u32 word0 asm("$2");
    u32 word1;
    u32 word2;
    Block16 *src;
    Block16 *dst;
    Block16 *end;
    FinishCameraCarAddress source;
    FinishCameraCarAddress destination;
    GameTrackPoint *track;
    TrackPointTableAddress pointAddress;
    TrackPointTableAddress trackAddress;
    GameTrackPoint *point;
    register s32 index asm("$3");
    s32 lastIndex;

    /* car is a car runtime block: the copy below moves 0x19C bytes of it into
     * g_CameraCar. Every one of the eleven g_CameraCar* split symbols lands
     * on one of its fields. Storing through
     * the source view is what lets the index reads below stay plain: both sides
     * carry the aggregate mark now, so 44a's exemption never fires. */
    source.car = car;
    asm("" : "=r"(source.words) : "0"(source.words));
    destination.car = &g_CameraCar;
    dst = destination.blocks;
    src = (Block16 *)source.words;
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

    index = source.car->trackPointIndex;
    track = g_TrackPoints;
    pointAddress.byteOffset = (index * 3) << 3;
    trackAddress.pointPointer = track;
    pointAddress.byteOffset += trackAddress.byteOffset;
    point = pointAddress.pointPointer;
    g_CameraCar.x = point->x;

    index = source.car->trackPointIndex;
    pointAddress.byteOffset = (index * 3) << 3;
    trackAddress.pointPointer = track;
    pointAddress.byteOffset += trackAddress.byteOffset;
    point = pointAddress.pointPointer;
    g_CameraCar.z = point->z;

    index = source.car->trackPointIndex;
    pointAddress.byteOffset = (index * 3) << 3;
    trackAddress.pointPointer = track;
    pointAddress.byteOffset += trackAddress.byteOffset;
    point = pointAddress.pointPointer;
    word0 = point->y;
    index = g_GrandPrixSeries;
    g_CameraCar.speed = 0;
    g_CameraCar.y = word0 - 0x30;

    lastIndex = source.car->trackPointIndex;
    index <<= 11;
    pointAddress.byteOffset = (lastIndex * 3) << 3;
    trackAddress.pointPointer = track;
    pointAddress.byteOffset += trackAddress.byteOffset;
    point = pointAddress.pointPointer;
    index += 0xC00;
    index -= point->angle;
    g_CameraCar.headingAngle = index;
    g_CameraCarSeedYaw = index;
    g_CameraCar.bodyYaw = index;
}
