#include "common.h"
void func_8005F2AC(s32 arg0);
void func_80063DAC(s32 arg0);
void func_8005F4D8(void);
void func_8005EB78(s32 arg0) {
    func_8005F2AC(arg0);
    func_80063DAC(arg0 & 0xFF);
    func_8005F4D8();
}
