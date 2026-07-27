#include "common.h"

void Gpu_ArmTimeout(void) asm("func_80067F04");

extern volatile u32 *D_800942BC;
extern volatile u32 *D_800942C8;
extern s32 D_800942EC;
extern s32 D_800942F0;

void func_80067984(void);
s32 Gpu_CheckTimeout(void) asm("func_80067F38");

/* Driver-table slot +0x3C, the body of DrawSync: mode 0 blocks until the
 * queue is empty and the GPU idle, any other mode returns the number of
 * queue entries still outstanding. */
s32 Gpu_DrawSync(s32 mode) asm("func_80067DBC");
s32 Gpu_DrawSync(s32 arg0) {
    s32 pending;

    if (arg0 == 0) {
        Gpu_ArmTimeout();
        goto pollState;

retry:
        func_80067984();
        if (Gpu_CheckTimeout() != 0) {
            return -1;
        }

pollState:
        if (D_800942EC == D_800942F0) {
            goto checkBusy;
        }
        goto retry;

waitReady:
        if (Gpu_CheckTimeout() != 0) {
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

extern s32 D_80094300;
extern s32 D_80094304;
s32 VSync(s32 mode) asm("func_8006DD30");
void Gpu_ArmTimeout(void) { D_80094300 = VSync(-1) + 240; D_80094304 = 0; }
