#include "common.h"

/* Local u32 view of g_EnvPaletteTable (render.h types it as u8 *). */
extern u32 g_EnvPaletteTable asm("D_801E4140");

void func_8004553C(u32 arg0) {
    g_EnvPaletteTable = arg0;
}
