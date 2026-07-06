#include "common.h"
#include "game/track.h"

extern GameTrackPoint *D_8009E688;
extern s32 D_8009E6A8;
extern GameTrackPoint *D_8019C7D0;
extern s32 D_801E40D8;
extern s16 D_8019C9A4;

void func_8002A6B0(s32 *trackData) {
    register s32 count asm("$3");
    register s32 limit asm("$4");
    register GameTrackPoint *points asm("$6");
    register s32 i asm("$5");
    register s32 index asm("$3");
    register GameTrackPoint *point asm("$2");
    register s32 total asm("$2");
    s32 pad[2];

    asm volatile("" : "=m"(pad));
    count = trackData[0];
    points = (GameTrackPoint *)(trackData + 1);
    D_8009E688 = points;
    D_801E40D8 = 0;
    D_8009E6A8 = count;
    D_8019C7D0 = (GameTrackPoint *)((count * sizeof(GameTrackPoint)) + (s32)points);

    i = 0;
    if (count > 0) {
        limit = count;
        do {
            index = i % limit;
            point = (GameTrackPoint *)((index * sizeof(GameTrackPoint)) + (s32)points);
            D_801E40D8 += point->segmentLength;
            i++;
        } while (i < limit);
    }

    total = D_801E40D8;
    D_8019C9A4 = (total >> 8) + 1;
}
