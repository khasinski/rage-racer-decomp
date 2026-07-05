#include "psyq/spu.h"

extern volatile u16 *D_8009AB7C;
extern volatile s32 D_8009ABB0;
extern s32 D_8009ABC8;

void func_8006A3D8(s32 arg0, s32 arg1);

void _spu_transferCallback(void) {
    volatile s32 i;
    volatile s32 delay;
    u16 cnt;
    s32 wait;
    void (*callback)(void);

    if (D_8009ABC8 == 0) {
        delay = 0xD;
        i = 0;
        while (i < 0xF0) {
            delay *= 3;
            i++;
        }

        delay = 0xD;
        i = 0;
        while (i < 0xF0) {
            delay *= 3;
            i++;
        }

        delay = 0xD;
        i = 0;
        while (i < 0xF0) {
            delay *= 3;
            i++;
        }
    }

    cnt = D_8009AB7C[0xD5];
    D_8009AB7C[0xD5] = cnt & 0xFFCF;

    wait = 0;
    if ((D_8009AB7C[0xD5] & 0x30) != 0) {
        while (1) {
            wait++;
            if (wait >= 0xF01) {
                break;
            }
            if ((D_8009AB7C[0xD5] & 0x30) == 0) {
                wait--;
                break;
            }
        }
    }

    if (D_8009ABB0 != 0) {
        callback = (void (*)(void))D_8009ABB0;
        callback();
    } else {
        func_8006A3D8(0xF0000009, 0x20);
    }
}
