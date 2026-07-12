#include "common.h"
#include "game/screens.h"

extern s16 D_801E4DAC;
extern s32 D_801E428C;
extern u8 *D_8019C900;
extern char D_80010DF0[];
extern char *D_8007D404[];

void func_80016EA0(s32 arg0, s32 arg1, void *arg2, s32 arg3);
void func_80016A18(s32 arg0, s32 arg1, void *arg2, s32 arg3);
s32 func_80032F34(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);

void GameDrawResultScreen(void) {
    register u8 *base asm("$16");
    register s32 *scratch asm("$17");
    register s32 width asm("$18");
    volatile s32 pad[12];
    s32 y;
    s32 next;

    (void)pad;
    func_80016EA0(0xDC, 0x1C, D_80010DF0, 0x7812);

    if (D_801E4DAC != 0) {
        y = 0x3C;
    } else {
        y = 0x39;
    }
    func_80016A18(0x60, y, D_8007D404[D_801E428C], 0x78CC);

    width = 0x140;
    base = D_8019C900;
    scratch = (s32 *)0x1F800000;
    base += 0xCC;

    next = func_80032F34(base, *scratch, 0, 0, width, 0x30, 0x85, 0x15, 0xE);
    *scratch = func_80032F34(base, next, 0, 0x30, width, 0x18, 0xF0, 0xF0, 0xF0);
}
