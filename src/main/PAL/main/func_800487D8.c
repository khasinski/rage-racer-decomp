#include "common.h"

extern s32 D_8019CB0C;

typedef struct DrawCommand {
    s16 time;
    s16 kind;
    void *arg1;
    s32 arg2;
} DrawCommand;

void func_80048078(s32 arg0, void *arg1, void *arg2, s32 arg3);
void func_80048210(s32 arg0, void *arg1, void *arg2);
void func_800483D4(s32 arg0, void *arg1, void *arg2);
void func_80048580(s32 arg0, void *arg1, void *arg2);

s32 func_800487D8(DrawCommand *commands, s32 *cursor, s32 delta) {
    register DrawCommand *base asm("$20") = commands;
    register s32 *pos asm("$18") = cursor;
    register s32 step_delta asm("$19") = delta;
    register s32 index asm("$17");
    register DrawCommand *cmd asm("$16");
    register DrawCommand *entry asm("$3");
    s32 next;

    index = 0;

    if (step_delta < 0) {
        next = *pos + step_delta;
        if (next > 0) {
            *pos = next;
        } else {
            *pos = 0;
        }
    }

    entry = &base[index];
    if (entry->time >= 0) {
        cmd = entry;
        do {
            register s32 step asm("$4");

            step = *pos - cmd->time;
            if (step >= 0) {
                register s32 kind asm("$7") = cmd->kind;

                if ((u32)kind < 0x28) {
                    switch (kind) {
                    case 0:
                    case 1:
                        func_80048078(step, cmd->arg1, (void *)cmd->arg2, kind);
                        break;
                    case 9:
                        if (D_8019CB0C == 0) {
                            func_80048078(step, cmd->arg1, (void *)cmd->arg2, kind);
                        }
                        break;
                    case 10:
                        func_80048210(step, cmd->arg1, (void *)cmd->arg2);
                        break;
                    case 19:
                        if (D_8019CB0C == 0) {
                            func_80048210(step, cmd->arg1, (void *)cmd->arg2);
                        }
                        break;
                    case 20:
                        func_800483D4(step, cmd->arg1, (void *)cmd->arg2);
                        break;
                    case 29:
                        if (D_8019CB0C == 0) {
                            func_800483D4(step, cmd->arg1, (void *)cmd->arg2);
                        }
                        break;
                    case 30:
                        func_80048580(step, cmd->arg1, (void *)cmd->arg2);
                        break;
                    case 39:
                        if (D_8019CB0C == 0) {
                            func_80048580(step, cmd->arg1, (void *)cmd->arg2);
                        }
                        break;
                    }
                }
            }

            cmd++;
            index++;
        } while (cmd->time >= 0);
    }

    if (step_delta >= 0) {
        register DrawCommand *tail asm("$2");
        register s32 old_pos asm("$3");
        register s32 limit asm("$4");
        register s32 next_tail asm("$6");

        tail = &base[index];
        old_pos = *pos;
        limit = tail->arg2;
        next_tail = step_delta + old_pos;
        if (next_tail < limit) {
            *pos = next_tail;
        } else {
            *pos = limit;
            return 1;
        }
    }

    return 0;
}

extern s16 D_80082520;
extern u8 *D_80082524;
extern u8 *D_80082528;
extern s32 D_8009B250[];

void func_80046A2C(void *ot, s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 r, s32 g, s32 b, s32 tex, s32 semi,
                   s32 one, s32 code);

void func_800489AC(s32 arg0, s32 arg1, s32 arg2) {
    register u8 *draw asm("t0") = D_80082524;
    register s32 progress asm("a0") = arg0 - D_80082520;
    register u8 *anim asm("t1") = D_80082528;
    register u8 *scratch asm("s6") = *(u8 **)0x1F800004;
    register s32 last asm("s5") = arg1;
    register s32 i asm("s2");
    register s32 packed asm("a1");
    s32 xoff;
    s32 yoff;
    register u8 *entry asm("s1");
    register s32 *fadep asm("s0");
    s32 scale;

    packed = *(s32 *)(anim + 0x10);
    i = 0;
    if (progress < 0) {
        return;
    }

    if (*(s32 *)anim < progress) {
        progress = *(s32 *)anim;
    }

    D_8009B250[arg2] = 0x1FC;

    if (packed & 0x8000) {
        scale = packed | 0xFFFF0000;
    } else {
        scale = packed & 0x7FFF;
    }
    xoff = (u32)(progress * scale) >> 5;

    if (packed < 0) {
        scale = (packed >> 16) | 0xFFFF0000;
    } else {
        scale = (packed >> 16) & 0x7FFF;
    }
    yoff = (u32)(progress * scale) >> 5;
    asm volatile("" : "=r"(yoff) : "0"(yoff));

    if (last < i) {
        return;
    }

    {
        register u8 *entryBase asm("v1") = (u8 *)&D_80082520;
        entry = entryBase + (i * 0xC);
    }

    do {
        register s32 fade asm("v1");
        register s32 *fadeBase asm("v1") = D_8009B250;

        fadep = &fadeBase[i];
        fade = *fadep & 0x1FF;
        *fadep = fade;
        fade = (u32)fade >> 2;

        func_80046A2C(scratch + 8, (s16)(*(u16 *)(anim + 4) + xoff), (s16)(*(u16 *)(anim + 6) + yoff),
                      *(s16 *)(draw + 0), *(s16 *)(draw + 2), draw[4], draw[5], fade, fade, fade,
                      *(u16 *)(anim + 8), 0, 1, draw[7]);

        fade = *fadep;
        {
            register s32 nextFade asm("a0") = 0;
            if (fade >= 0x3C) {
                nextFade = fade - 0x3C;
            }
            fade = nextFade;
        }

        entry += 0xC;
        i++;
        *fadep = fade;
        draw = *(u8 **)(entry + 4);
        anim = *(u8 **)(entry + 8);
    } while (i <= last);
}

