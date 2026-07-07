#include "common.h"
#include "psyq/cd.h"

extern s32 D_8009903C;
extern s32 D_80099040;
extern s32 D_8009904C;
extern s32 D_80099050;
extern u8 D_80099318;
extern volatile u8 D_8009931A;
extern u8 *D_80099300;
extern u8 *D_80099304;
extern u8 *D_80099308;
extern u8 *D_8009930C;
extern u32 *D_80099310;
extern CdRegisterMap *volatile D_80099314;

void func_8006DF34(void);
void func_8006C17C(void);
void func_8006DF64(s32 arg0, void *arg1);

asm(".globl func_8006BBD0_entry\nfunc_8006BBD0_entry = func_8006BBD0");
asm(".globl func_8006BCC4_entry\nfunc_8006BCC4_entry = func_8006BCC4");
asm(".globl func_8006BD14_entry\nfunc_8006BD14_entry = func_8006BD14");

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
    register u8 *v1_p asm("$3");
    register u32 v0 asm("$2");
    register u32 v1 asm("$3");
    register u32 a0 asm("$4");

    *D_80099300 = 1;

    if ((*D_8009930C & 7) != 0) {
        v1 = 7;
        a0 = 1;
        do {
            *D_80099300 = a0;
            *D_8009930C = v1;
            *D_80099308 = v1;
        } while ((*D_8009930C & 7) != 0);
    }

    D_8009931A = 0;
    asm("lui $3,%hi(D_8009931A)");
    asm("lbu $3,%lo(D_8009931A)($3)");
    asm("lui $2,%hi(D_80099319)");
    asm("addiu $2,$2,%lo(D_80099319)");
    asm("sb $3,0($2)");
    v1_p = D_80099300;
    v0 = 2;
    D_80099318 = v0;
    *v1_p = 0;
    *D_8009930C = 0;
    asm("lui $3,%hi(D_80099310)");
    asm("lw $3,%lo(D_80099310)($3)");
    asm("ori $2,$0,0x1325");
    asm("sw $2,0($3)");
}

int CD_initvol(void) {
    CdRegisterMap *temp_v1;
    u8 sp0[4];

    temp_v1 = D_80099314;

    if (temp_v1->status_mode_a == 0) {
        if (temp_v1->status_mode_b == 0) {
            temp_v1->cd_left_volume = 0x3FFF;
            temp_v1->cd_right_volume = 0x3FFF;
            temp_v1 = D_80099314;
        }
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

void func_8006BCC4(void) {
    D_80099040 = 0;
    D_8009903C = 0;
    D_80099050 = 0;
    D_8009904C = 0;
    func_8006DF34();
    func_8006DF64(2, (void *)func_8006C17C);
}

asm(
    ".set noreorder\n"
    ".set noat\n"
    ".globl func_8006BD14\n"
    "func_8006BD14:\n"
    ".word 0x27BDFFE8\n"
    ".word 0x3C048001\n"
    ".word 0x24843904\n"
    ".word 0xAFBF0010\n"
    ".word 0x0C018F0E\n"
    ".word 0x00000000\n"
    ".word 0x3C048001\n"
    ".word 0x24843910\n"
    ".word 0x3C05800A\n"
    ".word 0x24A5931C\n"
    ".word 0x0C0059D3\n"
    ".word 0x00000000\n"
    ".word 0x3C01800A\n"
    ".word 0xA020905D\n"
    ".word 0x3C01800A\n"
    ".word 0xA020905C\n"
    ".word 0x3C01800A\n"
    ".word 0xAC209040\n"
    ".word 0x3C01800A\n"
    ".word 0xAC20903C\n"
    ".word 0x3C01800A\n"
    ".word 0xAC209050\n"
    ".word 0x3C01800A\n"
    ".word 0xAC20904C\n"
    ".word 0x0C01B7CD\n"
    ".word 0x00000000\n"
    ".globl func_8006BD7C\n"
    "func_8006BD7C:\n"
    ".word 0x3C058007\n"
    ".word 0x24A5C17C\n"
    ".word 0x0C01B7D9\n"
    ".word 0x34040002\n"
    ".word 0x3C03800A\n"
    ".word 0x8C639300\n"
    ".word 0x34020001\n"
    ".word 0xA0620000\n"
    ".word 0x3C02800A\n"
    ".word 0x8C42930C\n"
    ".word 0x00000000\n"
    ".word 0x90420000\n"
    ".word 0x00000000\n"
    ".word 0x30420007\n"
    ".word 0x10400016\n"
    ".word 0x34040001\n"
    ".word 0x34030007\n"
    ".word 0x3C02800A\n"
    ".word 0x8C429300\n"
    ".word 0x00000000\n"
    ".word 0xA0440000\n"
    ".word 0x3C02800A\n"
    ".word 0x8C42930C\n"
    ".word 0x00000000\n"
    ".word 0xA0430000\n"
    ".word 0x3C02800A\n"
    ".word 0x8C429308\n"
    ".word 0x00000000\n"
    ".word 0xA0430000\n"
    ".word 0x3C02800A\n"
    ".word 0x8C42930C\n"
    ".word 0x00000000\n"
    ".word 0x90420000\n"
    ".word 0x00000000\n"
    ".word 0x30420007\n"
    ".word 0x1440FFED\n"
    ".word 0x00000000\n"
    ".word 0x34040001\n"
    ".word 0x00002821\n"
    ".word 0x00003021\n"
    ".word 0x3C01800A\n"
    ".word 0xA020931A\n"
    ".word 0x3C03800A\n"
    ".word 0x9063931A\n"
    ".word 0x3C02800A\n"
    ".word 0x24429319\n"
    ".word 0xA0430000\n"
    ".word 0x3C03800A\n"
    ".word 0x8C639300\n"
    ".word 0x34020002\n"
    ".word 0x3C01800A\n"
    ".word 0xA0229318\n"
    ".word 0xA0600000\n"
    ".word 0x3C02800A\n"
    ".word 0x8C42930C\n"
    ".word 0x00003821\n"
    ".word 0xA0400000\n"
    ".word 0x3C03800A\n"
    ".word 0x8C639310\n"
    ".word 0x34021325\n"
    ".word 0xAC620000\n"
    ".word 0x0C01AD88\n"
    ".word 0x00000000\n"
    ".word 0x3C02800A\n"
    ".word 0x8C42904C\n"
    ".word 0x00000000\n"
    ".word 0x30420010\n"
    ".word 0x10400005\n"
    ".word 0x34040001\n"
    ".word 0x00002821\n"
    ".word 0x00003021\n"
    ".word 0x0C01AD88\n"
    ".word 0x00003821\n"
    ".word 0x3404000A\n"
    ".word 0x00002821\n"
    ".word 0x00003021\n"
    ".word 0x0C01AD88\n"
    ".word 0x00003821\n"
    ".word 0x1440000E\n"
    ".word 0x2402FFFF\n"
    ".word 0x3404000C\n"
    ".word 0x00002821\n"
    ".word 0x00003021\n"
    ".word 0x0C01AD88\n"
    ".word 0x00003821\n"
    ".word 0x14400007\n"
    ".word 0x2402FFFF\n"
    ".word 0x00002021\n"
    ".word 0x0C01AC35\n"
    ".word 0x00002821\n"
    ".word 0x38420002\n"
    ".word 0x0002102B\n"
    ".word 0x00021023\n"
    ".word 0x8FBF0010\n"
    ".word 0x27BD0018\n"
    ".word 0x03E00008\n"
    ".word 0x00000000\n");
