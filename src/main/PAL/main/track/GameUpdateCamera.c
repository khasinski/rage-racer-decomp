#include "common.h"
#include "psyq/gte.h"
#include "game/render.h"

typedef struct {
    s32 words[4];
} WordVector;

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

extern void func_8001DAB0(void *);
extern s32 GameFindNearestTrackCamera() asm("func_80043B18");
extern s32 func_80068634(s32);
extern s32 func_800689A8(s32);
extern void *func_80068F80(void *, void *, void *);
extern void *func_80069CC8(void *, void *);
extern s32 D_8007F610;
extern s32 D_8007F614;
extern s32 D_8007F618;
extern s32 D_8009B1B8;
extern s32 D_8009B1BC;
extern s32 D_8009B1C0;
extern s32 D_8009B1C8;
extern s32 D_8009B1CC;
extern s32 D_8009B1D0;
extern s32 D_8009B1D8;
extern s32 D_8009B1DC;
extern s32 D_8009B1E0;
extern s32 D_8009B1E8;
extern s32 D_8009B1EC;
extern s32 D_8009B1F0;
extern s32 D_8009B1F4;
extern s32 D_8009B1F8;
extern s32 D_8009B1FC;
extern s32 D_8009B200;
extern s32 D_8009B204;
extern s32 D_8009B208;
extern s32 D_8009B20C;
extern s32 D_8009B210;
extern s32 D_8009B214;
extern u8 D_8009B218;
extern s32 D_8009B21C;
extern s32 D_8009B220;
extern s32 D_8009B224;
extern s32 D_8009B228;
extern s32 D_8009B22C;
extern s32 D_8009B230;
extern s32 D_8009B234;
extern s32 D_8009B238;
extern s32 D_8009B23C;
extern s32 D_8009B240;
extern s32 D_8009B244;
extern s32 D_8009B248;
extern s32 g_PlayerCar asm("D_8009E6D4");
extern u8 *g_TrackCameras asm("D_8019C7CC");

