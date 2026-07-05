#include "psyq/kernel.h"

extern volatile s32 D_8009A4EC;
extern char D_80013B2C[];

void func_80063C38(char *arg0);
void waitVSync(s32 arg0, s32 arg1) {
    volatile s32 timeout;

    timeout = arg1 << 15;
    if (D_8009A4EC < arg0) {
        do {
            if (--timeout == -1) {
                func_80063C38(D_80013B2C);
                ChangeClearRCnt(0);
                ChangeClearInterruptMask(3, 0);
                break;
            }
        } while (D_8009A4EC < arg0);
    }
}
