#include "common.h"
#include "game/render.h"
#include "game/scratchpad.h"
#include "game/track.h"
#include "game/vector.h"
#include "psyq/gte.h"

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

extern s32 rcos(s32);
extern s32 SquareRoot0(s32);
extern void *ApplyMatrixLV(void *, void *, void *);
/* Mode-3 camera path: the eye is eased from one track-camera node to the next
 * over `node->duration` frames. Each of offset (a local xyz applied through the
 * car's matrix) and orientation (pitch/yaw/roll/distance) keeps a start value,
 * a delta to the destination and the current interpolated value. Mode-3 nodes
 * therefore store angles in the first four words where modes 2/4 store a world
 * position -- the record is a union keyed on `node->mode`. */
/* The orientation quads, same three-group shape as the offsets: delta at
 * 0x8009B1E8, start at +0x10, current at +0x20. Elements 0..2 are pitch, yaw
 * and roll -- 12-bit angles, wrapped to +-0x800 on load and masked with 0xFFF
 * on store -- and element 3 is the pull-back distance, a plain length. */
#define CAMPATH_PITCH 0
#define CAMPATH_YAW 1
#define CAMPATH_ROLL 2
#define CAMPATH_DIST 3
extern s32 g_PlayerCar;
extern u8 *g_TrackCameras;

/*
 * Camera-mode state machine: `cameraModeSel` selects among the camera behaviours
 * (chase / bumper / replay-orbit / intro-pan, etc.); `car` is the followed
 * render/target object. Positions the eye and fills the scratchpad view state
 * (view[2..8]) with matrix/atan2/sqrt math, then submits via DrawPlayerCarModel.
 * Field accesses use the FIELD(base,type,offset) raw-offset macro to stay
 * byte-exact, so params/locals are not retyped.
 */
