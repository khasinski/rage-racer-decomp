#include "common.h"

s32 func_8006A534(s32 arg0, s32 arg1);
s32 func_8006A5A4(s32 arg0, void *arg1, s32 arg2);
s32 func_8006DD30(s32 arg0);
s32 func_8006CD0C(s32 arg0);

void func_8001F018(void *arg0) {
    register s32 status asm("$3");
    register s32 two asm("$2");
    u8 byte;

    do {
        while (func_8006A534(1, 0) == 0) {
        }

        byte = 0x80;
        while (func_8006A5A4(0xE, &byte, 0) == 0) {
        }

        {
            do {
                status = func_8006A534(1, 0);
                two = 2;
            } while (status == 0);

            if (status == two) {
                func_8006DD30(3);
            }

send:
            while (func_8006A5A4(0x15, arg0, 0) == 0) {
            }

            do {
                status = func_8006A534(1, 0);
                two = 2;
            } while (status == 0);
            if (status != two) {
                goto send;
            }
        }
    } while (func_8006CD0C(0x1E0) == 0);
}
