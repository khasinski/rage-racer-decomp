#include "common.h"
#include "psyq/cd.h"

extern s32 D_8007F600;
extern s32 D_8007F604;
extern s32 D_8007F608;
extern s32 D_8007F60C;
extern u8 D_8009B194;
extern u8 D_8009B1B0;
extern s32 D_8009B1B4;
extern CdlLOC D_8009AFD4[];

s32 func_8006A534(s32 arg0, s32 arg1);
s32 func_8006A5A4(s32 arg0, void *arg1, s32 arg2);
void func_80042FA0(s32 arg0);

void func_800432A8(void) {
    s32 state;
    register s32 status asm("$4");
    u8 track;

    switch (D_8007F608) {
    case 0:
        if (func_8006A534(1, 0) == 0) {
            goto done;
        }
        D_8009B1B4 = 0;
        D_8007F608 = 1;
    case 1:
        if (func_8006A5A4(0x16, &D_8009AFD4[D_8007F600], 0) == 0) {
            goto done;
        }
        D_8007F608 = 2;
        goto done;
    case 2:
        state = func_8006A534(1, 0);
        if (state == 2) {
            D_8007F608 = 3;
            goto done;
        }
        if (state == 5) {
            D_8007F608 = 1;
            goto done;
        }
        goto done;
    case 3:
        status = D_8009B194;
        track = *(u8 *)&D_8007F600;
        D_8007F600 = -1;
        D_8007F608 = 0;
        D_8009B1B0 = track;
        func_80042FA0(status);
        goto done;
    case 4:
        if (func_8006A534(1, 0) == 0) {
            goto done;
        }
        D_8007F608 = 5;
    case 5:
        if (func_8006A5A4(0x16, &D_8009AFD4[D_8007F600], 0) == 0) {
            goto done;
        }
        D_8007F608 = 6;
        goto done;
    case 6:
        state = func_8006A534(1, 0);
        if (state == 2) {
            D_8007F608 = 7;
            goto done;
        }
        if (state == 5) {
            D_8007F608 = state;
            goto done;
        }
        goto done;
    case 7:
        track = *(u8 *)&D_8007F600;
        D_8007F600 = -1;
        D_8007F608 = 0;
        D_8009B1B0 = track;
        break;
    }

done:
    return;
}

void func_80043494(void) {
    s32 state;
    s32 status;

    state = D_8007F60C;
    if (state == 1) {
        goto state_1;
    }
    if (state >= 2) {
        goto state_ge_2;
    }
    if (state == 0) {
        goto state_0;
    }
    goto done;

state_ge_2:
    if (state == 2) {
        goto state_2;
    }
    if (state == 3) {
        goto state_3;
    }
    goto done;

state_0:
    if (func_8006A534(1, 0) == 0) {
        goto done;
    }
    D_8007F60C = 1;
state_1:
    if (func_8006A5A4(3, 0, 0) == 0) {
        goto done;
    }
    D_8007F60C = 2;
    goto done;

state_2:
    status = func_8006A534(1, 0);
    if (status == state) {
        D_8007F60C = 3;
        goto done;
    }
    if (status == 5) {
        D_8007F60C = 1;
        goto done;
    }
    goto done;

state_3:
    D_8007F604 = -1;
    D_8007F60C = 0;

done:
    return;
}
