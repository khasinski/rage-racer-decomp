#include "common.h"
#include "psyq/cd.h"

extern s32 D_8007F5F8;
extern s32 D_8007F5FC;
extern s32 D_8007F600;
extern s32 D_8007F604;
extern s32 D_8007F608;
extern s32 D_8007F60C;
extern u8 D_8009AFD0[];
extern u8 D_8009B168;
extern u8 D_8009B16C;
extern u8 D_8009B16E;
extern u8 D_8009B16F;
extern u8 D_8009B194;
extern u8 D_8009B1B0;
extern s32 D_8009B1B4;
extern s32 D_8019C7BC;
extern s32 D_801E42E4;
extern CdlLOC D_8007F5B0[];

s32 func_8006A5A4(s32 arg0, void *arg1, s32 arg2);
s32 func_8006A534(s32 arg0, s32 arg1);
s32 func_8006A554(s32 arg0, void *arg1);
s32 CdPosToInt_Local(CdlLOC *loc) asm("func_8006AADC");
void func_800432A8(void);
void func_80043494(void);
void func_800437B8(void);
void func_800431BC(void);
void func_80042FA0(s32 arg0);
void func_80042D10(void);
void SsSetSpuInputAttr_Link(u8 source, u8 field, u8 value) asm("SsSetSpuInputAttr");

void func_80043598(void) {
    s32 state;
    s32 result;
    s32 currentTime;
    s32 bestTime;
    s32 enteredTime;

    state = D_8007F60C;

    switch (state) {
    case 0:
        if (func_8006A534(1, 0) == 0) {
            break;
        }
        D_8007F60C = 1;
        /* fallthrough */

    case 1:
        if (func_8006A5A4(0x11, 0, (s32)&D_8009B16C) != 0) {
            D_8007F60C = 2;
        }
        break;

    case 2:
        result = func_8006A534(1, 0);
        if (result == 2) {
            D_8007F60C = 3;
        } else if (result == 5) {
            D_8007F60C = 1;
        }
        break;

    case 3:
        D_8009AFD0[0] = D_8009B16E;
        D_8009AFD0[2] = 0;
        D_8009AFD0[1] = D_8009B16F;

        currentTime = CdPosToInt_Local(&D_8007F5B0[D_8009B1B0]);
        bestTime = CdPosToInt_Local(&D_8007F5B0[0]);
        if (bestTime < currentTime) {
            enteredTime = CdPosToInt_Local((CdlLOC *)D_8009AFD0);
            currentTime = CdPosToInt_Local(&D_8007F5B0[D_8009B1B0]);
            if (enteredTime >= currentTime) {
                D_8007F5F8 = 1;
            } else {
                D_8007F5F8 = 0;
            }
        } else {
            D_8007F5F8 = 0;
        }

        D_8007F60C = 4;
        /* fallthrough */

    case 4:
        if (func_8006A5A4(9, 0, 0) != 0) {
            D_8007F60C = 5;
        }
        break;

    case 5:
        result = func_8006A534(1, 0);
        if (result == 2) {
            D_8007F60C = 6;
        } else if (result == 5) {
            D_8007F60C = 4;
        }
        break;

    case 6:
        D_8007F604 = -1;
        D_8007F60C = 0;
        break;
    }
}

void func_800437B8(void) {
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

void func_800438BC(void) {
    SsSetSpuInputAttr_Link(0, 0, 1);
    asm(".set noreorder\n"
        "move $4,$0\n"
        "li $5,0x7FFF\n"
        ".word 0x0C01BAF8\n"
        ".globl func_800438E0\n"
        "func_800438E0:\n"
        ".word 0x34067FFF\n"
        ".set noreorder");
    asm(".set noreorder\n"
        "li $4,0xE\n"
        "lui $5,%hi(D_8009B168)\n"
        "addiu $5,$5,%lo(D_8009B168)\n"
        "move $6,$0\n"
        "li $2,0x7\n"
        ".word 0x0C01A969\n"
        "sb $2,0($5)\n"
        ".set noreorder");
    func_800431BC();

    D_8007F600 = -1;
    D_8007F604 = -1;
    D_8009B1B0 = 2;
    D_8007F608 = 0;
    D_8007F60C = 0;
    D_8007F5FC = 0;
    D_8007F5F8 = 0;
    D_8009B194 = 0x7F;
    D_8009B1B4 = 0;
    func_80042FA0(0x7F);
}

void func_80043974(void) {
    register s32 temp asm("$16");
    register s32 status asm("$17");
    s32 state;
    s32 value;

    if (D_8007F600 < 0) {
        state = D_8007F604;
        if (state == 2) {
            goto state_2;
        }
        if (state < 3) {
            if (state == 1) {
                goto state_1;
            }
            goto check_cd;
        }
        if (state == 3) {
            goto state_3;
        }
        goto check_cd;
state_1:
        func_80043494();
        goto check_cd;
state_2:
        func_80043598();
        goto check_cd;
state_3:
        func_800437B8();
        goto check_cd;
    } else {
        func_800432A8();
check_cd:
        status = func_8006A554(1, &D_8009B16C);
        if (status == 4) {
            if (D_801E42E4 == 0x1C) {
                D_8019C7BC = 1;
            } else {
                temp = CdPosToInt_Local(&D_8007F5B0[D_8009B1B0]);
                value = CdPosToInt_Local(&D_8007F5B0[0]);
                if (value < temp) {
                    D_8007F608 = status;
                    D_8007F604 = 1;
                    D_8007F60C = 0;
                    D_8007F600 = D_8009B1B0;
                }
            }
        }
    }
done:
    func_80042D10();
}
