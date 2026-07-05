#include "common.h"

extern s32 D_8007F5FC;
extern u8 D_8007F5A8[];
extern u8 D_8009B194;
extern s32 D_8009B184;
extern s32 D_8009B188;
extern s32 D_8009B18C;
extern s32 D_8009B190;
extern s32 D_8009B174;
extern s32 D_8009B178;
extern s32 D_8009B17C;
extern s32 D_8009B180;

void func_80042D10(void);

void func_80042FA0(s32 arg0) {
    register s32 offset asm("a1");
    register s32 scale asm("a2");
    register s32 magic asm("a0");
    register s32 product asm("v1");
    s32 value;

    offset = D_8007F5FC;
    D_8009B194 = arg0;
    scale = D_8009B194;
    offset <<= 2;

    product = D_8007F5A8[offset] * scale;
    magic = 0x81020409;
    value = (product / 127) << 12;
    D_8009B184 = value;
    D_8009B174 = value;

    product = D_8007F5A8[offset + 1] * scale;
    value = (product / 127) << 12;
    D_8009B188 = value;
    D_8009B178 = value;

    product = D_8007F5A8[offset + 2] * scale;
    value = (product / 127) << 12;
    D_8009B18C = value;
    D_8009B17C = value;

    product = D_8007F5A8[offset + 3] * scale;
    value = (product / 127) << 12;
    D_8009B190 = value;
    D_8009B180 = value;

    func_80042D10();
}
