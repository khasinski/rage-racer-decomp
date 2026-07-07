#include "common.h"

extern s32 D_8007F5F8;
extern s32 D_8007F5FC;
extern s32 D_8007F600;
extern s32 D_8007F604;
extern s32 D_8007F608;
extern s32 D_8007F60C;
extern u8 D_8009B168;
extern u8 D_8009B194;
extern u8 D_8009B1B0;
extern s32 D_8009B1B4;

s32 func_8006A5A4(s32 arg0, void *arg1, s32 arg2);
void func_800431BC(void);
void func_80042FA0(s32 arg0);
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

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_80043598", func_80043974);
