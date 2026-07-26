#include "common.h"
#include "game/screens.h"
#include "game/race.h"
#include "game/state.h"
#include "game/render.h"
#include "game/menu.h"
#include "game/car.h"

typedef struct {
    u8 left;
    u8 right;
} GrandPrixIntroPosition;

typedef struct {
    u8 x;
    u8 y;
    u8 width;
} GrandPrixIntroLayout;

typedef union {
    s16 palette;
    s16 layout;
    s16 width;
    s16 color;
} GrandPrixIntroSelection;

void *func_8001720C(
    void *ot, void *prim, s32 x, s32 y, s32 w, s32 h, s32 r, s32 g, s32 b);

void *func_80017390(void *ot, void *prim, s32 arg2);
extern s32 D_8019C8EC;
extern s32 D_801E6E78;
extern s32 D_8019CB6C;
extern s32 D_8009EC8C;
extern u8 D_8009E6D4;
extern s32 D_801F179C;
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
void GameDrawCourseScenery2(s32 arg0, s32 arg1) asm("func_8003E2E8");
void func_80045CD4(void);
void func_800418D4(void);
void func_8001FB8C(void);
void func_80019E84(s32 arg0);
void func_8001F330(s32 arg0, void *arg1, void *arg2);
extern char D_80010DF0[];
extern char *D_8007D404[];
void func_80016EA0(s32 arg0, s32 arg1, void *arg2, s32 arg3);
void func_80016A18(s32 arg0, s32 arg1, void *arg2, s32 arg3);
s32 func_80032F34(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
extern s32 D_801E4BA8;
extern s32 D_8019C7C4;
extern s32 D_8019CB74;
extern s32 D_8019C70C[][4][2];
extern s32 D_8009E858[];
extern u16 D_8007D41E[];
extern u16 D_8007D426[];
extern char D_80010DF8[];
extern char D_80010E10[];
extern char D_80010E1C[];
extern char D_80010E20[];
extern char D_80010E28[];
extern GrandPrixIntroLayout D_8007D414[];
extern GrandPrixIntroPosition D_8007D430[];

s32 func_80016EC4(
    void *arg0,
    s32 arg1,
    s32 arg2,
    s32 arg3,
    s32 arg4,
    s32 arg5,
    s32 arg6,
    s32 arg7,
    s32 arg8);

void func_800632F0(void *dst, void *fmt, ...);
void func_800200D0(void);
void *func_80021CD4(void *dst, s32 value);

void func_8001FC30(s32 x, s32 y) {
    void *ot;
    register void *prim asm("$6");
    s32 redStack;
    register s32 green asm("$8");
    register s32 temp asm("$3");
    register s32 quotient asm("$2");
    register s32 red asm("$5");
    s32 blue;
    s32 width;
    s32 height;

    ot = *(u8 **)((u8 *)0x801A0000 - 0x3700) + 0xCC;
    prim = *(void **)0x1F800000;

    redStack = y;
    /* Match note: make GCC materialize the red clamp from a3 into a1. */
    __asm__("" : "=r"(redStack) : "0"(redStack));
    temp = x;
    if (x < 0) {
        temp = x + 7;
    }
    quotient = temp >> 3;
    green = quotient + redStack;
    if (x < 0) {
        x += 3;
    }
    blue = (x >> 2) + redStack;

    if (redStack < 0) {
        goto red_zero;
    }
    red = redStack;
    if (red < 0x100) {
        goto red_done;
    }
    red = 0xFF;
    goto red_done;
red_zero:
    red = 0;
red_done:
    redStack = red;

    if (green < 0) {
        goto green_zero;
    }
    temp = green;
    if (temp < 0x100) {
        goto green_done;
    }
    temp = 0xFF;
    goto green_done;
green_zero:
    temp = 0;
green_done:
    green = temp;

    if (blue < 0) {
        goto blue_zero;
    }
    temp = blue;
    if (temp < 0x100) {
        goto blue_done;
    }
    temp = 0xFF;
    goto blue_done;
blue_zero:
    temp = 0;
blue_done:

    width = 0x140;
    height = 0xF0;
    prim = func_8001720C(ot, prim, 0, 0, width, height, redStack, green, temp);
    *(void **)0x1F800000 = func_80017390(ot, prim, 0x49);
}

void func_8001FD3C(void) {
    g_AnimTimer++;
    g_SceneTimer++;
    if (g_SceneTimer == 0x3C) {
        if (g_GrandPrixMode != 0) {
            if (D_8019C8EC == 0) {
                func_8005D6EC(g_RacePosition == 1 ? 0x40 : 0x41);
            }
        }
    }

    if (g_FadeStep < 0) {
        g_FadeLevel += g_FadeStep;
        if (g_FadeLevel < 0) {
            g_FadeLevel = 0;
            g_FadeStep = 0;
            D_801E6E78 = 0;
        }
        func_80033AA0(g_FadeLevel, 0x29);
    } else {
        if (D_8019C8EC != 0) {
            s32 cb = D_8019CB6C;
            s32 fc = g_SceneTimer;
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

        if (g_FadeStep == 0) {
            if ((g_PadEdge2 & 0x860) != 0) {
                g_FadeStep = 4;
                func_80042CCC(0x3C);
            } else if (g_SceneTimer == D_8019CB6C - 68) {
                g_FadeStep = 4;
                if (D_8009EC8C == 0) {
                    func_80042CCC(0x3C);
                }
            }
        } else {
            g_FadeLevel += g_FadeStep;
            if (g_FadeLevel >= 257) {
                s32 v = g_GrandPrixMode;
                g_MirrorMode = 0;
                g_SceneId = v == 0 ? 0x14 : 0x12;
            }
        }

        if (D_8019C8EC != 0) {
            if (((u32)(D_8019CB6C - 600) < (u32)g_SceneTimer) || (g_FadeLevel != 0)) {
                func_8001FC30(D_801E6E78, g_FadeLevel);
            }
        } else {
            if (g_FadeLevel != 0) {
                func_80033AA0(g_FadeLevel, 0x49);
            }
        }
    }

    func_8001F330(D_801F179C, &D_8009E6D4, (u8 *)g_Cars);
    D_801F179C++;
    if (D_801F179C == D_8019CB6C) {
        D_801F179C = 0;
    }
    func_80035040();
    func_80043BCC(2, &D_8009E6D4);
    *(s32 *)0x1F800084 = D_801E4030;
    func_80041888();
    if (g_GrandPrixMode != 0) {
        func_80038A88();
    }
    func_8004123C();
    GameDrawCourseScenery2(g_SceneTimer, 1);
    func_80045CD4();
    func_800418D4();
    func_8001FB8C();
    if (g_SceneTimer == 1) {
        func_80019E84(D_8009E74C);
    }
}

void GameDrawResultScreen(void) {
    u8 *base;
    s32 *scratch;
    s32 width;
    volatile s32 pad[12];
    s32 y;
    s32 next;

    (void)pad;
    func_80016EA0(0xDC, 0x1C, D_80010DF0, 0x7812);

    if (g_GrandPrixMode != 0) {
        y = 0x3C;
    } else {
        y = 0x39;
    }
    func_80016A18(0x60, y, D_8007D404[g_CourseIndex], 0x78CC);

    width = 0x140;
    base = g_DrawBuffer;
    scratch = (s32 *)0x1F800000;
    base += 0xCC;

    next = *scratch;
    next = func_80032F34(base, next, 0, 0, width, 0x30, 0x85, 0x15, 0xE);
    *scratch = func_80032F34(base, next, 0, 0x30, width, 0x18, 0xF0, 0xF0, 0xF0);
}

void func_800201D4(void) {
    u8 *base;
    char text[0x30];
    if ((D_8019C7C4 != 0) && (D_8019CB74 >= 5)) {
        s32 *scratch;
        s32 next;
        s32 height;
        s32 color;

        scratch = (s32 *)0x1F800000;
        base = g_DrawBuffer + 0xCC;
        height = 8;
        color = 0x78CB;
        next = func_80016EC4(
            base, *scratch, 0x14, 0x1C, 0x38, height, 0, 0xE8, color);
        next = func_80016EC4(
            base,
            next,
            0x4C,
            0x1C,
            D_8007D430[D_8019C7C4 - 1].right,
            height,
            0x84,
            D_8007D430[D_8019C7C4 - 1].left,
            color);
        next = func_80016EC4(
            base,
            next,
            D_8007D430[D_8019C7C4 - 1].right + 0x4E,
            0x1C,
            0x30,
            height,
            0,
            0xF0,
            color);
        next = func_80016EC4(
            base,
            next,
            D_8007D430[D_8019C7C4 - 1].right + 0x7C,
            0x1C,
            0x20,
            height,
            0,
            0xF8,
            color);
        *scratch = next;
    }

    {
        char *name;
        s32 classNumber;
        s32 current;

        current = g_GrandPrixClass;
        classNumber = current + 1;
        name = g_GrandPrixNames[g_GrandPrixSeries ? current + 6 : current];
        func_800632F0(text, D_80010DF8, classNumber, name);
    }
    func_80016A18(0x10, 0x34, text, 0x78CC);

    func_800632F0(text, D_80010E10, g_GrandPrixRound);
    func_80016A18(0x10, 0x3C, text, 0x78CC);

    {
        s32 *scratch;
        GrandPrixIntroSelection *selection;
        s32 next;
        s32 selectionIndex;

        scratch = (s32 *)0x1F800000;
        func_800200D0();

        base = g_DrawBuffer + 0xCC;
        selection = (GrandPrixIntroSelection *)&g_RacePosition;
        next = func_80016EC4(
            base,
            *scratch,
            0xB4,
            0x60,
            0x58,
            0x38,
            0xA8,
            0xA8,
            D_8007D426[selection->palette]);

        selectionIndex = selection->layout;
        selectionIndex -= 1;
        next = func_80016EC4(
            base,
            next,
            D_8007D414[selectionIndex].x,
            0x5C,
            D_8007D414[selectionIndex].y,
            0x1C,
            D_8007D414[selection->width - 1].width,
            0xCC,
            D_8007D41E[selection->color]);
        *scratch = next;
    }

    func_80016EA0(0x10, 0x50, D_80010E1C, 0x7812);
}

void func_800204F4(s32 arg0) {
    s32 base;
    s32 i;
    s32 *times;
    s32 *selectedPtr;
    s32 count;
    s32 x;
    s32 textPos;
    s32 column;
    s32 columnBase;
    s32 drawColor;
    s32 quotient;
    char text[24];
    s32 color;

    base = arg0;
    func_80016EA0(0x10, base + 0x80, D_80010E20, 0x7812);

    text[0] = 0x54;
    text[1] = 0x2F;
    func_80021CD4(&text[2], D_801E4BA8);

    color = 0x7812;
    if (D_8019C70C[g_GrandPrixSeries][g_CourseIndex][g_GrandPrixMode] == D_801E4BA8) {
        color = 0x784C;
    }
    drawColor = color;
    count = base + 0x90;
    func_80016EA0(0x14, count, text, drawColor);

    func_80016EA0(0x10, base + 0xA4, D_80010E28, 0x7812);

    count = 6;
    if (g_CourseIndex != 3) {
        count = 3;
    }

    i = 0;
    if (count != 0) {
        times = D_8009E858;
        selectedPtr = D_8009E858;
        do {
            x = 0xB0;
            if (i < 3) {
                x = 0x14;
            }
            quotient = i / 3;
            column = i - quotient * 3;
            text[0] = i + 0x31;
            columnBase = 0xB0;
            textPos = column * 0xC + (base + columnBase);
            func_80021CD4(&text[2], *times);
            color = 0x7812;
            if (*(s16 *)((char *)selectedPtr - 0x22) == i) {
                color = 0x784C;
            }
            func_80016EA0(x, textPos, text, color);
            i++;
            times++;
        } while (i < count);
    }
}
