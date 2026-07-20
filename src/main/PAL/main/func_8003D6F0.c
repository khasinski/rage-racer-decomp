#include "common.h"
#include "psyq/gte.h"

typedef struct {
    s32 x;
    s32 y;
    s32 z;
    s32 w;
} Vec4i;

extern Vec4i D_8007E2C0[];
extern s32 D_801E428C;
extern s32 D_8009E6A4;
extern u32 *D_801E6828;
extern s16 D_8007E2EA;
extern s32 D_801E4BAC;
extern u16 D_8009E834;
extern s32 D_8007E2E4;
extern s16 D_8007E2E8;
extern s16 D_8009AFCC;
extern s16 D_8007E2E0[];
extern s16 D_801E4DAC;
extern s32 D_801E40E4;

void func_8001A530(Matrix *mtx, s32 angle);
void func_8001A5A0(Matrix *mtx, s32 angle);
void func_80069458(Matrix *lhs, Matrix *rhs);
void func_80069568(Matrix *lhs, Matrix *rhs);
void func_80017794(void *arg0, Vec4i *state, Matrix *mtx);
void func_800296B4(void *arg0, s32 arg1);
s32 func_800632B0(void);

void func_8003D6F0(s32 arg0, s32 arg1) {
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

    if ((D_801E428C & 3) == 3) {
        state.z += 0x5000;
    }
    if (D_8009E6A4 == 5) {
        return;
    }

    bucket = state.z + 0x400;
    if (bucket < 0) {
        bucket = state.z + 0xBFF;
    }
    bucket >>= 11;
    bucket <<= 2;

    value = state.x;
    visibility = D_801E6828;
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
        D_8007E2E8 = D_8009E834;
        D_8009AFCC = (func_800632B0() & 7) / 3;
        if (D_8007E2E8 >= 4) {
            D_8007E2E8 = 0;
        }
    }

    func_8001A530(&mtx, state.w);
    func_8001A5A0(&mtx2, D_8007E2E0[arg1]);
    func_80069458(&mtx, &mtx2);
    func_80069568((Matrix *)0x1F800028, &mtx);

    if (D_801E4DAC == 0) {
        return;
    }

    D_8007E2E4 = ((arg0 >> 3) & 3) << 16;

    if (D_8007E2E8 != 0) {
        if (D_8007E2EA < 13) {
            func_80017794((void *)0x1F80011C, &state, &mtx);
            num = D_8007E2EA + 10;
            *(s32 *)0x1F800084 = 0;
            drawArg = (num < D_801E40E4) ? num : 1;
            func_800296B4((void *)0x1F800000, drawArg);
        } else {
            func_80017794((void *)0x1F80011C, &state, &mtx);
            num = D_8007E2E8;
            *(s32 *)0x1F800084 = 0;
            drawArg = (num < D_801E40E4) ? num : 1;
            func_800296B4((void *)0x1F800000, drawArg);
        }

        func_80017794((void *)0x1F80011C, &state, &mtx);
        sv = D_8007E2E4;
        *(s32 *)0x1F800084 = sv;
        num = D_8009AFCC + 4;
        lim2 = D_801E40E4;
        drawArg = (num < lim2) ? num : 1;
        func_800296B4((void *)0x1F800000, drawArg);
    } else {
        func_80017794((void *)0x1F80011C, &state, &mtx);
        num = D_8007E2EA + 0x18;
        scr = (s32 *)0x1F800084;
        *scr = 0;
        drawArg = (num < D_801E40E4) ? num : 1;
        func_800296B4((void *)0x1F800000, drawArg);

        func_80017794((void *)0x1F80011C, &state, &mtx);
        sv = D_8007E2E4;
        scr = (s32 *)0x1F800084;
        *scr = sv;
        num = D_8009AFCC + 7;
        lim2 = D_801E40E4;
        drawArg = (num < lim2) ? num : 1;
        func_800296B4((void *)0x1F800000, drawArg);
    }
}
