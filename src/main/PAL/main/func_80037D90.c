#include "common.h"

extern u32 D_801E40B8;
extern s16 D_8009EC88;
extern s16 D_8009E83C;
extern s32 D_801E40D8;
extern s32 D_8009E73C;
extern s32 D_8009E740;
extern s16 D_801E6E74;
extern s16 D_801E43FC;
extern s32 D_801E43F8;
extern s32 D_8009E694;
extern s32 D_8009E6D4;
extern s32 D_801E4030;
extern s32 D_801E428C;
extern s32 D_8009E744;
extern s16 D_8019C78C;
extern s16 D_8009E74C;
extern u8 D_80011494;

void func_80033AA0(s32 a, s32 b);
void func_80037714(void);
void func_800218A0(s32 a);
void func_80035258(s32 a);
void func_80016754(s32 a, s32 b, void *c, s32 d);
void func_8002BE18(void *a);
void func_8003C508(void *a);
void func_8002DEFC(void *a);
void func_8005D9F8(s32 a, s32 b);
void func_80037C04(void);
void func_80043BCC(s32 a, void *b);
void func_80019EFC(s32 a);
void func_80045CD4(void);
void func_800418D4(void);
void func_80041840(void);
void func_8004123C(void);
void func_8003E1A4(s32 a, s32 b, s32 c);
void func_800350B4(s32 a);
void func_8005B190(s32 a, s32 b);
void func_8002F458(void);
void func_8004087C(s32 a);
void func_8003425C(u32 a);
void func_800410BC(u32 a);
void func_80037860(void);
void func_80037AAC(void);

void func_80037D90(void) {
    s16 *p;
    register u32 a asm("$5");
    s32 x = 0;

    D_801E40B8 = D_801E40B8 + 1;
    if (D_801E40B8 < 61) {
        func_80033AA0(255 - (D_801E40B8 - 6) * 11, 0x49);
    }
    if (D_801E40B8 >= 571 && D_8009EC88 == 0) {
        D_8009EC88 = 1;
    }

    if (D_8009E740 + D_8009E73C >= D_8009E83C * D_801E40D8) {
        if (D_8009E83C < 257) {
            D_8009E83C = D_8009E83C + 1;
            func_80037714();
        }
    }
    if (D_8009E83C >= 257) {
        if (D_801E6E74 == 2) {
            D_801E6E74 = 4;
            D_801E43FC = 0;
        }
    }

    if (D_801E6E74 == 5) {
        if (D_801E43FC > 0) {
            func_800218A0(D_801E43FC * 3);
            func_80033AA0(D_801E43FC * 3, 0x49);
            x = 6;
        }
        if (D_801E43FC >= 101) {
            func_80035258(x);
        }
        D_801E43FC = D_801E43FC + 1;
    } else if (D_801E6E74 == 4) {
        func_80016754(0x5c, 0x78, &D_80011494, 0x7811);
        func_80033AA0(D_801E43FC * 2, 0x29);
        D_801E43FC = D_801E43FC + 1;
        if (D_801E43FC < 201) {
            D_801E43FC = D_801E43FC + 1;
        } else {
            func_80035258(6);
        }
    }

    a = D_801E40B8;
    D_8009E694 = D_8009E694 + 1;
    asm volatile("");
    if (a >= 90) {
        asm volatile("" :: "r"(a));
        if (D_801E6E74 == 0) {
            D_801E6E74 = 1;
            goto Lend;
        }
    } else {
        if (D_801E6E74 == 0) {
            func_8003C508(&D_8009E6D4);
            D_8009EC88 = 0;
            D_801E43F8 = 0;
            goto Lend;
        }
    }
    if (D_801E6E74 == 1) {
        if (D_801E40B8 >= 211) {
            func_8002BE18(&D_8009E6D4);
            D_801E6E74 = 2;
        }
    }
Lend:

    if (D_801E6E74 < 4) {
        func_8003425C(D_801E40B8);
        func_800410BC(D_801E40B8);
    }

    if (D_801E6E74 > 0) {
        func_8002DEFC(&D_8009E6D4);
    } else if (D_801E6E74 == 0) {
        func_8005D9F8(0, 1);
    }
    func_80037C04();

    if (D_801E6E74 > 0) {
        func_80043BCC(0, &D_8009E6D4);
    }

    p = &D_8009E74C;
    func_80019EFC(*p);
    func_80045CD4();
    func_800418D4();
    *(s32 *)0x1F800084 = D_801E4030;
    func_80041840();
    func_8004123C();
    func_8003E1A4(D_801E428C & 3, D_801E40B8, 1);
    func_800350B4(D_8009E744);
    func_8005B190(D_8019C78C, D_8019C78C);
    func_8002F458();
    func_8004087C(*p);
    if (D_801E6E74 < 3) {
        func_80037860();
        func_80037AAC();
    }
}
