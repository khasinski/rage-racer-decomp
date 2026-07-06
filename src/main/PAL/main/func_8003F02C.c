#include "common.h"
#include "psyq/gte.h"

extern s32 D_801E4168;
extern s32 D_801E4340;
extern s32 D_801E4350;
extern volatile s32 D_801E4354;
extern s32 D_801E4358;

void func_80017794(void *arg0, void *arg1, Matrix *mtx);
void func_80017A10(s32 arg0);
void func_8001A4C0(Matrix *mtx, s32 angle);
void func_8001A530(Matrix *mtx, s32 angle);
void func_8001A5A0(Matrix *mtx, s32 angle);
void func_80028DEC(void *arg0, s32 arg1);
void func_80069568_entry(Matrix *lhs, Matrix *rhs);

asm("func_80069568_entry = 0x80069568");

static inline void ClearScratchRenderMode3F02C(void) {
    __asm__ volatile(
        ".set push\n"
        ".set noat\n"
        "lui $1, 0x1F80\n"
        "sw $0, 0x84($1)\n"
        ".set pop\n"
        ::: "memory");
}

void func_8003F02C(void) {
    Matrix mtx0;
    Matrix mtx1;
    register Matrix *mtx1Ptr asm("$16");
    s32 frameValue;
    s32 drawId;

    func_8001A530(&mtx0, 0x800 - D_801E4354);
    mtx1Ptr = &mtx1;
    func_8001A5A0(mtx1Ptr, D_801E4350);
    func_80069568_entry(&mtx0, mtx1Ptr);
    func_80069568_entry((Matrix *)0x1F800028, mtx1Ptr);
    func_8001A4C0(&mtx0, D_801E4358);
    func_80069568_entry(mtx1Ptr, &mtx0);
    func_80017A10(1);
    func_80017794((void *)0x1F80011C, &D_801E4340, &mtx0);
    frameValue = D_801E4168;
    __asm__ volatile("" : "=r"(frameValue) : "0"(frameValue));
    ClearScratchRenderMode3F02C();
    drawId = 1;
    if (frameValue >= 0x26) {
        drawId = 0x25;
    }
    func_80028DEC((void *)0x1F800000, drawId);
}
