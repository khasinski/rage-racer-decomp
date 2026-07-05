#include "common.h"

extern u8 D_800136B8[];

s32 func_8006A428(s32 arg0);
s32 func_8006A574(s32 arg0);
s32 func_8006A58C(s32 arg0);
void func_8006A360(void);
void func_8006A388(void);
void func_8006A3B0(void);
void func_8002785C(void *arg0);
void func_8001674C(void *arg0);

s32 func_8006A2D0(void) {
    s32 retries;

    retries = 4;
loop:
    if (func_8006A428(1) != 1) {
        retries--;
        if (retries != -1) {
            goto loop;
        }

        func_8001674C(D_800136B8);
        return 0;
    }

    func_8006A574((s32)func_8006A360);
    func_8006A58C((s32)func_8006A388);
    func_8002785C(func_8006A3B0);
    return 1;
}
