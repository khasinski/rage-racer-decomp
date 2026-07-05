#include "common.h"
#include "psyq/gte.h"

extern Matrix D_8019CAD4, D_8007C758, D_8009E6AC, D_8007C778;
void func_800698B8(Matrix *arg0);
void func_80069888(Matrix *arg0);
void func_80069A18(s32 arg0, s32 arg1, s32 arg2);
void func_800686D4(s32 arg0, s32 arg1);
void func_80069A38(s32 arg0, s32 arg1, s32 arg2);
void func_8001D210(void) {
    D_8019CAD4 = D_8007C758;
    D_8009E6AC = D_8007C778;
    func_800698B8(&D_8019CAD4);
    func_80069888(&D_8009E6AC);
    func_80069A18(0x20, 0x20, 0x20);
    func_800686D4(0x1770, 0x140);
    func_80069A38(0x80, 0x80, 0x80);
}
