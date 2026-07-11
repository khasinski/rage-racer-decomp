#include "common.h"

extern s32 D_80082F4C;
extern s32 D_8009E668;
extern s32 D_8019C864;
extern s32 D_8019C8F0;
extern s32 D_801E825C;
extern char D_800127CC[];

void func_8005F2AC(void);
void func_800632F0(void *dst, void *fmt, s32 arg0, s32 arg1);
void func_8005F304(void);
s32 func_8005F35C(void);
s32 func_80063280(void *device);
s32 func_8005F55C(void);
void func_80063DAC(s32 arg0);
void func_80063DBC(s32 arg0);
void func_80063DEC(s32 arg0);

s32 func_8005ECE0(s32 arg0, s32 arg1) {
    s32 handle;
    s32 two;
    s32 status;
    s32 state;

    handle = (arg0 << 4) + arg1;

    switch (D_8009E668) {
    case 0:
        func_80063DAC(handle);
        D_8009E668 = 1;
        D_8019C864 = 0;
        goto clear_result;

    case 1:
        status = func_8005F35C();
        if (status == 0) {
            goto done;
        }

        two = 2;
        if (status == two) {
            goto fail_neg3_case1;
        }
        if (status < 3) {
            if (status == 1) {
                goto case1_ready;
            }
            goto fail_neg3_case1;
        }
        if (status == 3) {
            goto fail_neg1_case1;
        }
        if (status == 4) {
            goto case1_status4;
        }
        goto fail_neg3_case1;

case1_ready:
        D_801E825C = status;
        if (D_80082F4C == status) {
            D_8009E668 = 4;
        } else {
            D_8009E668 = two;
        }
        goto done;

fail_neg1_case1:
        state = -1;
        goto fail_case1;

case1_status4:
        D_801E825C = two;
        func_8005F304();
        func_80063DEC(handle);
        func_8005F55C();
        D_8009E668 = two;
        D_80082F4C = 0;
        goto done;

fail_neg3_case1:
        state = -3;

fail_case1:
        D_801E825C = state;
        D_8009E668 = 4;
        D_80082F4C = 0;
        goto done;

    case 2:
        func_8005F2AC();
        func_80063DBC(handle);
        D_8009E668 = 3;
        D_8019C864 = 0;
        goto done;

    case 3:
        status = func_8005F35C();
        if (status == 0) {
            goto done;
        }

        D_8009E668 = 4;
        if (status == 2) {
            goto fail_neg3_case3;
        }
        if (status < 3) {
            if (status == 1) {
                goto case3_ready;
            }
            goto fail_neg3_case3;
        }
        if (status == 3) {
            goto fail_neg1_case3;
        }
        if (status == 4) {
            goto fail_neg2_case3;
        }
        goto fail_neg3_case3;

case3_ready:
        D_80082F4C = status;
        goto done;

fail_neg1_case3:
        state = -1;
        goto fail_case3;

fail_neg2_case3:
        state = -2;
        goto fail_case3;

fail_neg3_case3:
        state = -3;

fail_case3:
        D_801E825C = state;
        D_80082F4C = 0;
        goto done;

    case 4:
        D_8009E668 = 0;
        D_8019C8F0 = D_801E825C;
        goto done;

    default:
        D_8009E668 = 0;
clear_result:
        D_8019C8F0 = 0;
        goto done;
    }

done:
    return D_8019C8F0;
}

s32 func_8005EF44(s32 arg0, s32 arg1) {
    char device[8];
    register s32 status asm("$3");

    func_800632F0(device, D_800127CC, arg0, arg1);
    func_8005F304();
    func_80063280(device);
    status = func_8005F55C();

    if (status != 1) {
        if (status == 3) {
            status = -1;
        } else {
            status = -3;
        }
    }

    return status;
}
