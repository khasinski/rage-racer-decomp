#include "common.h"

extern u32 D_801E42E4;
extern u32 D_8019C760;

void func_80042C94(void);

void func_800268EC(void) {
    D_801E42E4 = 3;
    D_8019C760 = 0;
    func_80042C94();
}
