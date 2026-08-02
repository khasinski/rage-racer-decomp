#include "common.h"
#include "psyq/cd.h"

extern volatile u_char *g_CdReg0 asm("D_80099300");
extern volatile u_char *g_CdReg1 asm("D_80099304");
extern volatile u_char *g_CdReg2 asm("D_80099308");
extern volatile u_char *g_CdReg3 asm("D_8009930C");
extern long g_CdSyncCallback asm("D_8009903C");
extern long g_CdReadyCallback asm("D_80099040");
extern long g_CdStatusByte asm("D_8009904C");
extern long g_CdErrorByte asm("D_80099050");
extern u_char g_CdModeByte asm("D_8009905C");
extern u_char D_8009905D;
extern volatile u_long *g_ComDelayReg asm("D_80099310");
extern CdRegisterMap *volatile g_CdSpuRegs asm("D_80099314");
extern u_char D_80013904[];
extern u_char D_80013910[];
extern void *g_CdDebugInfo[] asm("D_8009931C");

typedef struct CdState {
    u_char sync;
    u_char ready;
    u_char command;
} CdState;

extern volatile CdState g_CdSyncStatus asm("D_80099318");

void KernelCallbackSlot3(void) asm("func_8006DF34");
void func_8006C17C(void);
void KernelCallbackSlot2(long arg0, void *arg1) asm("func_8006DF64");
long func_8006B620(long arg0, void *arg1, long arg2, long arg3);
long func_8006B0D4(long arg0, u_char *arg1);
void func_80063C38(u_char *text);

long CD_vol(CdlATV *arg0) {
    *g_CdReg0 = 2;
    *g_CdReg2 = arg0->val0;
    *g_CdReg3 = arg0->val1;
    *g_CdReg0 = 3;
    *g_CdReg1 = arg0->val2;
    *g_CdReg2 = arg0->val3;
    *g_CdReg3 = 0x20;
    return 0;
}

void CD_flush(void) {
    volatile u_char *state;
    volatile u_char *reg;

    *g_CdReg0 = 1;

    if ((*g_CdReg3 & 7) != 0) {
        do {
            *g_CdReg0 = 1;
            *g_CdReg3 = 7;
            *g_CdReg2 = 7;
        } while ((*g_CdReg3 & 7) != 0);
    }

    state = &g_CdSyncStatus.ready;
    g_CdSyncStatus.command = 0;
    *state = g_CdSyncStatus.command;
    reg = g_CdReg0;
    g_CdSyncStatus.sync = 2;
    *reg = 0;
    *g_CdReg3 = 0;
    *g_ComDelayReg = 0x1325;
}

long CD_initvol(void) {
    CdRegisterMap *temp_v1;
    u_char sp0[4];

    temp_v1 = g_CdSpuRegs;
    if (temp_v1->status_mode_a == 0 && temp_v1->status_mode_b == 0) {
        temp_v1->cd_left_volume = 0x3FFF;
        temp_v1->cd_right_volume = 0x3FFF;
        temp_v1 = g_CdSpuRegs;
    }

    temp_v1->output_left_volume = 0x3FFF;
    temp_v1->output_right_volume = 0x3FFF;
    temp_v1->audio_control = 0xC001;

    sp0[2] = 0x80;
    sp0[0] = 0x80;
    sp0[3] = 0;
    sp0[1] = 0;

    *g_CdReg0 = 2;
    *g_CdReg2 = sp0[0];
    *g_CdReg3 = sp0[1];
    *g_CdReg0 = 3;
    *g_CdReg1 = sp0[2];
    *g_CdReg2 = sp0[3];
    *g_CdReg3 = 0x20;

    return 0;
}

void CD_initintr(void) {
    g_CdReadyCallback = 0;
    g_CdSyncCallback = 0;
    g_CdErrorByte = 0;
    g_CdStatusByte = 0;
    KernelCallbackSlot3();
    KernelCallbackSlot2(2, (void *)func_8006C17C);
}

long func_8006BD14(void) {
    func_80063C38(D_80013904);
    DebugPrintf(D_80013910, g_CdDebugInfo);

    D_8009905D = 0;
    g_CdModeByte = 0;
    g_CdReadyCallback = 0;
    g_CdSyncCallback = 0;
    g_CdErrorByte = 0;
    g_CdStatusByte = 0;
    KernelCallbackSlot3();
    KernelCallbackSlot2(2, func_8006C17C);

    *g_CdReg0 = 1;
    while ((*g_CdReg3 & 7) != 0) {
        *g_CdReg0 = 1;
        *g_CdReg3 = 7;
        *g_CdReg2 = 7;
    }

    g_CdSyncStatus.ready = g_CdSyncStatus.command = 0;
    g_CdSyncStatus.sync = 2;
    *g_CdReg0 = 0;
    *g_CdReg3 = 0;
    *g_ComDelayReg = 0x1325;

    func_8006B620(1, 0, 0, 0);
    if ((g_CdStatusByte & 0x10) != 0) {
        func_8006B620(1, 0, 0, 0);
    }

    if (func_8006B620(0xA, 0, 0, 0) != 0) {
        return -1;
    }
    if (func_8006B620(0xC, 0, 0, 0) != 0) {
        return -1;
    }
    if (func_8006B0D4(0, 0) != 2) {
        return -1;
    }
    return 0;
}
