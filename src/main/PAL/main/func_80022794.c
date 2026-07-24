#include "common.h"

extern u8 D_80010FB0[];
extern s32 D_8009E694;
extern s32 D_8019C8F8;
extern s32 D_8019CA14;
extern s16 D_8019CABC;
extern u8 D_8019CB78[];
extern s32 D_8019CE10;
extern s32 D_801E40B8;
extern u8 D_801E417C[];
extern s32 D_801E428C;
extern s32 D_801E42E4;
extern u16 D_801E436E;
extern u16 D_801E4370;
extern s32 D_801E6830;
extern s32 D_801E6C8C;
extern u8 D_801E7744[];
extern s32 D_801E8A48;
extern u8 D_801F17FC[];

void func_80018410(void);
void func_80021D68(void);
void func_80021DB8(u8 *arg0);
void func_80022068(u8 *arg0);
void func_8002229C(s32 arg0, s32 arg1);
void func_80033AA0(s32 arg0, s32 arg1);
void func_80042BC0(s32 arg0);
void func_80042BF0(void);
void func_80042CCC(s32 arg0);
void func_8005D6EC(s32 arg0);

void func_80022794(void) {
    u8 *name;
    s32 i;

    D_8009E694++;

    switch (D_801E6C8C) {
    case 0:
        D_801E40B8 -= 8;
        func_80033AA0(D_801E40B8, 0x49);
        if (D_801E40B8 == 0) {
            if (D_801E8A48 < 5 || D_8019CE10 < 5) {
                func_80042BC0(0xE);
                func_80042BF0();
            }
            if (D_801E8A48 < 5) {
                D_801E6830 = 0xB;
                D_8019C8F8 = 0;
                D_801E6C8C = 1;
            } else {
                D_801E6C8C = 2;
            }
        }
        func_80021DB8((u8 *)0);
        break;

    case 1: {
        u8 *timeName;
        u8 *timeRecordBase;
        u8 *rankingRecordBase;
        u8 *record;
        s32 previous;
        u16 buttons;

        previous = D_801E6830;
        if (D_801E4370 & 0x8000) {
            D_801E6830 = previous - 1;
        } else if (D_801E4370 & 0x2000) {
            D_801E6830 = previous + 1;
        }
        D_801E6830 = (D_801E6830 + 42) % 42;
        if (previous != D_801E6830) {
            func_8005D6EC(1);
        }

        D_801E417C[D_8019C8F8] = D_801E6830;
        buttons = D_801E436E;
        name = (u8 *)D_801E417C;
        if (buttons & 0x860) {
            func_8005D6EC(2);
            D_8019C8F8++;
            if (D_8019C8F8 == 6) {
                D_801E6C8C = 2;
                i = 0;
                if (D_8019CE10 < 5) {
                    timeRecordBase = D_8019CB78;
                    timeName = D_801F17FC;
                    do {
                        *timeName = D_801E417C[i];
                        record = (u8 *)((((D_801E428C * 5) + D_8019CE10) *
                                         0x10) +
                                        (D_8019CABC * 0x140) +
                                        (s32)timeRecordBase + i);
                        i++;
                        *record = D_80010FB0[*timeName];
                        timeName++;
                    } while (i < 6);
                }
            }
            D_801E6830 = D_801E417C[D_8019C8F8];
        } else if ((buttons & 0x90) && D_8019C8F8 > 0) {
            func_8005D6EC(3);
            D_8019C8F8--;
            D_801E6830 = name[D_8019C8F8];
        }

        if (D_801E6C8C == 1) {
            func_8002229C(D_8019C8F8, D_801E8A48);
        }
        i = 0;
        rankingRecordBase = D_801E7744;
        do {
            record = (u8 *)((((D_801E428C * 5) + D_801E8A48) * 0x10) +
                            (D_8019CABC * 0x140) +
                            (s32)rankingRecordBase + i);
            *record = D_80010FB0[D_801E417C[i]];
            i++;
        } while (i < 6);
        func_80021DB8((u8 *)0);
        break;
    }

    case 2:
        if (D_801E436E & 0x860) {
            D_801E6C8C = 3;
            D_8019CA14 = 0;
        }
        func_80021DB8((u8 *)0);
        break;

    case 3:
        D_8019CA14 -= 8;
        func_80021DB8((u8 *)D_8019CA14);
        func_80022068((u8 *)(D_8019CA14 + 0x140));
        if (D_8019CA14 < -0x13F) {
            if (D_8019CE10 < 5) {
                D_8019C8F8 = 0;
                D_801E6C8C = 4;
                D_801E6830 = D_801F17FC[0];
            } else {
                D_801E6C8C = 5;
            }
        }
        break;

    case 4: {
        u8 *recordBase;
        u8 *record;
        s32 previous;
        u16 buttons;

        previous = D_801E6830;
        if (D_801E4370 & 0x8000) {
            D_801E6830 = previous - 1;
        } else if (D_801E4370 & 0x2000) {
            D_801E6830 = previous + 1;
        }
        D_801E6830 = (D_801E6830 + 42) % 42;
        if (previous != D_801E6830) {
            func_8005D6EC(1);
        }

        D_801F17FC[D_8019C8F8] = D_801E6830;
        buttons = D_801E436E;
        name = (u8 *)D_801F17FC;
        if (buttons & 0x860) {
            func_8005D6EC(2);
            D_8019C8F8++;
            if (D_8019C8F8 == 6) {
                D_801E6C8C = 5;
            }
            D_801E6830 = name[D_8019C8F8];
        } else if ((buttons & 0x90) && D_8019C8F8 > 0) {
            func_8005D6EC(3);
            D_8019C8F8--;
            D_801E6830 = name[D_8019C8F8];
        }

        if (D_801E6C8C == 4) {
            func_8002229C(D_8019C8F8, D_8019CE10);
        }
        i = 0;
        recordBase = D_8019CB78;
        do {
            record = (u8 *)((((D_801E428C * 5) + D_8019CE10) * 0x10) +
                            (D_8019CABC * 0x140) + (s32)recordBase + i);
            *record = D_80010FB0[D_801F17FC[i]];
            i++;
        } while (i < 6);
        func_80022068((u8 *)0);
        break;
    }

    case 5:
        if (D_801E436E & 0x860) {
            if (D_801E8A48 < 5 || D_8019CE10 < 5) {
                func_80042CCC(0x78);
                func_80042BF0();
            }
            D_801E6C8C = 6;
            D_8019CA14 = 0;
        }
        func_80022068((u8 *)0);
        break;

    case 6:
        D_801E40B8 += 2;
        func_80033AA0(D_801E40B8, 0x49);
        if ((u32)D_801E40B8 >= 0x100) {
            func_80018410();
            D_801E42E4 = 6;
        }
        func_80022068((u8 *)0);
        break;
    }

    func_80021D68();
}
