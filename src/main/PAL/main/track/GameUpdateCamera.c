#include "common.h"
#include "psyq/gte.h"
#include "game/render.h"

typedef struct {
    s32 words[4];
} WordVector;

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

extern void GameDrawPlayerCarModel(void *) asm("func_8001DAB0");
extern s32 GameFindNearestTrackCamera() asm("func_80043B18");
extern s32 func_80068634(s32);
extern s32 func_800689A8(s32);
extern void *ApplyMatrixLV(void *, void *, void *) asm("func_80068F80");
extern void *func_80069CC8(void *, void *);
/* Chase-view (mode 1) distance preset, 0..2: eye height 0x3A / 0x59 / 0x97 and
 * pull-back 0x118 / 0x140 / 0x190, plus a -0x90 vs -0x60 pitch bias. Sits in
 * .data at 0, and nothing in the image ever writes it, so retail is always 0. */
extern s32 g_ChaseCameraPreset asm("D_8007F610");
/* Mode-5 orbit camera: a fixed yaw about the car and a fixed pull-back.
 * Both are .data constants with no writer anywhere: 0 and 330 (0x14A). */
extern s32 g_OrbitCameraYaw asm("D_8007F614");
extern s32 g_OrbitCameraDistance asm("D_8007F618");
/* Mode-3 camera path: the eye is eased from one track-camera node to the next
 * over `node->duration` frames. Each of offset (a local xyz applied through the
 * car's matrix) and orientation (pitch/yaw/roll/distance) keeps a start value,
 * a delta to the destination and the current interpolated value. Mode-3 nodes
 * therefore store angles in the first four words where modes 2/4 store a world
 * position -- the record is a union keyed on `node->mode`. */
extern s32 g_CamPathOffsetDeltaX asm("D_8009B1B8");
extern s32 g_CamPathOffsetDeltaY asm("D_8009B1BC");
extern s32 g_CamPathOffsetDeltaZ asm("D_8009B1C0");
extern s32 g_CamPathOffsetStartX asm("D_8009B1C8");
extern s32 g_CamPathOffsetStartY asm("D_8009B1CC");
extern s32 g_CamPathOffsetStartZ asm("D_8009B1D0");
extern s32 g_CamPathOffsetX asm("D_8009B1D8");
extern s32 g_CamPathOffsetY asm("D_8009B1DC");
extern s32 g_CamPathOffsetZ asm("D_8009B1E0");
extern s32 g_CamPathPitchDelta asm("D_8009B1E8");
/* Left raw on purpose: this one slot carries two unrelated quantities. Mode 3
 * uses it as the yaw delta of the camera path (between g_CamPathPitchDelta and
 * g_CamPathRollDelta); mode 1 uses it as the chase camera's yaw carried over
 * from the previous frame. No single name is honest for both. */
extern s32 D_8009B1EC;
extern s32 g_CamPathRollDelta asm("D_8009B1F0");
extern s32 g_CamPathDistDelta asm("D_8009B1F4");
extern s32 g_CamPathPitchStart asm("D_8009B1F8");
extern s32 g_CamPathYawStart asm("D_8009B1FC");
extern s32 g_CamPathRollStart asm("D_8009B200");
extern s32 g_CamPathDistStart asm("D_8009B204");
extern s32 g_CamPathPitch asm("D_8009B208");
extern s32 g_CamPathYaw asm("D_8009B20C");
extern s32 g_CamPathRoll asm("D_8009B210");
extern s32 g_CamPathDist asm("D_8009B214");
/* Camera mode the previous frame ran (0..5); every case ends by writing it. */
extern u8 g_CameraModePrev asm("D_8009B218");
/* Mode-1 chase smoothing. The target is the car heading; the camera walks
 * towards it by a quadratic ramp (ramp += 8 each frame, step = ramp^2/damping),
 * clamped to g_ChaseYawStepLimit. Damping falls from ~222 at rest to 1 at top
 * speed, i.e. the camera snaps to the car the faster it goes. */
