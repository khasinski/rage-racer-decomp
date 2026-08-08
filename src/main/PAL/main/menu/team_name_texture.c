#include "psyq/gpu.h"
#include "game/menu.h"

void ClearTeamNameTexture(void) {
    s16 sp[4];
    sp[0] = 0x282;
    sp[1] = 0x37;
    sp[2] = 0xC;
    sp[3] = 8;
    LoadImage((Rect *)sp, &D_8007D24C);
}

void UploadTeamNameTexture(u8 *str, s32 len) {
    s16 sp[4];
    ClearTeamNameTexture();
    sp[0] = 0x288 - len;
    sp[1] = 0x37;
    sp[2] = 2;
    sp[3] = 8;
    while (len > 0) {
        LoadImage((Rect *)sp, &D_8007C7CC[*str++ << 5]);
        sp[0] += 2;
        len--;
    }
}