/*
 * Camera-mode state machine: `cameraModeSel` selects among the camera behaviours
 * (chase / bumper / replay-orbit / intro-pan, etc.); `arg1` is the followed
 * render/target object. Positions the eye and fills the scratchpad view state
 * (view[2..8]) with matrix/atan2/sqrt math, then submits via func_8001DAB0.
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
    temp_v1_35 = D_8009B240;
    D_8009B240 = temp_v0_30;
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
        D_8009B218 = 0;
        break;
    case 1:                                         /* switch 1 */
        *(WordVector *)&scratch[2] = *(WordVector *)arg1;
        temp_v0_200 = FIELD(arg1, s32 *, 0x24);
        temp_a0_157 = temp_v0_200 & 0xFFF;
        temp_v1_541 = FIELD(arg1, s32 *, 0xA4);
        D_8009B23C = temp_v1_541;
        previousMode = D_8009B218;
        D_8009B21C = temp_a0_157;
        if (previousMode == 1) {
            modeAngle = &D_8009B1EC;
            *modeAngle &= 0xFFF;
            D_8009B228 &= 0xFFF;
            D_8009B22C &= 0xFFF;
        } else {
            D_8009B1EC = temp_a0_157;
            D_8009B228 = 0;
            D_8009B22C = 0;
        }
        if (D_8009B23C >= 0x321) {
            temp_v0_200 = 0x4E2 - D_8009B23C;
            D_8009B238 = temp_v0_200;
            if (temp_v0_200 < 6) {
                D_8009B238 = 6;
            }
            D_8009B238 = ((((D_8009B238 * 8) / 50) + 8) / 10) + 1;
        } else {
            temp_a1_230 = 0x4E2 - D_8009B23C;
            D_8009B238 = temp_a1_230;
        D_8009B238 = ((((D_8009B238 * 6 * temp_a1_230) / 2500) - ((temp_a1_230 * 0x46) / 50)) + 0xE0) / 10;
        }
        temp_a0_272 = D_8009B21C - D_8009B1EC;
        if (temp_a0_272 >= 5) {
            if (temp_a0_272 >= 0x800) {
                temp_v0_288 = (((0x1000 - temp_a0_272) / 17) * 2) & 0xFFF;
                D_8009B230 = temp_v0_288;
                if (temp_v0_288 >= 0x41) {
                    D_8009B230 = 0x40;
                }
                turnFactor = D_8009B238;
                turnAccel = ((D_8009B228 + 8) * (D_8009B228 + 8)) / turnFactor;
                turnLimit = D_8009B230;
                D_8009B22C = 0;
                D_8009B228 += 8;
                D_8009B234 = turnAccel;
                if (turnLimit >= turnAccel) {
                    var_v0_713 = 0 - turnAccel;
                    goto block_36;
                }
                goto block_34;
            }
            temp_v0_340 = (((D_8009B21C - D_8009B1EC) / 17) * 2) & 0xFFF;
            D_8009B230 = temp_v0_340;
            if (temp_v0_340 >= 0x41) {
                D_8009B230 = 0x40;
            }
            turnFactor = D_8009B238;
            turnAccel = ((D_8009B22C + 8) * (D_8009B22C + 8)) / turnFactor;
            turnLimit = D_8009B230;
            D_8009B228 = 0;
            D_8009B22C += 8;
            D_8009B234 = turnAccel;
            if (turnLimit < turnAccel) {
                goto block_29;
            }
            goto block_30;
        }
        if (temp_a0_272 < -4) {
            if (temp_a0_272 < -0x7FF) {
                temp_v0_399 = (((0x1000 - (D_8009B1EC - D_8009B21C)) / 17) * 2) & 0xFFF;
                D_8009B230 = temp_v0_399;
                if (temp_v0_399 >= 0x41) {
                    D_8009B230 = 0x40;
                }
                turnFactor = D_8009B238;
                turnAccel = ((D_8009B22C + 8) * (D_8009B22C + 8)) / turnFactor;
                turnLimit = D_8009B230;
                D_8009B228 = 0;
                D_8009B22C += 8;
                D_8009B234 = turnAccel;
                if (turnLimit < turnAccel) {
block_29:
                    D_8009B224 = turnLimit;
                    D_8009B22C = func_800689A8(turnLimit * turnFactor);
                } else {
block_30:
                    D_8009B224 = turnAccel;
                }
            } else {
                temp_v0_466 = (((D_8009B1EC - D_8009B21C) / 17) * 2) & 0xFFF;
                D_8009B230 = temp_v0_466;
                if (temp_v0_466 >= 0x41) {
                    D_8009B230 = 0x40;
                }
                turnFactor = D_8009B238;
                turnAccel = ((D_8009B228 + 8) * (D_8009B228 + 8)) / turnFactor;
                turnLimit = D_8009B230;
                D_8009B22C = 0;
                D_8009B228 += 8;
                D_8009B234 = turnAccel;
                if (turnLimit < turnAccel) {
block_34:
                    D_8009B224 = 0 - turnLimit;
                    D_8009B228 = func_800689A8(turnLimit * turnFactor);
                } else {
                    var_v0_713 = 0 - turnAccel;
block_36:
                    D_8009B224 = var_v0_713;
                }
            }
        } else {
            D_8009B224 = 0;
            D_8009B228 = 0;
            D_8009B22C = 0;
        }
        rawAngle = D_8009B21C;
        temp_v1_541 = (D_8009B1EC + D_8009B224) & 0xFFF;
        D_8009B220 = temp_v1_541;
        if (rawAngle < temp_v1_541) {
            temp_v0_546 = rawAngle - temp_v1_541;
            D_8009B224 = temp_v0_546;
            var_v1_549 = temp_v0_546;
            if (var_v1_549 < -0x7FF) {
                var_v1_549 += 0x1000;
            }
            angleState = &D_8009B224;
            *angleState = var_v1_549;
        } else {
            temp_v0_546 = rawAngle - temp_v1_541;
            D_8009B224 = temp_v0_546;
            var_v1_549 = temp_v0_546;
            if (var_v1_549 >= 0x800) {
                var_v1_549 -= 0x1000;
            }
            angleState = &D_8009B224;
            *angleState = var_v1_549;
        }
        angleState = (s32 *)&sp88[0];
        GameBuildRotMatrixY(angleState, 0 - D_8009B224);
        GameBuildRotMatrixX(&sp68[0], -0x80);
        MulMatrix2(&sp68[0], &sp88[0]);
        D_8009B1EC = D_8009B220;
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
        func_80068F80(&spA8[0], &sp18[0], &sp28[0]);
        sp18[0] = 0;
        scratch[2] += sp28[0];
        scratch[3] += sp28[1];
        scratch[4] += sp28[2];
        switch (D_8007F610) {                       /* switch 2; irregular */
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
        func_80068F80(&sp68[0], &sp18[0], &sp38[0]);
        scratch[2] -= sp38[0];
        scratch[3] -= sp38[1];
        scratch[4] -= sp38[2];
        temp_v0_687 = func_800689A8((sp38[0] * sp38[0]) + (sp38[2] * sp38[2]));
        sp38[3] = temp_v0_687;
        scratch[6] = 0x400 - (GameAtan2(sp38[1] + 0x28, temp_v0_687) & 0xFFF);
        scratch[7] = 0x400 - (GameAtan2(sp38[0], sp38[2]) & 0xFFF);
        scratch[8] = FIELD(arg1, s32 *, 0x28) - FIELD(arg1, s32 *, 0x64);
        if (D_8007F610 == 0) {
            var_v0_713 = scratch[6] - 0x90;
        } else {
            var_v0_713 = scratch[6] - 0x60;
        }
        scratch[6] = var_v0_713;
        D_8009B218 = 1;
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
        func_80068F80(&spA8[0], &sp18[0], &sp28[0]);
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
        if ((temp_v1_40 & 0xFF) || (D_8009B218 != 3)) {
            D_8009B248 = temp_v0_30;
            D_8009B244 = 0;
            if (D_8009B218 == 3) {
                D_8009B1C8 = D_8009B1D8;
                D_8009B1CC = D_8009B1DC;
                D_8009B1D0 = D_8009B1E0;
                D_8009B1F8 = D_8009B208;
                D_8009B1FC = D_8009B20C;
                D_8009B200 = D_8009B210;
                D_8009B204 = D_8009B214;
            } else {
                temp_v0_944 = (temp_v0_30 * 0x24) + g_TrackCameras;
                D_8009B1C8 = FIELD(temp_v0_944, s32 *, 0x10);
                D_8009B1CC = FIELD(temp_v0_944, s32 *, 0x14);
                D_8009B1D0 = FIELD(temp_v0_944, s32 *, 0x18);
                D_8009B1F8 = FIELD(temp_v0_944, s32 *, 0);
                D_8009B1FC = FIELD(temp_v0_944, s32 *, 4);
                D_8009B200 = FIELD(temp_v0_944, s32 *, 8);
                D_8009B204 = FIELD(temp_v0_944, s32 *, 0xC);
            }
            temp_a0_976 = (D_8009B248 * 0x24) + g_TrackCameras;
            D_8009B1B8 = FIELD(temp_a0_976, s32 *, 0x10) - D_8009B1C8;
            D_8009B1BC = FIELD(temp_a0_976, s32 *, 0x14) - D_8009B1CC;
            D_8009B1C0 = FIELD(temp_a0_976, s32 *, 0x18) - D_8009B1D0;
            var_a1_886 = FIELD(temp_a0_976, s32 *, 0) - D_8009B1F8;
            temp_a2_1183 = (s32)&D_8009B1E8;
            *(s32 *)temp_a2_1183 = var_a1_886;
            D_8009B1EC = FIELD(temp_a0_976, s32 *, 4) - D_8009B1FC;
            D_8009B1F0 = FIELD(temp_a0_976, s32 *, 8) - D_8009B200;
            D_8009B1F4 = FIELD(temp_a0_976, s32 *, 0xC) - D_8009B204;
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
            case3Angle = &D_8009B1F0;
            if (*case3Angle > 0) {
                if (*case3Angle >= 0x800) {
                    *case3Angle -= 0x1000;
                }
            } else if (*case3Angle < -0x7FF) {
                *case3Angle += 0x1000;
            }
        } else if (D_8009B244 < FIELD(((D_8009B248 * 0x24) + g_TrackCameras), s32 *, 0x1C)) {
            D_8009B244 += 1;
        }
        temp_a1_1117 = 0x1000 - func_80068634((s32) (D_8009B244 << 0xB) / (s32) FIELD(((D_8009B248 * 0x24) + g_TrackCameras), s32 *, 0x1C));
        var_a0_1119 = temp_a1_1117 * D_8009B1B8;
        if (var_a0_1119 < 0) {
            var_a0_1119 += 0x1FFF;
        }
        temp_t2_1131 = (var_a0_1119 >> 0xD) + D_8009B1C8;
        var_a0_1132 = temp_a1_1117 * D_8009B1BC;
        sp18[0] = temp_t2_1131;
        if (var_a0_1132 < 0) {
            var_a0_1132 += 0x1FFF;
        }
        temp_t1_1144 = (var_a0_1132 >> 0xD) + D_8009B1CC;
        var_a0_1145 = temp_a1_1117 * D_8009B1C0;
        sp18[1] = temp_t1_1144;
        if (var_a0_1145 < 0) {
            var_a0_1145 += 0x1FFF;
        }
        temp_t0_1157 = (var_a0_1145 >> 0xD) + D_8009B1D0;
        var_a0_1158 = temp_a1_1117 * D_8009B1E8;
        sp18[2] = temp_t0_1157;
        if (var_a0_1158 < 0) {
            var_a0_1158 += 0x1FFF;
        }
        temp_a3_1170 = (var_a0_1158 >> 0xD) + D_8009B1F8;
        var_a0_1171 = temp_a1_1117 * D_8009B1EC;
        sp38[0] = temp_a3_1170;
        if (var_a0_1171 < 0) {
            var_a0_1171 += 0x1FFF;
        }
        temp_a2_1183 = (var_a0_1171 >> 0xD) + D_8009B1FC;
        var_a0_1184 = temp_a1_1117 * D_8009B1F0;
        sp38[1] = temp_a2_1183;
        if (var_a0_1184 < 0) {
            var_a0_1184 += 0x1FFF;
        }
        temp_v1_1196 = (var_a0_1184 >> 0xD) + D_8009B200;
        var_a0_1197 = temp_a1_1117 * D_8009B1F4;
        sp38[2] = temp_v1_1196;
        if (var_a0_1197 < 0) {
            var_a0_1197 += 0x1FFF;
        }
        D_8009B208 = temp_a3_1170 & 0xFFF;
        D_8009B20C = temp_a2_1183 & 0xFFF;
        D_8009B210 = temp_v1_1196 & 0xFFF;
        D_8009B1D8 = temp_t2_1131;
        D_8009B1DC = temp_t1_1144;
        D_8009B1E0 = temp_t0_1157;
        temp_v0_1221 = (var_a0_1197 >> 0xD) + D_8009B204;
        sp38[3] = temp_v0_1221;
        D_8009B214 = temp_v0_1221;
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
        func_80068F80(&spA8[0], &sp18[0], &sp38[0]);
        sp28[0] = 0;
        sp28[1] = 0;
        scratch[2] += sp38[0];
        scratch[3] += sp38[1];
        scratch[4] += sp38[2];
        sp28[2] = D_8009B214;
        func_80068F80(&sp68[0], &sp28[0], &sp38[0]);
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
        func_80068F80(&sp88[0], &sp18[0], &sp28[0]);
        func_80069CC8(&sp68[0], &sp88[0]);
        func_80068F80(&sp88[0], &sp28[0], &sp18[0]);
        scratch[8] = 0x400 - (GameAtan2(sp18[1], sp18[0]) & 0xFFF);
        D_8009B218 = 3;
        break;
    case 4:                                         /* switch 1 */
        case4Base = (u32)g_TrackCameras;
        temp_a3_1372 = temp_v0_30 * 0x24;
        temp_v1_1373 = temp_a3_1372 + case4Base;
        *(WordVector *)&scratch[2] = *(WordVector *)temp_v1_1373;
        if ((temp_v1_40 & 0xFF) || (D_8009B218 != 4)) {
            D_8009B244 = 0;
        } else if (D_8009B244 < FIELD((temp_a3_1372 + (u32)g_TrackCameras), s32 *, 0x1C)) {
            D_8009B244 += 1;
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
        func_80068F80(&spA8[0], &sp18[0], &sp28[0]);
        temp_s0_1448 = temp_s0_1437 + g_TrackCameras;
        temp_v0_1452 = FIELD(temp_s0_1448, s32 *, 0x10) - scratch[2];
        sp38[0] = temp_v0_1452;
        temp_v0_1458 = FIELD(temp_s0_1448, s32 *, 0x14) - scratch[3];
        sp38[1] = temp_v0_1458;
        temp_v0_1463 = FIELD(temp_s0_1448, s32 *, 0x18) - scratch[4];
        sp38[2] = temp_v0_1463;
        scratch[2] += (s32) (temp_v0_1452 * D_8009B244) / (s32) FIELD(temp_s0_1448, s32 *, 0x1C);
        scratch[3] += (s32) (sp38[1] * D_8009B244) / (s32) FIELD(temp_s0_1448, s32 *, 0x1C);
        scratch[4] += (s32) (sp38[2] * D_8009B244) / (s32) FIELD(temp_s0_1448, s32 *, 0x1C);
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
        D_8009B218 = var_v0_881;
        break;
    case 5:                                         /* switch 1 */
        *(WordVector *)&scratch[2] = *(WordVector *)arg1;
        GameBuildRotMatrixY(&sp88[0], 0 - D_8007F614);
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
        func_80068F80(&spA8[0], &sp18[0], &sp28[0]);
        sp18[0] = 0;
        sp18[1] = 0;
        scratch[2] += sp28[0];
        scratch[3] += sp28[1];
        scratch[4] += sp28[2];
        sp18[2] = D_8007F618;
        func_80068F80(&sp68[0], &sp18[0], &sp38[0]);
        scratch[6] = 0x400 - (GameAtan2(sp38[1], D_8007F618) & 0xFFF);
        scratch[7] = 0x400 - (GameAtan2(sp38[0], sp38[2]) & 0xFFF);
        scratch[8] = FIELD(arg1, s32 *, 0x28);
        D_8009B218 = 5;
        scratch[2] -= sp38[0];
        adjustedY = scratch[3] - 0x28;
        scratch[3] = adjustedY - sp38[1];
        scratch[4] -= sp38[2];
        break;
    }
    GameSetCameraRotMatrix();
    if ((cameraModeSel > 0) && (arg1 == &g_PlayerCar)) {
        GameSelectModelBank(0);
        func_8001DAB0(arg1);
    }
}

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
