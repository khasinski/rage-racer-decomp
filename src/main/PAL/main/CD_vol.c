#include "common.h"
#include "psyq/cd.h"

extern volatile u8 *D_80099300;
extern volatile u8 *D_80099304;
extern volatile u8 *D_80099308;
extern volatile u8 *D_8009930C;
extern s32 D_8009903C;
extern s32 D_80099040;
extern s32 D_8009904C;
extern s32 D_80099050;
extern u8 D_8009905C;
extern u8 D_8009905D;
extern volatile u32 *D_80099310;
extern CdRegisterMap *volatile D_80099314;
extern u8 D_80013904[];
extern u8 D_80013910[];
extern void *D_8009931C[];

typedef struct {
    u8 sync;
    u8 ready;
    u8 command;
} CdState;

extern volatile CdState D_80099318;

void func_8006DF34(void);
void func_8006C17C(void);
void func_8006DF64(s32 arg0, void *arg1);
s32 func_8006B620(s32 arg0, void *arg1, s32 arg2, s32 arg3);
s32 func_8006B0D4(s32 arg0, u8 *arg1);
s32 func_8001674C(u8 *format, ...);
void func_80063C38(u8 *text);

s32 CD_vol(CdlATV *arg0) {
    *D_80099300 = 2;
    *D_80099308 = arg0->val0;
    *D_8009930C = arg0->val1;
    *D_80099300 = 3;
    *D_80099304 = arg0->val2;
    *D_80099308 = arg0->val3;
    *D_8009930C = 0x20;
    return 0;
}

void CD_flush(void) {
    volatile u8 *state;
    volatile u8 *reg;

    *D_80099300 = 1;

    if ((*D_8009930C & 7) != 0) {
        do {
            *D_80099300 = 1;
            *D_8009930C = 7;
            *D_80099308 = 7;
        } while ((*D_8009930C & 7) != 0);
    }

    state = &D_80099318.ready;
    D_80099318.command = 0;
    *state = D_80099318.command;
    reg = D_80099300;
    D_80099318.sync = 2;
    *reg = 0;
    *D_8009930C = 0;
    *D_80099310 = 0x1325;
}

int CD_initvol(void) {
    CdRegisterMap *temp_v1;
    u8 sp0[4];

    temp_v1 = D_80099314;
    if (temp_v1->status_mode_a == 0 && temp_v1->status_mode_b == 0) {
        temp_v1->cd_left_volume = 0x3FFF;
        temp_v1->cd_right_volume = 0x3FFF;
        temp_v1 = D_80099314;
    }

    temp_v1->output_left_volume = 0x3FFF;
    temp_v1->output_right_volume = 0x3FFF;
    temp_v1->audio_control = 0xC001;

    sp0[2] = 0x80;
    sp0[0] = 0x80;
    sp0[3] = 0;
    sp0[1] = 0;

    *D_80099300 = 2;
    *D_80099308 = sp0[0];
    *D_8009930C = sp0[1];
    *D_80099300 = 3;
    *D_80099304 = sp0[2];
    *D_80099308 = sp0[3];
    *D_8009930C = 0x20;

    return 0;
}

void CD_initintr(void) {
    D_80099040 = 0;
    D_8009903C = 0;
    D_80099050 = 0;
    D_8009904C = 0;
    func_8006DF34();
    func_8006DF64(2, (void *)func_8006C17C);
}

s32 func_8006BD14(void) {
    func_80063C38(D_80013904);
    func_8001674C(D_80013910, D_8009931C);

    D_8009905D = 0;
    D_8009905C = 0;
    D_80099040 = 0;
    D_8009903C = 0;
    D_80099050 = 0;
    D_8009904C = 0;
    func_8006DF34();
    func_8006DF64(2, func_8006C17C);

    *D_80099300 = 1;
    while ((*D_8009930C & 7) != 0) {
        *D_80099300 = 1;
        *D_8009930C = 7;
        *D_80099308 = 7;
    }

    D_80099318.ready = D_80099318.command = 0;
    D_80099318.sync = 2;
    *D_80099300 = 0;
    *D_8009930C = 0;
    *D_80099310 = 0x1325;

    func_8006B620(1, 0, 0, 0);
    if ((D_8009904C & 0x10) != 0) {
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
