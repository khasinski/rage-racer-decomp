#include "common.h"
typedef struct { s32 w[8]; } Buf32;
extern Buf32 D_8019CAD4, D_8007D548, D_8009E6AC, D_8007D568;
void func_800698B8(void *arg0);
void func_80069888(void *arg0);
void func_80069A18(s32 arg0, s32 arg1, s32 arg2);
void func_80069A38(s32 arg0, s32 arg1, s32 arg2);
void func_800686D4(s32 arg0, s32 arg1);
void func_800234DC(void) {
    D_8019CAD4 = D_8007D548;
    D_8009E6AC = D_8007D568;
    func_800698B8(&D_8019CAD4);
    func_80069888(&D_8009E6AC);
    func_80069A18(0x20, 0x20, 0x20);
    func_80069A38(0, 0, 0);
    func_800686D4(0x4E20, 0x140);
}
