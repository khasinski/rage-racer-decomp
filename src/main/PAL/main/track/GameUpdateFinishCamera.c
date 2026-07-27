#include "common.h"
#include "game/render.h"
#include "game/track.h"
#include "psyq/gte.h"

extern GameRenderObject g_CameraCar asm("D_801E3E14");
extern s32 g_CameraCarTrackPoint asm("D_801E3E44");
extern s32 g_CameraCarZ asm("D_801E3E1C");
extern s32 g_CameraCarHeading asm("D_801E3EB4");
extern s32 g_CameraCarSpeed asm("D_801E3EB8");
extern s32 g_CameraCarStepX asm("D_801E3ED8");
extern s32 g_CameraCarStepZ asm("D_801E3EE0");

void func_8002FC84(s32 arg0, s32 *out, s32 weight);
s32 func_80068568(s32 arg0);
s32 func_80068634(s32 arg0);
void func_8002C168(void *arg0);
void GameUpdateCarTrackState(void *arg0, s32 arg1, void *arg2) asm("func_80031298");
void GameDrawPlayerCarModel(GameRenderObject *obj) asm("func_8001DAB0");

typedef struct Block16 {
    s32 w0;
    s32 w1;
    s32 w2;
    s32 w3;
} Block16;

/*
 * Camera track-follower: advances a look-ahead track point, aims the eye object
 * g_CameraCar (a GameRenderObject) toward the sampled centre-line point
 * (func_8002FC84 + atan2), nudges its position, then seeds the scratchpad view
 * state (view[2..4]=eye XYZ, view[6]=pitch, view[7]=yaw, view[8]=roll) from the
 * eye object and submits the render object (GameDrawPlayerCarModel). markerClamp is the
 * zeroed clamp record passed to the track-marker builder GameUpdateCarTrackState.
 */
void GameUpdateFinishCamera(GameRenderObject *obj) asm("func_8003CB3C");
void GameUpdateFinishCamera(GameRenderObject *obj) {
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

    func_8002FC84(index, coords, g_CameraCar.field_38);
    angle = 0x400 - GameAtan2(coords[0] - g_CameraCar.x, coords[2] - g_CameraCarZ);

    g_CameraCarHeading += GameGetAngleDelta(g_CameraCarHeading, angle);

    value = func_80068568(g_CameraCarHeading) * g_CameraCarSpeed;
    if (value < 0) {
        value += 0xFF;
    }
    g_CameraCarStepX = value >> 8;

    zValue = func_80068634(g_CameraCarHeading) * g_CameraCarSpeed;
    if (zValue < 0) {
        zValue += 0xFF;
    }
    g_CameraCarStepZ = zValue >> 8;

    g_CameraCar.x = g_CameraCarStepX / 256 + g_CameraCar.x;
    g_CameraCarZ = g_CameraCarStepZ / 256 + g_CameraCarZ;

    func_8002C168(&g_CameraCar);
    markerClamp[0] = 0;
    markerClamp[1] = 0;
    GameUpdateCarTrackState(&g_CameraCar, g_CameraCarTrackPoint, markerClamp);

    *(Block16 *)(view + 2) = *(Block16 *)&g_CameraCar;
    view[3] -= 64;

    delta[0] = obj->x - view[2];
    delta[1] = obj->y - view[3];
    delta[2] = obj->z - view[4];

    view[7] = 0x400 - GameAtan2(delta[0], delta[2]);
    value = SquareRoot12(delta[0] * delta[0] + delta[2] * delta[2]);
    view[6] = 0x400 - GameAtan2(delta[1], value >> 6);
    view[8] = 0;

    GameSetCameraRotMatrix();
    GameSelectModelBank(0);
    GameDrawPlayerCarModel(obj);
}
