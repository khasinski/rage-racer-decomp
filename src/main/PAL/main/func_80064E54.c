#include "common.h"

void func_80064E54(u32 *arg0, u32 arg1) {
    *arg0 = (*arg0 & 0xFF000000) | (arg1 & 0x00FFFFFF);
}

void func_80064E78(u32 *arg0) {
    *arg0 |= 0x00FFFFFF;
}
