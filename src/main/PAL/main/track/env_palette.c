#include "common.h"
#include "game/render.h"
#include "game/track_internal.h"

void SetEnvPaletteTable(void *table) {
    g_EnvPaletteTable = table;
}


void LerpEnvColor(GameEnvColor *from, GameEnvColor *to, GameEnvColor *out,
                  s32 blend) {
    s32 local[3];
    u8 result[4];

    local[0] = from->bytes.r << 4;
    local[1] = from->bytes.g << 4;
    local[2] = from->bytes.b << 4;
    SetFarColor(to->bytes.r, to->bytes.g, to->bytes.b);
    Intpl(local, blend, result);
    out->bytes.r = result[0];
    out->bytes.g = result[1];
    out->bytes.b = result[2];
}
