#include "common.h"
#include "game/render.h"
DrawEnv *GetDrawEnv(DrawEnv *env) { MemCopy((u_char *)env, (u_char *)&g_DrawEnvCache, 0x5C); return env; }