extern s32 g_ChaseTargetYaw asm("D_8009B21C");
extern s32 g_ChaseYaw asm("D_8009B220");
extern s32 g_ChaseYawLag asm("D_8009B224");
extern s32 g_ChaseYawRampNeg asm("D_8009B228");
extern s32 g_ChaseYawRampPos asm("D_8009B22C");
extern s32 g_ChaseYawStepLimit asm("D_8009B230");
extern s32 g_ChaseYawStep asm("D_8009B234");
extern s32 g_ChaseYawDamping asm("D_8009B238");
extern s32 g_ChaseCarSpeed asm("D_8009B23C");
/* Track-camera node nearest the followed car this frame; a change arms the
 * mode-3 / mode-4 hand-over. g_CamPathNode is the node being eased towards and
 * g_CamPathFrame the elapsed frames of that ease. */
extern s32 g_CameraNodeIndex asm("D_8009B240");
extern s32 g_CamPathFrame asm("D_8009B244");
extern s32 g_CamPathNode asm("D_8009B248");
extern s32 g_PlayerCar asm("D_8009E6D4");
extern u8 *g_TrackCameras asm("D_8019C7CC");

/*
 * Camera-mode state machine: `cameraModeSel` selects among the camera behaviours
 * (chase / bumper / replay-orbit / intro-pan, etc.); `arg1` is the followed
 * render/target object. Positions the eye and fills the scratchpad view state
 * (view[2..8]) with matrix/atan2/sqrt math, then submits via GameDrawPlayerCarModel.
 * Field accesses use the FIELD(base,type,offset) raw-offset macro to stay
 * byte-exact, so params/locals are not retyped.
 */
