#include "common.h"
#include "game/render.h"
#include "game/scratchpad.h"
#include "game/track.h"
#include "game/vector.h"
#include "psyq/gte.h"

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

extern void DrawPlayerCarModel(void *);
extern s32 rcos(s32);
extern s32 SquareRoot0(s32);
extern void *ApplyMatrixLV(void *, void *, void *);
extern void *TransposeMatrixWide(void *, void *) asm("TransposeMatrix");
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
 * (chase / bumper / replay-orbit / intro-pan, etc.); `arg1` is the followed
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
    s32 var_v1_44;
    s32 chaseTargetYaw;
    s32 temp_a0_272;
    s32 temp_a1_1117;
    s32 temp_a1_1227;
    s32 temp_a1_230;
    s32 temp_a1_781;
    s32 temp_a2_1183;
    s32 temp_a3_1170;
    s32 temp_a3_1372;
    s32 squaredX;
    s32 squaredZ;
    s32 temp_s0_1437;
    s32 temp_s2_728;
    s32 temp_t0_1157;
    s32 temp_t1_1144;
    s32 camPathOffset;
    s32 camPathAngle;
    s32 temp_v0_1320;
    s32 temp_v0_1452;
    s32 temp_v0_1458;
    s32 temp_v0_1463;
    s32 temp_v0_1535;
    s32 temp_v0_1549;
    s32 chaseYawDamping;
    s32 chaseYawStepLimit;
    s32 cameraNodeIndex;
    s32 temp_v0_340;
    s32 temp_v0_399;
    s32 temp_v0_466;
    s32 chaseYawLag;
    s32 temp_v0_687;
    s32 temp_v0_845;
    s32 temp_v0_859;
    s32 temp_v1_1196;
    s32 temp_v1_35;
    u8 temp_v1_40;
    s32 chaseCarSpeed;
    s32 var_a0_1119;
    s32 var_a0_1132;
    s32 var_a0_1145;
    s32 var_a0_1158;
    s32 var_a0_1171;
    s32 var_a0_1184;
    s32 var_a0_1197;
    s32 var_a1_886;
    s32 var_s0_879;
    s32 var_v0_652;
    s32 var_v0_713;
    s32 var_v1_549;
    u8 var_v0_881;
    void *temp_a0_976;
    void *temp_s0_1448;
    void *temp_s2_794;
    void *temp_v0_732;
    void *temp_v0_764;
    void *temp_v0_944;
    u32 temp_v1_1373;

    cameraNodeIndex = FindNearestTrackCamera(car);
    scratch = &SCRATCH_PRIM_CURSOR_WORD;
    temp_v1_35 = g_CameraNodeIndex;
    g_CameraNodeIndex = cameraNodeIndex;
    temp_v1_40 = cameraNodeIndex != temp_v1_35;
    if (cameraModeSel < 2) {
        var_v1_44 = cameraModeSel;
    } else {
        var_v1_44 = FIELD(((cameraNodeIndex * 0x24) + g_TrackCameras), s16 *, 0x20);
    }
    switch (var_v1_44) {
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
            temp_a1_230 = 0x4E2 - g_ChaseCarSpeed;
            g_ChaseYawDamping = temp_a1_230;
        g_ChaseYawDamping = ((((g_ChaseYawDamping * 6 * temp_a1_230) / 2500) - ((temp_a1_230 * 0x46) / 50)) + 0xE0) / 10;
        }
        temp_a0_272 = g_ChaseTargetYaw - D_8009B1EC;
        if (temp_a0_272 >= 5) {
            if (temp_a0_272 >= 0x800) {
                chaseYawStepLimit = (((0x1000 - temp_a0_272) / 17) * 2) & 0xFFF;
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
                    var_v0_713 = 0 - turnAccel;
                    goto block_36;
                }
                goto block_34;
            }
            temp_v0_340 = (((g_ChaseTargetYaw - D_8009B1EC) / 17) * 2) & 0xFFF;
            g_ChaseYawStepLimit = temp_v0_340;
            if (temp_v0_340 >= 0x41) {
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
        if (temp_a0_272 < -4) {
            if (temp_a0_272 < -0x7FF) {
                temp_v0_399 = (((0x1000 - (D_8009B1EC - g_ChaseTargetYaw)) / 17) * 2) & 0xFFF;
                g_ChaseYawStepLimit = temp_v0_399;
                if (temp_v0_399 >= 0x41) {
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
                temp_v0_466 = (((D_8009B1EC - g_ChaseTargetYaw) / 17) * 2) & 0xFFF;
                g_ChaseYawStepLimit = temp_v0_466;
                if (temp_v0_466 >= 0x41) {
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
                    var_v0_713 = 0 - turnAccel;
block_36:
                    g_ChaseYawLag = var_v0_713;
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
            var_v1_549 = chaseYawLag;
            if (var_v1_549 < -0x7FF) {
                var_v1_549 += 0x1000;
            }
            angleState = &g_ChaseYawLag;
            *angleState = var_v1_549;
        } else {
            chaseYawLag = rawAngle - chaseCarSpeed;
            g_ChaseYawLag = chaseYawLag;
            var_v1_549 = chaseYawLag;
            if (var_v1_549 >= 0x800) {
                var_v1_549 -= 0x1000;
            }
            angleState = &g_ChaseYawLag;
            *angleState = var_v1_549;
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
        TransposeMatrixWide(&sp48[0], &spA8[0]);
        MulMatrix2(angleState, &sp48[0]);
        TransposeMatrixWide(&sp48[0], &sp68[0]);
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
            var_v0_652 = 0x118;
            goto block_52;
        case 1:
            sp18[1] = 0x59;
            var_v0_652 = 0x140;
            goto block_52;
        case 2:
            sp18[1] = 0x97;
            var_v0_652 = 0x190;
block_52:
            sp18[2] = var_v0_652;
            break;
        }
        ApplyMatrixLV(&sp68[0], &sp18[0], &sp38[0]);
        scratch[2] -= sp38[0];
        scratch[3] -= sp38[1];
        scratch[4] -= sp38[2];
        temp_v0_687 = SquareRoot0((sp38[0] * sp38[0]) + (sp38[2] * sp38[2]));
        sp38[3] = temp_v0_687;
        scratch[6] = 0x400 - (Atan2(sp38[1] + 0x28, temp_v0_687) & 0xFFF);
        scratch[7] = 0x400 - (Atan2(sp38[0], sp38[2]) & 0xFFF);
        scratch[8] = FIELD(car, s32 *, 0x28) - FIELD(car, s32 *, 0x64);
        if (g_ChaseCameraPreset == 0) {
            var_v0_713 = scratch[6] - 0x90;
        } else {
            var_v0_713 = scratch[6] - 0x60;
        }
        scratch[6] = var_v0_713;
        g_CameraModePrev = 1;
        break;
    case 2:
        temp_s2_728 = cameraNodeIndex * 0x24;
        temp_v0_732 = temp_s2_728 + g_TrackCameras;
        *(Block16 *)&scratch[2] = *(Block16 *)temp_v0_732;
        BuildRotMatrixY(&sp48[0], FIELD(car, s32 *, 0x24));
        BuildRotMatrixX(&sp68[0], FIELD(car, s32 *, 0x20));
        MulMatrix2(&sp68[0], &sp48[0]);
        BuildRotMatrixZ(&sp68[0], FIELD(car, s32 *, 0x28));
        MulMatrix2(&sp68[0], &sp48[0]);
        TransposeMatrixWide(&sp48[0], &spA8[0]);
        temp_v0_764 = temp_s2_728 + g_TrackCameras;
        sp18[0] = FIELD(temp_v0_764, s32 *, 0x10);
        sp18[1] = FIELD(temp_v0_764, s32 *, 0x14);
        sp18[2] = FIELD(temp_v0_764, s32 *, 0x18) + 0x32;
        ApplyMatrixLV(&spA8[0], &sp18[0], &sp28[0]);
        temp_a1_781 = scratch[2] - (FIELD(car, s32 *, 0) + sp28[0]);
        sp38[0] = temp_a1_781;
        sp38[1] = scratch[3] - (FIELD(car, s32 *, 4) + sp28[1]);
        temp_s2_794 = temp_s2_728 + g_TrackCameras;
        sp38[2] = scratch[4] - (FIELD(car, s32 *, 8) + sp28[2]);
        scratch[2] -= (temp_a1_781 * FIELD(temp_s2_794, s32 *, 0xC)) / 10000;
        scratch[3] -= (sp38[1] * FIELD(temp_s2_794, s32 *, 0xC)) / 10000;
        scratch[4] -= (sp38[2] * FIELD(temp_s2_794, s32 *, 0xC)) / 10000;
        temp_v0_845 = scratch[2] - (FIELD(car, s32 *, 0) + sp28[0]);
        squaredX = temp_v0_845 * temp_v0_845;
        sp38[0] = temp_v0_845;
        sp38[1] = scratch[3] - (FIELD(car, s32 *, 4) + sp28[1]);
        temp_v0_859 = scratch[4] - (FIELD(car, s32 *, 8) + sp28[2]);
        sp38[2] = temp_v0_859;
        squaredZ = temp_v0_859 * temp_v0_859;
        scratch[6] = 0x400 - (Atan2(0 - sp38[1], SquareRoot0(squaredX + squaredZ)) & 0xFFF);
        var_s0_879 = 0x400 - (Atan2(0 - sp38[0], 0 - sp38[2]) & 0xFFF);
        var_v0_881 = 2;
        scratch[7] = var_s0_879;
        scratch[8] = 0;
        g_CameraModePrev = var_v0_881;
        break;
    case 3:
        *(Block16 *)&scratch[2] = *(Block16 *)car;
        if (((u8)temp_v1_40) || (g_CameraModePrev != 3)) {
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
                temp_v0_944 = (cameraNodeIndex * 0x24) + g_TrackCameras;
                g_CamPathOffsetStart[0] = FIELD(temp_v0_944, s32 *, 0x10);
                g_CamPathOffsetStart[1] = FIELD(temp_v0_944, s32 *, 0x14);
                g_CamPathOffsetStart[2] = FIELD(temp_v0_944, s32 *, 0x18);
                g_CamPathAngleStart[CAMPATH_PITCH] = FIELD(temp_v0_944, s32 *, 0);
                g_CamPathAngleStart[CAMPATH_YAW] = FIELD(temp_v0_944, s32 *, 4);
                g_CamPathAngleStart[CAMPATH_ROLL] = FIELD(temp_v0_944, s32 *, 8);
                g_CamPathAngleStart[CAMPATH_DIST] = FIELD(temp_v0_944, s32 *, 0xC);
            }
            temp_a0_976 = (g_CamPathNode * 0x24) + g_TrackCameras;
            g_CamPathOffsetDelta[0] = FIELD(temp_a0_976, s32 *, 0x10) - g_CamPathOffsetStart[0];
            g_CamPathOffsetDelta[1] = FIELD(temp_a0_976, s32 *, 0x14) - g_CamPathOffsetStart[1];
            g_CamPathOffsetDelta[2] = FIELD(temp_a0_976, s32 *, 0x18) - g_CamPathOffsetStart[2];
            var_a1_886 = FIELD(temp_a0_976, s32 *, 0) - g_CamPathAngleStart[CAMPATH_PITCH];
            temp_a2_1183 = (s32)&g_CamPathAngleDelta[CAMPATH_PITCH];
            *(s32 *)temp_a2_1183 = var_a1_886;
            g_CamPathAngleDelta[CAMPATH_YAW] = FIELD(temp_a0_976, s32 *, 4) - g_CamPathAngleStart[CAMPATH_YAW];
            g_CamPathAngleDelta[CAMPATH_ROLL] = FIELD(temp_a0_976, s32 *, 8) - g_CamPathAngleStart[CAMPATH_ROLL];
            g_CamPathAngleDelta[CAMPATH_DIST] = FIELD(temp_a0_976, s32 *, 0xC) - g_CamPathAngleStart[CAMPATH_DIST];
            if (var_a1_886 > 0) {
                if (var_a1_886 >= 0x800) {
                    *(s32 *)temp_a2_1183 = var_a1_886 - 0x1000;
                }
            } else if (var_a1_886 < -0x7FF) {
                *(s32 *)temp_a2_1183 = var_a1_886 + 0x1000;
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
        temp_a1_1117 = 0x1000 - rcos((s32) (g_CamPathFrame << 0xB) / (s32) FIELD(((g_CamPathNode * 0x24) + g_TrackCameras), s32 *, 0x1C));
        var_a0_1119 = temp_a1_1117 * g_CamPathOffsetDelta[0];
        if (var_a0_1119 < 0) {
            var_a0_1119 += 0x1FFF;
        }
        camPathOffset = (var_a0_1119 >> 0xD) + g_CamPathOffsetStart[0];
        var_a0_1132 = temp_a1_1117 * g_CamPathOffsetDelta[1];
        sp18[0] = camPathOffset;
        if (var_a0_1132 < 0) {
            var_a0_1132 += 0x1FFF;
        }
        temp_t1_1144 = (var_a0_1132 >> 0xD) + g_CamPathOffsetStart[1];
        var_a0_1145 = temp_a1_1117 * g_CamPathOffsetDelta[2];
        sp18[1] = temp_t1_1144;
        if (var_a0_1145 < 0) {
            var_a0_1145 += 0x1FFF;
        }
        temp_t0_1157 = (var_a0_1145 >> 0xD) + g_CamPathOffsetStart[2];
        var_a0_1158 = temp_a1_1117 * g_CamPathAngleDelta[CAMPATH_PITCH];
        sp18[2] = temp_t0_1157;
        if (var_a0_1158 < 0) {
            var_a0_1158 += 0x1FFF;
        }
        temp_a3_1170 = (var_a0_1158 >> 0xD) + g_CamPathAngleStart[CAMPATH_PITCH];
        var_a0_1171 = temp_a1_1117 * g_CamPathAngleDelta[CAMPATH_YAW];
        sp38[0] = temp_a3_1170;
        if (var_a0_1171 < 0) {
            var_a0_1171 += 0x1FFF;
        }
        temp_a2_1183 = (var_a0_1171 >> 0xD) + g_CamPathAngleStart[CAMPATH_YAW];
        var_a0_1184 = temp_a1_1117 * g_CamPathAngleDelta[CAMPATH_ROLL];
        sp38[1] = temp_a2_1183;
        if (var_a0_1184 < 0) {
            var_a0_1184 += 0x1FFF;
        }
        temp_v1_1196 = (var_a0_1184 >> 0xD) + g_CamPathAngleStart[CAMPATH_ROLL];
        var_a0_1197 = temp_a1_1117 * g_CamPathAngleDelta[CAMPATH_DIST];
        sp38[2] = temp_v1_1196;
        if (var_a0_1197 < 0) {
            var_a0_1197 += 0x1FFF;
        }
        g_CamPathAngle[CAMPATH_PITCH] = temp_a3_1170 & 0xFFF;
        g_CamPathAngle[CAMPATH_YAW] = temp_a2_1183 & 0xFFF;
        g_CamPathAngle[CAMPATH_ROLL] = temp_v1_1196 & 0xFFF;
        g_CamPathOffset[0] = camPathOffset;
        g_CamPathOffset[1] = temp_t1_1144;
        g_CamPathOffset[2] = temp_t0_1157;
        camPathAngle = (var_a0_1197 >> 0xD) + g_CamPathAngleStart[CAMPATH_DIST];
        sp38[3] = camPathAngle;
        g_CamPathAngle[CAMPATH_DIST] = camPathAngle;
        temp_a1_1227 = temp_a2_1183 - FIELD(car, s32 *, 0x24);
        sp38[1] = temp_a1_1227;
        BuildRotMatrixY(&sp88[0], temp_a1_1227);
        BuildRotMatrixX(&sp68[0], sp38[0]);
        MulMatrix2(&sp68[0], &sp88[0]);
        BuildRotMatrixZ(&sp68[0], sp38[2]);
        MulMatrix2(&sp68[0], &sp88[0]);
        BuildRotMatrixY(&sp48[0], FIELD(car, s32 *, 0x24));
        BuildRotMatrixX(&sp68[0], FIELD(car, s32 *, 0x20));
        MulMatrix2(&sp68[0], &sp48[0]);
        BuildRotMatrixZ(&sp68[0], FIELD(car, s32 *, 0x28));
        MulMatrix2(&sp68[0], &sp48[0]);
        TransposeMatrixWide(&sp48[0], &spA8[0]);
        MulMatrix2(&sp88[0], &sp48[0]);
        TransposeMatrixWide(&sp48[0], &sp68[0]);
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
        temp_v0_1320 = SquareRoot0((sp38[0] * sp38[0]) + (sp38[2] * sp38[2]));
        sp38[3] = temp_v0_1320;
        scratch[6] = 0x400 - (Atan2(sp38[1], temp_v0_1320) & 0xFFF);
        scratch[7] = 0x400 - (Atan2(sp38[0], sp38[2]) & 0xFFF);
        scratch[8] = 0;
        sp18[0] = 0x1000;
        sp18[1] = 0;
        sp18[2] = 0;
        BuildRotMatrixY(&sp88[0], 0 - scratch[7]);
        ApplyMatrixLV(&sp88[0], &sp18[0], &sp28[0]);
        TransposeMatrixWide(&sp68[0], &sp88[0]);
        ApplyMatrixLV(&sp88[0], &sp28[0], &sp18[0]);
        scratch[8] = 0x400 - (Atan2(sp18[1], sp18[0]) & 0xFFF);
        g_CameraModePrev = 3;
        break;
    case 4:
        case4Base = (u32)g_TrackCameras;
        temp_a3_1372 = cameraNodeIndex * 0x24;
        temp_v1_1373 = temp_a3_1372 + case4Base;
        *(Block16 *)&scratch[2] = *(Block16 *)temp_v1_1373;
        if (((u8)temp_v1_40) || (g_CameraModePrev != 4)) {
            g_CamPathFrame = 0;
        } else if (g_CamPathFrame < FIELD((temp_a3_1372 + (u32)g_TrackCameras), s32 *, 0x1C)) {
            g_CamPathFrame += 1;
        }
        BuildRotMatrixY(&sp48[0], FIELD(car, s32 *, 0x24));
        BuildRotMatrixX(&sp68[0], FIELD(car, s32 *, 0x20));
        MulMatrix2(&sp68[0], &sp48[0]);
        BuildRotMatrixZ(&sp68[0], FIELD(car, s32 *, 0x28));
        MulMatrix2(&sp68[0], &sp48[0]);
        TransposeMatrixWide(&sp48[0], &spA8[0]);
        sp18[0] = 0;
        temp_s0_1437 = cameraNodeIndex * 0x24;
        sp18[1] = FIELD((temp_s0_1437 + (u32)g_TrackCameras), s32 *, 0xC);
        sp18[2] = 0x32;
        ApplyMatrixLV(&spA8[0], &sp18[0], &sp28[0]);
        temp_s0_1448 = temp_s0_1437 + g_TrackCameras;
        temp_v0_1452 = FIELD(temp_s0_1448, s32 *, 0x10) - scratch[2];
        sp38[0] = temp_v0_1452;
        temp_v0_1458 = FIELD(temp_s0_1448, s32 *, 0x14) - scratch[3];
        sp38[1] = temp_v0_1458;
        temp_v0_1463 = FIELD(temp_s0_1448, s32 *, 0x18) - scratch[4];
        sp38[2] = temp_v0_1463;
        scratch[2] += (s32) (temp_v0_1452 * g_CamPathFrame) / (s32) FIELD(temp_s0_1448, s32 *, 0x1C);
        scratch[3] += (s32) (sp38[1] * g_CamPathFrame) / (s32) FIELD(temp_s0_1448, s32 *, 0x1C);
        scratch[4] += (s32) (sp38[2] * g_CamPathFrame) / (s32) FIELD(temp_s0_1448, s32 *, 0x1C);
        temp_v0_1535 = scratch[2] - (FIELD(car, s32 *, 0) + sp28[0]);
        squaredX = temp_v0_1535 * temp_v0_1535;
        sp38[0] = temp_v0_1535;
        sp38[1] = scratch[3] - (FIELD(car, s32 *, 4) + sp28[1]);
        temp_v0_1549 = scratch[4] - (FIELD(car, s32 *, 8) + sp28[2]);
        sp38[2] = temp_v0_1549;
        squaredZ = temp_v0_1549 * temp_v0_1549;
        scratch[6] = 0x400 - (Atan2(0 - sp38[1], SquareRoot0(squaredX + squaredZ)) & 0xFFF);
        var_s0_879 = 0x400 - (Atan2(0 - sp38[0], 0 - sp38[2]) & 0xFFF);
        var_v0_881 = 4;
        scratch[7] = var_s0_879;
        scratch[8] = 0;
        g_CameraModePrev = var_v0_881;
        break;
    case 5:
        *(Block16 *)&scratch[2] = *(Block16 *)car;
        BuildRotMatrixY(&sp88[0], 0 - g_OrbitCameraYaw);
        BuildRotMatrixY(&sp48[0], FIELD(car, s32 *, 0x24));
        BuildRotMatrixX(&sp68[0], FIELD(car, s32 *, 0x20));
        MulMatrix2(&sp68[0], &sp48[0]);
        BuildRotMatrixZ(&sp68[0], FIELD(car, s32 *, 0x28));
        MulMatrix2(&sp68[0], &sp48[0]);
        TransposeMatrixWide(&sp48[0], &spA8[0]);
        MulMatrix2(&sp88[0], &sp48[0]);
        TransposeMatrixWide(&sp48[0], &sp68[0]);
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
