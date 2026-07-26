#include "common.h"
#include "psyq/gte.h"
#include "game/race.h"
#include "game/track.h"
#include "game/menu.h"
#include "game/render.h"

extern s32 D_801E3EB4;
extern s32 D_801E3EB8;
extern s32 D_801E3EBC;
extern s32 D_801E3ED8;
extern s32 D_801E3EE0;
extern s16 D_8007E288[];
extern s16 g_PlayerTrackSection asm("D_8009E74C");
extern s32 g_PlayerProgressA asm("D_8009E73C");
extern s32 g_PlayerProgressB asm("D_8009E740");
extern s32 g_PlayerTrackProgress asm("D_8009E744");

typedef struct Cam {
    s32 x;        /* 0x00 */
    s32 field_04; /* 0x04 */
    s32 z;        /* 0x08 */
    s32 field_0C; /* 0x0C */
    u8 pad10[0x10];
    s32 field_20; /* 0x20 */
    s32 field_24; /* 0x24 */
    s32 field_28; /* 0x28 */
    s32 field_2C; /* 0x2C */
    s32 field_30; /* 0x30 */
    u8 pad34[4];
    s32 field_38; /* 0x38 */
    u8 pad3C[0x2C];
    s32 field_68; /* 0x68 */
    s32 field_6C; /* 0x6C */
    s32 field_70; /* 0x70 */
    s32 field_74; /* 0x74 */
    u16 field_78; /* 0x78 */
} Cam;

extern Cam D_801E3E14;

void func_8002FC84(s32 arg0, s32 *out, s32 weight);
s32 func_80068568(s32 arg0);
s32 func_80068634(s32 arg0);
void func_8002C168(void *arg0);
void func_80031298(void *arg0, s32 arg1, void *arg2);

typedef struct Block16 {
    s32 w0;
    s32 w1;
    s32 w2;
    s32 w3;
} Block16;

/*
 * Fuller sibling of func_8003CB3C: camera track-follower with bob/shake. Aims
 * the eye object D_801E3E14 at a look-ahead centre-line point, and when
 * updateMotion (arg1) is set, ramps the follow distance and applies the shake
 * offsets D_8007E288[] driven by the input bits in g_PadHeld. Builds and
 * transposes the view rotation matrices, projects the eye-forward point, and
 * writes the scratchpad view state (view[2..4]=eye XYZ, view[6]=pitch,
 * view[7]=yaw, view[8]=roll). markerClamp is the zeroed clamp record for the
 * track-marker builder func_80031298.
 */
