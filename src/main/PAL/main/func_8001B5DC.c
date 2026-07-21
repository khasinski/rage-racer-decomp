#include "common.h"

extern volatile u16 D_801E436E;
extern s32 D_801E8260;
extern s32 D_801E4184;
extern s16 D_8019CAC0;
extern s32 D_8007BED8;

extern s32 *D_8019C7C8;
extern s32 *D_801E4FAC;
extern s32 *D_8009E67C;
extern s16 D_801E4034;
extern s32 D_801E40A0;
extern s32 D_801E6E88;
extern s32 D_8009E6A4;
extern s32 D_801E428C;
extern s32 D_8009F098;
extern s32 D_8019C7B4;

extern s32 D_801E4F44;
extern s32 D_801E4094;
extern s32 D_801E42EC;
extern s32 D_8019C914;
extern s32 D_801E6E7C;
extern s32 D_8009E874;
extern s32 D_801E4388;
extern s32 D_8019C980;

extern void func_8005D6EC(s32);
extern void func_80017BE4(void);
extern void func_8001B488(void);
extern void func_8001965C(void);
extern void func_8001839C(void);
extern void func_800182D0(void);
extern void func_80018B98(void);
extern void func_8001B2D4(void);

void func_8001B5DC(void) {
    volatile u16 *flagp = &D_801E436E;
    s32 idx;
    u16 flags;

    if (*flagp != 0) {
        D_801E8260 = 0;
    }
    flags = *flagp;
    idx = D_801E4184;

    if (flags & 0x1000) {
        s32 n = idx - 1;
        D_801E4184 = n;
        if (D_8019CAC0 == 0 && n == 1) {
            D_801E4184 = idx - 2;
        }
    } else if (flags & 0x4000) {
        s32 n = idx + 1;
        D_801E4184 = n;
        if (D_8019CAC0 == 0 && n == 1) {
            D_801E4184 = idx + 2;
        }
    }

    {
        s32 m = D_801E4184 + 5;
        m = m % 5;
        D_801E4184 = m;
        if (idx != m) {
            func_8005D6EC(1);
        }
    }

    if (D_801E436E & 0x860) {
        func_8005D6EC(2);
        if (D_8007BED8 != 0) {
            func_80017BE4();
        }
        func_8001B488();
        switch (D_801E4184) {
        case 0:
            D_8019C7C8 = &D_801E4F44;
            D_801E4FAC = &D_801E4094;
            D_8009E67C = &D_801E42EC;
            D_801E4034 = 0;
            if (D_801E40A0 == -1) {
                D_8009E6A4 = 0;
                D_801E428C = 3;
                func_8001965C();
            } else {
                func_8001839C();
            }
            break;
        case 1:
            D_8019C7C8 = &D_8019C914;
            D_801E4FAC = &D_801E6E7C;
            D_8009E67C = &D_8009E874;
            D_801E4034 = 1;
            if (D_801E6E88 == -1) {
                D_8009E6A4 = 0;
                D_801E428C = 3;
                func_8001965C();
            } else {
                func_8001839C();
            }
            break;
        case 2:
            D_8019C7C8 = &D_801E4388;
            D_801E4FAC = &D_8019C980;
            D_801E4034 = 0;
            func_8001839C();
            break;
        case 3:
            func_800182D0();
            break;
        case 4:
            func_80018B98();
            D_8019C7B4 = 0;
            break;
        }
        D_8009F098 = 3;
    }
    func_8001B2D4();
}
