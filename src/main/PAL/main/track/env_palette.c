#include "common.h"
#include "game/render.h"
#include "game/track_internal.h"

void SetEnvPaletteTable(void *table) {
    g_EnvPaletteTable = table;
}


void LerpEnvColor(u8 *from, u8 *to, u8 *out, s32 blend) {
    s32 local[3];
    u8 result[4];

    local[0] = from[0] << 4;
    local[1] = from[1] << 4;
    local[2] = from[2] << 4;
    SetFarColor(to[0], to[1], to[2]);
    Intpl(local, blend, result);
    out[0] = result[0];
    out[1] = result[1];
    out[2] = result[2];
}
