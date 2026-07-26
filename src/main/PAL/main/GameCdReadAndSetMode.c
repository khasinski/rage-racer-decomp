#include "common.h"
void func_8005F304(s32 arg0);
s32 func_80063DEC(s32 arg0);
void func_8005F55C(void);
void func_8005F2AC(void);
void func_80063DBC(s32 arg0);
void func_8005F4D8(void);
void func_8005EB20(s32 arg0) {
    func_8005F304(arg0);
    while (func_80063DEC(arg0 & 0xFF) == 0) {}
    func_8005F55C();
    func_8005F2AC();
    func_80063DBC(arg0 & 0xFF);
    func_8005F4D8();
}
