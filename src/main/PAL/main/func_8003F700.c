#include "common.h"

typedef struct {
    s32 w[4];
} Blk16;

typedef struct {
    s16 h[4];
} Blk8;

typedef struct {
    s16 prev;
    s16 current;
    s16 firstNeg;
    s16 secondNeg;
} StateHead;

extern s32 D_801E408C;
extern u8 *D_801E4BA4;
extern u8 *D_801E4174;
extern u8 *D_801E42DC;
extern u8 *D_801E42E8;
extern StateHead D_801E4DB0;
extern Blk16 D_801E4DB8;
extern Blk8 D_801E4DC8;
extern s16 D_801E4DE0;
extern s16 D_801E4DE2;
extern u16 D_801E4DE4;
extern u16 D_801E4DE6;
extern u16 D_801E4DE8;
extern u16 D_801E4DEA;
extern s16 D_801E4DEC;
extern s16 D_801E4DEE;
extern s32 D_801E4DF0;
extern s16 D_801E4DD0;
extern s16 D_801E4DD2;
extern s16 D_801E4DD4;
extern s16 D_801E4DD8;
extern s16 D_801E4DDA;
extern s16 D_801E4DDC;

void func_8003F700(void) {
    s32 lev;
    u8 *tblA;
    u8 *tblB;
    s32 ia;
    s32 ib;

    lev = D_801E408C;
    tblA = D_801E4BA4;
    tblB = D_801E4174;
    ia = *(s16 *)(tblA + (lev * 2));
    ib = *(s16 *)(tblB + (lev * 2));
    D_801E4DB0.current = 0;
    D_801E4DB0.prev = 0;
    D_801E42DC = tblA;
    D_801E42DC = D_801E42DC + ((ia * 20) + 4);
    D_801E42E8 = tblB;
    D_801E42E8 = D_801E42E8 + ((ib * 12) + 4);
    D_801E4DB8 = *(Blk16 *)D_801E42DC;

    {
        u8 *copySrc;
        u8 *entryA;
        u8 *entryB;
        s16 sv;

        copySrc = D_801E42E8;
        entryA = D_801E42DC;
        D_801E4DC8 = *(Blk8 *)copySrc;
        D_801E4DE0 = 0;
        D_801E4DE2 = 0;
        D_801E4DE4 = *(u16 *)(entryA + 0x10);
        entryB = D_801E42E8;
        D_801E4DE6 = *(u16 *)(entryB + 0x8);
        D_801E4DE8 = *(u16 *)(entryA + 0x12);
        D_801E4DEA = *(u16 *)(entryB + 0xA);

        sv = *(s16 *)(entryA + 0x12);
        if (sv < 0) {
            sv = -sv;
            D_801E4DE8 = sv;
            D_801E4DB0.firstNeg = 1;
        } else {
            if (sv == 0) {
                sv = 1;
            }
            D_801E4DE8 = sv;
            D_801E4DB0.firstNeg = 0;
        }
    }

    {
        u8 *entryB;
        s16 sv;

        entryB = D_801E42E8;
        sv = *(s16 *)(entryB + 0xA);
        if (sv < 0) {
            sv = -sv;
            D_801E4DEA = sv;
            D_801E4DB0.secondNeg = 1;
        } else {
            if (sv == 0) {
                sv = 1;
            }
            D_801E4DEA = sv;
            D_801E4DB0.secondNeg = 0;
        }
    }

    {
        u8 *entry;

        entry = D_801E42DC;
        D_801E4DF0 = 0;
        D_801E4DEC = 0;
        D_801E4DEE = 0;
        D_801E4DD8 = (*(s32 *)(entry + 0x14) - *(s32 *)(entry + 0x0)) / 2;
        D_801E4DDA = (*(s32 *)(entry + 0x18) - *(s32 *)(entry + 0x4)) / 2;
        D_801E4DDC = (*(s32 *)(entry + 0x1C) - *(s32 *)(entry + 0x8)) / 2;
        entry = D_801E42E8;
        D_801E4DD0 = (*(s16 *)(entry + 0xC) - *(s16 *)(entry + 0x0)) / 2;
        D_801E4DD2 = (*(s16 *)(entry + 0xE) - *(s16 *)(entry + 0x2)) / 2;
        D_801E4DD4 = (*(s16 *)(entry + 0x10) - *(s16 *)(entry + 0x4)) / 2;
    }
}
