#include "common.h"

extern s32 D_8009AF9C;
extern u8 *D_8009E67C;
extern s32 D_8009E694;
extern s32 D_8009E6A4;
extern u8 D_8009E6AC[];
extern u8 D_8009E6D4[];
extern s32 D_8009E744;
extern s16 D_8009E74C;
extern s16 D_8009E78C;
extern s32 D_8009E870;
extern s16 D_8019C750;
extern s16 D_8019C78C;
extern s16 D_8019CA10;
extern s16 D_801E3E8C;
extern s32 D_801E4030;
extern s32 D_801E408C;
extern s32 D_801E40B8;
extern s16 D_801E414C;
extern s32 D_801E428C;
extern u8 D_801E4369;
extern u16 D_801E436A;
extern u16 D_801E436E;
extern s16 D_801E43FC;
extern s32 D_801E4408[][4][2];
extern s16 D_801E4B6C[];
extern s32 D_801E4BAC;
extern s16 D_801E4DAC;
extern s16 D_801E6E74;
extern s32 D_801E774C[][4][20];
extern s16 D_801E8A8C;

s32 func_8001A9A8(void);
s32 func_8003591C(void *arg0, s32 arg1);
void func_80019EFC(s32 arg0);
void func_8001ABD8(void);
void func_8001ADF4(s32 arg0);
void func_8001C974(void);
void func_8001F9D8(void);
void func_800215B8(s32 arg0);
void func_800218A0(s32 arg0);
void func_8002BE18(void *arg0, s32 arg1);
void func_8002DEFC(void *arg0);
void func_8002F458(void);
void func_80032E9C(s32 arg0);
void func_80033090(void);
void func_800331F8(s32 arg0);
void func_80033230(void);
void func_800333DC(void);
void func_80033AA0(s32 arg0, s32 arg1);
void func_8003425C(s32 arg0);
void func_8003479C(s32 arg0);
void func_800350B4(s32 arg0);
void func_80035258(s32 arg0);
void func_800352B8(void *arg0, s32 arg1, s32 arg2);
void func_800357BC(void);
void func_800389F0(void);
void func_8003AE2C(void);
void func_8003B0D4(void);
void func_8003C508(void *arg0, s32 arg1);
void func_8003CA14(void *arg0);
void func_8003CB3C(void *arg0);
void func_8003D458(s32 arg0);
void func_8003E1A4(s32 arg0, s32 arg1, s32 arg2);
void func_8003F608(s32 arg0);
void func_8004087C(s32 arg0);
void func_80040ADC(s32 arg0);
void func_80040DB4(s32 arg0);
void func_80040F24(void);
void func_800410BC(s32 arg0);
void func_8004123C(void);
void func_80041840(void);
void func_800418D4(void);
void func_80042BC0(s32 arg0);
void func_80042BF0(void);
void func_80042C0C(void);
void func_80042C28(void);
void func_80042CCC(s32 arg0);
void func_80043BCC(s32 arg0, void *arg1);
void func_80045CD4(void);
void func_8005B190(s32 arg0, s32 arg1);
void func_8005BEA8(s32 arg0, s32 arg1);
void func_8005C31C(s32 arg0, s32 arg1, s32 arg2);
void func_8005D6EC(s32 arg0);
void func_8005D9F8(s32 arg0, s32 arg1);
void func_8005E4A4(s32 arg0);
void func_80069888(void *arg0);

