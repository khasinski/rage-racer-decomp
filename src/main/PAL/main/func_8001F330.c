#include "common.h"

typedef struct {
    s32 x;
    s32 y;
    s32 z;
    u8 pad0C[0x14];
    s32 rotX;
    s32 rotY;
    s32 rotZ;
    u8 pad2C[0x18];
    s32 field44;
    s32 flags48;
    u8 pad4C[0x14];
    s32 z2;
    u8 pad64[0x28];
    s16 field8C;
    u8 pad8E[0x20];
    s16 variantAE;
} MenuObj;

typedef struct {
    u16 x0;
    s16 y0;
    u16 z0;
    s16 z20;
    s16 rotX0;
    s16 rotY0;
    s16 rotZ0;
    s16 flags0;
    u16 x1;
    s16 y1;
    u16 z1;
    s16 z21;
    s16 rotX1;
    s16 rotY1;
    s16 rotZ1;
    s16 flags1;
    u16 field20;
    u8 pad22[0xA];
    s16 field44_0;
    s16 field44_1;
} MenuBigFrame;

typedef struct {
    u16 x;
    s16 y;
    u16 z;
    s16 z2;
    s16 rotX;
    s16 rotY;
    s16 rotZ;
    s16 flags;
    u16 field10;
    u8 pad12[0x6];
    s16 field44;
} MenuSmallFrame;

extern s16 D_801E4DAC;
extern u16 D_801E4D8C;
extern u16 D_801E4BC0;
extern MenuBigFrame *D_8009F0A4;
extern u8 *D_8019C7A4;

#define AVG(a, b) (tmp = (a), tmp += (b), (s32)(tmp + ((u32)tmp >> 31)) >> 1)

void func_8001F330(s32 arg0, MenuObj *arg1, MenuObj *arg2) {
    s32 index;
    MenuBigFrame *big;
    MenuSmallFrame *small;
    register s32 tmp asm("$2");

    if (D_801E4DAC != 0) {
        arg1->variantAE = D_801E4D8C;
        arg2->variantAE = D_801E4BC0;
        if ((arg0 & 1) == 0) {
            index = arg0 >> 1;
            big = &D_8009F0A4[index];
            arg1->x = big->x0;
            arg1->y = big->y0;
            arg1->z = big->z0;
            arg1->z2 = big->z20;
            arg1->rotX = big->rotX0;
            arg1->rotY = big->rotY0;
            arg1->rotZ = big->rotZ0;
            arg1->flags48 = big->flags0;
            arg1->field44 = big->field44_0;
            arg2->x = big->x1;
            arg2->y = big->y1;
            arg2->z = big->z1;
            arg2->z2 = big->z21;
            arg2->rotX = big->rotX1;
            arg2->rotY = big->rotY1;
            arg2->rotZ = big->rotZ1;
            arg2->flags48 = big->flags1;
            arg2->field44 = big->field44_1;
        } else {
            arg0 = arg0 >> 1;
            arg0 += 1;
            if (arg0 == 0x2EE) {
                arg0 = 0;
            }
            big = &D_8009F0A4[arg0];
            arg1->x = AVG(big->x0, arg1->x);
            arg1->y = AVG(big->y0, arg1->y);
            arg1->z = AVG(big->z0, arg1->z);
            arg1->z2 = AVG(big->z20, arg1->z2);
            arg1->rotX = AVG(big->rotX0, arg1->rotX);
            arg1->rotY = AVG(big->rotY0, arg1->rotY);
            arg1->rotZ = AVG(big->rotZ0, arg1->rotZ);
            arg1->flags48 = AVG(big->flags0, arg1->flags48);
            arg1->field44 = AVG(big->field44_0, arg1->field44);
            arg2->x = AVG(big->x1, arg2->x);
            arg2->y = AVG(big->y1, arg2->y);
            arg2->z = AVG(big->z1, arg2->z);
            arg2->z2 = AVG(big->z21, arg2->z2);
            arg2->rotX = AVG(big->rotX1, arg2->rotX);
            arg2->rotY = AVG(big->rotY1, arg2->rotY);
            arg2->rotZ = AVG(big->rotZ1, arg2->rotZ);
            arg2->flags48 = AVG(big->flags1, arg2->flags48);
            arg2->field44 = AVG(big->field44_1, arg2->field44);
        }
        arg1->field8C = big->field20;
    } else {
        arg1->variantAE = D_801E4D8C;
        if ((arg0 & 1) == 0) {
            index = arg0 >> 1;
            small = (MenuSmallFrame *)(D_8019C7A4 + (((index << 3) - index) << 2));
            arg1->x = small->x;
            arg1->y = small->y;
            arg1->z = small->z;
            arg1->z2 = small->z2;
            arg1->rotX = small->rotX;
            arg1->rotY = small->rotY;
            arg1->rotZ = small->rotZ;
            arg1->flags48 = small->flags;
            arg1->field44 = small->field44;
        } else {
            arg0 = arg0 >> 1;
            arg0 += 1;
            if (arg0 == 0x505) {
                arg0 = 0;
            }
            small = (MenuSmallFrame *)(D_8019C7A4 + (((arg0 << 3) - arg0) << 2));
            arg1->x = AVG(small->x, arg1->x);
            arg1->y = AVG(small->y, arg1->y);
            arg1->z = AVG(small->z, arg1->z);
            arg1->z2 = AVG(small->z2, arg1->z2);
            arg1->rotX = AVG(small->rotX, arg1->rotX);
            arg1->rotY = AVG(small->rotY, arg1->rotY);
            arg1->rotZ = AVG(small->rotZ, arg1->rotZ);
            arg1->flags48 = AVG(small->flags, arg1->flags48);
            arg1->field44 = AVG(small->field44, arg1->field44);
        }
        arg1->field8C = small->field10;
    }
}
