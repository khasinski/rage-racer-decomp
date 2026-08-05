#include "common.h"

/* Local u32 view of g_EnvPaletteTable (render.h types it as u8 *). */
extern u32 g_EnvPaletteTable asm("D_801E4140");

void SetEnvPaletteTable(u32 arg0);
void SetEnvPaletteTable(u32 arg0) {
    g_EnvPaletteTable = arg0;
}

void SetFarColor(s32 arg0, s32 arg1, s32 arg2) asm("func_80069A38");
void Intpl(void *arg0, s32 arg1, void *arg2) asm("func_80069B14");

void LerpEnvColor(u8 *arg0, u8 *arg1, u8 *out, s32 arg3);
void LerpEnvColor(u8 *arg0, u8 *arg1, u8 *out, s32 arg3) {
    s32 local[3];
    u8 result[4];

    local[0] = arg0[0] << 4;
    local[1] = arg0[1] << 4;
    local[2] = arg0[2] << 4;
    SetFarColor(arg1[0], arg1[1], arg1[2]);
    Intpl(local, arg3, result);
    out[0] = result[0];
    out[1] = result[1];
    out[2] = result[2];
}
