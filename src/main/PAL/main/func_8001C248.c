#include "common.h"
#include "psyq/gte.h"
#include "game/asset.h"
#include "game/race.h"
#include "game/state.h"
#include "game/render.h"
#include "game/menu.h"

extern s16 D_8019CE34;
extern Matrix D_8019CAD4;
extern Matrix D_8007C778;
void func_800698B8(Matrix *arg0);

void func_8001C248(s32 a0, Matrix *a1) {
    Matrix out;
    s32 s1;

    if (D_8019CE34 != 0) {
        s1 = 0x100 - (a0 * 3) / 4;
        out.m[0][0] = D_8019CAD4.m[0][0] * s1 / 256;
        out.m[0][1] = D_8019CAD4.m[0][1] * s1 / 256;
        out.m[0][2] = D_8019CAD4.m[0][2] * s1 / 256;
        out.m[1][0] = D_8019CAD4.m[1][0] * s1 / 256;
        out.m[1][1] = D_8019CAD4.m[1][1] * s1 / 256;
        out.m[1][2] = D_8019CAD4.m[1][2] * s1 / 256;
        out.m[2][0] = D_8019CAD4.m[2][0] * s1 / 256;
        out.m[2][1] = D_8019CAD4.m[2][1] * s1 / 256;
        out.m[2][2] = D_8019CAD4.m[2][2] * s1 / 256;
        func_800698B8(&out);
    } else {
        s32 k;
        s32 h;
        s32 kb;
        out.m[0][0] = D_8019CAD4.m[0][0];
        out.m[0][1] = D_8019CAD4.m[0][1];
        out.m[0][2] = D_8019CAD4.m[0][2];
        h = a0 / 2;
        k = 0x100;
        s1 = k - h;
        out.m[1][0] = D_8019CAD4.m[1][0] * s1 / 256;
        out.m[1][1] = D_8019CAD4.m[1][1] * s1 / 256;
        out.m[1][2] = D_8019CAD4.m[1][2] * s1 / 256;
        s1 = k - (a0 * 3) / 4;
        out.m[2][0] = D_8019CAD4.m[2][0] * s1 / 256;
        out.m[2][1] = D_8019CAD4.m[2][1] * s1 / 256;
        out.m[2][2] = D_8019CAD4.m[2][2] * s1 / 256;
        func_800698B8(&out);

        kb = k - a0;
        a1->m[0][0] = a1->m[0][0] * kb / 256 + D_8007C778.m[0][0] * a0 / 256;
        a1->m[0][1] = a1->m[0][1] * kb / 256 + D_8007C778.m[0][1] * a0 / 256;
        a1->m[0][2] = a1->m[0][2] * kb / 256 + D_8007C778.m[0][2] * a0 / 256;
        a1->m[1][0] = a1->m[1][0] * kb / 256 + D_8007C778.m[1][0] * a0 / 256;
        a1->m[1][1] = a1->m[1][1] * kb / 256 + D_8007C778.m[1][1] * a0 / 256;
        a1->m[1][2] = a1->m[1][2] * kb / 256 + D_8007C778.m[1][2] * a0 / 256;
        a1->m[2][0] = a1->m[2][0] * kb / 256 + D_8007C778.m[2][0] * a0 / 256;
        a1->m[2][1] = a1->m[2][1] * kb / 256 + D_8007C778.m[2][1] * a0 / 256;
        a1->m[2][2] = a1->m[2][2] * kb / 256 + D_8007C778.m[2][2] * a0 / 256;
    }
}

extern Matrix D_8019CAD4;
void func_800698B8(Matrix *arg0);
void func_8001C794(void) { func_800698B8(&D_8019CAD4); }

extern s32 D_8019C768;
extern s32 D_801E4B30;
extern u8 *D_8009E67C;

void func_80065860(s32 arg0);
void func_8005B9CC(void);
void func_8001A3C0(s32 arg0);
void func_80018F08(void);

void func_8001C7BC(void) {
    s32 count;
    u8 *ptr;
    u8 *end;

    func_80065860(0);
    D_8019C768 = 0x80;

    if (g_AssetLoadState != 1) {
        func_8005B9CC();
        func_8001A3C0(D_801E4B30);
        func_80018F08();

        D_8019C768 = 0x180;
        g_SceneTimer = 0;
        g_SceneId = 10;
        g_FadeLevel = 0;
        count = (g_GrandPrixClass < 2) ? 3 : 4;
        g_GrandPrixRound = 0;

        if (count != 0) {
            ptr = D_8009E67C;
            end = (u8 *)(count + (s32)ptr);
            do {
                if (*ptr != 0) {
                    g_GrandPrixRound++;
                }
                ptr++;
            } while ((s32)ptr < (s32)end);
        }

        if (D_8009E67C[g_CourseIndex] == 0) {
            g_GrandPrixRound++;
        }
    }
}

