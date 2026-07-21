#include "common.h"

extern s32 D_8019CAF4;
extern s32 D_8019CE00;
extern s32 D_8019C7BC;
extern s32 D_8019C99C;
extern s32 D_8007D6B4;
extern s32 D_8007D6B0;
extern s32 D_8009E6CC;
extern u8 D_801E7734[];
extern s32 D_801E4404;
extern s32 D_801E40A8;
extern s32 D_801E40B8;
extern s32 D_801E42A0;
extern u16 D_801E436E;
extern s32 D_801E4B84;
extern s32 D_801E412C;
extern s32 D_801E42E0;
extern s32 D_801E4030;
extern s32 D_8009E694;
extern s32 D_8009E66C;
extern s32 D_8009E870;

typedef struct {
    u8 _a[120];
    s16 f120;
    u8 _b[290];
} Arr412;
extern Arr412 D_801F1854[];

void func_80042BC0();
void func_80042BF0();
void func_80025E54();
void func_80065860();
void func_8001B488();
void func_80042CCC();
void func_80033AA0();
void func_80018B98();
void func_80025C58();
void func_8003BB50();
void func_80019EFC();
void func_80043BCC();
void func_800389F0();
void func_80045CD4();
void func_800418D4();
void func_80041888();
void func_8004123C();
void func_8003E2E8();
int func_8001A0E4();

void func_80025ED8(void) {
    s32 t;
    if (D_8019CAF4 <= 0) goto L78;
    t = D_8019CAF4 - 1;
    D_8019CAF4 = t;
    if (t == 4) goto L13c;
    if (t != 0) goto L13c;
    if (D_8019CE00 == 12) D_8019CE00 = 17;
    func_80042BC0(D_8019CE00);
    func_80042BF0();
    D_8019C7BC = 0;
    goto L13c;

L78:
    if (D_8019C7BC == 0) goto L13c;
    D_8019CAF4 = 6;
    if (D_8007D6B4 == 0) goto Ld0;
    D_801E4404 = D_801E7734[D_8009E6CC];
    func_80025E54(D_801E4404);
    goto L124;
Ld0:
    D_801E4404 = D_801E4404 + 1;
    D_801E4404 = (D_801E4404 + D_801E40A8) % D_801E40A8;
L124:
    D_8019CE00 = D_801E4404 + 3;

L13c:
    if (D_801E40B8 == 2) func_80065860(1);
    if (D_801E42A0 != 0) goto L424;

    if (D_801E436E & 0x8000) {
        if (D_801E4B84 > 0) D_801E4B84 = D_801E4B84 - 1;
    }
    if (D_801E436E & 0x2000) {
        if (D_801E4B84 < 2) D_801E4B84 = D_801E4B84 + 1;
    }
    if (D_801E436E & 1) {
        register s32 p asm("$5");
        register s32 h0 asm("$2");
        func_8001B488();
        h0 = D_801E7734[0];
        p = D_801E4404;
        if (p == h0) {
            u8 tmp = D_801E7734[D_801E40A8 - 1];
            D_801E7734[0] = tmp;
            D_801E7734[D_801E40A8 - 1] = p;
        }
        D_8007D6B4 = 1;
        D_8007D6B0 = 60;
    }
    {
        u16 f = D_801E436E;
        if (f & 2) {
            D_8007D6B4 = 0;
            D_8007D6B0 = 0;
        }
        if (f & 0x860) {
            switch (D_801E4B84) {
            case 0:
                if (D_8007D6B4 == 0) {
                    D_801E4404 = D_801E4404 - 1;
                    D_801E4404 = (D_801E4404 + D_801E40A8) % D_801E40A8;
                }
                goto lab380;
            case 2:
                if (D_8007D6B4 != 0) {
                    D_801E4404 = D_801E7734[D_8009E6CC];
                    func_80025E54(D_801E4404);
                } else {
                    D_801E4404 = D_801E4404 + 1;
                    D_801E4404 = (D_801E4404 + D_801E40A8) % D_801E40A8;
                }
            lab380:
                if (D_8019CAF4 == 0) {
                    func_80042CCC(60);
                    D_8019CAF4 = 0x40;
                }
                D_8019CE00 = D_801E4404 + 3;
                break;
            case 1:
                func_80042CCC(60);
                D_801E42A0 = 4;
                break;
            }
        } else if (f & 0x90) {
            func_80042CCC(60);
            D_801E42A0 = 4;
        }
    }
    {
        u16 f = D_801E436E;
        if (f & 4) D_801E412C = 1;
        if (f & 8) D_801E412C = 0;
    }
    goto L48c;

L424:
    func_80033AA0(D_801E42E0, 0x49);
    D_801E42E0 = D_801E42E0 + D_801E42A0;
    if (D_801E42E0 >= 256) {
        func_80018B98();
        D_8019C99C = 3;
        D_801E42E0 = 256;
        D_801E42A0 = -4;
    }

L48c:
    if (D_801E412C != 0) func_80025C58();
    D_8009E694 = D_8009E694 + 1;
    D_8009E66C = func_8001A0E4(0xff, D_8009E66C);
    func_8003BB50();
    func_80019EFC(D_801F1854[D_8009E66C].f120);
    func_80043BCC(D_8009E870, &D_801F1854[D_8009E66C]);
    func_800389F0();
    func_80045CD4();
    func_800418D4();
    *(s32 *)0x1F800084 = D_801E4030;
    func_80041888();
    func_8004123C();
    func_8003E2E8(D_8009E694, 1);
}