void func_8003CF14(s32 arg0, s32 updateMotion) {
    s32 *view = (s32 *)0x1F800000;
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

    offset = D_801E3E14.field_30;
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

    func_8002FC84(index, coords, D_801E3E14.field_38);
    angle = 0x400 - GameAtan2(coords[0] - D_801E3E14.x, coords[2] - D_801E3E14.z);
    D_801E3EB4 += GameGetAngleDelta(D_801E3EB4, angle);
    D_801E3E14.field_24 = D_801E3EB4;

    if (updateMotion != 0) {
        if (D_801E3EBC < 50) {
            D_801E3EBC += 3;
        }
        D_801E3EB8 = (D_801E3EB8 + D_801E3EBC) * 94 / 100;

        value = func_80068568(D_801E3EB4) * D_801E3EB8;
        if (value < 0) {
            value += 0xFF;
        }
        D_801E3ED8 = value >> 8;

        zValue = func_80068634(D_801E3EB4) * D_801E3EB8;
        if (zValue < 0) {
            zValue += 0xFF;
        }
        D_801E3EE0 = zValue >> 8;

        D_801E3E14.x = D_801E3ED8 / 256 + D_801E3E14.x;
        D_801E3E14.z = D_801E3EE0 / 256 + D_801E3E14.z;

        if (g_PadHeld & 0x1000) {
            D_8007E288[0] -= 8;
        } else if (g_PadHeld & 0x4000) {
            D_8007E288[0] += 8;
        }
        if (g_PadHeld & 0x8000) {
            D_8007E288[1] -= 16;
        } else if (g_PadHeld & 0x2000) {
            D_8007E288[1] += 16;
        }
    }
    if (D_8007E288[0] >= 0) {
        D_8007E288[0] = 0;
    } else if (D_8007E288[0] < -255) {
        D_8007E288[0] = -256;
    }

    D_801E3E14.field_24 = D_8007E288[1] + D_801E3E14.field_24;
    func_8002C168(&D_801E3E14);
    markerClamp[0] = 0;
    markerClamp[1] = 0;
    func_80031298(&D_801E3E14, D_801E3E14.field_30, markerClamp);

    *(Block16 *)(view + 2) = *(Block16 *)&D_801E3E14.x;
    view[3] -= 48;
    *(Block16 *)(view + 6) = *(Block16 *)&D_801E3E14.field_20;
    view[6] = D_8007E288[0] + view[6];

    GameBuildRotMatrixY(&m1, view[7]);
    GameBuildRotMatrixX(&m2, view[6]);
    MulMatrix2(&m2, &m1);
    GameBuildRotMatrixZ(&m2, view[8]);
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
    view[7] = c400 - GameAtan2(delta[0], delta[2]);
    value = SquareRoot12(delta[0] * delta[0] + delta[2] * delta[2]);
    view[6] = c400 - GameAtan2(delta[1], value >> 6);

    g_PlayerTrackSection = D_801E3E14.field_78;
    g_PlayerProgressA = D_801E3E14.field_68;
    g_PlayerProgressB = D_801E3E14.field_6C;
    g_PlayerTrackProgress = D_801E3E14.field_70;
    GameSetCameraRotMatrix();
}

typedef struct {
    s32 x;
    s32 y;
    s32 z;
    s32 w;
} Vec4i;

extern s32 g_CourseModelCount asm("D_801E40E4");
extern s32 D_8007E2B8[];
extern struct {
    s16 x;
    s16 y;
} D_8007E290[];
extern Vec4i D_8007E298[];

void func_80017794(void *arg0, Vec4i *state, Matrix *mtx);

void func_8003D458(s32 arg0) {
    Matrix mtx;
    Vec4i state;
    s32 s1;
    s32 s0;
    register s32 value asm("$2");
    s32 drawArg;
    s32 rem;
    s32 lim;

    if (g_RacePhase < 2 && arg0 >= 0x51) {
        GameBuildRotMatrixY(&mtx, D_8007E2B8[g_RaceSeries]);
        MulMatrix2((Matrix *)0x1F800028, &mtx);
        if (arg0 - 90 > 0) {
            state = D_8007E298[g_RaceSeries];
            s1 = (arg0 - 90) / 3;
            state.x += D_8007E290[g_RaceSeries].x * (s0 = s1 / 15);
            state.z += D_8007E290[g_RaceSeries].y * s0;
            if (g_CourseIndex % 4 == 3) {
                state.z += 0x5000;
            }
            func_80017794((void *)0x1F80011C, &state, &mtx);
            rem = s1 - s0 * 15;
            lim = g_CourseModelCount;
            __asm__ __volatile__("");
            value = rem + 0x28;
            *(s32 *)0x1F800084 = 0;
            drawArg = (value < lim) ? value : 1;
        } else {
            state = D_8007E298[g_RaceSeries];
            if (g_CourseIndex % 4 == 3) {
                state.z += 0x5000;
            }
            func_80017794((void *)0x1F80011C, &state, &mtx);
            lim = g_CourseModelCount;
            __asm__ __volatile__("");
            value = 0x28;
            *(s32 *)0x1F800084 = 0;
            drawArg = (value < lim) ? value : 1;
        }
        GameSubmitCourseModel((void *)0x1F800000, drawArg);
    }
}

void func_8003D6E8(void) {
}

extern Vec4i D_8007E2C0[];
extern u32 *g_VisibleCellMask asm("D_801E6828");
extern s16 D_8007E2EA;
extern s32 D_801E4BAC;
extern s32 D_8007E2E4;
extern s16 D_8007E2E8;
extern s16 D_8009AFCC;
extern s16 D_8007E2E0[];
s32 GameRandom15(void) asm("func_800632B0");

