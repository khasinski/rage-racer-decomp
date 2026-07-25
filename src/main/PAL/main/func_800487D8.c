#include "common.h"

typedef struct {
    s16 time;
    s16 type;
    s32 arg0;
    s32 arg1;
} TimedDrawCommand;

extern s32 D_8019CB0C;

void func_80048078(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_80048210(s32 arg0, s32 arg1, s32 arg2);
void func_800483D4(s32 arg0, s32 arg1, s32 arg2);
void func_80048580(s32 arg0, s32 arg1, s32 arg2);

s32 func_800487D8(TimedDrawCommand *commands, s32 *progress, s32 step) {
    register TimedDrawCommand *base asm("$20") = commands;
    register s32 *progressPtr asm("$18") = progress;
    register s32 stepReg asm("$19") = step;
    register TimedDrawCommand *cmd asm("$16");
    register TimedDrawCommand *cmdTmp asm("$3");
    register s32 index = 0;
    s32 remaining;
    s32 type;
    register s32 nextProgress asm("$2");
    register s32 updatedProgress asm("$6");
    register s32 limit asm("$4");

    asm("" : "=r"(base), "=r"(progressPtr), "=r"(stepReg) : "0"(base), "1"(progressPtr), "2"(stepReg));
    if (stepReg < 0) {
        nextProgress = *progressPtr + stepReg;
        if (nextProgress > 0) {
            *progressPtr = nextProgress;
        } else {
            *progressPtr = 0;
        }
    }

    nextProgress = ((index << 1) + index) << 2;
    cmdTmp = (TimedDrawCommand *)(nextProgress + (s32)base);
    if (cmdTmp->time < 0) {
        goto after_loop;
    }
    cmd = cmdTmp;
loop_body:
    remaining = *progressPtr - cmd->time;
    if (remaining >= 0) {
        type = cmd->type;
        if ((u32)type < 40) {
            switch (type) {
            case 9:
                if (D_8019CB0C != 0) {
                    break;
                }
                func_80048078(remaining, cmd->arg0, cmd->arg1, type);
                cmd++;
                goto loop_continue;
            case 0:
            case 1:
                func_80048078(remaining, cmd->arg0, cmd->arg1, type);
                cmd++;
                goto loop_continue;
            case 19:
                if (D_8019CB0C != 0) {
                    break;
                }
                func_80048210(remaining, cmd->arg0, cmd->arg1);
                cmd++;
                goto loop_continue;
            case 10:
                func_80048210(remaining, cmd->arg0, cmd->arg1);
                cmd++;
                goto loop_continue;
            case 29:
                if (D_8019CB0C != 0) {
                    break;
                }
                func_800483D4(remaining, cmd->arg0, cmd->arg1);
                cmd++;
                goto loop_continue;
            case 20:
                func_800483D4(remaining, cmd->arg0, cmd->arg1);
                cmd++;
                goto loop_continue;
            case 39:
                if (D_8019CB0C != 0) {
                    break;
                }
                func_80048580(remaining, cmd->arg0, cmd->arg1);
                goto loop_next;
            case 30:
                func_80048580(remaining, cmd->arg0, cmd->arg1);
                goto loop_next;
            default:
                break;
            }
        }
    }
loop_next:
    cmd++;
loop_continue:
    index++;
    if (cmd->time >= 0) {
        goto loop_body;
    }

after_loop:
    if (stepReg >= 0) {
        cmdTmp = (TimedDrawCommand *)*progressPtr;
        updatedProgress = stepReg + (s32)cmdTmp;
        limit = base[index].arg1;
        if (updatedProgress < limit) {
            *progressPtr = updatedProgress;
        } else {
            *progressPtr = limit;
            return 1;
        }
    }

    return 0;
}
extern TimedDrawCommand D_80082520[];
extern s32 D_8009B250[];

void func_80046A2C();

void func_800489AC(s32 progress, s32 count, s32 slot) {
    register u8 *arg0Ptr asm("$8");
    register u8 *arg1Ptr asm("$9");
    register void *ot asm("$22");
    register s32 countReg asm("$21");
    register s32 i asm("$18");
    register TimedDrawCommand *cmd asm("$17");
    register s32 *timer asm("$16");
    register s32 xOffset asm("$20");
    register s32 yOffset asm("$19");
    register s32 nextTimer asm("$4");
    register s32 value asm("$2");
    register s32 temporary asm("$3");
    register void *basePtr asm("$3");
    register s32 offset asm("$2");
    register s32 done asm("$2");
    register s32 timerValue asm("$3");
    register s32 fade asm("$3");
    register s32 drawX asm("$5");
    register s32 drawY asm("$6");
    register s32 drawW asm("$7");
    register s32 drawH asm("$2");
    s32 elapsed;
    s32 limit;
    s32 packed;

    arg0Ptr = (u8 *)D_80082520[0].arg0;
    elapsed = progress - D_80082520[0].time;
    arg1Ptr = (u8 *)D_80082520[0].arg1;
    ot = *(void **)0x1F800004;
    countReg = count;
    packed = *(s32 *)(arg1Ptr + 0x10);
    i = 0;

    if (elapsed < 0) {
        return;
    }

    limit = *(s32 *)arg1Ptr;
    if (limit < elapsed) {
        elapsed = limit;
    }

    D_8009B250[slot] = 0x1FC;

    if (packed & 0x8000) {
        value = packed | 0xFFFF0000;
    } else {
        value = packed & 0x7FFF;
    }
    value = elapsed * value;
    xOffset = (u32)value >> 5;

    if (packed < 0) {
        value = packed >> 0x10;
        temporary = 0xFFFF0000;
        value |= temporary;
    } else {
        value = (packed >> 0x10) & 0x7FFF;
    }
    /* Match note: keep the $s5 save ahead of $s2 in the GCC 2.6.3 prologue. */
    countReg++;
    countReg--;
    value = elapsed * value;
    yOffset = (u32)value >> 5;

    if (countReg < i) {
        return;
    }

    offset = i << 1;
    basePtr = D_80082520;
    offset = (offset + i) << 2;
    cmd = (TimedDrawCommand *)((s32)basePtr + offset);

loop:
    basePtr = D_8009B250;
    offset = i << 2;
    timer = (s32 *)((s32)basePtr + offset);

    fade = *timer & 0x1FF;
    *timer = fade;
    fade >>= 2;

    drawH = *(s16 *)(arg0Ptr + 2);
    drawX = *(u16 *)(arg1Ptr + 4);
    drawY = *(u16 *)(arg1Ptr + 6);
    drawW = *(s16 *)arg0Ptr;
    drawX = drawX + xOffset;
    drawX <<= 0x10;
    drawY = drawY + yOffset;
    drawY = (u32)drawY << 16;
    drawX >>= 16;
    drawY >>= 16;

    func_80046A2C((u8 *)ot + 8,
                  drawX,
                  drawY,
                  drawW,
                  drawH,
                  arg0Ptr[4],
                  arg0Ptr[5],
                  fade,
                  fade,
                  fade,
                  *(u16 *)(arg1Ptr + 8),
                  0,
                  1,
                  arg0Ptr[7]);

    timerValue = *timer;
    nextTimer = 0;
    if (timerValue >= 60) {
        nextTimer = timerValue - 60;
    }
    cmd++;
    i++;
    done = countReg < i;
    *timer = nextTimer;
    arg0Ptr = (u8 *)cmd->arg0;
    arg1Ptr = (u8 *)cmd->arg1;
    if (!done) {
        goto loop;
    }
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
