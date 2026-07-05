#include "common.h"

extern s32 D_8019C99C;
extern s32 D_801E40B8;
extern void (*D_8007D6B8[])(void);

void func_80026570(void) {
    void (*func)(void);

    func = D_8007D6B8[D_8019C99C];
    D_801E40B8++;
    func();
}
