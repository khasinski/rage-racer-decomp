#include "common.h"

extern s32 D_801E40B8;
extern s32 D_801E3E0C;
extern void *D_8009E67C;
extern char D_80010E80[];
extern char D_80010E8C[];
extern char D_80010E98[];
extern char D_80010EA0[];
extern s16 D_8007D438[];

void func_80016EA0(s32 arg0, s32 arg1, void *arg2, s32 arg3);
void func_80016754(s32 arg0, s32 arg1, void *arg2, s32 arg3);
void func_800215B8(s32 arg0);

void func_80021654(void) {
    s32 color = 0x7812;
    s32 drawColor;
    s16 index;

    if (D_801E40B8 & 4) {
        color = 0x784C;
    }

    drawColor = 0x7812;
    if (D_801E3E0C == 0) {
        drawColor = color;
    }
    func_80016EA0(0x6A, 0x68, D_80010E80, drawColor);

    drawColor = 0x7812;
    if (D_801E3E0C != 0) {
        drawColor = color;
    }
    func_80016EA0(0x70, 0x78, D_80010E8C, drawColor);

    func_80016EA0(0x76, 0xB8, D_80010E98, 0x7812);

    index = *(s16 *)((u8 *)D_8009E67C + 6);
    func_80016EA0(0xBE, 0xB8, &D_8007D438[index], 0x7812);

    func_80016754(0x58, 0xD0, D_80010EA0, 0x78CC);
    func_800215B8(0xFF);
}
