#include "common.h"
#include "game/track.h"

extern GameTrackPoint *D_8019C7D0;
extern s16 D_8019C9A4;

/*
 * Installs the track-point table from a loaded blob: word 0 is the point count,
 * the rest is the GameTrackPoint array. Publishes g_TrackPoints (points),
 * g_TrackPointCount (count) and D_8019C7D0 (marker array right after the points), then
 * sums every point's segmentLength into the total track length g_TrackLength and
 * derives D_8019C9A4 = (total >> 8) + 1.
 */
void GameInstallTrackPoints(s32 *trackData) asm("func_8002A6B0");
void GameInstallTrackPoints(s32 *trackData) {
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
    g_TrackPoints = points;
    g_TrackLength = 0;
    g_TrackPointCount = count;
    D_8019C7D0 = (GameTrackPoint *)((count * sizeof(GameTrackPoint)) + (s32)points);

    i = 0;
    if (count > 0) {
        limit = count;
        do {
            index = i % limit;
            point = (GameTrackPoint *)((index * sizeof(GameTrackPoint)) + (s32)points);
            g_TrackLength += point->segmentLength;
            i++;
        } while (i < limit);
    }

    total = g_TrackLength;
    D_8019C9A4 = (total >> 8) + 1;
}
