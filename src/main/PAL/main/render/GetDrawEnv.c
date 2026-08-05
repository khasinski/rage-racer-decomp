#include "common.h"
#include "game/render.h"
void MemCopy(s32 arg0, void *arg1, s32 arg2);
s32 GetDrawEnv(s32 arg0);
s32 GetDrawEnv(s32 arg0) { MemCopy(arg0, &g_DrawEnvCache, 0x5C); return arg0; }
