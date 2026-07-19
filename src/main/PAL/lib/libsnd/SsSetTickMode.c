#include "common.h"

extern s32 D_8009A558;
extern s32 D_8009A55C;
extern s32 D_801E6C6C;

s32 func_8006EAEC(s32 arg);

void SsSetTickMode(s32 spec) asm("func_800720F4");

void SsSetTickMode(s32 spec) {
    s32 v;
    s32 m;

    v = func_8006EAEC(spec);
    if (spec & 0x1000) {
        D_8009A55C = 1;
        D_8009A558 = spec & 0xFFF;
    } else {
        D_8009A55C = 0;
        D_8009A558 = spec;
    }

    m = D_8009A558;
    if (m < 6) {
    switch (m) {
    case 4:
        D_801E6C6C = 50;
        D_8009A558 = (v == 1) ? 5 : 50;
        break;
    case 1:
        D_801E6C6C = 60;
        D_8009A558 = (v == 0) ? 5 : 60;
        break;
    case 3:
        D_801E6C6C = 120;
        break;
    case 2:
        D_801E6C6C = 240;
        break;
    case 5:
        if (v == 0) {
            D_801E6C6C = 60;
        } else if (v == 1) {
            D_801E6C6C = 50;
        } else {
            D_801E6C6C = 60;
        }
        break;
    case 0:
        if (v == 0) {
            D_801E6C6C = 60;
        } else if (v == 1) {
            D_801E6C6C = 50;
        } else {
            D_801E6C6C = 60;
        }
        break;
    default:
        D_801E6C6C = 60;
        break;
    }
    } else {
        D_801E6C6C = m;
    }
}
