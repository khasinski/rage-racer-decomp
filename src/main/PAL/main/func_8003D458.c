#include "common.h"
#include "psyq/gte.h"

typedef struct {
    s32 x;
    s32 y;
    s32 z;
    s32 w;
} Vec4i;

extern s16 D_801E6E74;
extern s32 D_801E408C;
extern s32 D_801E428C;
extern s32 D_801E40E4;
extern s32 D_8007E2B8[];
extern struct {
    s16 x;
    s16 y;
} D_8007E290[];
extern Vec4i D_8007E298[];

void func_8001A530(Matrix *mtx, s32 angle);
void func_80069568(Matrix *lhs, Matrix *rhs);
void func_80017794(void *arg0, Vec4i *state, Matrix *mtx);
void func_800296B4(void *arg0, s32 arg1);

void func_8003D458(s32 arg0) {
    Matrix mtx;
    Vec4i state;
    s32 s1;
    s32 s0;
    register s32 value asm("$2");
    s32 drawArg;
    s32 rem;
    s32 lim;

    if (D_801E6E74 < 2 && arg0 >= 0x51) {
        func_8001A530(&mtx, D_8007E2B8[D_801E408C]);
        func_80069568((Matrix *)0x1F800028, &mtx);
        if (arg0 - 90 > 0) {
            state = D_8007E298[D_801E408C];
            s1 = (arg0 - 90) / 3;
            state.x += D_8007E290[D_801E408C].x * (s0 = s1 / 15);
            state.z += D_8007E290[D_801E408C].y * s0;
            if (D_801E428C % 4 == 3) {
                state.z += 0x5000;
            }
            func_80017794((void *)0x1F80011C, &state, &mtx);
            rem = s1 - s0 * 15;
            lim = D_801E40E4;
            __asm__ __volatile__("");
            value = rem + 0x28;
            *(s32 *)0x1F800084 = 0;
            drawArg = (value < lim) ? value : 1;
        } else {
            state = D_8007E298[D_801E408C];
            if (D_801E428C % 4 == 3) {
                state.z += 0x5000;
            }
            func_80017794((void *)0x1F80011C, &state, &mtx);
            lim = D_801E40E4;
            __asm__ __volatile__("");
            value = 0x28;
            *(s32 *)0x1F800084 = 0;
            drawArg = (value < lim) ? value : 1;
        }
        func_800296B4((void *)0x1F800000, drawArg);
    }
}