void GameUpdateCamera(s32 cameraModeSel, void *arg1) asm("func_80043BCC");
void GameUpdateCamera(s32 cameraModeSel, void *arg1) {
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
    s32 temp_a0_157;
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
    s32 temp_t2_1131;
    s32 temp_v0_1221;
    s32 temp_v0_1320;
    s32 temp_v0_1452;
    s32 temp_v0_1458;
    s32 temp_v0_1463;
    s32 temp_v0_1535;
    s32 temp_v0_1549;
    s32 temp_v0_200;
    s32 temp_v0_288;
    s32 temp_v0_30;
    s32 temp_v0_340;
    s32 temp_v0_399;
    s32 temp_v0_466;
    s32 temp_v0_546;
    s32 temp_v0_687;
    s32 temp_v0_845;
    s32 temp_v0_859;
    s32 temp_v1_1196;
    s32 temp_v1_35;
    u8 temp_v1_40;
    s32 temp_v1_541;
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

    temp_v0_30 = GameFindNearestTrackCamera(arg1);
    scratch = (s32 *)0x1F800000;
    temp_v1_35 = g_CameraNodeIndex;
    g_CameraNodeIndex = temp_v0_30;
    temp_v1_40 = temp_v0_30 != temp_v1_35;
    if (cameraModeSel < 2) {
        var_v1_44 = cameraModeSel;
    } else {
        var_v1_44 = FIELD(((temp_v0_30 * 0x24) + g_TrackCameras), s16 *, 0x20);
    }
    switch (var_v1_44) {                            /* switch 1 */
    case 0:                                         /* switch 1 */
        *(WordVector *)&scratch[2] = *(WordVector *)arg1;
        *(WordVector *)&scratch[6] = *(WordVector *)((u8 *)arg1 + 0x20);
        GameBuildRotMatrixY(&sp48[0], scratch[7]);
        GameBuildRotMatrixX(&sp68[0], scratch[6]);
        MulMatrix2(&sp68[0], &sp48[0]);
        GameBuildRotMatrixZ(&sp68[0], scratch[8]);
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
        scratch[6] += FIELD(arg1, s16 *, 0x8C);
        g_CameraModePrev = 0;
        break;
    case 1:                                         /* switch 1 */
        *(WordVector *)&scratch[2] = *(WordVector *)arg1;
        temp_v0_200 = FIELD(arg1, s32 *, 0x24);
        temp_a0_157 = temp_v0_200 & 0xFFF;
        temp_v1_541 = FIELD(arg1, s32 *, 0xA4);
        g_ChaseCarSpeed = temp_v1_541;
        previousMode = g_CameraModePrev;
        g_ChaseTargetYaw = temp_a0_157;
        if (previousMode == 1) {
            modeAngle = &D_8009B1EC;
            *modeAngle &= 0xFFF;
            g_ChaseYawRampNeg &= 0xFFF;
            g_ChaseYawRampPos &= 0xFFF;
        } else {
            D_8009B1EC = temp_a0_157;
            g_ChaseYawRampNeg = 0;
            g_ChaseYawRampPos = 0;
        }
        if (g_ChaseCarSpeed >= 0x321) {
            temp_v0_200 = 0x4E2 - g_ChaseCarSpeed;
            g_ChaseYawDamping = temp_v0_200;
            if (temp_v0_200 < 6) {
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
                temp_v0_288 = (((0x1000 - temp_a0_272) / 17) * 2) & 0xFFF;
                g_ChaseYawStepLimit = temp_v0_288;
                if (temp_v0_288 >= 0x41) {
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
                    g_ChaseYawRampPos = func_800689A8(turnLimit * turnFactor);
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
                    g_ChaseYawRampNeg = func_800689A8(turnLimit * turnFactor);
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
        temp_v1_541 = (D_8009B1EC + g_ChaseYawLag) & 0xFFF;
        g_ChaseYaw = temp_v1_541;
        if (rawAngle < temp_v1_541) {
            temp_v0_546 = rawAngle - temp_v1_541;
            g_ChaseYawLag = temp_v0_546;
            var_v1_549 = temp_v0_546;
            if (var_v1_549 < -0x7FF) {
                var_v1_549 += 0x1000;
            }
            angleState = &g_ChaseYawLag;
            *angleState = var_v1_549;
        } else {
            temp_v0_546 = rawAngle - temp_v1_541;
            g_ChaseYawLag = temp_v0_546;
            var_v1_549 = temp_v0_546;
            if (var_v1_549 >= 0x800) {
                var_v1_549 -= 0x1000;
            }
            angleState = &g_ChaseYawLag;
            *angleState = var_v1_549;
        }
        angleState = (s32 *)&sp88[0];
        GameBuildRotMatrixY(angleState, 0 - g_ChaseYawLag);
        GameBuildRotMatrixX(&sp68[0], -0x80);
        MulMatrix2(&sp68[0], &sp88[0]);
        D_8009B1EC = g_ChaseYaw;
        GameBuildRotMatrixY(&sp48[0], FIELD(arg1, s32 *, 0x24));
        GameBuildRotMatrixX(&sp68[0], FIELD(arg1, s32 *, 0x20));
        MulMatrix2(&sp68[0], &sp48[0]);
        GameBuildRotMatrixZ(&sp68[0], FIELD(arg1, s32 *, 0x28));
        MulMatrix2(&sp68[0], &sp48[0]);
        func_80069CC8(&sp48[0], &spA8[0]);
        MulMatrix2(angleState, &sp48[0]);
        func_80069CC8(&sp48[0], &sp68[0]);
        sp18[1] = -0x3C;
        sp18[0] = 0;
        sp18[2] = 0x32;
        ApplyMatrixLV(&spA8[0], &sp18[0], &sp28[0]);
        sp18[0] = 0;
        scratch[2] += sp28[0];
        scratch[3] += sp28[1];
        scratch[4] += sp28[2];
        switch (g_ChaseCameraPreset) {                       /* switch 2; irregular */
        case 0:                                     /* switch 2 */
            sp18[1] = 0x3A;
            var_v0_652 = 0x118;
            goto block_52;
        case 1:                                     /* switch 2 */
            sp18[1] = 0x59;
            var_v0_652 = 0x140;
            goto block_52;
        case 2:                                     /* switch 2 */
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
        temp_v0_687 = func_800689A8((sp38[0] * sp38[0]) + (sp38[2] * sp38[2]));
        sp38[3] = temp_v0_687;
        scratch[6] = 0x400 - (GameAtan2(sp38[1] + 0x28, temp_v0_687) & 0xFFF);
        scratch[7] = 0x400 - (GameAtan2(sp38[0], sp38[2]) & 0xFFF);
        scratch[8] = FIELD(arg1, s32 *, 0x28) - FIELD(arg1, s32 *, 0x64);
        if (g_ChaseCameraPreset == 0) {
            var_v0_713 = scratch[6] - 0x90;
        } else {
            var_v0_713 = scratch[6] - 0x60;
        }
        scratch[6] = var_v0_713;
        g_CameraModePrev = 1;
        break;
    case 2:                                         /* switch 1 */
        temp_s2_728 = temp_v0_30 * 0x24;
        temp_v0_732 = temp_s2_728 + g_TrackCameras;
        *(WordVector *)&scratch[2] = *(WordVector *)temp_v0_732;
        GameBuildRotMatrixY(&sp48[0], FIELD(arg1, s32 *, 0x24));
        GameBuildRotMatrixX(&sp68[0], FIELD(arg1, s32 *, 0x20));
        MulMatrix2(&sp68[0], &sp48[0]);
        GameBuildRotMatrixZ(&sp68[0], FIELD(arg1, s32 *, 0x28));
        MulMatrix2(&sp68[0], &sp48[0]);
        func_80069CC8(&sp48[0], &spA8[0]);
        temp_v0_764 = temp_s2_728 + g_TrackCameras;
        sp18[0] = FIELD(temp_v0_764, s32 *, 0x10);
        sp18[1] = FIELD(temp_v0_764, s32 *, 0x14);
        sp18[2] = FIELD(temp_v0_764, s32 *, 0x18) + 0x32;
        ApplyMatrixLV(&spA8[0], &sp18[0], &sp28[0]);
        temp_a1_781 = scratch[2] - (FIELD(arg1, s32 *, 0) + sp28[0]);
        sp38[0] = temp_a1_781;
        sp38[1] = scratch[3] - (FIELD(arg1, s32 *, 4) + sp28[1]);
        temp_s2_794 = temp_s2_728 + g_TrackCameras;
        sp38[2] = scratch[4] - (FIELD(arg1, s32 *, 8) + sp28[2]);
        scratch[2] -= (temp_a1_781 * FIELD(temp_s2_794, s32 *, 0xC)) / 10000;
        scratch[3] -= (sp38[1] * FIELD(temp_s2_794, s32 *, 0xC)) / 10000;
        scratch[4] -= (sp38[2] * FIELD(temp_s2_794, s32 *, 0xC)) / 10000;
        temp_v0_845 = scratch[2] - (FIELD(arg1, s32 *, 0) + sp28[0]);
        squaredX = temp_v0_845 * temp_v0_845;
        sp38[0] = temp_v0_845;
        sp38[1] = scratch[3] - (FIELD(arg1, s32 *, 4) + sp28[1]);
        temp_v0_859 = scratch[4] - (FIELD(arg1, s32 *, 8) + sp28[2]);
        sp38[2] = temp_v0_859;
        squaredZ = temp_v0_859 * temp_v0_859;
        scratch[6] = 0x400 - (GameAtan2(0 - sp38[1], func_800689A8(squaredX + squaredZ)) & 0xFFF);
        var_s0_879 = 0x400 - (GameAtan2(0 - sp38[0], 0 - sp38[2]) & 0xFFF);
        var_v0_881 = 2;
        goto block_101;
    case 3:                                         /* switch 1 */
        *(WordVector *)&scratch[2] = *(WordVector *)arg1;
        if ((temp_v1_40 & 0xFF) || (g_CameraModePrev != 3)) {
            g_CamPathNode = temp_v0_30;
            g_CamPathFrame = 0;
            if (g_CameraModePrev == 3) {
                g_CamPathOffsetStartX = g_CamPathOffsetX;
                g_CamPathOffsetStartY = g_CamPathOffsetY;
                g_CamPathOffsetStartZ = g_CamPathOffsetZ;
                g_CamPathPitchStart = g_CamPathPitch;
                g_CamPathYawStart = g_CamPathYaw;
                g_CamPathRollStart = g_CamPathRoll;
                g_CamPathDistStart = g_CamPathDist;
            } else {
                temp_v0_944 = (temp_v0_30 * 0x24) + g_TrackCameras;
                g_CamPathOffsetStartX = FIELD(temp_v0_944, s32 *, 0x10);
                g_CamPathOffsetStartY = FIELD(temp_v0_944, s32 *, 0x14);
                g_CamPathOffsetStartZ = FIELD(temp_v0_944, s32 *, 0x18);
                g_CamPathPitchStart = FIELD(temp_v0_944, s32 *, 0);
                g_CamPathYawStart = FIELD(temp_v0_944, s32 *, 4);
                g_CamPathRollStart = FIELD(temp_v0_944, s32 *, 8);
                g_CamPathDistStart = FIELD(temp_v0_944, s32 *, 0xC);
            }
            temp_a0_976 = (g_CamPathNode * 0x24) + g_TrackCameras;
            g_CamPathOffsetDeltaX = FIELD(temp_a0_976, s32 *, 0x10) - g_CamPathOffsetStartX;
            g_CamPathOffsetDeltaY = FIELD(temp_a0_976, s32 *, 0x14) - g_CamPathOffsetStartY;
            g_CamPathOffsetDeltaZ = FIELD(temp_a0_976, s32 *, 0x18) - g_CamPathOffsetStartZ;
            var_a1_886 = FIELD(temp_a0_976, s32 *, 0) - g_CamPathPitchStart;
            temp_a2_1183 = (s32)&g_CamPathPitchDelta;
            *(s32 *)temp_a2_1183 = var_a1_886;
            D_8009B1EC = FIELD(temp_a0_976, s32 *, 4) - g_CamPathYawStart;
            g_CamPathRollDelta = FIELD(temp_a0_976, s32 *, 8) - g_CamPathRollStart;
            g_CamPathDistDelta = FIELD(temp_a0_976, s32 *, 0xC) - g_CamPathDistStart;
            if (var_a1_886 > 0) {
                if (var_a1_886 >= 0x800) {
                    *(s32 *)temp_a2_1183 = var_a1_886 - 0x1000;
                }
            } else if (var_a1_886 < -0x7FF) {
                *(s32 *)temp_a2_1183 = var_a1_886 + 0x1000;
            }
            case3Angle = &D_8009B1EC;
            if (*case3Angle > 0) {
                if (*case3Angle >= 0x800) {
                    *case3Angle -= 0x1000;
                }
            } else if (*case3Angle < -0x7FF) {
                *case3Angle += 0x1000;
            }
            case3Angle = &g_CamPathRollDelta;
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
        temp_a1_1117 = 0x1000 - func_80068634((s32) (g_CamPathFrame << 0xB) / (s32) FIELD(((g_CamPathNode * 0x24) + g_TrackCameras), s32 *, 0x1C));
        var_a0_1119 = temp_a1_1117 * g_CamPathOffsetDeltaX;
        if (var_a0_1119 < 0) {
            var_a0_1119 += 0x1FFF;
        }
        temp_t2_1131 = (var_a0_1119 >> 0xD) + g_CamPathOffsetStartX;
        var_a0_1132 = temp_a1_1117 * g_CamPathOffsetDeltaY;
        sp18[0] = temp_t2_1131;
        if (var_a0_1132 < 0) {
            var_a0_1132 += 0x1FFF;
        }
        temp_t1_1144 = (var_a0_1132 >> 0xD) + g_CamPathOffsetStartY;
        var_a0_1145 = temp_a1_1117 * g_CamPathOffsetDeltaZ;
        sp18[1] = temp_t1_1144;
        if (var_a0_1145 < 0) {
            var_a0_1145 += 0x1FFF;
        }
        temp_t0_1157 = (var_a0_1145 >> 0xD) + g_CamPathOffsetStartZ;
        var_a0_1158 = temp_a1_1117 * g_CamPathPitchDelta;
        sp18[2] = temp_t0_1157;
        if (var_a0_1158 < 0) {
            var_a0_1158 += 0x1FFF;
        }
        temp_a3_1170 = (var_a0_1158 >> 0xD) + g_CamPathPitchStart;
        var_a0_1171 = temp_a1_1117 * D_8009B1EC;
        sp38[0] = temp_a3_1170;
        if (var_a0_1171 < 0) {
            var_a0_1171 += 0x1FFF;
        }
        temp_a2_1183 = (var_a0_1171 >> 0xD) + g_CamPathYawStart;
        var_a0_1184 = temp_a1_1117 * g_CamPathRollDelta;
        sp38[1] = temp_a2_1183;
        if (var_a0_1184 < 0) {
            var_a0_1184 += 0x1FFF;
        }
        temp_v1_1196 = (var_a0_1184 >> 0xD) + g_CamPathRollStart;
        var_a0_1197 = temp_a1_1117 * g_CamPathDistDelta;
        sp38[2] = temp_v1_1196;
        if (var_a0_1197 < 0) {
            var_a0_1197 += 0x1FFF;
        }
        g_CamPathPitch = temp_a3_1170 & 0xFFF;
        g_CamPathYaw = temp_a2_1183 & 0xFFF;
        g_CamPathRoll = temp_v1_1196 & 0xFFF;
        g_CamPathOffsetX = temp_t2_1131;
        g_CamPathOffsetY = temp_t1_1144;
        g_CamPathOffsetZ = temp_t0_1157;
        temp_v0_1221 = (var_a0_1197 >> 0xD) + g_CamPathDistStart;
        sp38[3] = temp_v0_1221;
        g_CamPathDist = temp_v0_1221;
        temp_a1_1227 = temp_a2_1183 - FIELD(arg1, s32 *, 0x24);
        sp38[1] = temp_a1_1227;
        GameBuildRotMatrixY(&sp88[0], temp_a1_1227);
        GameBuildRotMatrixX(&sp68[0], sp38[0]);
        MulMatrix2(&sp68[0], &sp88[0]);
        GameBuildRotMatrixZ(&sp68[0], sp38[2]);
        MulMatrix2(&sp68[0], &sp88[0]);
        GameBuildRotMatrixY(&sp48[0], FIELD(arg1, s32 *, 0x24));
        GameBuildRotMatrixX(&sp68[0], FIELD(arg1, s32 *, 0x20));
        MulMatrix2(&sp68[0], &sp48[0]);
        GameBuildRotMatrixZ(&sp68[0], FIELD(arg1, s32 *, 0x28));
        MulMatrix2(&sp68[0], &sp48[0]);
        func_80069CC8(&sp48[0], &spA8[0]);
        MulMatrix2(&sp88[0], &sp48[0]);
        func_80069CC8(&sp48[0], &sp68[0]);
        sp18[2] += 0x32;
        ApplyMatrixLV(&spA8[0], &sp18[0], &sp38[0]);
        sp28[0] = 0;
        sp28[1] = 0;
        scratch[2] += sp38[0];
        scratch[3] += sp38[1];
        scratch[4] += sp38[2];
        sp28[2] = g_CamPathDist;
        ApplyMatrixLV(&sp68[0], &sp28[0], &sp38[0]);
        scratch[2] -= sp38[0];
        scratch[3] -= sp38[1];
        scratch[4] -= sp38[2];
        temp_v0_1320 = func_800689A8((sp38[0] * sp38[0]) + (sp38[2] * sp38[2]));
        sp38[3] = temp_v0_1320;
        scratch[6] = 0x400 - (GameAtan2(sp38[1], temp_v0_1320) & 0xFFF);
        scratch[7] = 0x400 - (GameAtan2(sp38[0], sp38[2]) & 0xFFF);
        scratch[8] = 0;
        sp18[0] = 0x1000;
        sp18[1] = 0;
        sp18[2] = 0;
        GameBuildRotMatrixY(&sp88[0], 0 - scratch[7]);
        ApplyMatrixLV(&sp88[0], &sp18[0], &sp28[0]);
        func_80069CC8(&sp68[0], &sp88[0]);
        ApplyMatrixLV(&sp88[0], &sp28[0], &sp18[0]);
        scratch[8] = 0x400 - (GameAtan2(sp18[1], sp18[0]) & 0xFFF);
        g_CameraModePrev = 3;
        break;
    case 4:                                         /* switch 1 */
        case4Base = (u32)g_TrackCameras;
        temp_a3_1372 = temp_v0_30 * 0x24;
        temp_v1_1373 = temp_a3_1372 + case4Base;
        *(WordVector *)&scratch[2] = *(WordVector *)temp_v1_1373;
        if ((temp_v1_40 & 0xFF) || (g_CameraModePrev != 4)) {
            g_CamPathFrame = 0;
        } else if (g_CamPathFrame < FIELD((temp_a3_1372 + (u32)g_TrackCameras), s32 *, 0x1C)) {
            g_CamPathFrame += 1;
        }
        GameBuildRotMatrixY(&sp48[0], FIELD(arg1, s32 *, 0x24));
        GameBuildRotMatrixX(&sp68[0], FIELD(arg1, s32 *, 0x20));
        MulMatrix2(&sp68[0], &sp48[0]);
        GameBuildRotMatrixZ(&sp68[0], FIELD(arg1, s32 *, 0x28));
        MulMatrix2(&sp68[0], &sp48[0]);
        func_80069CC8(&sp48[0], &spA8[0]);
        sp18[0] = 0;
        temp_s0_1437 = temp_v0_30 * 0x24;
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
        temp_v0_1535 = scratch[2] - (FIELD(arg1, s32 *, 0) + sp28[0]);
        squaredX = temp_v0_1535 * temp_v0_1535;
        sp38[0] = temp_v0_1535;
        sp38[1] = scratch[3] - (FIELD(arg1, s32 *, 4) + sp28[1]);
        temp_v0_1549 = scratch[4] - (FIELD(arg1, s32 *, 8) + sp28[2]);
        sp38[2] = temp_v0_1549;
        squaredZ = temp_v0_1549 * temp_v0_1549;
        scratch[6] = 0x400 - (GameAtan2(0 - sp38[1], func_800689A8(squaredX + squaredZ)) & 0xFFF);
        var_s0_879 = 0x400 - (GameAtan2(0 - sp38[0], 0 - sp38[2]) & 0xFFF);
        var_v0_881 = 4;
block_101:
        scratch[7] = var_s0_879;
        scratch[8] = 0;
        g_CameraModePrev = var_v0_881;
        break;
    case 5:                                         /* switch 1 */
        *(WordVector *)&scratch[2] = *(WordVector *)arg1;
        GameBuildRotMatrixY(&sp88[0], 0 - g_OrbitCameraYaw);
        GameBuildRotMatrixY(&sp48[0], FIELD(arg1, s32 *, 0x24));
        GameBuildRotMatrixX(&sp68[0], FIELD(arg1, s32 *, 0x20));
        MulMatrix2(&sp68[0], &sp48[0]);
        GameBuildRotMatrixZ(&sp68[0], FIELD(arg1, s32 *, 0x28));
        MulMatrix2(&sp68[0], &sp48[0]);
        func_80069CC8(&sp48[0], &spA8[0]);
        MulMatrix2(&sp88[0], &sp48[0]);
        func_80069CC8(&sp48[0], &sp68[0]);
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
        scratch[6] = 0x400 - (GameAtan2(sp38[1], g_OrbitCameraDistance) & 0xFFF);
        scratch[7] = 0x400 - (GameAtan2(sp38[0], sp38[2]) & 0xFFF);
        scratch[8] = FIELD(arg1, s32 *, 0x28);
        g_CameraModePrev = 5;
        scratch[2] -= sp38[0];
        adjustedY = scratch[3] - 0x28;
        scratch[3] = adjustedY - sp38[1];
        scratch[4] -= sp38[2];
        break;
    }
    GameSetCameraRotMatrix();
    if ((cameraModeSel > 0) && (arg1 == &g_PlayerCar)) {
        GameSelectModelBank(0);
        GameDrawPlayerCarModel(arg1);
    }
}

/* Deliberately raw: the environment script header word 0 is stored here and
 * never read anywhere in the image (docs/names.md 15g). */
extern u32 D_801E4D84;
extern u32 *g_EnvScriptCues asm("D_801E42F4");
extern u32 g_EnvScriptLength asm("D_8019C774");

void GameSetEnvironmentScript(u32 *arg0) asm("func_8004550C");
void GameSetEnvironmentScript(u32 *arg0) {
    u32 value0;
    u32 value1;

    value0 = *arg0;
    arg0++;
    D_801E4D84 = value0;
    value1 = *arg0;
    arg0++;
    g_EnvScriptCues = arg0;
    g_EnvScriptLength = value1;
}
