#include "common.h"
#include "psyq/gte.h"

extern s32 D_8007E35C;
extern s32 D_801E4030;
extern s32 D_801E40E4;

void func_80017794(void *arg0, void *arg1, Matrix *mtx);
void func_8001A530(Matrix *mtx, s32 angle);
void func_800296B4(void *arg0, s32 arg1);
void func_80029E50(void *arg0, s32 arg1);
void func_80069568(Matrix *lhs, Matrix *rhs);

static inline void SetScratchRenderMode3E0D0(s32 mode) {
    __asm__ volatile(
        ".set push\n"
        ".set noat\n"
        "lui $1, 0x1F80\n"
        "sw %0, 0x84($1)\n"
        ".set pop\n"
        :
        : "r"(mode)
        : "memory");
}

static inline void ClearScratchRenderMode3E0D0(void) {
    __asm__ volatile(
        ".set push\n"
        ".set noat\n"
        "lui $1, 0x1F80\n"
        "sw $0, 0x84($1)\n"
        ".set pop\n"
        ::: "memory");
}

void func_8003E0D0(void) {
    Matrix mtx;
    s32 pad[4];
    register s32 *state asm("$16");
    register s32 frameValue asm("$2");
    register s32 drawArg asm("$5");

    (void)pad;
    state = &D_8007E35C;
    func_8001A530(&mtx, state[0]);
    func_80069568((Matrix *)0x1F800028, &mtx);

    if (D_801E4030 != 0) {
        func_80017794((void *)0x1F80011C, state - 3, &mtx);
        frameValue = D_801E40E4;
        SetScratchRenderMode3E0D0(0x10000);
        drawArg = 1;
        if (frameValue >= 0x40) {
            drawArg = 0x3F;
        }
        func_800296B4((void *)0x1F800000, drawArg);
    } else {
        func_80017794((void *)0x1F80011C, state - 3, &mtx);
        frameValue = D_801E40E4;
        ClearScratchRenderMode3E0D0();
        drawArg = 1;
        if (frameValue >= 0x40) {
            drawArg = 0x3F;
        }
        func_80029E50((void *)0x1F800000, drawArg);
    }
}
