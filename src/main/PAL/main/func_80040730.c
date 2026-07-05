#include "common.h"
#include "psyq/gte.h"

extern s16 D_801E4DCA;
extern s16 D_801E4DC8;
extern s16 D_801E4DCC;
extern s16 D_801E4DB8;
extern s32 D_801E4168;
extern s32 D_801E40B8;

void func_8001A530(Matrix *mtx, s32 angle);
void func_8001A5A0(Matrix *mtx, s32 angle);
void func_8001A4C0(Matrix *mtx, s32 angle);
void func_80069568(Matrix *lhs, Matrix *rhs);
void func_80017A10(s32 arg0);
void func_80017794(void *arg0, void *arg1, Matrix *mtx);
void func_80028DEC(void *arg0, s32 arg1);

static inline void ClearScratchRenderMode(void) {
    __asm__ volatile(
        ".set push\n"
        ".set noat\n"
        "lui $1, 0x1F80\n"
        "sw $0, 0x84($1)\n"
        ".set pop\n"
        ::: "memory");
}

void func_80040730(void) {
    Matrix mtx0;
    Matrix mtx1;
    volatile s32 pad[4];
    s32 drawId;
    s32 frameValue;
    register s16 *anglePtr asm("$16");
    register Matrix *mtx1Ptr asm("$17");
    register Matrix *mtx0Ptr asm("$4");
    register void *scratchVec asm("$4");

    mtx0Ptr = &mtx0;
    __asm__("" : "=r"(mtx0Ptr) : "0"(mtx0Ptr));
    anglePtr = &D_801E4DCA;
    mtx1Ptr = &mtx1;

    func_8001A530(mtx0Ptr, 0x800 - anglePtr[0]);
    func_8001A5A0(mtx1Ptr, D_801E4DC8);
    func_80069568(&mtx0, mtx1Ptr);
    func_80069568((Matrix *)0x1F800028, mtx1Ptr);
    func_8001A4C0(&mtx0, D_801E4DCC);
    func_80069568(mtx1Ptr, &mtx0);

    func_80017A10(1);
    scratchVec = (void *)0x1F80011C;
    __asm__("" : "=r"(scratchVec) : "0"(scratchVec));
    anglePtr = (s16 *)((u8 *)anglePtr - 0x12);
    func_80017794(scratchVec, anglePtr, &mtx0);
    frameValue = D_801E4168;
    ClearScratchRenderMode();
    drawId = 1;
    if (frameValue >= 0x24) {
        drawId = 0x23;
    }
    func_80028DEC((void *)0x1F800000, drawId);

    {
        register s32 base asm("$3");
        register s32 acc asm("$5");
        register s32 tmp asm("$2");

        base = D_801E40B8;
        acc = base << 2;
        acc += base;
        tmp = acc << 5;
        acc += tmp;
        acc <<= 1;
        acc += base;
        func_8001A530(mtx1Ptr, acc & 0xFFF);
    }
    func_80069568(&mtx0, mtx1Ptr);
    func_80017794((void *)0x1F80011C, anglePtr, mtx1Ptr);
    frameValue = D_801E4168;
    ClearScratchRenderMode();
    drawId = 1;
    if (frameValue >= 0x25) {
        drawId = 0x24;
    }
    func_80028DEC((void *)0x1F800000, drawId);
}
