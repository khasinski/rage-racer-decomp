#include "common.h"
#include "game/menu.h"

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

void func_80021DB8(u8 *arg0) {
    register u8 *panel;
    register s32 iter;
    register s32 countOrIndex;
    register s32 xOrField;
    register s32 destination;
    register s32 color;
    register s32 scoreOrX;
    char text[56];
    s32 mode;
    s32 row;
    s32 doubledRow;
    s32 value;
    s32 scoreValue;
    s32 limit;

    panel = arg0;
    func_80016EA0(panel + 0x10, 0x4C, D_80010F90, 0x7852);
    mode = D_801E428C;
    text[1] = 0x2F;
    limit = 6;
    if (mode != 3) {
        limit = 3;
    }
    iter = 0;
    if (limit > 0) {
        scoreOrX = (s32)D_8009E858;
        do {
            scoreValue = iter + (((u32)iter) >> 31);
            row = scoreValue >> 1;
            doubledRow = row * 2;
            value = iter - doubledRow;
            value <<= 3;
            xOrField = value + 0x58;
            func_80021CD4(
                &text[2],
                ({
                    (void)(*(volatile char *)&text[0] = iter + 0x31);
                    doubledRow = (doubledRow + row) << 5;
                    scoreValue = *((s32 *)scoreOrX);
                    value = (destination = (s32)(panel + 0x14));
                    scoreValue;
                }));
            destination = doubledRow;
            destination = destination + value;
            color = 0x78CC;
            if (D_8019CAC8 == iter) {
                color = 0x780F;
            }
            func_80016754((void *)destination, xOrField, text, color);
            iter++;
            scoreOrX += 4;
        } while (iter < limit);
    }
    func_80016EA0(panel + 0x10, 0x6C, D_80010F98, 0x7812);
    countOrIndex = 0;
    scoreOrX = 0x82;
    destination = 0x78;
    do {
        text[0] = D_8007D4C0[countOrIndex][0];
        text[1] = D_8007D4C0[countOrIndex][1];
        text[2] = D_8007D4C0[countOrIndex][2];
        text[3] = 0x2F;
        func_80021CD4(&text[4], D_801E7744[D_8019CABC][D_801E428C][countOrIndex].v8);
        xOrField = D_801E7744[D_8019CABC][D_801E428C][countOrIndex].vC;
        func_800632F0(&text[0xC], D_80010F9C,
                      &D_801E7744[D_8019CABC][D_801E428C][countOrIndex],
                      D_8007D508[xOrField]);
        color = 0x78CC;
        if (D_801E8A48 == countOrIndex) {
            color = 0x780F;
        }
        func_80016754(panel + 0x14, destination, text, color);
        func_800632F0(text, D_80010FA4, D_8007D4D4[xOrField]);
        func_80016754(panel + 0x2C, scoreOrX, text, color);
        destination += 0x14;
        scoreOrX += 0x14;
        countOrIndex++;
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
