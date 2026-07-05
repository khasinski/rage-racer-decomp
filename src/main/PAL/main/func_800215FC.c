#include "common.h"
extern s32 D_8019C768;
extern s32 D_801E42E4;
extern s32 D_801E3E0C;
extern s32 D_801E40B8;
void func_8005B190(s32 arg0, s32 arg1);
void func_800215B8(s32 arg0);
void func_800215FC(void) {
    D_8019C768 = 0x80;
    func_8005B190(0x28, 0x28);
    D_801E42E4 = 0xE;
    D_801E3E0C = 0;
    D_801E40B8 = -1;
    func_800215B8(0xFF);
}
