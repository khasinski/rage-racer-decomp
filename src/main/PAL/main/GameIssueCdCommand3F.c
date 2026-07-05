#include "common.h"
#include "game/cd.h"

void func_80063E24(s32 arg0, s32 arg1, s32 arg2);
void func_80063E34(s32 arg0);

s32 GameIssueCdCommand3F(s32 arg0) {
    register s32 temp asm("$16") = arg0;

    func_80063E34(arg0);
    func_80063E24(temp, 0x3F, 0);
}
