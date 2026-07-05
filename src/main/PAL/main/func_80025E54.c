#include "common.h"

extern u32 D_8009E6CC;
extern s32 D_801E40A8;
extern u8 D_801E7734[];

void func_8001B488(void);

void func_80025E54(u32 arg0) {
    u8 *first;
    u8 *before;
    u8 *other;
    u8 value;

    D_8009E6CC++;
    if (D_8009E6CC == D_801E40A8) {
        func_8001B488();

        first = D_801E7734;
        before = first - 1;
        if (arg0 == first[0]) {
            other = D_801E40A8 + before;
            value = *other;
            first[0] = value;
            *other = arg0;
        }
    }
}
