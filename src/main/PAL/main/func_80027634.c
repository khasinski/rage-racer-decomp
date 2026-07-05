#include "common.h"


extern volatile s32 D_8007D7A4;
extern volatile s32 D_8007D7B4;
extern volatile s32 D_8007D7B8;

void func_8006A574(s32 arg0);
void func_8006A58C(s32 arg0);
s32 func_8006A5A4(s32 arg0, void *arg1, s32 arg2);

void func_80027634(void) {
    register volatile s32 *ptr asm("$2");

    ptr = &D_8007D7A4;
    *ptr = 0;
    func_8006A574(D_8007D7B4);
    func_8006A58C(D_8007D7B8);
    func_8006A5A4(9, 0, 0);
}