extern s16 D_8007C798[];

s32 func_8001C8F0(s32 arg0) {
    s32 value;
    s32 ret;

    if (g_SceneId == 10) {
        s32 offset;
        s32 counter;

        offset = arg0 << 1;
        counter = g_SceneTimer;
        value = (counter << 2) - *(s16 *)((u8 *)D_8007C798 + offset);
    } else {
        value = g_FadeLevel;
        if (value > 0) {
            value--;
            g_FadeLevel = value;
        }
        value = g_FadeLevel;
    }

    if (value >= 0) {
        ret = value;
        if (ret < 0x80) {
            return ret;
        }
        ret = 0x7F;
    } else {
        ret = 0;
    }
    return ret;
}

extern s32 D_801E42CC;
extern char *D_8007C7A0[];
extern char D_80010D2C[];

extern char D_80010C30[];
extern char D_80010C40[];
extern char D_80010C44[];
extern char D_80010C50[];
extern char D_80010C5C[];
extern char D_80010C68[];
extern char D_80010C70[];

extern s32 D_8019C70C[][4][2];
extern s32 D_801E4408[][4][2];

s32 func_8001C8F0(s32 arg0);
void func_80046A2C(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, s32 u0, s32 v0, s32 r, s32 g,
                   s32 b, s32 clutX, s32 shadeTex, s32 semiTrans, s32 flags);
void func_800632F0(void *dst, void *fmt, s32 v);
void func_80016B7C(u32 a0, u32 a1, void *a2, u32 a3, u32 a4);
void func_80021CD4(void *dst, s32 v);

void func_8001C974(void) {
    char buf[88];
    s32 col;
    s32 y0;
    void *ot = g_DrawBuffer + 204;

    col = func_8001C8F0(0);
    func_80046A2C(ot, 0x74, 0x14, 0x58, 0x38, 0xa8, 0xa8, col, col, col, 0x1f, 0, 1, 0x29);
    func_80046A2C(ot, 0x44, 0x50, 0xb8, 0x14, 0x48, 0xe8, col, col, col, 0x80, 0, 1, 0x29);

    col = func_8001C8F0(1);
    if (g_GrandPrixMode != 0) {
        func_800632F0(buf, D_80010C30, g_GrandPrixRound);
        func_80016B7C(0x5e, 0x68, buf, 0x7812, col);
        y0 = 0x78;
    } else {
        y0 = 0x68;
    }
    func_80046A2C(ot, 0x5e, y0, 0x84, 0xc, 0, g_CourseIndex * 12 + 156, col, col, col, 0x12, 0, 1, 0x29);

    col = func_8001C8F0(2);
    if (g_GrandPrixMode != 0) {
        func_80016B7C(0x80, 0x88, D_80010C40, 0x7812, col);
        func_800632F0(buf, D_80010C44, g_PrizeMoney[g_CourseIndex][g_GrandPrixClass][0]);
        func_80016B7C(0x56, 0x98, buf, 0x7812, col);
        func_800632F0(buf, D_80010C50, g_PrizeMoney[g_CourseIndex][g_GrandPrixClass][1]);
        func_80016B7C(0x56, 0xa4, buf, 0x7812, col);
        func_800632F0(buf, D_80010C5C, g_PrizeMoney[g_CourseIndex][g_GrandPrixClass][2]);
        func_80016B7C(0x56, 0xb0, buf, 0x7812, col);
    } else {
        func_80016B7C(0x62, 0x7c, D_80010C68, 0x7812, col);
        func_80021CD4(buf, D_8019C70C[g_GrandPrixSeries][g_CourseIndex][g_GrandPrixMode]);
        func_80016B7C(0x6a, 0x8c, buf, 0x7812, col);
        func_80016B7C(0x6a, 0x9c, D_80010C70, 0x7812, col);
        func_80021CD4(buf, D_801E4408[g_GrandPrixSeries][g_CourseIndex][g_GrandPrixMode]);
        func_80016B7C(0x6a, 0xac, buf, 0x7812, col);
    }
}