void func_800363D4(void) {
    s32 option;
    s32 value;
    s16 selection;
    s32 next;
    u16 mode;

    value = D_801E40B8 + 1;
    D_801E40B8 = value;
    option = 0;
    if ((u32)value < 0x3D) {
        func_8001C974();
        func_80033AA0(0xFF - ((D_801E40B8 - 6) * 0xB), 0x49);
    }

    if (D_8019C750 > 0) {
        D_8019C750--;
    }

    mode = D_801E6E74;
    if ((u32)(mode - 1) < 2 && (D_801E436E & 0x800) && D_8019C750 <= 0) {
        D_8019C750 = 5;
        value = (u32)D_801E4BAC < 1;
        D_801E4BAC = value;

        if (value != 0) {
            func_80042C0C();
            func_8005E4A4(0);
            D_801E414C = 0;
            func_8005D6EC(2);
        } else if (D_801E414C == (2 - D_801E4DAC)) {
            D_801E43FC = 0;
            if (D_801E4DAC == 0 || (s16)mode < 2) {
                D_801E6E74 = 7;
                if (D_801E4DAC == 0) {
                    D_801E4408[D_801E408C][D_801E428C][0] =
                        D_801E774C[D_801E408C][D_801E428C][0];
                }
            } else {
                value = *(s16 *)(D_8009E67C + 6);
                D_801E6E74 = 5;
                if (value != 0) {
                    func_8005D6EC(0x3D);
                }
            }
            func_8003CA14(D_8009E6D4);
            func_80042CCC(8);
        } else if (D_801E414C == 1) {
            if (D_801E4DAC == 0) {
                func_80035258(0xB);
                D_801E6E74 = 8;
            } else {
                goto set_countdown;
            }
        } else {
set_countdown:
            D_8019C750 = 0x1E;
            func_8005E4A4(1);
            if (D_801E6E74 >= 2) {
                func_80042C28();
            }
        }
    }

    if (D_801E6E74 == 5) {
        if (((D_801E4DAC == 1) && (*(s16 *)(D_8009E67C + 6) == 0)) ||
            (D_801E4DAC == 0)) {
            if (D_801E43FC >= 0x15) {
                func_800218A0((D_801E43FC - 0x14) * 3);
                func_80033AA0((D_801E43FC - 0x14) * 3, 0x49);
                option = 0xF;
            }
            if (D_801E43FC == 0xA) {
                func_80042BC0(0xF);
                func_80042BF0();
            }
            if (D_801E43FC >= 0x65) {
                func_80035258(option);
            }
        } else if ((D_801E4DAC == 1) && (*(s16 *)(D_8009E67C + 6) > 0)) {
            func_800215B8(D_801E43FC * 2);
            func_80033AA0(D_801E43FC * 2, 0x49);
            option = 0xD;
            if (D_801E43FC >= 0x7E) {
                func_80035258(0xD);
            }
        }
        D_8019CA10 = 0;
        D_801E43FC++;
    } else if (D_801E6E74 == 7) {
        func_80035258(6);
    }

    if (D_801E4BAC != 0) {
        func_8005B190(0x28, 0x28);
        if ((D_801E436E & 0x1000) && D_801E414C > 0) {
            D_801E414C--;
            func_8005D6EC(1);
        }
        if (D_801E436E & 0x4000) {
            selection = D_801E414C;
            if (selection < (2 - D_801E4DAC)) {
                D_801E414C = selection + 1;
                func_8005D6EC(1);
            }
        }

        D_801E40B8--;
        func_8003479C(D_801E414C);
        if (D_801E4DAC == 0) {
            func_800357BC();
        }
        func_80032E9C(D_801E4DAC);
        if (D_801E4DAC != 0) {
            func_800331F8(D_8009AF9C);
            func_80033230();
        }
        func_80033090();
        func_8003425C(D_801E40B8);
        func_800350B4(D_8009E744);
        func_8002F458();

        {
            s32 selectorMask;
            u16 inputMask;

            selectorMask = D_801E4369;
            inputMask = D_801E436A;
            selectorMask = (u32)(selectorMask ^ 0x23) < 1;
            if ((inputMask & D_801E4B6C[selectorMask * 8]) &&
                D_8009E870 == 0 && D_801E6E74 == 2) {
                if (D_801E436E & 8) {
                    D_8019CA10 = 1;
                } else if (D_801E436E & 4) {
                    D_8019CA10 = 0;
                }
            }
        }

        func_80043BCC(D_8009E870, D_8009E6D4);
        func_80019EFC(D_8009E74C);
        if (D_801E4DAC != 0) {
            func_800389F0();
        }
        if ((D_8009E78C != D_801E408C) && (D_801E8A8C >= 0xA)) {
            func_800333DC();
        }
        func_800418D4();
        *(s32 *)0x1F800084 = D_801E4030;
        func_80041840();
        func_8004123C();
        if (D_801E4DAC != 0) {
            if (D_8009E6A4 != 5) {
                func_8003D458(D_801E40B8);
            }
            func_80069888(D_8009E6AC);
            func_8003F608(0);
            func_8001ADF4(D_801E40B8);
        }
        func_8003E1A4(D_801E428C & 3, D_801E40B8, 0);
        if (func_8001A9A8() != 0) {
            func_8003E1A4(D_801E428C & 3, D_801E40B8, 0);
            func_8001ABD8();
        }
    } else {
        s32 frameValue;

        D_8009E694++;
        if ((D_801E6E74 >= 2) && (D_801E4DAC != 0)) {
            D_8009AF9C--;
        }

        frameValue = D_801E40B8;
        if ((u32)frameValue >= 0x5A) {
            if (D_801E6E74 == 0) {
                D_801E6E74 = 1;
            } else {
                goto update_race;
            }
        } else if (D_801E6E74 == 0) {
            func_8003C508(D_8009E6D4, frameValue);
        } else {
update_race:
            if ((D_801E6E74 == 1) && ((u32)D_801E40B8 >= 0xD3)) {
                func_8002BE18(D_8009E6D4, frameValue);
                func_80042BF0();
                D_801E6E74 = 2;
                D_8019C750 = 0x1E;
            }
        }

        if (D_801E6E74 < 4) {
            func_8003425C(D_801E40B8);
            func_800410BC(D_801E40B8);
        }

        if (D_801E6E74 < 5) {
            option = func_8003591C(D_8009E6D4, D_801E4DAC);
            func_800352B8(D_8009E6D4, D_801E4DAC, option);
            if (option < 2) {
                func_80033090();
            }
        }

        if (D_801E6E74 < 4) {
            if (D_801E4DAC != 0) {
                func_800331F8(D_8009AF9C);
            }
            if (D_8009AF9C <= 0) {
                if (*(s16 *)(D_8009E67C + 6) != 0) {
                    func_8005D6EC(0x3D);
                }
                func_8005E4A4(0);
                D_801E6E74 = 5;
                D_801E43FC = 0;
                func_8003CA14(D_8009E6D4);
                func_80042CCC(8);
            }
        }

        if (D_801E4DAC != 0) {
            if (D_801E6E74 < 4) {
                func_8003AE2C();
                func_80033230();
            }
        }
        if (option < 2 && D_801E6E74 < 5) {
            func_80032E9C(D_801E4DAC);
        }

        if (D_801E6E74 > 0) {
            func_8002DEFC(D_8009E6D4);
        } else if (D_801E6E74 == 0) {
            func_8005D9F8(0, 0);
        }

        if ((D_801E6E74 >= 2) && (D_801E4DAC != 0)) {
            func_8003B0D4();
        }

        {
            s32 selectorMask;
            u16 inputMask;

            selectorMask = D_801E4369;
            inputMask = D_801E436E;
            selectorMask = (u32)(selectorMask ^ 0x23) < 1;
            if ((inputMask & D_801E4B6C[selectorMask * 8]) &&
                (u32)((u16)D_801E6E74 - 2) < 2) {
                D_8009E870 ^= 1;
            }
        }

        if (D_801E6E74 == 5) {
            func_8003CB3C(D_8009E6D4);
        } else if (D_801E6E74 > 0) {
            func_80043BCC(D_8009E870, D_8009E6D4);
        }

        if (D_801E6E74 != 5) {
            next = D_8009E74C;
        } else {
            next = D_801E3E8C;
        }
        func_80019EFC(next);

        if (D_801E4DAC != 0) {
            func_800389F0();
        }
        func_80045CD4();
        func_800418D4();

        if ((D_8009E78C != D_801E408C) && (D_801E6E74 < 4)) {
            s16 counter;

            counter = D_801E8A8C + 1;
            D_801E8A8C = counter;
            if (counter >= 0xA) {
                func_800333DC();
                if (D_801E8A8C >= 0x51) {
                    D_801E8A8C = 0xA;
                }
                if ((u8)D_801E40B8 == 0) {
                    func_8005D6EC(0x2C);
                }
            }
        } else {
            D_801E8A8C = 0;
        }

        *(s32 *)0x1F800084 = D_801E4030;
        func_80041840();
        func_8004123C();
        if (D_801E4DAC != 0) {
            if (D_8009E6A4 != 5) {
                func_8003D458(D_801E40B8);
            }
            func_80069888(D_8009E6AC);
            func_8003F608(1);
            func_8001ADF4(D_801E40B8);
        }
        func_8003E1A4(D_801E428C & 3, D_801E40B8, 1);
        if (func_8001A9A8() != 0) {
            func_8003E1A4(D_801E428C & 3, D_801E40B8, 0);
            func_8001ABD8();
        }

        func_800350B4(D_8009E744);
        if (D_801E6E74 >= 4) {
            D_8019C78C = 0;
        }
        func_8005B190(D_8019C78C, D_8019C78C);
        if ((D_801E6E74 != 0) && (option < 2) && (D_801E6E74 < 5)) {
            func_8002F458();
        }

        if (D_801E6E74 < 4) {
            s32 *valuePtr;

            valuePtr = &D_8009E744;
            func_80040DB4(*valuePtr);
            func_80040ADC(*valuePtr);
            func_8004087C(D_8009E74C);
            func_80040F24();
        } else {
            func_8005BEA8(0, 0);
            func_8005C31C(2, 0, 0);
            func_8005C31C(3, 0, 0);
            func_8005C31C(0, 0, 0);
            func_8005C31C(1, 0, 0);
        }
        func_8001F9D8();
    }

}
