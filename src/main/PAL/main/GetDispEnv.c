#include "common.h"
extern u8 D_80094254;
void MemCopy(s32 arg0, void *arg1, s32 arg2) asm("func_800681BC");
s32 GetDispEnv(s32 arg0) asm("func_8006655C");
s32 GetDispEnv(s32 arg0) { MemCopy(arg0, &D_80094254, 0x14); return arg0; }
