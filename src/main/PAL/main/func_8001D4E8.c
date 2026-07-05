#include "common.h"
extern u8 D_8007D24C;
void func_80065B24(void *arg0, void *arg1);
void func_8001D4E8(void) {
    s16 sp[4];
    sp[0] = 0x282;
    sp[1] = 0x37;
    sp[2] = 0xC;
    sp[3] = 8;
    func_80065B24(sp, &D_8007D24C);
}
