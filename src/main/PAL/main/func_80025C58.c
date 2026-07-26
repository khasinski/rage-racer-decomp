#include "common.h"
#include "game/state.h"
#include "game/render.h"
#include "game/menu.h"
#include "game/car.h"

extern s32 D_801E4B84;
extern s32 D_8007D6B0;
extern s32 D_801E4404;
s32 func_80016EC4(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 func_8001720C(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 func_80017390(u8 *arg0, s32 arg1, s32 arg2);
extern u32 D_8009E6CC;
extern s32 g_BgmTrackCount asm("D_801E40A8");
extern u8 D_801E7734[];
void func_8001B488(void);
extern s32 D_8019CAF4;
extern s32 D_8019CE00;
extern s32 D_8019C7BC;
extern s32 D_8019C99C;
extern s32 D_8007D6B4;
extern s32 D_801E412C;
extern s32 D_8009E66C;
extern s32 g_CameraViewMode asm("D_8009E870");
void func_80042BC0();
void func_80042BF0();
void func_80025E54();
void func_80065860();
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
void GameDrawCourseScenery2() asm("func_8003E2E8");
int func_8001A0E4();

void func_80025C58(void) {
    u8 *base;
    s32 arg4;
    s32 arg5;
    s32 temp;
    s32 product;
    s32 value;
    s32 next;

    base = g_DrawBuffer + 0xD0;
    next = *(s32 *)0x1F800000;
    temp = (D_801E4B84 == 0) ? 0x3FEC : 0x3FEF;
    arg4 = 0x14;
    arg5 = 0x10;

    next = func_80016EC4(base, next, 0x20, 0xC1, arg4, arg5, 0, 0, temp);
    temp = (D_801E4B84 == 1) ? 0x3FEC : 0x3FEF;
    next = func_80016EC4(base, next, 0x36, 0xC1, arg4, arg5, arg4, 0, temp);
    temp = (D_801E4B84 == 2) ? 0x3FEC : 0x3FEF;
    next = func_80016EC4(base, next, 0x4C, 0xC1, arg4, arg5, 0x28, 0, temp);

    if (D_8007D6B0 != 0) {
        D_8007D6B0--;
        temp = 0x10;
    } else {
        value = D_801E4404;
        product = value << 1;
        product += value;
        product <<= 2;
        temp = product + 0x1C;
    }

    next = func_80016EC4(base, next, 0x64, 0xC2, 0xBA, 0xC, 0, temp, 0x3FED);
    next = func_80016EC4(base, next, 0x62, 0xC0, 0xBE, 0x10, 0x3C, 0, 0x3FEE);
    next = func_8001720C(base, next, 0x14, 0xB8, 0x118, 0x20, 0, 0, 0);
    *(s32 *)0x1F800000 = func_80017390(base, next, 0xB);
}

void func_80025E54(u32 arg0) {
    u8 *first;
    u8 *before;
    u8 *other;
    u8 value;

    D_8009E6CC++;
    if (D_8009E6CC == g_BgmTrackCount) {
        func_8001B488();

        first = D_801E7734;
        before = first - 1;
        if (arg0 == first[0]) {
            other = g_BgmTrackCount + before;
            value = *other;
            first[0] = value;
            *other = arg0;
        }
    }
}

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
    D_801E4404 = (D_801E4404 + g_BgmTrackCount) % g_BgmTrackCount;
L124:
    D_8019CE00 = D_801E4404 + 3;

L13c:
    if (g_SceneTimer == 2) func_80065860(1);
    if (g_FadeStep != 0) goto L424;

    if (g_PadEdge2 & 0x8000) {
        if (D_801E4B84 > 0) D_801E4B84 = D_801E4B84 - 1;
    }
    if (g_PadEdge2 & 0x2000) {
        if (D_801E4B84 < 2) D_801E4B84 = D_801E4B84 + 1;
    }
    if (g_PadEdge2 & 1) {
        s32 p;
        s32 h0;
        func_8001B488();
        h0 = D_801E7734[0];
        p = D_801E4404;
        if (p == h0) {
            u8 tmp = D_801E7734[g_BgmTrackCount - 1];
            D_801E7734[0] = tmp;
            D_801E7734[g_BgmTrackCount - 1] = p;
        }
        D_8007D6B4 = 1;
        D_8007D6B0 = 60;
    }
    {
        u16 f = g_PadEdge2;
        if (f & 2) {
            D_8007D6B4 = 0;
            D_8007D6B0 = 0;
        }
        if (f & 0x860) {
            switch (D_801E4B84) {
            case 0:
                if (D_8007D6B4 == 0) {
                    D_801E4404 = D_801E4404 - 1;
                    D_801E4404 = (D_801E4404 + g_BgmTrackCount) % g_BgmTrackCount;
                }
                goto lab380;
            case 2:
                if (D_8007D6B4 != 0) {
                    D_801E4404 = D_801E7734[D_8009E6CC];
                    func_80025E54(D_801E4404);
                } else {
                    D_801E4404 = D_801E4404 + 1;
                    D_801E4404 = (D_801E4404 + g_BgmTrackCount) % g_BgmTrackCount;
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
                g_FadeStep = 4;
                break;
            }
        } else if (f & 0x90) {
            func_80042CCC(60);
            g_FadeStep = 4;
        }
    }
    {
        u16 f = g_PadEdge2;
        if (f & 4) D_801E412C = 1;
        if (f & 8) D_801E412C = 0;
    }
    goto L48c;

L424:
    func_80033AA0(g_FadeLevel, 0x49);
    g_FadeLevel = g_FadeLevel + g_FadeStep;
    if (g_FadeLevel >= 256) {
        func_80018B98();
        D_8019C99C = 3;
        g_FadeLevel = 256;
        g_FadeStep = -4;
    }

L48c:
    if (D_801E412C != 0) func_80025C58();
    g_AnimTimer = g_AnimTimer + 1;
    D_8009E66C = func_8001A0E4(0xff, D_8009E66C);
    func_8003BB50();
    func_80019EFC(g_Cars[D_8009E66C].field_78);
    func_80043BCC(g_CameraViewMode, &g_Cars[D_8009E66C]);
    func_800389F0();
    func_80045CD4();
    func_800418D4();
    *(s32 *)0x1F800084 = g_IsEnvironmentMode4;
    func_80041888();
    func_8004123C();
    GameDrawCourseScenery2(g_AnimTimer, 1);
}
