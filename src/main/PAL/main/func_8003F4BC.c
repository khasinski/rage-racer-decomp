#include "common.h"
#include "psyq/gte.h"

extern u8 D_801E4FB8[];
extern u32 *D_801E6828;
extern s32 D_801E428C;
extern s32 D_801E40E4;

void func_80017794(void *arg0, void *arg1, Matrix *mtx);
void func_8001A4C0(Matrix *mtx, s32 angle);
void func_8001A530(Matrix *mtx, s32 angle);
void func_800296B4(void *arg0, s32 arg1);
void func_80069568(Matrix *lhs, Matrix *rhs);

static inline void ClearScratchRenderMode3F4BC(void) {
    __asm__ volatile(
        ".set push\n"
        ".set noat\n"
        "lui $1, 0x1F80\n"
        "sw $0, 0x84($1)\n"
        ".set pop\n"
        ::: "memory");
}

void func_8003F4BC(s32 arg0) {
    Matrix mtx0;
    Matrix mtx1;
    register u8 *state asm("$18");
    register u8 *base asm("$3");
    register Matrix *mtx1Ptr asm("$17");
    register s32 drawArg asm("$16");
    register s32 drawValue asm("$5");
    register s32 offset asm("$2");
    register s32 bucket asm("$2");
    register s32 bit asm("$5");
    register s32 firstValue asm("$3");
    register s32 value asm("$6");
    register u32 *visibility asm("$3");
    register u32 *wordPtr asm("$4");
    register s32 visible asm("$3");
    s32 frameValue;

    offset = (((arg0 * 3) << 2) + arg0) << 2;
    base = D_801E4FB8;
    state = base + offset;
    firstValue = *(s32 *)(state + 0x18);
    bucket = firstValue + 0x400;
    if (bucket < 0) {
        bucket = firstValue + 0xBFF;
    }
    bucket >>= 11;
    bucket <<= 2;
    value = *(s32 *)(state + 0x10);
    visibility = D_801E6828;
    bit = value + 0x400;
    wordPtr = (u32 *)(bucket + (s32)visibility);
    if (bit < 0) {
        bit = value + 0xBFF;
    }
    bucket = bit >> 11;
    visible = 1 << bucket;
    visible &= *wordPtr;

    if ((visible != 0) || (D_801E428C == 2)) {
        drawArg = 0x3F;
        func_8001A530(&mtx0, *(s32 *)(state + 0x24));
        mtx1Ptr = &mtx1;
        func_8001A4C0(mtx1Ptr, *(s32 *)(state + 0x28));
        func_80069568(&mtx0, mtx1Ptr);
        func_80069568((Matrix *)0x1F800028, mtx1Ptr);
        if ((D_801E428C & 3) >= 2) {
            drawArg = 0x3C;
        }
        func_80017794((void *)0x1F80011C, state + 0x10, mtx1Ptr);
        frameValue = D_801E40E4;
        ClearScratchRenderMode3F4BC();
        drawValue = 1;
        if (drawArg < frameValue) {
            drawValue = drawArg;
        }
        func_800296B4((void *)0x1F800000, drawValue);
    }
}
