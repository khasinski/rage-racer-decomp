#include "common.h"

extern s32 D_8009E694;
extern s32 D_801E40B8;
extern s16 D_801E4DAC;
extern s32 D_8019C8EC;
extern s16 D_8009E834;
extern s32 D_801E42A0;
extern s32 D_801E42E0;
extern s32 D_801E6E78;
extern s32 D_8019CB6C;
extern u16 D_801E436E;
extern s32 D_8009EC8C;
extern s32 D_8019CACC;
extern s32 D_801E42E4;
extern u8 D_8009E6D4;
extern s32 D_801F179C;
extern u8 D_801F1854;
extern s32 D_801E4030;
extern s16 D_8009E74C;

void func_8005D6EC(s32 arg0);
void func_80033AA0(s32 arg0, s32 arg1);
void func_80042CCC(s32 arg0);
void func_8001FC30(s32 arg0, s32 arg1);
void func_80035040(void);
void func_80043BCC(s32 arg0, void *arg1);
void func_80041888(void);
void func_80038A88(void);
void func_8004123C(void);
void func_8003E2E8(s32 arg0, s32 arg1);
void func_80045CD4(void);
void func_800418D4(void);
void func_8001FB8C(void);
void func_80019E84(s32 arg0);
void func_8001F330(s32 arg0, void *arg1, void *arg2);

void func_8001FD3C(void) {
    D_8009E694++;
    D_801E40B8++;
    if (D_801E40B8 == 0x3C) {
        if (D_801E4DAC != 0) {
            if (D_8019C8EC == 0) {
                func_8005D6EC(D_8009E834 == 1 ? 0x40 : 0x41);
            }
        }
    }

    if (D_801E42A0 < 0) {
        D_801E42E0 += D_801E42A0;
        if (D_801E42E0 < 0) {
            D_801E42E0 = 0;
            D_801E42A0 = 0;
            D_801E6E78 = 0;
        }
        func_80033AA0(D_801E42E0, 0x29);
    } else {
        if (D_8019C8EC != 0) {
            s32 cb = D_8019CB6C;
            s32 fc = D_801E40B8;
            if ((u32)(cb - 600) < (u32)fc) {
                s32 t = fc + 600;
                s32 c;
                t = t - cb;
                D_801E6E78 = t;
                if (t >= 0) {
                    c = t;
                    if (t >= 256) {
                        c = 255;
                    }
                } else {
                    c = 0;
                }
                D_801E6E78 = c;
            }
        }

        if (D_801E42A0 == 0) {
            if ((D_801E436E & 0x860) != 0) {
                D_801E42A0 = 4;
                func_80042CCC(0x3C);
            } else if (D_801E40B8 == D_8019CB6C - 68) {
                D_801E42A0 = 4;
                if (D_8009EC8C == 0) {
                    func_80042CCC(0x3C);
                }
            }
        } else {
            D_801E42E0 += D_801E42A0;
            if (D_801E42E0 >= 257) {
                s32 v = D_801E4DAC;
                D_8019CACC = 0;
                D_801E42E4 = v == 0 ? 0x14 : 0x12;
            }
        }

        if (D_8019C8EC != 0) {
            if (((u32)(D_8019CB6C - 600) < (u32)D_801E40B8) || (D_801E42E0 != 0)) {
                func_8001FC30(D_801E6E78, D_801E42E0);
            }
        } else {
            if (D_801E42E0 != 0) {
                func_80033AA0(D_801E42E0, 0x49);
            }
        }
    }

    func_8001F330(D_801F179C, &D_8009E6D4, &D_801F1854);
    D_801F179C++;
    if (D_801F179C == D_8019CB6C) {
        D_801F179C = 0;
    }
    func_80035040();
    func_80043BCC(2, &D_8009E6D4);
    *(s32 *)0x1F800084 = D_801E4030;
    func_80041888();
    if (D_801E4DAC != 0) {
        func_80038A88();
    }
    func_8004123C();
    func_8003E2E8(D_801E40B8, 1);
    func_80045CD4();
    func_800418D4();
    func_8001FB8C();
    if (D_801E40B8 == 1) {
        func_80019E84(D_8009E74C);
    }
}
