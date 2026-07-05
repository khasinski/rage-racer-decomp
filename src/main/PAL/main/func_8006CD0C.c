#include "common.h"

extern s32 D_8019C7A0;

s32 func_8006A5A4(s32 arg0, void *arg1, s32 arg2);
void func_8006A994(s32 arg0);
void func_8006A58C(s32 arg0);
void func_8006CDA0(void);
void data_ready_callback(void) asm("func_8006CE78");

s32 func_8006CD0C(s32 arg0) {
    u8 byte;

    byte = arg0;
    func_8006A5A4(0xE, &byte, 0);

    if (arg0 & 0x100) {
        if (arg0 & 0x20) {
            D_8019C7A0 = 0;
        } else {
            D_8019C7A0 = 1;
        }
        func_8006A994((s32)data_ready_callback);
        func_8006A58C((s32)func_8006CDA0);
    }

    return func_8006A5A4(0x1B, 0, 0);
}