void GameDrawAnimatedScenery(s32 arg0, s32 arg1) asm("func_8003D6F0");

void GameDrawAnimatedScenery(s32 arg0, s32 arg1) {
    Matrix mtx;
    Matrix mtx2;
    Vec4i state;
    register s32 bucket asm("$2");
    register s32 value asm("$6");
    register u32 *visibility asm("$3");
    register u32 *wordPtr asm("$4");
    register s32 bit asm("$5");
    register s32 visible asm("$3");
    register s32 num asm("$3");
    register s32 drawArg asm("$5");
    register s32 sv asm("$2");
    register s32 lim2 asm("$2");
    register s32 *scr asm("$8");

    state = D_8007E2C0[arg1];

    if ((g_CourseIndex & 3) == 3) {
        state.z += 0x5000;
    }
    if (g_GrandPrixClass == 5) {
        return;
    }

    bucket = state.z + 0x400;
    if (bucket < 0) {
        bucket = state.z + 0xBFF;
    }
    bucket >>= 11;
    bucket <<= 2;

    value = state.x;
    visibility = g_VisibleCellMask;
    bit = value + 0x400;
    wordPtr = (u32 *)(bucket + (s32)visibility);
    if (bit < 0) {
        bit = value + 0xBFF;
    }
    bucket = bit >> 11;
    visible = 1 << bucket;
    visible &= *wordPtr;
    if (visible == 0) {
        return;
    }

    D_8007E2EA = (arg0 / 4) % 16;
    if (D_8007E2EA == 0 && (arg0 & 7) == 0 && D_801E4BAC == 0) {
        D_8007E2E4 = 0;
        D_8007E2E8 = g_RacePosition;
        D_8009AFCC = (GameRandom15() & 7) / 3;
        if (D_8007E2E8 >= 4) {
            D_8007E2E8 = 0;
        }
    }

    GameBuildRotMatrixY(&mtx, state.w);
    GameBuildRotMatrixX(&mtx2, D_8007E2E0[arg1]);
    MulMatrix(&mtx, &mtx2);
    MulMatrix2((Matrix *)0x1F800028, &mtx);

    if (g_GrandPrixMode == 0) {
        return;
    }

    D_8007E2E4 = ((arg0 >> 3) & 3) << 16;

    if (D_8007E2E8 != 0) {
        if (D_8007E2EA < 13) {
            func_80017794((void *)0x1F80011C, &state, &mtx);
            num = D_8007E2EA + 10;
            *(s32 *)0x1F800084 = 0;
            drawArg = (num < g_CourseModelCount) ? num : 1;
            GameSubmitCourseModel((void *)0x1F800000, drawArg);
        } else {
            func_80017794((void *)0x1F80011C, &state, &mtx);
            num = D_8007E2E8;
            *(s32 *)0x1F800084 = 0;
            drawArg = (num < g_CourseModelCount) ? num : 1;
            GameSubmitCourseModel((void *)0x1F800000, drawArg);
        }

        func_80017794((void *)0x1F80011C, &state, &mtx);
        sv = D_8007E2E4;
        *(s32 *)0x1F800084 = sv;
        num = D_8009AFCC + 4;
        lim2 = g_CourseModelCount;
        drawArg = (num < lim2) ? num : 1;
        GameSubmitCourseModel((void *)0x1F800000, drawArg);
    } else {
        func_80017794((void *)0x1F80011C, &state, &mtx);
        num = D_8007E2EA + 0x18;
        scr = (s32 *)0x1F800084;
        *scr = 0;
        drawArg = (num < g_CourseModelCount) ? num : 1;
        GameSubmitCourseModel((void *)0x1F800000, drawArg);

        func_80017794((void *)0x1F80011C, &state, &mtx);
        sv = D_8007E2E4;
        scr = (s32 *)0x1F800084;
        *scr = sv;
        num = D_8009AFCC + 7;
        lim2 = g_CourseModelCount;
        drawArg = (num < lim2) ? num : 1;
        GameSubmitCourseModel((void *)0x1F800000, drawArg);
    }
}

