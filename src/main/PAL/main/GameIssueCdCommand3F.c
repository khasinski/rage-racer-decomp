#include "common.h"
#include "game/cd.h"

void func_80063E24(s32 arg0, s32 arg1, s32 arg2);
void func_80063E34(s32 arg0);

s32 GameIssueCdCommand3F(s32 arg0) {
    func_80063E34(arg0);
    func_80063E24(arg0, 0x3F, 0);
}
