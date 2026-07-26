#include "common.h"
extern u8 D_800941F8;
void MemCopy(s32 arg0, void *arg1, s32 arg2) asm("func_800681BC");
s32 GetDrawEnv(s32 arg0) asm("func_80066074");
s32 GetDrawEnv(s32 arg0) { MemCopy(arg0, &D_800941F8, 0x5C); return arg0; }
