#include "common.h"
#include "game/vector.h"
#include "game/render.h"
#include "game/track.h"
#include "psyq/gte.h"

extern GameRenderObject g_CameraCar;
extern s32 g_CameraCarTrackPoint;
extern s32 g_CameraCarZ;
extern s32 g_CameraCarHeading;
extern s32 g_CameraCarSpeed;
extern s32 g_CameraCarStepX;
extern s32 g_CameraCarStepZ;

void InterpolateTrackPoint(s32 arg0, s32 *out, s32 weight);
s32 rsin(s32 arg0);
s32 rcos(s32 arg0);
void AccumulateLapProgress(void *arg0);
void UpdateCarTrackState(void *arg0, s32 arg1, void *arg2);
void DrawPlayerCarModel(GameRenderObject *obj);


/*
 * Camera track-follower: advances a look-ahead track point, aims the eye object
 * g_CameraCar (a GameRenderObject) toward the sampled centre-line point
 * (InterpolateTrackPoint + atan2), nudges its position, then seeds the scratchpad view
 * state (view[2..4]=eye XYZ, view[6]=pitch, view[7]=yaw, view[8]=roll) from the
 * eye object and submits the render object (DrawPlayerCarModel). markerClamp is the
 * zeroed clamp record passed to the track-marker builder UpdateCarTrackState.
 */
void UpdateFinishCamera(GameRenderObject *obj);
void UpdateFinishCamera(GameRenderObject *obj) {
    s32 *view = (s32 *)0x1F800000;
    s32 delta[3];
    s32 coords[3];
    s16 markerClamp[2];
    s32 index;
    s32 rem;
    s32 offset;
    s32 angle;
    s32 value;
    s32 zValue;

    offset = g_CameraCarTrackPoint;
    if (*(s16 *)&obj->field_B8 != 0) {
        index = offset + 2;
    } else {
        index = offset - 2;
    }
    rem = index;
    if (index < 0) {
        rem = index + g_TrackPointCount;
    }
    index = rem % g_TrackPointCount;

    InterpolateTrackPoint(index, coords, g_CameraCar.field_38);
    angle = 0x400 - Atan2(coords[0] - g_CameraCar.x, coords[2] - g_CameraCarZ);

    g_CameraCarHeading += GetAngleDelta(g_CameraCarHeading, angle);

    value = rsin(g_CameraCarHeading) * g_CameraCarSpeed;
    if (value < 0) {
        value += 0xFF;
    }
    g_CameraCarStepX = value >> 8;

    zValue = rcos(g_CameraCarHeading) * g_CameraCarSpeed;
    if (zValue < 0) {
        zValue += 0xFF;
    }
    g_CameraCarStepZ = zValue >> 8;

    g_CameraCar.x = g_CameraCarStepX / 256 + g_CameraCar.x;
    g_CameraCarZ = g_CameraCarStepZ / 256 + g_CameraCarZ;

    AccumulateLapProgress(&g_CameraCar);
    markerClamp[0] = 0;
    markerClamp[1] = 0;
    UpdateCarTrackState(&g_CameraCar, g_CameraCarTrackPoint, markerClamp);

    *(Block16 *)(view + 2) = *(Block16 *)&g_CameraCar;
    view[3] -= 64;

    delta[0] = obj->x - view[2];
    delta[1] = obj->y - view[3];
    delta[2] = obj->z - view[4];

    view[7] = 0x400 - Atan2(delta[0], delta[2]);
    value = SquareRoot12(delta[0] * delta[0] + delta[2] * delta[2]);
    view[6] = 0x400 - Atan2(delta[1], value >> 6);
    view[8] = 0;

    SetCameraRotMatrix();
    SelectModelBank(0);
    DrawPlayerCarModel(obj);
}
