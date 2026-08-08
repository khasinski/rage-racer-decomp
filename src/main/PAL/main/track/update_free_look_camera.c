#include "common.h"
#include "game/race.h"
#include "game/random.h"
#include "game/render.h"
#include "game/scratchpad.h"
#include "game/state.h"
#include "game/track.h"
#include "game/vector.h"
#include "psyq/gte.h"

extern s32 g_PlayerTrackProgress;

extern GameRenderObject g_CameraCar;

s32 rsin(s32 angle);
s32 rcos(s32 angle);
void AccumulateLapProgress(void *car);
void UpdateCarTrackState(void* obj, s32 trackPointIndex, void* clampPair);

/*
 * Fuller sibling of UpdateFinishCamera: camera track-follower with bob/shake. Aims
 * the eye object g_CameraCar at a look-ahead centre-line point, and when
 * updateMotion is set, ramps the follow distance and applies the shake
 * offsets g_FreeCameraAngleOffset[] driven by the input bits in g_PadHeld. Builds and
 * transposes the view rotation matrices, projects the eye-forward point, and
 * writes the scratchpad view state (view[2..4]=eye XYZ, view[6]=pitch,
 * view[7]=yaw, view[8]=roll). markerClamp is the zeroed clamp record for the
 * track-marker builder UpdateCarTrackState.
 */
