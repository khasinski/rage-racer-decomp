#include "common.h"

extern u8 D_8009E6D4[];
extern s16 D_8009E74C;
extern s32 D_8009E694;
extern s32 D_8009E870;
extern s16 D_8009E6A0;
extern s32 D_8009AF8C;
extern s32 D_8009AF90;
extern s32 D_8009AF94;
extern s32 D_8009AF98;
extern s32 D_8009AF9C;
extern s32 D_8009AFA0;
extern s16 D_8019C750;
extern s32 D_8019C768;
extern u8 *D_8019C9A8;
extern s16 D_8019C9AC;
extern s32 D_801E408C;
extern s32 D_801E40B8;
extern s32 D_801E40CC;
extern s32 D_801E40D8;
extern s32 D_801E40E0;
extern s32 D_801E4148;
extern s32 *D_801E4150;
extern s32 D_801E41E8[];
extern s32 D_801E4248;
extern s32 D_801E428C;
extern s32 D_801E42E4;
extern s32 D_801E4364;
extern s16 D_801E43FC;
extern s32 D_801E4408[];
extern s32 D_801E4BA8;
extern s32 D_801E4BB4;
extern s32 D_801E4BCC;
extern s16 D_801E4CF8;
extern s32 D_801E4D64;
extern s32 D_801E4D98;
extern s32 D_801E4D9C;
extern s32 D_801E4DA0;
extern s16 D_801E4DAC;
extern s16 D_801E6C90;
extern s16 D_801E6E74;
extern s16 D_801E6F20;
extern s16 D_801E6F22;
extern s16 D_801E6F24;
extern s16 D_801E6F26;
extern s32 D_801E7A50;
extern u8 D_80011488[];

void func_8001BE9C(s32, s32, s32);
void func_80017884(s32);
void func_8001F100(void);
void func_8001D30C(void);
void func_8001D210(void);
void func_8002C478(void *);
void func_80019E84(s32);
void func_80038844(void);
void func_8001A980(void);
void func_800458CC(s32);
void func_800340D8(void);
void func_80032D5C(s32);
void func_8003D6E8(void);
void func_8003F0F8(void);
void func_8003E464(void);
void func_8003EBCC(void);
void func_8003F700(void);
void func_80042BC0(s32);
void func_8005DC1C(void);
void func_8001C974(void);
void func_8001674C(void *);

void func_8003609C(void) {
    s32 pad[2];
    u8 *new_var;
    u8 *base;
    s32 mode;
    s32 scene;
    s32 tableOffset;
    s32 trackLength;
    s32 *entry;
    s32 count;
    s32 i;
    s32 *first;
    s32 new_var2;
    s32 *second;

    func_8001BE9C(0, 0, 0);
    func_80017884(5);
    func_8001F100();
    func_8001D30C();
    func_8001D210();
    D_801E40CC = *D_801E4150;
    if (D_801E428C == 3) {
        D_801E4364 = 6;
    } else {
        D_801E4364 = 3;
    }
    base = D_8009E6D4;
    func_8002C478(base);
    func_80019E84(D_8009E74C);
    func_80038844();
    trackLength = D_801E40D8;
    mode = (count = D_801E428C);
    scene = D_801E408C;
    D_801E4D64 = 0;
    D_801E4248 = 0;
    D_8009AFA0 = 0;
    D_801E4DA0 = trackLength;
    D_801E4D98 = trackLength / 3;
    D_801E4D9C = D_801E4D98 * 2;
    tableOffset = (mode * 12) + (scene * 48);
    D_8009AF90 = *(s32 *)((u8 *)D_801E41E8 + tableOffset);
    scene *= 32;
    D_8009AF94 = *(s32 *)((u8 *)D_801E41E8 + tableOffset + 4);
    mode *= 8;
    do {
    } while (0);
    do {
        do {
            D_801E4148 = -2;
        } while (0);
        new_var = (u8 *)D_801E4408 + scene;
        count = mode + (s32)new_var;
        new_var2 = D_801E4DAC << 2;
        new_var2 += count;
        entry = (s32 *)new_var2;
        D_8009AF98 = *(s32 *)((u8 *)D_801E41E8 + tableOffset + 8);
    } while (0);
    D_8009AF8C = *entry;
    count = (new_var2 = D_801E4364);
    D_8009AF9C = 0x3A98;
    D_801E4BCC = D_8009AF8C;
    if (count > 0) {
        i = 0;
        second = (s32 *)(base + 0x184);
        first = (s32 *)(base + 0x16C);
        do {
            *first = 0;
            *second = 0;
            second++;
            i++;
            first++;
        } while (i < count);
    }
    D_801E4BA8 = 0;
    func_8001A980();
    func_800458CC(*(s32 *)(D_8019C9A8 + 8));
    func_800340D8();
    func_80032D5C(D_801E4DAC);
    D_8009E694 = 0;
    D_801E40B8 = 0;
    D_8009E870 = 0;
    D_801E6E74 = 0;
    D_801E7A50 = 0;
    D_801E4BB4 = 0x1FE;
    D_801E6F26 = 0;
    D_801E6F24 = 0;
    D_801E6F22 = 0;
    D_801E6F20 = 0;
    func_8003D6E8();
    func_8003F0F8();
    func_8003E464();
    func_8003EBCC();
    func_8003F700();
    func_80042BC0(D_801E40E0 + 3);
    D_8019C750 = 0;
    D_801E43FC = 0;
    func_8005DC1C();
    D_8009E6A0 = 1;
    D_801E4CF8 = (D_8019C9AC = (D_801E6C90 = 0));
    do {
    } while (0);
    D_801E42E4 = 12;
    D_8019C768 = 0x180;
    func_8001C974();
    func_8001674C(D_80011488);

    (void)pad;
}