s32 func_80016EC4(void *ot, s32 p, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
s32 func_80017390(void *ot, s32 p, s32 a);
s32 func_80032F34(void *ot, s32 p, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void func_80016754(s32 x, s32 y, void *str, s32 col);

void func_8001CD54(void) {
    s32 x;
    char buf[88];
    s32 *scr = (s32 *)0x1F800000;
    s32 p;
    void *ot = g_DrawBuffer + 208;

    p = *scr;
    p = func_80016EC4(ot, p, 0x14, 0xce, 0x58, 8, 0xa8, 0xe0, 0x7812);
    x = (D_801E42CC == 0xa) ? 0x6c : 0x70;
    p = func_80016EC4(ot, p, x, 0xce, 8, 8, 0x84, 0xc4, 0x7812);
    p = func_80016EC4(ot, p, (D_801E42CC == 0xa) ? 0x84 : 0x80, 0xce, 8, 8, 0x8c, 0xc4, 0x7812);
    p = func_80017390(ot, p, 0x29);
    p = func_80032F34(ot, p, 0x10, 0xcc, 0x5b, 0xc, 0x85, 0x15, 0xe);
    p = func_80032F34(ot, p, 0x6c, 0xcc, 0x1f, 0xc, 0x40, 0x40, 0x40);
    p = func_80032F34(ot, p, 0x8c, 0xcc, 0xa4, 0xc, 0, 0, 0);
    p = func_80032F34(ot, p, 0xf, 0xcb, 0x122, 0xe, 0xff, 0xff, 0xff);
    *scr = p;

    func_800632F0(buf, D_80010D2C, D_801E42CC);
    x = (D_801E42CC == 0xa) ? 0x74 : 0x78;
    func_80016754(x, 0xce, buf, 0x78cc);
    func_80016754(0x90, 0xce, D_8007C7A0[D_801E42CC], 0x78cc);
}

extern s32 D_8009E6CC;
extern s32 D_801E40A8;
extern u8 D_801E7734[];
extern s32 D_801E40E0;

void func_80065860(s32 a);
void func_8001BE9C(s32 a, s32 b, s32 c);
void func_8005D6EC(s32 a);
s32 func_80018FC4(void);

void func_8001CFB4(void) {
    if ((u32)g_SceneTimer < 10000) {
        g_SceneTimer = g_SceneTimer + 1;
    }
    if (g_SceneTimer == 0xf) {
        func_80065860(1);
    }
    if (g_SceneTimer == 1) {
        func_8001BE9C(0, 0, 0);
    }
    func_8001C974();
    if (g_SceneTimer == 0x20) {
        func_8005D6EC(0x19);
    }
    if (g_FadeLevel == 0) {
        if (func_80018FC4() == 0) {
            g_FadeLevel = 0x80;
        }
    } else if ((u32)g_SceneTimer >= 121) {
        g_SceneId = 0xb;
        if ((g_PadHeld & 0x80c) == 0x80c) {
            g_MirrorMode = 1;
        } else {
            g_MirrorMode = 0;
        }
        if (D_801E42CC == 0) {
            s32 idx = D_8009E6CC;
            u8 val = D_801E7734[idx];
            D_8009E6CC = idx + 1;
            D_801E40E0 = val;
            if (D_8009E6CC == D_801E40A8) {
                D_8009E6CC = 0;
            }
        } else {
            D_801E40E0 = D_801E42CC - 1;
        }
        if (D_801E40E0 == 9) {
            D_801E40E0 = 0xe;
        }
    }
    if (g_SceneId == 0xa) {
        u16 flags = g_PadEdge2;
        if (flags & 0x8000) {
            D_801E42CC = D_801E42CC - 1;
        } else if (flags & 0x2000) {
            D_801E42CC = D_801E42CC + 1;
        }
        D_801E42CC = (D_801E42CC + D_801E40A8 + 1) % (D_801E40A8 + 1);
        func_8001CD54();
    }
}

extern Matrix D_8019CAD4, D_8007C758, D_8009E6AC, D_8007C778;
void func_800698B8(Matrix *arg0);
void func_80069888(Matrix *arg0);
void func_80069A18(s32 arg0, s32 arg1, s32 arg2);
void func_800686D4(s32 arg0, s32 arg1);
void func_80069A38(s32 arg0, s32 arg1, s32 arg2);
void func_8001D210(void) {
    D_8019CAD4 = D_8007C758;
    D_8009E6AC = D_8007C778;
    func_800698B8(&D_8019CAD4);
    func_80069888(&D_8009E6AC);
    func_80069A18(0x20, 0x20, 0x20);
    func_800686D4(0x1770, 0x140);
    func_80069A38(0x80, 0x80, 0x80);
}
