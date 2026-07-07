#include "common.h"


extern volatile u32 *D_800942BC;
extern volatile u32 *D_800942C8;
extern s32 D_800942EC;
extern s32 D_800942F0;

void func_80067984(void);
void func_80067F04(void);
s32 func_80067F38(void);

s32 func_80067DBC(s32 arg0) {
    s32 pending;

    if (arg0 == 0) {
        func_80067F04();
        goto pollState;

retry:
        func_80067984();
        if (func_80067F38() != 0) {
            return -1;
        }

pollState:
        if (D_800942EC == D_800942F0) {
            goto checkBusy;
        }
        goto retry;

waitReady:
        if (func_80067F38() != 0) {
            return -1;
        }

checkBusy:
        if (*D_800942C8 & 0x01000000) {
            goto waitReady;
        }

        if (*D_800942BC & 0x04000000) {
            return 0;
        }
        goto waitReady;
    }

    pending = (D_800942EC - D_800942F0) & 0x3F;
    if (pending != 0) {
        func_80067984();
    }

    if (*D_800942C8 & 0x01000000) {
        goto tail;
    }

    if (*D_800942BC & 0x04000000) {
        goto returnPending;
    }

tail:
    if (pending == 0) {
        return 1;
    }
    return pending;

returnPending:
    return pending;
}
