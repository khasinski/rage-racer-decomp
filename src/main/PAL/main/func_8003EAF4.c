#include "common.h"
#include "psyq/gte.h"

extern s32 D_801E4168;
extern s32 D_801E431C;
extern s32 D_801E4320;
extern s32 D_801E4324;
extern s32 D_801E42FC[];

void func_80017794(void *arg0, void *arg1, Matrix *mtx);
void func_80017A10(s32 arg0);
void func_8001A4C0(Matrix *mtx, s32 angle);
void func_8001A530(Matrix *mtx, s32 angle);
void func_8001A5A0(Matrix *mtx, s32 angle);
void func_80028DEC(void *arg0, s32 arg1);
void func_80069568(Matrix *lhs, Matrix *rhs);

static inline void ClearScratchRenderMode3EAF4(void) {
    __asm__ volatile(
        ".set push\n"
        ".set noat\n"
        "lui $1, 0x1F80\n"
        "sw $0, 0x84($1)\n"
        ".set pop\n"
        ::: "memory");
}

void func_8003EAF4(void) {
    Matrix mtx0;
    Matrix mtx1;
    register Matrix *mtx0Ptr asm("$4");
    register s32 *state asm("$17");
    register Matrix *mtx1Ptr asm("$16");
    register s32 angle asm("$2");
    register s32 baseAngle asm("$5");
    register void *drawBase asm("$4");
    s32 frameValue;

    state = D_801E42FC;
    if (state[0] > 0) {
        mtx0Ptr = &mtx0;
        angle = D_801E4320;
        __asm__ volatile("" : "=r"(angle) : "0"(angle));
        baseAngle = 0x800;
        __asm__ volatile("" : "=r"(baseAngle) : "0"(baseAngle), "r"(angle));
        func_8001A530(mtx0Ptr, baseAngle - angle);
        mtx1Ptr = &mtx1;
        func_8001A5A0(mtx1Ptr, D_801E431C);
        func_80069568(&mtx0, mtx1Ptr);
        func_80069568((Matrix *)0x1F800028, mtx1Ptr);
        func_8001A4C0(&mtx0, D_801E4324);
        func_80069568(mtx1Ptr, &mtx0);
        func_80017A10(2);
        func_80017794((void *)0x1F80011C, state + 4, &mtx0);
        frameValue = D_801E4168;
        __asm__ volatile("" : "=r"(frameValue) : "0"(frameValue));
        drawBase = (void *)0x1F800000;
        ClearScratchRenderMode3EAF4();
        func_80028DEC(drawBase, frameValue < 1);
    }
}