void UpdateCamera(s32 cameraModeSel, void *car) {
    s16 sp10[4];
    s32 sp18[3];
    s32 sp28[3];
    s32 sp38[4];
    u16 sp48[16];
    u16 sp68[16];
    u16 sp88[16];
    u16 spA8[16];
    s32 previousMode;
    s32 rawAngle;
    s32 adjustedY;
    u32 case4Base;
    s32 turnLimit;
    s32 turnAccel;
    s32 turnFactor;
    s32 *angleState;
    s32 *case3Angle;
    s32 *modeAngle;
    s32 *scratch;
    s32 cameraMode;
    s32 chaseTargetYaw;
    s32 yawError;
    s32 pathBlend;
    s32 pathYawRelative;
    s32 speedDamping;
    s32 toCarX;
    s32 pathYaw;
    s32 pathPitch;
    s32 introNodeOffset;
    s32 squaredX;
    s32 squaredZ;
    s32 orbitNodeOffset;
    s32 chaseNodeOffset;
    s32 pathOffsetZ;
    s32 pathOffsetY;
    s32 camPathOffset;
    s32 camPathAngle;
    s32 pathDistance;
    s32 orbitToCarX;
    s32 orbitToCarY;
    s32 orbitToCarZ;
    s32 orbitDx;
    s32 orbitDz;
    s32 chaseYawDamping;
    s32 chaseYawStepLimit;
    s32 cameraNodeIndex;
    s32 yawStepAhead;
    s32 yawStepWrapped;
    s32 yawStepBehind;
    s32 chaseYawLag;
    s32 chaseDistance;
    s32 chaseDx;
    s32 chaseDz;
    s32 pathRoll;
    s32 previousNodeIndex;
    u8 nodeChanged;
    s32 chaseCarSpeed;
    s32 offsetXProduct;
    s32 offsetYProduct;
    s32 offsetZProduct;
    s32 pitchProduct;
    s32 yawProduct;
    s32 rollProduct;
    s32 distProduct;
    s32 pitchDelta;
    s32 chaseYaw;
    s32 screenWidth;
    s32 negatedAccel;
    s32 wrappedLag;
    u8 nextPrevMode;
    void *pathNode;
    void *orbitNode;
    void *chaseNodeAgain;
    void *chaseNode;
    void *chaseNodeOffsets;
    void *prevNode;
    u32 introNode;

    cameraNodeIndex = FindNearestTrackCamera(car);
    scratch = &SCRATCH_PRIM_CURSOR_WORD;
    previousNodeIndex = g_CameraNodeIndex;
    g_CameraNodeIndex = cameraNodeIndex;
    nodeChanged = cameraNodeIndex != previousNodeIndex;
    if (cameraModeSel < 2) {
        cameraMode = cameraModeSel;
    } else {
        cameraMode = FIELD(((cameraNodeIndex * 0x24) + g_TrackCameras), s16 *, 0x20);
    }
    switch (cameraMode) {
    case 0:
        *(Block16 *)&scratch[2] = *(Block16 *)car;
        *(Block16 *)&scratch[6] = *(Block16 *)((u8 *)car + 0x20);
        BuildRotMatrixY(&sp48[0], scratch[7]);
        BuildRotMatrixX(&sp68[0], scratch[6]);
        MulMatrix2(&sp68[0], &sp48[0]);
        BuildRotMatrixZ(&sp68[0], scratch[8]);
        MulMatrix2(&sp68[0], &sp48[0]);
        sp10[0] = 0;
        sp10[1] = -0x1C0;
        sp10[2] = 0;
        sp68[0] = sp48[0];
        sp68[1] = sp48[3];
        sp68[2] = sp48[6];
        sp68[3] = sp48[1];
        sp68[4] = sp48[4];
        sp68[5] = sp48[7];
        sp68[6] = sp48[2];
        sp68[7] = sp48[5];
        sp68[8] = sp48[8];
        ApplyMatrix((s32 *)&sp68[0], (s32 *)&sp10[0], &sp38[0]);
        scratch[2] += sp38[0] >> 4;
        scratch[3] += sp38[1] >> 4;
        scratch[4] += sp38[2] >> 4;
        scratch[6] += FIELD(car, s16 *, 0x8C);
        g_CameraModePrev = 0;
        break;
    case 1:
        *(Block16 *)&scratch[2] = *(Block16 *)car;
        chaseYawDamping = FIELD(car, s32 *, 0x24);
        chaseTargetYaw = chaseYawDamping & 0xFFF;
        chaseCarSpeed = FIELD(car, s32 *, 0xA4);
        g_ChaseCarSpeed = chaseCarSpeed;
        previousMode = g_CameraModePrev;
        g_ChaseTargetYaw = chaseTargetYaw;
        if (previousMode == 1) {
            modeAngle = &D_8009B1EC;
            *modeAngle &= 0xFFF;
            g_ChaseYawRampNeg &= 0xFFF;
            g_ChaseYawRampPos &= 0xFFF;
        } else {
            D_8009B1EC = chaseTargetYaw;
            g_ChaseYawRampNeg = 0;
            g_ChaseYawRampPos = 0;
        }
        if (g_ChaseCarSpeed >= 0x321) {
            chaseYawDamping = 0x4E2 - g_ChaseCarSpeed;
            g_ChaseYawDamping = chaseYawDamping;
            if (chaseYawDamping < 6) {
                g_ChaseYawDamping = 6;
            }
            g_ChaseYawDamping = ((((g_ChaseYawDamping * 8) / 50) + 8) / 10) + 1;
        } else {
            speedDamping = 0x4E2 - g_ChaseCarSpeed;
            g_ChaseYawDamping = speedDamping;
        g_ChaseYawDamping = ((((g_ChaseYawDamping * 6 * speedDamping) / 2500) - ((speedDamping * 0x46) / 50)) + 0xE0) / 10;
        }
        yawError = g_ChaseTargetYaw - D_8009B1EC;
        if (yawError >= 5) {
            if (yawError >= 0x800) {
                chaseYawStepLimit = (((0x1000 - yawError) / 17) * 2) & 0xFFF;
                g_ChaseYawStepLimit = chaseYawStepLimit;
                if (chaseYawStepLimit >= 0x41) {
                    g_ChaseYawStepLimit = 0x40;
                }
                turnFactor = g_ChaseYawDamping;
                turnAccel = ((g_ChaseYawRampNeg + 8) * (g_ChaseYawRampNeg + 8)) / turnFactor;
                turnLimit = g_ChaseYawStepLimit;
                g_ChaseYawRampPos = 0;
                g_ChaseYawRampNeg += 8;
                g_ChaseYawStep = turnAccel;
                if (turnLimit >= turnAccel) {
                    negatedAccel = 0 - turnAccel;
                    goto block_36;
                }
                goto block_34;
            }
            yawStepAhead = (((g_ChaseTargetYaw - D_8009B1EC) / 17) * 2) & 0xFFF;
            g_ChaseYawStepLimit = yawStepAhead;
            if (yawStepAhead >= 0x41) {
                g_ChaseYawStepLimit = 0x40;
            }
            turnFactor = g_ChaseYawDamping;
            turnAccel = ((g_ChaseYawRampPos + 8) * (g_ChaseYawRampPos + 8)) / turnFactor;
            turnLimit = g_ChaseYawStepLimit;
            g_ChaseYawRampNeg = 0;
            g_ChaseYawRampPos += 8;
            g_ChaseYawStep = turnAccel;
            if (turnLimit < turnAccel) {
                goto block_29;
            }
            goto block_30;
        }
        if (yawError < -4) {
            if (yawError < -0x7FF) {
                yawStepWrapped = (((0x1000 - (D_8009B1EC - g_ChaseTargetYaw)) / 17) * 2) & 0xFFF;
                g_ChaseYawStepLimit = yawStepWrapped;
                if (yawStepWrapped >= 0x41) {
                    g_ChaseYawStepLimit = 0x40;
                }
                turnFactor = g_ChaseYawDamping;
                turnAccel = ((g_ChaseYawRampPos + 8) * (g_ChaseYawRampPos + 8)) / turnFactor;
                turnLimit = g_ChaseYawStepLimit;
                g_ChaseYawRampNeg = 0;
                g_ChaseYawRampPos += 8;
                g_ChaseYawStep = turnAccel;
                if (turnLimit < turnAccel) {
block_29:
                    g_ChaseYawLag = turnLimit;
                    g_ChaseYawRampPos = SquareRoot0(turnLimit * turnFactor);
                } else {
block_30:
                    g_ChaseYawLag = turnAccel;
                }
            } else {
                yawStepBehind = (((D_8009B1EC - g_ChaseTargetYaw) / 17) * 2) & 0xFFF;
                g_ChaseYawStepLimit = yawStepBehind;
                if (yawStepBehind >= 0x41) {
                    g_ChaseYawStepLimit = 0x40;
                }
                turnFactor = g_ChaseYawDamping;
                turnAccel = ((g_ChaseYawRampNeg + 8) * (g_ChaseYawRampNeg + 8)) / turnFactor;
                turnLimit = g_ChaseYawStepLimit;
                g_ChaseYawRampPos = 0;
                g_ChaseYawRampNeg += 8;
                g_ChaseYawStep = turnAccel;
                if (turnLimit < turnAccel) {
block_34:
                    g_ChaseYawLag = 0 - turnLimit;
                    g_ChaseYawRampNeg = SquareRoot0(turnLimit * turnFactor);
                } else {
                    negatedAccel = 0 - turnAccel;
block_36:
                    g_ChaseYawLag = negatedAccel;
                }
            }
        } else {
            g_ChaseYawLag = 0;
            g_ChaseYawRampNeg = 0;
            g_ChaseYawRampPos = 0;
        }
        rawAngle = g_ChaseTargetYaw;
        chaseCarSpeed = (D_8009B1EC + g_ChaseYawLag) & 0xFFF;
        g_ChaseYaw = chaseCarSpeed;
        if (rawAngle < chaseCarSpeed) {
            chaseYawLag = rawAngle - chaseCarSpeed;
            g_ChaseYawLag = chaseYawLag;
            wrappedLag = chaseYawLag;
            if (wrappedLag < -0x7FF) {
                wrappedLag += 0x1000;
            }
            angleState = &g_ChaseYawLag;
            *angleState = wrappedLag;
        } else {
            chaseYawLag = rawAngle - chaseCarSpeed;
            g_ChaseYawLag = chaseYawLag;
            wrappedLag = chaseYawLag;
            if (wrappedLag >= 0x800) {
                wrappedLag -= 0x1000;
            }
            angleState = &g_ChaseYawLag;
            *angleState = wrappedLag;
        }
        angleState = (s32 *)&sp88[0];
        BuildRotMatrixY(angleState, 0 - g_ChaseYawLag);
        BuildRotMatrixX(&sp68[0], -0x80);
        MulMatrix2(&sp68[0], &sp88[0]);
        D_8009B1EC = g_ChaseYaw;
        BuildRotMatrixY(&sp48[0], FIELD(car, s32 *, 0x24));
        BuildRotMatrixX(&sp68[0], FIELD(car, s32 *, 0x20));
        MulMatrix2(&sp68[0], &sp48[0]);
        BuildRotMatrixZ(&sp68[0], FIELD(car, s32 *, 0x28));
        MulMatrix2(&sp68[0], &sp48[0]);
        TransposeMatrix((Matrix *)&sp48[0], (Matrix *)&spA8[0]);
        MulMatrix2(angleState, &sp48[0]);
        TransposeMatrix((Matrix *)&sp48[0], (Matrix *)&sp68[0]);
        sp18[1] = -0x3C;
        sp18[0] = 0;
        sp18[2] = 0x32;
        ApplyMatrixLV(&spA8[0], &sp18[0], &sp28[0]);
        sp18[0] = 0;
        scratch[2] += sp28[0];
        scratch[3] += sp28[1];
        scratch[4] += sp28[2];
        switch (g_ChaseCameraPreset) {
        case 0:
            sp18[1] = 0x3A;
            screenWidth = 0x118;
            goto block_52;
        case 1:
            sp18[1] = 0x59;
            screenWidth = 0x140;
            goto block_52;
        case 2:
            sp18[1] = 0x97;
            screenWidth = 0x190;
block_52:
            sp18[2] = screenWidth;
            break;
        }
        ApplyMatrixLV(&sp68[0], &sp18[0], &sp38[0]);
        scratch[2] -= sp38[0];
        scratch[3] -= sp38[1];
        scratch[4] -= sp38[2];
        chaseDistance = SquareRoot0((sp38[0] * sp38[0]) + (sp38[2] * sp38[2]));
        sp38[3] = chaseDistance;
        scratch[6] = 0x400 - (Atan2(sp38[1] + 0x28, chaseDistance) & 0xFFF);
        scratch[7] = 0x400 - (Atan2(sp38[0], sp38[2]) & 0xFFF);
        scratch[8] = FIELD(car, s32 *, 0x28) - FIELD(car, s32 *, 0x64);
        if (g_ChaseCameraPreset == 0) {
            negatedAccel = scratch[6] - 0x90;
        } else {
            negatedAccel = scratch[6] - 0x60;
        }
        scratch[6] = negatedAccel;
        g_CameraModePrev = 1;
        break;
    case 2:
        chaseNodeOffset = cameraNodeIndex * 0x24;
        chaseNode = chaseNodeOffset + g_TrackCameras;
        *(Block16 *)&scratch[2] = *(Block16 *)chaseNode;
        BuildRotMatrixY(&sp48[0], FIELD(car, s32 *, 0x24));
        BuildRotMatrixX(&sp68[0], FIELD(car, s32 *, 0x20));
        MulMatrix2(&sp68[0], &sp48[0]);
        BuildRotMatrixZ(&sp68[0], FIELD(car, s32 *, 0x28));
        MulMatrix2(&sp68[0], &sp48[0]);
        TransposeMatrix((Matrix *)&sp48[0], (Matrix *)&spA8[0]);
        chaseNodeOffsets = chaseNodeOffset + g_TrackCameras;
        sp18[0] = FIELD(chaseNodeOffsets, s32 *, 0x10);
        sp18[1] = FIELD(chaseNodeOffsets, s32 *, 0x14);
        sp18[2] = FIELD(chaseNodeOffsets, s32 *, 0x18) + 0x32;
        ApplyMatrixLV(&spA8[0], &sp18[0], &sp28[0]);
        toCarX = scratch[2] - (FIELD(car, s32 *, 0) + sp28[0]);
        sp38[0] = toCarX;
        sp38[1] = scratch[3] - (FIELD(car, s32 *, 4) + sp28[1]);
        chaseNodeAgain = chaseNodeOffset + g_TrackCameras;
        sp38[2] = scratch[4] - (FIELD(car, s32 *, 8) + sp28[2]);
        scratch[2] -= (toCarX * FIELD(chaseNodeAgain, s32 *, 0xC)) / 10000;
        scratch[3] -= (sp38[1] * FIELD(chaseNodeAgain, s32 *, 0xC)) / 10000;
        scratch[4] -= (sp38[2] * FIELD(chaseNodeAgain, s32 *, 0xC)) / 10000;
        chaseDx = scratch[2] - (FIELD(car, s32 *, 0) + sp28[0]);
        squaredX = chaseDx * chaseDx;
        sp38[0] = chaseDx;
        sp38[1] = scratch[3] - (FIELD(car, s32 *, 4) + sp28[1]);
        chaseDz = scratch[4] - (FIELD(car, s32 *, 8) + sp28[2]);
        sp38[2] = chaseDz;
        squaredZ = chaseDz * chaseDz;
        scratch[6] = 0x400 - (Atan2(0 - sp38[1], SquareRoot0(squaredX + squaredZ)) & 0xFFF);
        chaseYaw = 0x400 - (Atan2(0 - sp38[0], 0 - sp38[2]) & 0xFFF);
        nextPrevMode = 2;
        scratch[7] = chaseYaw;
        scratch[8] = 0;
        g_CameraModePrev = nextPrevMode;
        break;
    case 3:
        *(Block16 *)&scratch[2] = *(Block16 *)car;
        if (((u8)nodeChanged) || (g_CameraModePrev != 3)) {
            g_CamPathNode = cameraNodeIndex;
            g_CamPathFrame = 0;
            if (g_CameraModePrev == 3) {
                g_CamPathOffsetStart[0] = g_CamPathOffset[0];
                g_CamPathOffsetStart[1] = g_CamPathOffset[1];
                g_CamPathOffsetStart[2] = g_CamPathOffset[2];
                g_CamPathAngleStart[CAMPATH_PITCH] = g_CamPathAngle[CAMPATH_PITCH];
                g_CamPathAngleStart[CAMPATH_YAW] = g_CamPathAngle[CAMPATH_YAW];
                g_CamPathAngleStart[CAMPATH_ROLL] = g_CamPathAngle[CAMPATH_ROLL];
                g_CamPathAngleStart[CAMPATH_DIST] = g_CamPathAngle[CAMPATH_DIST];
            } else {
                prevNode = (cameraNodeIndex * 0x24) + g_TrackCameras;
                g_CamPathOffsetStart[0] = FIELD(prevNode, s32 *, 0x10);
                g_CamPathOffsetStart[1] = FIELD(prevNode, s32 *, 0x14);
                g_CamPathOffsetStart[2] = FIELD(prevNode, s32 *, 0x18);
                g_CamPathAngleStart[CAMPATH_PITCH] = FIELD(prevNode, s32 *, 0);
                g_CamPathAngleStart[CAMPATH_YAW] = FIELD(prevNode, s32 *, 4);
                g_CamPathAngleStart[CAMPATH_ROLL] = FIELD(prevNode, s32 *, 8);
                g_CamPathAngleStart[CAMPATH_DIST] = FIELD(prevNode, s32 *, 0xC);
            }
            pathNode = (g_CamPathNode * 0x24) + g_TrackCameras;
            g_CamPathOffsetDelta[0] = FIELD(pathNode, s32 *, 0x10) - g_CamPathOffsetStart[0];
            g_CamPathOffsetDelta[1] = FIELD(pathNode, s32 *, 0x14) - g_CamPathOffsetStart[1];
            g_CamPathOffsetDelta[2] = FIELD(pathNode, s32 *, 0x18) - g_CamPathOffsetStart[2];
            pitchDelta = FIELD(pathNode, s32 *, 0) - g_CamPathAngleStart[CAMPATH_PITCH];
            pathYaw = (s32)&g_CamPathAngleDelta[CAMPATH_PITCH];
            *(s32 *)pathYaw = pitchDelta;
            g_CamPathAngleDelta[CAMPATH_YAW] = FIELD(pathNode, s32 *, 4) - g_CamPathAngleStart[CAMPATH_YAW];
            g_CamPathAngleDelta[CAMPATH_ROLL] = FIELD(pathNode, s32 *, 8) - g_CamPathAngleStart[CAMPATH_ROLL];
            g_CamPathAngleDelta[CAMPATH_DIST] = FIELD(pathNode, s32 *, 0xC) - g_CamPathAngleStart[CAMPATH_DIST];
            if (pitchDelta > 0) {
                if (pitchDelta >= 0x800) {
                    *(s32 *)pathYaw = pitchDelta - 0x1000;
                }
            } else if (pitchDelta < -0x7FF) {
                *(s32 *)pathYaw = pitchDelta + 0x1000;
            }
            case3Angle = &g_CamPathAngleDelta[CAMPATH_YAW];
            if (*case3Angle > 0) {
                if (*case3Angle >= 0x800) {
                    *case3Angle -= 0x1000;
                }
            } else if (*case3Angle < -0x7FF) {
                *case3Angle += 0x1000;
            }
            case3Angle = &g_CamPathAngleDelta[CAMPATH_ROLL];
            if (*case3Angle > 0) {
                if (*case3Angle >= 0x800) {
                    *case3Angle -= 0x1000;
                }
            } else if (*case3Angle < -0x7FF) {
                *case3Angle += 0x1000;
            }
        } else if (g_CamPathFrame < FIELD(((g_CamPathNode * 0x24) + g_TrackCameras), s32 *, 0x1C)) {
            g_CamPathFrame += 1;
        }
        pathBlend = 0x1000 - rcos((s32) (g_CamPathFrame << 0xB) / (s32) FIELD(((g_CamPathNode * 0x24) + g_TrackCameras), s32 *, 0x1C));
        offsetXProduct = pathBlend * g_CamPathOffsetDelta[0];
        if (offsetXProduct < 0) {
            offsetXProduct += 0x1FFF;
        }
        camPathOffset = (offsetXProduct >> 0xD) + g_CamPathOffsetStart[0];
        offsetYProduct = pathBlend * g_CamPathOffsetDelta[1];
        sp18[0] = camPathOffset;
        if (offsetYProduct < 0) {
            offsetYProduct += 0x1FFF;
        }
        pathOffsetY = (offsetYProduct >> 0xD) + g_CamPathOffsetStart[1];
        offsetZProduct = pathBlend * g_CamPathOffsetDelta[2];
        sp18[1] = pathOffsetY;
        if (offsetZProduct < 0) {
            offsetZProduct += 0x1FFF;
        }
        pathOffsetZ = (offsetZProduct >> 0xD) + g_CamPathOffsetStart[2];
        pitchProduct = pathBlend * g_CamPathAngleDelta[CAMPATH_PITCH];
        sp18[2] = pathOffsetZ;
        if (pitchProduct < 0) {
            pitchProduct += 0x1FFF;
        }
        pathPitch = (pitchProduct >> 0xD) + g_CamPathAngleStart[CAMPATH_PITCH];
        yawProduct = pathBlend * g_CamPathAngleDelta[CAMPATH_YAW];
        sp38[0] = pathPitch;
        if (yawProduct < 0) {
            yawProduct += 0x1FFF;
        }
        pathYaw = (yawProduct >> 0xD) + g_CamPathAngleStart[CAMPATH_YAW];
        rollProduct = pathBlend * g_CamPathAngleDelta[CAMPATH_ROLL];
        sp38[1] = pathYaw;
        if (rollProduct < 0) {
            rollProduct += 0x1FFF;
        }
        pathRoll = (rollProduct >> 0xD) + g_CamPathAngleStart[CAMPATH_ROLL];
        distProduct = pathBlend * g_CamPathAngleDelta[CAMPATH_DIST];
        sp38[2] = pathRoll;
        if (distProduct < 0) {
            distProduct += 0x1FFF;
        }
        g_CamPathAngle[CAMPATH_PITCH] = pathPitch & 0xFFF;
        g_CamPathAngle[CAMPATH_YAW] = pathYaw & 0xFFF;
        g_CamPathAngle[CAMPATH_ROLL] = pathRoll & 0xFFF;
        g_CamPathOffset[0] = camPathOffset;
        g_CamPathOffset[1] = pathOffsetY;
        g_CamPathOffset[2] = pathOffsetZ;
        camPathAngle = (distProduct >> 0xD) + g_CamPathAngleStart[CAMPATH_DIST];
        sp38[3] = camPathAngle;
        g_CamPathAngle[CAMPATH_DIST] = camPathAngle;
        pathYawRelative = pathYaw - FIELD(car, s32 *, 0x24);
        sp38[1] = pathYawRelative;
        BuildRotMatrixY(&sp88[0], pathYawRelative);
        BuildRotMatrixX(&sp68[0], sp38[0]);
        MulMatrix2(&sp68[0], &sp88[0]);
        BuildRotMatrixZ(&sp68[0], sp38[2]);
        MulMatrix2(&sp68[0], &sp88[0]);
        BuildRotMatrixY(&sp48[0], FIELD(car, s32 *, 0x24));
        BuildRotMatrixX(&sp68[0], FIELD(car, s32 *, 0x20));
        MulMatrix2(&sp68[0], &sp48[0]);
        BuildRotMatrixZ(&sp68[0], FIELD(car, s32 *, 0x28));
        MulMatrix2(&sp68[0], &sp48[0]);
        TransposeMatrix((Matrix *)&sp48[0], (Matrix *)&spA8[0]);
        MulMatrix2(&sp88[0], &sp48[0]);
        TransposeMatrix((Matrix *)&sp48[0], (Matrix *)&sp68[0]);
        sp18[2] += 0x32;
        ApplyMatrixLV(&spA8[0], &sp18[0], &sp38[0]);
        sp28[0] = 0;
        sp28[1] = 0;
        scratch[2] += sp38[0];
        scratch[3] += sp38[1];
        scratch[4] += sp38[2];
        sp28[2] = g_CamPathAngle[CAMPATH_DIST];
        ApplyMatrixLV(&sp68[0], &sp28[0], &sp38[0]);
        scratch[2] -= sp38[0];
        scratch[3] -= sp38[1];
        scratch[4] -= sp38[2];
        pathDistance = SquareRoot0((sp38[0] * sp38[0]) + (sp38[2] * sp38[2]));
        sp38[3] = pathDistance;
        scratch[6] = 0x400 - (Atan2(sp38[1], pathDistance) & 0xFFF);
        scratch[7] = 0x400 - (Atan2(sp38[0], sp38[2]) & 0xFFF);
        scratch[8] = 0;
        sp18[0] = 0x1000;
        sp18[1] = 0;
        sp18[2] = 0;
        BuildRotMatrixY(&sp88[0], 0 - scratch[7]);
        ApplyMatrixLV(&sp88[0], &sp18[0], &sp28[0]);
        TransposeMatrix((Matrix *)&sp68[0], (Matrix *)&sp88[0]);
        ApplyMatrixLV(&sp88[0], &sp28[0], &sp18[0]);
        scratch[8] = 0x400 - (Atan2(sp18[1], sp18[0]) & 0xFFF);
        g_CameraModePrev = 3;
        break;
    case 4:
        case4Base = (u32)g_TrackCameras;
        introNodeOffset = cameraNodeIndex * 0x24;
        introNode = introNodeOffset + case4Base;
        *(Block16 *)&scratch[2] = *(Block16 *)introNode;
        if (((u8)nodeChanged) || (g_CameraModePrev != 4)) {
            g_CamPathFrame = 0;
        } else if (g_CamPathFrame < FIELD((introNodeOffset + (u32)g_TrackCameras), s32 *, 0x1C)) {
            g_CamPathFrame += 1;
        }
        BuildRotMatrixY(&sp48[0], FIELD(car, s32 *, 0x24));
        BuildRotMatrixX(&sp68[0], FIELD(car, s32 *, 0x20));
        MulMatrix2(&sp68[0], &sp48[0]);
        BuildRotMatrixZ(&sp68[0], FIELD(car, s32 *, 0x28));
        MulMatrix2(&sp68[0], &sp48[0]);
        TransposeMatrix((Matrix *)&sp48[0], (Matrix *)&spA8[0]);
        sp18[0] = 0;
        orbitNodeOffset = cameraNodeIndex * 0x24;
        sp18[1] = FIELD((orbitNodeOffset + (u32)g_TrackCameras), s32 *, 0xC);
        sp18[2] = 0x32;
        ApplyMatrixLV(&spA8[0], &sp18[0], &sp28[0]);
        orbitNode = orbitNodeOffset + g_TrackCameras;
        orbitToCarX = FIELD(orbitNode, s32 *, 0x10) - scratch[2];
        sp38[0] = orbitToCarX;
        orbitToCarY = FIELD(orbitNode, s32 *, 0x14) - scratch[3];
        sp38[1] = orbitToCarY;
        orbitToCarZ = FIELD(orbitNode, s32 *, 0x18) - scratch[4];
        sp38[2] = orbitToCarZ;
        scratch[2] += (s32) (orbitToCarX * g_CamPathFrame) / (s32) FIELD(orbitNode, s32 *, 0x1C);
        scratch[3] += (s32) (sp38[1] * g_CamPathFrame) / (s32) FIELD(orbitNode, s32 *, 0x1C);
        scratch[4] += (s32) (sp38[2] * g_CamPathFrame) / (s32) FIELD(orbitNode, s32 *, 0x1C);
        orbitDx = scratch[2] - (FIELD(car, s32 *, 0) + sp28[0]);
        squaredX = orbitDx * orbitDx;
        sp38[0] = orbitDx;
        sp38[1] = scratch[3] - (FIELD(car, s32 *, 4) + sp28[1]);
        orbitDz = scratch[4] - (FIELD(car, s32 *, 8) + sp28[2]);
        sp38[2] = orbitDz;
        squaredZ = orbitDz * orbitDz;
        scratch[6] = 0x400 - (Atan2(0 - sp38[1], SquareRoot0(squaredX + squaredZ)) & 0xFFF);
        chaseYaw = 0x400 - (Atan2(0 - sp38[0], 0 - sp38[2]) & 0xFFF);
        nextPrevMode = 4;
        scratch[7] = chaseYaw;
        scratch[8] = 0;
        g_CameraModePrev = nextPrevMode;
        break;
    case 5:
        *(Block16 *)&scratch[2] = *(Block16 *)car;
        BuildRotMatrixY(&sp88[0], 0 - g_OrbitCameraYaw);
        BuildRotMatrixY(&sp48[0], FIELD(car, s32 *, 0x24));
        BuildRotMatrixX(&sp68[0], FIELD(car, s32 *, 0x20));
        MulMatrix2(&sp68[0], &sp48[0]);
        BuildRotMatrixZ(&sp68[0], FIELD(car, s32 *, 0x28));
        MulMatrix2(&sp68[0], &sp48[0]);
        TransposeMatrix((Matrix *)&sp48[0], (Matrix *)&spA8[0]);
        MulMatrix2(&sp88[0], &sp48[0]);
        TransposeMatrix((Matrix *)&sp48[0], (Matrix *)&sp68[0]);
        sp18[0] = 0;
        sp18[1] = 0;
        sp18[2] = 0x32;
        ApplyMatrixLV(&spA8[0], &sp18[0], &sp28[0]);
        sp18[0] = 0;
        sp18[1] = 0;
        scratch[2] += sp28[0];
        scratch[3] += sp28[1];
        scratch[4] += sp28[2];
        sp18[2] = g_OrbitCameraDistance;
        ApplyMatrixLV(&sp68[0], &sp18[0], &sp38[0]);
        scratch[6] = 0x400 - (Atan2(sp38[1], g_OrbitCameraDistance) & 0xFFF);
        scratch[7] = 0x400 - (Atan2(sp38[0], sp38[2]) & 0xFFF);
        scratch[8] = FIELD(car, s32 *, 0x28);
        g_CameraModePrev = 5;
        scratch[2] -= sp38[0];
        adjustedY = scratch[3] - 0x28;
        scratch[3] = adjustedY - sp38[1];
        scratch[4] -= sp38[2];
        break;
    }
    SetCameraRotMatrix();
    if ((cameraModeSel > 0) && (car == &g_PlayerCar)) {
        SelectModelBank(0);
        DrawPlayerCarModel(car);
    }
}

/* Deliberately raw: the environment script header word 0 is stored here and
 * never read anywhere in the image (docs/names.md 15g). */
extern u32 g_SkyRowBase;
extern u32 g_EnvScriptLength;

void SetEnvironmentScript(u32 *script) {
    u32 value0;
    u32 value1;

    value0 = *script;
    script++;
    g_SkyRowBase = value0;
    value1 = *script;
    script++;
    g_EnvScriptCues = script;
    g_EnvScriptLength = value1;
}
