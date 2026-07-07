#include "common.h"
#include "psyq/cd.h"

extern s32 D_8007F5F8;
extern s32 D_8007F5FC;
extern s32 D_8007F600;
extern s32 D_8007F604;
extern s32 D_8007F608;
extern s32 D_8007F60C;
extern u8 D_8009B168;
extern u8 D_8009B16C;
extern u8 D_8009B194;
extern u8 D_8009B1B0;
extern s32 D_8009B1B4;
extern s32 D_8019C7BC;
extern s32 D_801E42E4;
extern CdlLOC D_8007F5B0[];

s32 func_8006A5A4(s32 arg0, void *arg1, s32 arg2);
s32 func_8006A554(s32 arg0, void *arg1);
s32 CdPosToInt_Local(CdlLOC *loc) asm("func_8006AADC");
void func_800432A8(void);
void func_80043494(void);
void func_800437B8(void);
void func_800431BC(void);
void func_80042FA0(s32 arg0);
void func_80042D10(void);
void SsSetSpuInputAttr_Link(u8 source, u8 field, u8 value) asm("SsSetSpuInputAttr");

INCLUDE_RODATA("asm/PAL/main/nonmatchings/main/func_80043598", func_80043598_rodata);
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_80043598", func_80043598);
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_80043598", func_800437B8);

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
