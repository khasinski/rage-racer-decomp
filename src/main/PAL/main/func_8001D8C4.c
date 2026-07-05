#include "common.h"

extern u32 D_8009E698;
extern u32 D_8009E87C;

void func_8001D748(u32 arg0, u32 arg1);
void func_80017B5C(u32 arg0);

void func_8001D8C4(u32 arg0) {
    func_8001D748(arg0, *(u32 *)(D_8009E698 + 0x24));
    func_80017B5C(D_8009E87C);
}