void UpdateFreeLookCamera(s32 mode, s32 updateMotion) {
    s32 *view = &SCRATCH_PRIM_CURSOR_WORD;
    s32 delta[3];
    s32 coords[3];
    s16 markerClamp[2];
    s16 vec[3];
    s32 out[3];
    Matrix m1;
    Matrix m2;
    s32 offset;
    s32 index;
    s32 rem;
    s32 angle;
    s32 value;
    s32 zValue;
    s32 c400;

    offset = g_CameraCar.trackPointIndex;
    if (g_GrandPrixSeries != 0) {
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
    angle = 0x400 - Atan2(coords[0] - g_CameraCar.x, coords[2] - g_CameraCar.z);
    g_CameraCarHeading += GetAngleDelta(g_CameraCarHeading, angle);
    g_CameraCar.angleY = g_CameraCarHeading;

    if (updateMotion != 0) {
        if (g_CameraCarSpeedRamp < 50) {
            g_CameraCarSpeedRamp += 3;
        }
        g_CameraCarSpeed = (g_CameraCarSpeed + g_CameraCarSpeedRamp) * 94 / 100;

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
        g_CameraCar.z = g_CameraCarStepZ / 256 + g_CameraCar.z;

        if (g_PadHeld & PAD_UP) {
            g_FreeCameraAngleOffset[0] -= 8;
        } else if (g_PadHeld & PAD_DOWN) {
            g_FreeCameraAngleOffset[0] += 8;
        }
        if (g_PadHeld & PAD_LEFT) {
            g_FreeCameraAngleOffset[1] -= 16;
        } else if (g_PadHeld & PAD_RIGHT) {
            g_FreeCameraAngleOffset[1] += 16;
        }
    }
    if (g_FreeCameraAngleOffset[0] >= 0) {
        g_FreeCameraAngleOffset[0] = 0;
    } else if (g_FreeCameraAngleOffset[0] < -255) {
        g_FreeCameraAngleOffset[0] = -256;
    }

    g_CameraCar.angleY = g_FreeCameraAngleOffset[1] + g_CameraCar.angleY;
    AccumulateLapProgress(&g_CameraCar);
    markerClamp[0] = 0;
    markerClamp[1] = 0;
    UpdateCarTrackState(&g_CameraCar, g_CameraCar.trackPointIndex, markerClamp);

    *(Block16 *)(view + 2) = *(Block16 *)&g_CameraCar.x;
    view[3] -= 48;
    *(Block16 *)(view + 6) = *(Block16 *)&g_CameraCar.angleX;
    view[6] = g_FreeCameraAngleOffset[0] + view[6];

    BuildRotMatrixY(&m1, view[7]);
    BuildRotMatrixX(&m2, view[6]);
    MulMatrix2(&m2, &m1);
    BuildRotMatrixZ(&m2, view[8]);
    MulMatrix2(&m2, &m1);

    vec[0] = 0;
    vec[1] = 0;
    vec[2] = 0x18C0;
    m2.m[0][0] = m1.m[0][0];
    m2.m[0][1] = m1.m[1][0];
    m2.m[0][2] = m1.m[2][0];
    m2.m[1][0] = m1.m[0][1];
    m2.m[1][1] = m1.m[1][1];
    m2.m[1][2] = m1.m[2][1];
    m2.m[2][0] = m1.m[0][2];
    m2.m[2][1] = m1.m[1][2];
    m2.m[2][2] = m1.m[2][2];
    ApplyMatrix(&m2, vec, out);

    coords[0] = (out[0] >> 4) + view[2];
    coords[1] = (out[1] >> 4) + view[3];
    coords[2] = (out[2] >> 4) + view[4];
    delta[0] = coords[0] - view[2];
    delta[1] = coords[1] - view[3];
    delta[2] = coords[2] - view[4];
    c400 = 0x400;
    view[7] = c400 - Atan2(delta[0], delta[2]);
    value = SquareRoot12(delta[0] * delta[0] + delta[2] * delta[2]);
    view[6] = c400 - Atan2(delta[1], value >> 6);

    g_PlayerTrackSection = g_CameraCar.trackSection;
    g_PlayerProgressA = g_CameraCar.progressA;
    g_PlayerProgressB = g_CameraCar.progressB;
    g_PlayerTrackProgress = g_CameraCar.trackProgress;
    SetCameraRotMatrix();
}

extern struct {
    s16 x;
    s16 y;
} g_StartGridSceneryStep[];

void DrawStartGridScenery(s32 flags) {
    Matrix mtx;
    Vec4 state;
    s32 s1;
    s32 s0;
    register s32 value asm("$2");
    s32 drawArg;
    s32 rem;
    s32 lim;

    if (g_RacePhase < 2 && flags >= 0x51) {
        BuildRotMatrixY(&mtx, g_StartGridSceneryAngle[g_RaceSeries]);
        MulMatrix2(SCRATCH_VIEW_MATRIX_GTE, &mtx);
        if (flags - 90 > 0) {
            state = g_StartGridSceneryPos[g_RaceSeries];
            s1 = (flags - 90) / 3;
            state.x += g_StartGridSceneryStep[g_RaceSeries].x * (s0 = s1 / 15);
            state.z += g_StartGridSceneryStep[g_RaceSeries].y * s0;
            if (g_CourseIndex % 4 == 3) {
                state.z += 0x5000;
            }
            SetGteObjectMatrix((void *)0x1F80011C, &state, &mtx);
            rem = s1 - s0 * 15;
            lim = g_CourseModelCount;
            __asm__ __volatile__("");
            value = rem + 0x28;
            SCRATCH_ENV_MODE4 = 0;
            drawArg = (value < lim) ? value : 1;
        } else {
            state = g_StartGridSceneryPos[g_RaceSeries];
            if (g_CourseIndex % 4 == 3) {
                state.z += 0x5000;
            }
            SetGteObjectMatrix((void *)0x1F80011C, &state, &mtx);
            lim = g_CourseModelCount;
            __asm__ __volatile__("");
            value = 0x28;
            SCRATCH_ENV_MODE4 = 0;
            drawArg = (value < lim) ? value : 1;
        }
        SubmitCourseModel((void *)SCRATCHPAD_ADDR, drawArg);
    }
}

void ResetFreeLookCamera(void) {
}

extern u32 *g_VisibleCellMask;

void DrawAnimatedScenery(s32 timer, s32 instance) {
    Matrix mtx;
    Matrix mtx2;
    Vec4 state;
    s32 wordIndex;
    s32 bitIndex;
    s32 value;
    u32 *visibility;
    u32 *wordPtr;
    s32 bit;
    s32 visible;
    s32 num;
    s32 drawArg;
    register s32 sv asm("$2");
    register s32 *scr asm("$8");

    state = g_AnimSceneryPos[instance];

    if ((g_CourseIndex & 3) == 3) {
        state.z += 0x5000;
    }
    if (g_GrandPrixClass == 5) {
        return;
    }

    wordIndex = state.z + 0x400;
    if (wordIndex < 0) {
        wordIndex = state.z + 0xBFF;
    }
    wordIndex >>= 11;

    value = state.x;
    visibility = g_VisibleCellMask;
    bit = value + 0x400;
    wordPtr = (u32 *)((wordIndex << 2) + (s32)visibility);
    if (bit < 0) {
        bit = value + 0xBFF;
    }
    bitIndex = bit >> 11;
    visible = 1 << bitIndex;
    visible &= *wordPtr;
    if (visible == 0) {
        return;
    }

    g_AnimSceneryFrame = (timer / 4) % 16;
    if (g_AnimSceneryFrame == 0 && (timer % 8) == 0 && g_RacePaused == 0) {
        g_AnimSceneryTint = 0;
        g_AnimSceneryRacePosition = g_RacePosition;
        g_AnimSceneryVariant = (Random15() & 7) / 3;
        if (g_AnimSceneryRacePosition >= 4) {
            g_AnimSceneryRacePosition = 0;
        }
    }

    BuildRotMatrixY(&mtx, state.w);
    BuildRotMatrixX(&mtx2, g_AnimSceneryPitch[instance]);
    MulMatrix(&mtx, &mtx2);
    MulMatrix2(SCRATCH_VIEW_MATRIX_GTE, &mtx);

    if (g_GrandPrixMode == 0) {
        return;
    }

    g_AnimSceneryTint = ((timer >> 3) & 3) << 16;

    if (g_AnimSceneryRacePosition != 0) {
        if (g_AnimSceneryFrame < 13) {
            SetGteObjectMatrix((void *)0x1F80011C, &state, &mtx);
            num = g_AnimSceneryFrame + 10;
            SCRATCH_ENV_MODE4 = 0;
            drawArg = (num < g_CourseModelCount) ? num : 1;
            SubmitCourseModel((void *)SCRATCHPAD_ADDR, drawArg);
        } else {
            SetGteObjectMatrix((void *)0x1F80011C, &state, &mtx);
            num = g_AnimSceneryRacePosition;
            SCRATCH_ENV_MODE4 = 0;
            drawArg = (num < g_CourseModelCount) ? num : 1;
            SubmitCourseModel((void *)SCRATCHPAD_ADDR, drawArg);
        }

        SetGteObjectMatrix((void *)0x1F80011C, &state, &mtx);
        sv = g_AnimSceneryTint;
        SCRATCH_ENV_MODE4 = sv;
        num = g_AnimSceneryVariant + 4;
        sv = g_CourseModelCount;
        drawArg = (num < sv) ? num : 1;
        SubmitCourseModel((void *)SCRATCHPAD_ADDR, drawArg);
    } else {
        SetGteObjectMatrix((void *)0x1F80011C, &state, &mtx);
        num = g_AnimSceneryFrame + 0x18;
        scr = &SCRATCH_ENV_MODE4;
        *scr = 0;
        drawArg = (num < g_CourseModelCount) ? num : 1;
        SubmitCourseModel((void *)SCRATCHPAD_ADDR, drawArg);

        SetGteObjectMatrix((void *)0x1F80011C, &state, &mtx);
        sv = g_AnimSceneryTint;
        scr = &SCRATCH_ENV_MODE4;
        *scr = sv;
        num = g_AnimSceneryVariant + 7;
        sv = g_CourseModelCount;
        drawArg = (num < sv) ? num : 1;
        SubmitCourseModel((void *)SCRATCHPAD_ADDR, drawArg);
    }
}

void DrawAnimatedScenery2(s32 timer, s32 instance, s32 isReplay, s32 animate) {
    Matrix mtx;
    Matrix mtx2;
    Vec4 state;
    s32 wordIndex;
    s32 bitIndex;
    s32 value;
    u32 *visibility;
    u32 *wordPtr;
    s32 bit;
    s32 visible;
    s32 num;
    s32 drawArg;
    s32 sv;
    register s32 *scr asm("$8");

    if (g_GrandPrixMode == 0) {
        return;
    }
    if (g_GrandPrixClass == 5) {
        return;
    }

    state = g_AnimSceneryPos[instance];
    if ((g_CourseIndex & 3) == 3) {
        state.z += 0x5000;
    }

    wordIndex = state.z + 0x400;
    if (wordIndex < 0) {
        wordIndex = state.z + 0xBFF;
    }
    wordIndex >>= 11;

    value = state.x;
    visibility = g_VisibleCellMask;
    bit = value + 0x400;
    wordPtr = (u32 *)((wordIndex << 2) + (s32)visibility);
    if (bit < 0) {
        bit = value + 0xBFF;
    }
    bitIndex = bit >> 11;
    visible = 1 << bitIndex;
    visible &= *wordPtr;
    if (visible == 0) {
        return;
    }

    g_AnimScenery2Frame = (timer / 4) % 16;
    if (g_AnimScenery2Frame == 0 && (timer % 8) == 0 && animate == 1) {
        g_AnimScenery2Tint = 0;
        g_AnimScenery2Variant = (Random15() & 7) / 3;
    }

    BuildRotMatrixY(&mtx, state.w);
    BuildRotMatrixX(&mtx2, g_AnimSceneryPitch[instance]);
    MulMatrix(&mtx, &mtx2);
    MulMatrix2(SCRATCH_VIEW_MATRIX_GTE, &mtx);

    g_AnimScenery2Tint = ((timer >> 3) & 3) << 16;

    if (isReplay != 0) {
        SetGteObjectMatrix((void *)0x1F80011C, &state, &mtx);
        num = g_AnimScenery2Frame + 0xA;
        scr = &SCRATCH_ENV_MODE4;
        *scr = 0;
        drawArg = (num < g_CourseModelCount) ? num : 1;
        SubmitCourseModel((void *)SCRATCHPAD_ADDR, drawArg);

        SetGteObjectMatrix((void *)0x1F80011C, &state, &mtx);
        sv = g_AnimScenery2Tint;
        drawArg = 1;
        scr = &SCRATCH_ENV_MODE4;
        *scr = sv;
        sv = g_CourseModelCount;
        num = g_AnimScenery2Variant;
        __asm__("" : "=r"(num) : "0"(num), "r"(sv));
        num = num + 4;
    } else {
        SetGteObjectMatrix((void *)0x1F80011C, &state, &mtx);
        num = g_AnimScenery2Frame + 0x18;
        scr = &SCRATCH_ENV_MODE4;
        *scr = 0;
        drawArg = (num < g_CourseModelCount) ? num : 1;
        SubmitCourseModel((void *)SCRATCHPAD_ADDR, drawArg);

        SetGteObjectMatrix((void *)0x1F80011C, &state, &mtx);
        sv = g_AnimScenery2Tint;
        drawArg = 1;
        scr = &SCRATCH_ENV_MODE4;
        *scr = sv;
        sv = g_CourseModelCount;
        num = g_AnimScenery2Variant;
        __asm__("" : "=r"(num) : "0"(num), "r"(sv));
        num = num + 7;
    }

    if (num < sv) {
        drawArg = num;
    }
    SubmitCourseModel((void *)SCRATCHPAD_ADDR, drawArg);
}
