#include "common.h"

typedef struct {
    u8 bytes[8];
} Unaligned8;

extern s32 D_801E408C;
extern s16 *D_801E4BA4;
extern s16 *D_801E4174;
extern s32 *D_801E42DC;
extern u8 *D_801E42E8;
extern s16 D_801E4DB0;
extern s16 D_801E4DB2;
extern s16 D_801E4DB4;
extern s16 D_801E4DB6;
extern s32 D_801E4DB8;
extern Unaligned8 D_801E4DC8;
extern s16 D_801E4DD0;
extern s16 D_801E4DD2;
extern s16 D_801E4DD4;
extern s16 D_801E4DD8;
extern s16 D_801E4DDA;
extern s16 D_801E4DDC;
extern s16 D_801E4DE0;
extern s16 D_801E4DE2;
extern s16 D_801E4DE4;
extern s16 D_801E4DE6;
extern s16 D_801E4DE8;
extern s16 D_801E4DEA;
extern s16 D_801E4DEC;
extern s16 D_801E4DEE;
extern s32 D_801E4DF0;

#define HALF_TOWARD_ZERO(value) ((s32)((value) + ((u32)(value) >> 31)) >> 1)

void func_8003F700(void) {
    s32 index;
    s32 first;
    s32 second;
    register s16 *signFlags asm("t0") = &D_801E4DB2;
    s32 value;

    index = D_801E408C;
    first = D_801E4BA4[index];
    second = D_801E4174[index];
    *signFlags = 0;
    signFlags[-1] = 0;

    D_801E42DC = (s32 *)((u8 *)D_801E4BA4 + (first * 0x14) + 4);
    D_801E42E8 = (u8 *)D_801E4174 + (second * 0xC) + 4;

    *(s32 *)((u8 *)&D_801E4DB8 + 0x0) = D_801E42DC[0];
    *(s32 *)((u8 *)&D_801E4DB8 + 0x4) = D_801E42DC[1];
    *(s32 *)((u8 *)&D_801E4DB8 + 0x8) = D_801E42DC[2];
    *(s32 *)((u8 *)&D_801E4DB8 + 0xC) = D_801E42DC[3];

    D_801E4DC8 = *(Unaligned8 *)D_801E42E8;

    D_801E4DE0 = 0;
    D_801E4DE2 = 0;
    D_801E4DE4 = *(u16 *)((u8 *)D_801E42DC + 0x10);
    D_801E4DE6 = *(u16 *)(D_801E42E8 + 0x8);
    D_801E4DE8 = *(u16 *)((u8 *)D_801E42DC + 0x12);
    D_801E4DEA = *(u16 *)(D_801E42E8 + 0xA);

    {
        register s32 signValue asm("v1");
        register s32 signCopy asm("v0");

        signValue = *(s16 *)((u8 *)D_801E42DC + 0x12);
        signCopy = signValue;
        if (signValue < 0) {
            signCopy = -signCopy;
            D_801E4DE8 = signCopy;
            D_801E4DB4 = 1;
        } else {
            if (signValue == 0) {
                signCopy = 1;
            }
            D_801E4DE8 = signCopy;
            D_801E4DB4 = 0;
        }
    }

    {
        register s32 signValue asm("v1");
        register s32 signCopy asm("v0");

        signValue = *(s16 *)(D_801E42E8 + 0xA);
        signCopy = signValue;
        if (signValue < 0) {
            signCopy = -signCopy;
            D_801E4DEA = signCopy;
            D_801E4DB6 = 1;
        } else {
            if (signValue == 0) {
                signCopy = 1;
            }
            D_801E4DEA = signCopy;
            D_801E4DB6 = 0;
        }
    }

    D_801E4DF0 = 0;
    D_801E4DEC = 0;
    D_801E4DEE = 0;

    value = D_801E42DC[5] - D_801E42DC[0];
    D_801E4DD8 = HALF_TOWARD_ZERO(value);
    value = D_801E42DC[6] - D_801E42DC[1];
    D_801E4DDA = HALF_TOWARD_ZERO(value);
    value = D_801E42DC[7] - D_801E42DC[2];
    D_801E4DDC = HALF_TOWARD_ZERO(value);

    value = *(s16 *)(D_801E42E8 + 0xC) - *(s16 *)(D_801E42E8 + 0x0);
    D_801E4DD0 = HALF_TOWARD_ZERO(value);
    value = *(s16 *)(D_801E42E8 + 0xE) - *(s16 *)(D_801E42E8 + 0x2);
    D_801E4DD2 = HALF_TOWARD_ZERO(value);
    value = *(s16 *)(D_801E42E8 + 0x10) - *(s16 *)(D_801E42E8 + 0x4);
    D_801E4DD4 = HALF_TOWARD_ZERO(value);
}
