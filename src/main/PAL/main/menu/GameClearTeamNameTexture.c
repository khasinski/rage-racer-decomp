#include "common.h"
#include "psyq/gpu.h"

extern u8 D_8007D24C;
void LoadImage(Rect *rect, void *data) asm("func_80065B24");

void GameClearTeamNameTexture(void) asm("func_8001D4E8");
void GameClearTeamNameTexture(void) {
    s16 sp[4];
    sp[0] = 0x282;
    sp[1] = 0x37;
    sp[2] = 0xC;
    sp[3] = 8;
    LoadImage((Rect *)sp, &D_8007D24C);
}

extern u8 D_8007C7CC[];
void GameClearTeamNameTexture(void) asm("func_8001D4E8");
void LoadImage(Rect *rect, void *data) asm("func_80065B24");

void GameUploadTeamNameTexture(u8 *str, s32 len) asm("func_8001D530");
void GameUploadTeamNameTexture(u8 *str, s32 len) {
    s16 sp[4];
    GameClearTeamNameTexture();
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
