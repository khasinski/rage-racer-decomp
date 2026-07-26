#include "common.h"
#include "game/render.h"


void func_800332E0(s32 arg0) {
    *(u16 *)(g_DrawBuffer + 0x237A6) = arg0 ? 0x7811 : 0x7800;
}
