#include "common.h"
#include "game/track.h"

extern GameTrackPoint *g_TrackArcCenters asm("D_8019C7D0");
extern s16 g_TrackSectionCount asm("D_8019C9A4");

/*
 * Installs the track-point table from a loaded blob: word 0 is the point count,
 * the rest is the GameTrackPoint array. Publishes g_TrackPoints (points),
 * g_TrackPointCount (count) and g_TrackArcCenters (marker array right after the points), then
 * sums every point's segmentLength into the total track length g_TrackLength and
 * derives g_TrackSectionCount = (total >> 8) + 1.
 */
void GameInstallTrackPoints(s32 *trackData) asm("func_8002A6B0");
void GameInstallTrackPoints(s32 *trackData) {
    s32 count;
    s32 limit;
    GameTrackPoint *points;
    s32 i;
    s32 index;
    GameTrackPoint *point;
    s32 total;
    s32 pad[2];

    count = trackData[0];
    points = (GameTrackPoint *)(trackData + 1);
    g_TrackPoints = points;
    g_TrackLength = 0;
    g_TrackPointCount = count;
    g_TrackArcCenters = (GameTrackPoint *)((count * sizeof(GameTrackPoint)) + (s32)points);

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
    g_TrackSectionCount = (total >> 8) + 1;
}