extern s16 D_8007E2F2;
extern s32 D_8007E2EC;
extern s16 D_8007E2F0;

void GameDrawAnimatedScenery2(s32 arg0, s32 arg1, s32 arg2, s32 arg3) asm("func_8003DA90");

void GameDrawAnimatedScenery2(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    Matrix mtx;
    Matrix mtx2;
    Vec4i state;
    register s32 bucket asm("$2");
    register s32 value asm("$5");
    register u32 *visibility asm("$3");
    register u32 *wordPtr asm("$3");
    register s32 bit asm("$4");
    register s32 visible asm("$2");
    register s32 num asm("$3");
    register s32 drawArg asm("$5");
    register s32 sv asm("$2");
    register s32 lim2 asm("$2");
    register s32 *scr asm("$8");

    if (g_GrandPrixMode == 0) {
        return;
    }
    if (g_GrandPrixClass == 5) {
        return;
    }

    state = D_8007E2C0[arg1];
    if ((g_CourseIndex & 3) == 3) {
        state.z += 0x5000;
    }

    bucket = state.z + 0x400;
    if (bucket < 0) {
        bucket = state.z + 0xBFF;
    }
    bucket >>= 11;
    bucket <<= 2;

    value = state.x;
    visibility = g_VisibleCellMask;
    bit = value + 0x400;
    wordPtr = (u32 *)(bucket + (s32)visibility);
    if (bit < 0) {
        bit = value + 0xBFF;
    }
    bucket = bit >> 11;
    visible = 1 << bucket;
    visible &= *wordPtr;
    if (visible == 0) {
        return;
    }

    D_8007E2F2 = (arg0 / 4) % 16;
    if (D_8007E2F2 == 0 && (arg0 & 7) == 0 && arg3 == 1) {
        D_8007E2EC = 0;
        D_8007E2F0 = (GameRandom15() & 7) / 3;
    }

    GameBuildRotMatrixY(&mtx, state.w);
    GameBuildRotMatrixX(&mtx2, D_8007E2E0[arg1]);
    MulMatrix(&mtx, &mtx2);
    MulMatrix2((Matrix *)0x1F800028, &mtx);

    D_8007E2EC = ((arg0 >> 3) & 3) << 16;

    if (arg2 != 0) {
        func_80017794((void *)0x1F80011C, &state, &mtx);
        num = D_8007E2F2 + 0xA;
        scr = (s32 *)0x1F800084;
        *scr = 0;
        drawArg = (num < g_CourseModelCount) ? num : 1;
        GameSubmitCourseModel((void *)0x1F800000, drawArg);

        func_80017794((void *)0x1F80011C, &state, &mtx);
        sv = D_8007E2EC;
        drawArg = 1;
        scr = (s32 *)0x1F800084;
        *scr = sv;
        lim2 = g_CourseModelCount;
        num = D_8007E2F0;
        __asm__("" : "=r"(num) : "0"(num), "r"(lim2));
        num = num + 4;
    } else {
        func_80017794((void *)0x1F80011C, &state, &mtx);
        num = D_8007E2F2 + 0x18;
        scr = (s32 *)0x1F800084;
        *scr = 0;
        drawArg = (num < g_CourseModelCount) ? num : 1;
        GameSubmitCourseModel((void *)0x1F800000, drawArg);

        func_80017794((void *)0x1F80011C, &state, &mtx);
        sv = D_8007E2EC;
        drawArg = 1;
        scr = (s32 *)0x1F800084;
        *scr = sv;
        lim2 = g_CourseModelCount;
        num = D_8007E2F0;
        __asm__("" : "=r"(num) : "0"(num), "r"(lim2));
        num = num + 7;
    }

    if (num < lim2) {
        drawArg = num;
    }
    GameSubmitCourseModel((void *)0x1F800000, drawArg);
}
