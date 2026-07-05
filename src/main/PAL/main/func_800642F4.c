#include "common.h"

extern volatile u32 *D_80083178;
extern volatile u32 *D_80083198;
extern char D_800132E4[];
extern char D_800132F4[];

void func_80064424(char *arg0);

s32 func_800642F4(void) {
    volatile s32 timeout;

    timeout = 0x100000;
    if (*D_80083198 & 0x20000000) {
        do {
            if (--timeout == -1) {
                func_80064424(D_800132E4);
                return -1;
            }
        } while (*D_80083198 & 0x20000000);
    }
    return 0;
}

s32 func_8006438C(void) {
    volatile s32 timeout;

    timeout = 0x100000;
    if (*D_80083178 & 0x01000000) {
        do {
            if (--timeout == -1) {
                func_80064424(D_800132F4);
                return -1;
            }
        } while (*D_80083178 & 0x01000000);
    }
    return 0;
}