void func_80047958(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7);
void func_80047634(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7);
void func_80047024(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, s32 a5, s32 a6, s32 a7, s32 a8);
void func_80047460(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, s32 a5, s32 a6, s32 a7, s32 a8);

void func_80048B88(s32 arg0, s32 arg1, s32 arg2, s32 arg3,
                   u8 arg4, u8 arg5, u8 arg6,
                   s32 flags, s32 arg8, s32 arg9, s32 arg10) {
    register s32 f asm("$16") = flags;
    register s32 p0 asm("$18") = arg0;
    register void *ot asm("$19") = *(void **)0x1F800004;
    register s32 p1 asm("$20") = arg1;
    register s32 p2 asm("$21") = arg2;
    register s32 p3 asm("$17") = arg3;

    if (flags & 0x10) {
        if (flags & 1) {
            func_80047958((s16)(arg0 + arg8), (s16)(arg1 + arg9), arg10,
                          0x7f, 0x7f, 0x7f, 0x244, (flags & 8) ? 0x20 : 0x40);
        } else {
            func_80047634((s16)(arg0 + arg8), (s16)(arg1 + arg9), arg10,
                          0x7f, 0x7f, 0x7f, 0x244, (flags & 8) ? 0x20 : 0x40);
        }
    }
    func_80047460(ot, (s16)p0, (s16)p1, (s16)p2, (s16)p3,
                  0xb4, 0xb4, 0xb4, (f & 4) ? (f & 0x60) : 0xff);
    func_80047024(ot, (s16)p0, (s16)p1, (s16)p2, (s16)p3,
                  arg4, arg5, arg6, (f & 2) ? (f & 0x60) : 0xff);
    __asm__("" : : "r"(p0), "r"(p1), "r"(p2), "r"(p3), "r"(f));
}

extern s32 D_8009B264;
extern s32 D_8009E694;

s32 func_80068568(s32 arg0);
void func_80047460(
    void *ot, s32 x0, s32 y0, s32 x1, s32 y1, s32 arg5, s32 color, s32 arg7, s32 arg8);

void func_80048D64(s32 x0, s32 y0, s32 x1, s32 y1, s32 useFlash) {
    void *ot;
    s32 savedX0;
    s32 savedY0;
    s32 savedX1;
    s32 savedY1;
    s32 color;
    s32 white;
    register s32 counter asm("$2");
    register s32 phaseBase asm("$4");

    ot = *(void **)0x1F800004;
    savedX0 = x0;
    savedY0 = y0;
    savedX1 = x1;
    savedY1 = y1;
    if (useFlash != 0) {
        if (D_8009E694 & 2) {
            color = 0xFF;
        } else {
            color = 0x60;
        }
    } else {
        counter = D_8009B264;
        phaseBase = counter;
        if (counter < 0) {
            phaseBase = counter + 0xFFF;
        }
        phaseBase = (phaseBase >> 12) << 12;
        counter = func_80068568(counter - phaseBase);
        if (counter < 0) {
            counter += 0x3F;
        }
        color = (counter >> 6) - 0x41;
    }

    white = 0xFF;
    func_80047460(
        ot,
        (s16)(savedX0 - 1),
        (s16)(savedY0 - 2),
        (s16)(savedX1 + 2),
        (s16)(savedY1 + 4),
        0,
        color & 0xFF,
        0,
        white);
    func_80047460(
        ot, (s16)savedX0, (s16)savedY0, (s16)savedX1, (s16)(savedY1 + 0), 0, color & 0xFF, 0, white);
    D_8009B264 += 0x60;
}
