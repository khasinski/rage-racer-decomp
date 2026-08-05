#include "common.h"
#include "psyq/cd.h"
long CD_sync(long arg0, u_char *arg1) asm("func_8006B0D4");

extern volatile u_char *g_CdReg0 asm("D_80099300");
extern volatile u_char *g_CdReg1 asm("D_80099304");
extern volatile u_char *g_CdReg2 asm("D_80099308");
extern volatile u_char *g_CdReg3 asm("D_8009930C");
extern long g_CdSyncCallback asm("D_8009903C");
extern long g_CdReadyCallback asm("D_80099040");
extern long g_CdStatusByte asm("D_8009904C");
extern long g_CdErrorByte asm("D_80099050");
extern u_char g_CdModeByte asm("D_8009905C");
extern u_char g_CdLastCommand asm("D_8009905D");
extern volatile u_long *g_ComDelayReg asm("D_80099310");
extern CdRegisterMap *volatile g_CdSpuRegs asm("D_80099314");
extern u_char D_80013904[];
extern u_char D_80013910[];
extern void *g_CdDebugInfo[] asm("D_8009931C");

extern volatile CdIntr g_CdSyncStatus asm("D_80099318");

void KernelCallbackSlot3(void) asm("func_8006DF34");
void CdDispatchInterrupts(void);
void KernelCallbackSlot2(long arg0, void *arg1) asm("func_8006DF64");
long func_8006B620(long arg0, void *arg1, long arg2, long arg3);
void LibcPutString(u_char *text) asm("func_80063C38");

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

    if ((*g_CdReg3 % 8) != 0) {
        do {
            *g_CdReg0 = 1;
            *g_CdReg3 = 7;
            *g_CdReg2 = 7;
        } while ((*g_CdReg3 % 8) != 0);
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
    CdRegisterMap *cdSpuRegs;
    u_char sp0[4];

    cdSpuRegs = g_CdSpuRegs;
    if (cdSpuRegs->status_mode_a == 0 && cdSpuRegs->status_mode_b == 0) {
        cdSpuRegs->cd_left_volume = 0x3FFF;
        cdSpuRegs->cd_right_volume = 0x3FFF;
        cdSpuRegs = g_CdSpuRegs;
    }

    cdSpuRegs->output_left_volume = 0x3FFF;
    cdSpuRegs->output_right_volume = 0x3FFF;
    cdSpuRegs->audio_control = 0xC001;

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
    KernelCallbackSlot2(2, (void *)CdDispatchInterrupts);
}

long CdResetState(void) asm("func_8006BD14");
long CdResetState(void) {
    LibcPutString(D_80013904);
    DebugPrintf(D_80013910, g_CdDebugInfo);

    g_CdLastCommand = 0;
    g_CdModeByte = 0;
    g_CdReadyCallback = 0;
    g_CdSyncCallback = 0;
    g_CdErrorByte = 0;
    g_CdStatusByte = 0;
    KernelCallbackSlot3();
    KernelCallbackSlot2(2, CdDispatchInterrupts);

    *g_CdReg0 = 1;
    while ((*g_CdReg3 % 8) != 0) {
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
    if (CD_sync(0, 0) != 2) {
        return -1;
    }
    return 0;
}
