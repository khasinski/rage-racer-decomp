#include "common.h"

extern u8 *D_801E4FAC;
extern s32 D_8019CACC;
extern s32 D_8019C768;
extern s32 D_801E428C;
extern s32 D_801E40D4;
extern s32 D_8009E6A4;
extern s32 D_8019C908;
extern s16 D_801E4DAC;
extern s32 D_801E42E4;
extern s32 D_801E40B8;
extern u16 D_801E4034;
extern s16 D_8019CABC;

extern s32 D_80082568;
extern s32 D_80082D6C;

extern void *D_8019C764;
extern void *D_801E40B4;
extern void *D_8019C794;
extern void *D_801E8A44;
extern void *D_8009F0B0;
extern void *D_8019CB00;
extern void *D_801E4188;

extern s32 D_8009B34C;
extern s32 D_8009B350;
extern s32 D_8009B2F8;
extern s32 D_8009B2FC;
extern s32 D_8009B300;
extern s32 D_8009B304;
extern s32 D_8009B308;
extern s32 D_8009B30C;
extern s32 D_8009B310;
extern s32 D_8009B314;
extern s32 D_8009B318;
extern s32 D_8009B31C;
extern s32 D_8009B320;
extern s32 D_8009B324;
extern s32 D_8009B328;
extern s32 D_8009B32C;
extern s32 D_8009B330;
extern s32 D_8009B334;
extern s32 D_8009B338;
extern s32 D_8009B33C;
extern s32 D_8009B340;
extern s32 D_8009B344;
extern s32 D_8009B354;
extern s32 D_8009B358;
extern s32 D_8009B35C;
extern s32 D_8009B360;
extern s32 D_8009B364;
extern s32 D_8009B368;
extern s32 D_8009B36C;
extern s32 D_8009B370;
extern s32 D_8009B374;
extern s32 D_8009B378;

extern s32 D_8019C9F0;
extern s32 D_8019C9F8;
extern s32 D_8019C7AC;
extern s32 D_801E4138;
extern s32 D_8019C7C0;
extern s32 D_8019C758;
extern s32 D_801E4D74;

void func_80065860(s32);
void func_80017884(s32);
void func_8001C088(s32, s32, s32);
void func_80050B1C(void);
void func_8001A610(void);
void func_80069728(void *, void *);
void func_8005290C(s32);
void func_80054C84(s32);
void func_800551BC(s32);
void func_800562C8(s32);
void func_80056E64(s32);
void func_800576BC(s32);
void func_8005803C(s32);
void func_800586B0(s32);
void func_80058B88(s32);
void func_80059248(s32);
void func_8005A2CC(s32);
void func_800496F0(s32);
void func_8004CF30(s32);
void func_800509C4(s32);

void func_80050C18(void) {
    u8 *p;
    s32 *scratch;

    func_80065860(0);
    D_8019CACC = 0;
    p = D_801E4FAC;
    {
        s32 t0 = *(s32 *)(p + 0);
        s32 t1 = *(s32 *)(p + 4);
        s32 t2 = *(s32 *)(p + 8);
        s32 t3 = *(s32 *)(p + 16);
        D_8019C768 = 0x80;
        D_801E428C = t0;
        D_801E40D4 = t1;
        D_8009E6A4 = t2;
        D_8019C908 = t3;
    }
    func_80017884(1);

    func_8001C088(0, 0, 0);
    D_801E42E4 = 8;
    D_801E40B8 = 0;
    if (D_801E4DAC != 0) {
        D_8019CABC = D_801E4034;
    } else {
        D_8019CABC = *(u16 *)((u8 *)D_801E4FAC + 16);
    }
    D_801E428C = ((s32)D_8019CABC << 2) | D_801E428C;
    func_80050B1C();

    scratch = (s32 *)0x1F800000;
    scratch[3] = -64;
    scratch[4] = -256;
    scratch[2] = 0;
    scratch[6] = 0x100;
    scratch[7] = 0;
    scratch[8] = 0;
    func_8001A610();
    func_80069728((void *)0x1F800028, &D_80082D6C);

    D_8019C764 = &D_80082568;
    D_801E40B4 = &D_80082568;
    D_8019C794 = &D_80082568;
    D_801E8A44 = &D_80082568;
    D_8009F0B0 = &D_80082568;
    D_8019CB00 = &D_80082568;
    D_801E4188 = &D_80082568;
    D_8009B34C = 500000;
    D_8009B350 = 500000;
    D_8019C9F0 = 0;
    D_8009B2F8 = 0;
    D_8009B2FC = 0;
    D_8009B300 = 0;
    D_8009B304 = 0;
    D_8009B308 = 0;
    D_8009B30C = 0;
    D_8009B310 = 0;
    D_8009B354 = 0;
    D_8009B358 = 0;
    D_8009B35C = 0;
    D_8009B360 = 0;
    D_8009B364 = 0;
    D_8009B368 = 0;
    D_8009B370 = -1;
    D_8009B374 = 0;
    D_8009B378 = -1;
    D_8009B318 = 0;
    D_8009B31C = 0;
    D_8009B320 = 0;
    D_8009B324 = 0;
    D_8009B328 = 0;
    D_8009B36C = D_801E428C;
    D_8009B32C = 0;
    D_8009B330 = 0;
    D_8009B334 = 0;
    D_8009B314 = 1;
    D_8009B340 = -1;
    D_8009B344 = -1;
    D_8009B338 = 0;
    D_8009B33C = 0;
    D_8019C9F8 = 0;
    D_8019C7AC = 0;
    D_801E4138 = 0;
    D_8019C7C0 = 0;
    D_8019C758 = 0;
    D_801E4D74 = 0;

    func_8005290C(0);
    func_80054C84(0);
    func_800551BC(0);
    func_800562C8(0);
    func_80056E64(0);
    func_800576BC(0);
    func_8005803C(0);
    func_800586B0(0);
    func_80058B88(0);
    func_80059248(0);
    func_8005A2CC(0);
    func_800496F0(0);
    func_8004CF30(0);
    func_800509C4(0);
}
