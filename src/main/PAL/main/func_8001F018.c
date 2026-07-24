#include "common.h"

s32 func_8006A534(s32 arg0, s32 arg1);
s32 func_8006A5A4(s32 arg0, void *arg1, s32 arg2);
s32 func_8006DD30(s32 arg0);
s32 func_8006CD0C(s32 arg0);

void func_8001F018(void *arg0) {
    u8 byte;

outer:
    while (func_8006A534(1, 0) == 0) {
    }

    byte = 0x80;
    while (func_8006A5A4(0xE, &byte, 0) == 0) {
    }

pollFirst:
    switch (func_8006A534(1, 0)) {
    case 0:
        goto pollFirst;
    case 2:
        func_8006DD30(3);
        break;
    }

send:
    while (func_8006A5A4(0x15, arg0, 0) == 0) {
    }

pollNext:
    switch (func_8006A534(1, 0)) {
    case 0:
        goto pollNext;
    case 2:
        break;
    default:
        goto send;
    }

    if (func_8006CD0C(0x1E0) == 0) {
        goto outer;
    }
}
