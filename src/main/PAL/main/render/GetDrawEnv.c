#include "common.h"
#include "game/render.h"
void MemCopy(s32 dst, void *src, s32 size);
s32 GetDrawEnv(s32 env);
s32 GetDrawEnv(s32 env) { MemCopy(env, &g_DrawEnvCache, 0x5C); return env; }
