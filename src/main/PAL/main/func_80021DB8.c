#include "common.h"

typedef struct {
    s8 pad[8];
    s32 v8;
    s16 vC;
    s16 vE;
} S22;

extern char D_80010F90[], D_80010FA8[], D_80010F98[], D_80010F9C[], D_80010FA4[];
extern s32 D_8009E858[];
extern s32 D_801E4BA8;
extern s16 D_8019CABC;
extern s32 D_801E428C;
extern s32 D_8019CAC8;
extern s32 D_8019CE10;
extern s32 D_801E8A48;
extern u8 *D_8007D4C0[];
extern s32 D_8007D508[];
extern s32 D_8007D4D4[];
extern S22 D_801E7744[][4][5];
extern S22 D_8019CB78[][4][5];

void func_80016EA0(void *dst, s32 len, void *src, s32 arg3);
void func_80016754(void *dst, s32 x, void *src, s32 color);
void *func_80021CD4(void *dst, s32 value);
void func_800632F0();

void func_80021DB8(u8 *s6) {
    register u8 *panel asm("$22");
    register s32 iter asm("$16");
    register s32 countOrIndex asm("$20");
    register s32 xOrField asm("$18");
    register s32 destination asm("$19");
    register s32 color asm("$17");
    register s32 scoreOrX asm("$21");
    register void *callDst asm("$4");
    char text[56];
    s32 mode;
    register s32 row asm("$3");
    register s32 doubledRow asm("$6");
    register s32 value asm("$2");
    s32 scoreValue;

    panel = s6;
    func_80016EA0(panel + 0x10, 0x4C, D_80010F90, 0x7852);

    mode = D_801E428C;
    text[1] = 0x2F;
    countOrIndex = 6;
    if (mode != 3) {
        countOrIndex = 3;
    }
    iter = 0;
    if (countOrIndex > 0) {
        scoreOrX = (s32)D_8009E858;
        do {
            row = (u32)iter >> 31;
            asm("addu %0,%2,%0" : "=r"(row) : "0"(row), "r"(iter));
            row >>= 1;
            doubledRow = row * 2;
            value = iter - doubledRow;
            value <<= 3;
            xOrField = value + 0x58;
            asm(".globl func_80022038\nfunc_80022038 = func_80021DB8 + 0x280"
                : "=r"(xOrField) : "0"(xOrField));
            callDst = &text[2];
            asm("" : "=r"(callDst) : "0"(callDst));
            value = iter + 0x31;
            doubledRow = (doubledRow + row) << 5;
            text[0] = value;
            func_80021CD4(callDst, ({
                scoreValue = *(s32 *)scoreOrX;
                value = (s32)(panel + 0x14);
                destination = doubledRow + value;
                scoreValue;
            }));

            color = 0x78CC;
            if (D_8019CAC8 == iter) {
                color = 0x780F;
            }
            func_80016754((void *)destination, xOrField, text, color);
            iter++;
            scoreOrX += 4;
        } while (iter < countOrIndex);
    }

    func_80016EA0(panel + 0x10, 0x6C, D_80010F98, 0x7812);

    countOrIndex = 0;
    scoreOrX = 0x82;
    destination = 0x78;
    do {
        asm("" : "=r"(countOrIndex) : "0"(countOrIndex));
        text[0] = D_8007D4C0[countOrIndex][0];
        text[1] = D_8007D4C0[countOrIndex][1];
        text[2] = D_8007D4C0[countOrIndex][2];
        text[3] = 0x2F;
        iter = countOrIndex << 4;
        func_80021CD4(&text[4], D_801E7744[D_8019CABC][D_801E428C][countOrIndex].v8);

        {
            register s32 formatReg asm("$5");
            register S22 *entry asm("$6");

            callDst = &text[0xC];
            asm("" : "=r"(callDst) : "0"(callDst));
            /* Match note: preserve ASPSX's v1/a2 indexed-record schedule. */
            asm volatile(
                ".set noat\n\t"
                "lui $2,%%hi(D_801E428C)\n\t"
                "lw $2,%%lo(D_801E428C)($2)\n\t"
                "lui $5,%%hi(D_8019CABC)\n\t"
                "lh $5,%%lo(D_8019CABC)($5)\n\t"
                "sll $3,$2,2\n\t"
                "addu $3,$3,$2\n\t"
                "sll $3,$3,4\n\t"
                "addu $2,$16,$3\n\t"
                "sll $6,$5,2\n\t"
                "addu $6,$6,$5\n\t"
                "sll $6,$6,6\n\t"
                "addu $2,$2,$6\n\t"
                "la %1,D_80010F9C\n\t"
                "lui $1,%%hi(D_801E7744)\n\t"
                "addu $1,$1,$2\n\t"
                "lh %0,%%lo(D_801E7744+0xC)($1)\n\t"
                "la $2,D_801E7744\n\t"
                "addu $3,$3,$2\n\t"
                "addu %2,%2,$3\n\t"
                ".set at"
                : "=r"(xOrField), "=r"(formatReg), "=r"(entry)
                :
                : "$2", "$3", "memory");
            func_800632F0(callDst, formatReg,
                          &entry[countOrIndex],
                          D_8007D508[xOrField]);
        }

        color = 0x78CC;
        if (D_801E8A48 == countOrIndex) {
            color = 0x780F;
        }
        func_80016754(panel + 0x14, destination, text, color);

        {
            register char *formatReg asm("$5");
            register s32 valueReg asm("$6");

            callDst = text;
            asm("" : "=r"(callDst) : "0"(callDst));
            formatReg = D_80010FA4;
            asm("" : "=r"(formatReg) : "0"(formatReg));
            valueReg = D_8007D4D4[xOrField];
            destination += 0x14;
            countOrIndex++;
            func_800632F0(callDst, formatReg, valueReg);
        }
        func_80016754(panel + 0x2C, scoreOrX, text, color);
        scoreOrX += 0x14;
    } while (countOrIndex < 5);
}

void func_80022068(u8 *s5) {
    char text[48];
    s32 s4, s3;
    s32 s2, color, idx;

    func_80016EA0(s5 + 0x10, 0x4C, D_80010FA8, 0x7852);

    text[0] = 0x54;
    text[1] = 0x2F;
    func_80021CD4(&text[2], D_801E4BA8);
    func_80016754(s5 + 0x14, 0x58, text, 0x78CC);

    func_80016EA0(s5 + 0x10, 0x6C, D_80010F98, 0x7812);

    s2 = 0;
    s4 = 0x82;
    s3 = 0x78;
    for (; s2 < 5; s2++) {
        text[0] = D_8007D4C0[s2][0];
        text[1] = D_8007D4C0[s2][1];
        text[2] = D_8007D4C0[s2][2];
        text[3] = 0x2F;
        func_80021CD4(&text[4], D_8019CB78[D_8019CABC][D_801E428C][s2].v8);

        idx = D_8019CB78[D_8019CABC][D_801E428C][s2].vC;
        func_800632F0(&text[0xC], D_80010F9C,
                      &D_8019CB78[D_8019CABC][D_801E428C][s2], D_8007D508[idx]);

        color = 0x78CC;
        if (D_8019CE10 == s2) {
            color = 0x780F;
        }
        func_80016754(s5 + 0x14, s3, text, color);

        func_800632F0(text, D_80010FA4, D_8007D4D4[idx]);

        func_80016754(s5 + 0x2C, s4, text, color);
        s3 += 0x14;
        s4 += 0x14;
    }
}
