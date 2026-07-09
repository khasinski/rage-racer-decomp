#include "common.h"
#include "psyq/gte.h"

typedef struct {
    s32 x;
    s32 y;
    s32 z;
    s32 w;
} Vec4i;

extern Vec4i D_8007E340;
extern s32 D_8007E34C;
extern u32 *D_801E6828;
extern s32 D_801E4030;
extern s32 D_801E40E4;

void func_80017794(void *arg0, void *arg1, Matrix *mtx);
void func_8001A530(Matrix *mtx, s32 angle);
void func_800296B4(void *arg0, s32 arg1);
void func_80029E50(void *arg0, s32 arg1);
void func_80069568(Matrix *lhs, Matrix *rhs);

static inline void ClearScratchRenderMode3DF68(void) {
    *(s32 *)0x1F800084 = 0;
}

void func_8003DF68(s32 arg0) {
    Matrix mtx;
    Vec4i state;
    register s32 *statePtr asm("$16");
    register s32 bucket asm("$2");
    register s32 value asm("$6");
    register u32 *visibility asm("$3");
    register u32 *wordPtr asm("$4");
    register s32 bit asm("$5");
    register s32 visible asm("$3");
    register s32 drawArg asm("$5");
    register s32 frameValue asm("$2");

    state = D_8007E340;
    statePtr = (s32 *)&state;

    if (arg0 != 0) {
        state.z += 0x5000;
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

    if (visible != 0) {
        func_8001A530(&mtx, D_8007E34C);
        func_80069568((Matrix *)0x1F800028, &mtx);

        if (D_801E4030 != 0) {
            func_80017794((void *)0x1F80011C, statePtr, &mtx);
            frameValue = D_801E40E4;
            ClearScratchRenderMode3DF68();
            drawArg = 1;
            if (frameValue >= 0x3B) {
                drawArg = 0x3A;
            }
            func_800296B4((void *)0x1F800000, drawArg);
        } else {
            func_80017794((void *)0x1F80011C, statePtr, &mtx);
            frameValue = D_801E40E4;
            ClearScratchRenderMode3DF68();
            drawArg = 1;
            if (frameValue >= 0x3A) {
                drawArg = 0x39;
            }
            func_80029E50((void *)0x1F800000, drawArg);
        }
    }
}
