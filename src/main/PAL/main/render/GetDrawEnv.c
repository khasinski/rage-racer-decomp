#include "common.h"
#include "game/render.h"
void MemCopy(s32 arg0, void *arg1, s32 arg2);
s32 GetDrawEnv(s32 env);
s32 GetDrawEnv(s32 env) { MemCopy(env, &g_DrawEnvCache, 0x5C); return env; }
