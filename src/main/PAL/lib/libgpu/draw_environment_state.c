#include "common.h"
#include "game/render.h"
s32 GetDrawEnv(s32 env) { MemCopy(env, &g_DrawEnvCache, 0x5C); return env; }
