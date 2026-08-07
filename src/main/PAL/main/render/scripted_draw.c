#include "common.h"
#include "game/render.h"
#include "game/state.h"
#include "game/menu.h"

void GameDrawSpriteWide(
    void *ot,
    s32 x,
    s32 y,
    s32 arg3,
    s32 arg4,
    s32 arg5,
    s32 arg6,
    s32 arg7,
    s32 arg8,
    s32 arg9,
    s32 arg10,
    s32 arg11,
    s32 arg12,
    s32 arg13) asm("DrawSprite");

void DrawScriptedSprite(s32 arg0, u8 *arg1, u8 *arg2, s32 arg3) {
    register u8 *record asm("$10") = arg2;
    register u8 *style asm("$9");
    s32 flags8;
    void *otBase;
    s32 mode;
    s32 flags4;
    register s32 limit asm("$8");
    s32 packed;
    s32 x;
    s32 y;
    s32 temp;
    s32 interp;
    s32 flagByte;
    s32 alpha;

    /* Match note: materialize record in $t2 before the first load. */
    limit = *(s32 *)record;
    otBase = *(void **)0x1F800004;
    packed = *(s32 *)(record + 0x10);
    style = arg1;
    if (limit < arg0) {
        arg0 = limit;
    }

    x = *(s16 *)(record + 4);
    if (packed & 0x8000) {
        temp = packed | 0xFFFF0000;
    } else {
        temp = packed & 0x7FFF;
    }
    interp = (u32)(arg0 * temp) / 32;

    asm volatile("");
    y = *(s16 *)(record + 6);
    x += interp;
    if (packed < 0) {
        s32 hi;
        register s32 mask asm("$3");

        hi = packed >> 16;
        mask = 0xFFFF0000;
        temp = hi | mask;
    } else {
        temp = (packed >> 16) & 0x7FFF;
    }
    interp = (u32)(arg0 * temp) / 32;
    y += interp;
    asm("" : "=r"(y) : "0"(y));

    switch (style[6] & 3) {
    case 0:
        mode = 0;
        break;
    case 1:
        mode = 3;
        break;
    case 2:
        mode = 5;
        break;
    case 3:
        mode = 0x2BE;
        break;
    }

    flagByte = style[6];
    flags8 = flagByte & 8;
    flags4 = flagByte & 4;
    if (arg3 != 0) {
        temp = style[7] & 0x7F;
        asm("" : "=r"(temp) : "0"(temp));
        alpha = (u8)temp;
    } else {
        alpha = 0x80;
    }

    GameDrawSpriteWide(
        (u8 *)otBase + (mode * 4),
        (s16)x,
        (s16)y,
        *(s16 *)(style + 0),
        *(s16 *)(style + 2),
        style[4],
        style[5],
        record[0xA],
        record[0xB],
        record[0xC],
        *(u16 *)(record + 8),
        flags8,
        flags4,
        alpha);
}

void GameDrawLineWide(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, u8 r, u8 g, u8 b, u8 alpha) asm("DrawLine");

void DrawScriptedLine(s32 arg0, u8 *arg1, u8 *arg2) {
    register u8 *record asm("$8") = arg2;
    u8 *style;
    void *otBase;
    s32 mode;
    register s32 y1Reg asm("$2");
    s32 x0;
    s32 y0Call;
    s32 x1Base;
    s32 x1;
    s32 y1;
    s32 otPtr;
    s32 limit;
    s32 xPacked;
    s32 yPacked;
    s32 temp;
    s32 interp;
    s32 alpha;

    /* Match note: materialize record in $t0 before the first load. */
    limit = *(s32 *)record;
    otBase = *(void **)0x1F800004;
    xPacked = *(s32 *)(record + 0xC);
    yPacked = *(s32 *)(record + 0x10);
    style = arg1;
    if (limit < arg0) {
        arg0 = limit;
    }

    y1 = *(s16 *)(record + 4);
    if (xPacked & 0x8000) {
        temp = xPacked | 0xFFFF0000;
    } else {
        temp = xPacked & 0x7FFF;
    }
    interp = (u32)(arg0 * temp) / 32;
    x0 = y1 + interp;

    y1 = *(s16 *)(record + 6);
    if (xPacked < 0) {
        s32 mask;

        y1Reg = xPacked >> 16;
        mask = 0xFFFF0000;
        temp = y1Reg | mask;
    } else {
        temp = (xPacked >> 16) & 0x7FFF;
    }
    interp = (u32)(arg0 * temp) / 32;
    y1 += interp;

    asm volatile("");
    x1Base = *(s16 *)(record + 8);
    if (yPacked & 0x8000) {
        y0Call = y1;
        temp = yPacked | 0xFFFF0000;
    } else {
        y0Call = y1;
        temp = yPacked & 0x7FFF;
    }
    interp = (u32)(arg0 * temp) / 32;

    asm volatile("");
    y1 = *(s16 *)(record + 0xA);
    x1 = x1Base + interp;
    if (yPacked < 0) {
        y1Reg = yPacked >> 16;
        x1Base = 0xFFFF0000;
        temp = y1Reg | x1Base;
    } else {
        temp = (yPacked >> 16) & 0x7FFF;
    }
    interp = (u32)(arg0 * temp) / 32;
    y1 += interp;
    asm("" : "=r"(y1) : "0"(y1));

    switch (style[3] & 3) {
    case 0:
        mode = 0;
        break;
    case 1:
        mode = 3;
        break;
    case 2:
        mode = 5;
        break;
    case 3:
        mode = 0x2BE;
        break;
    }

    if (style[3] & 4) {
        alpha = style[3] & 0x60;
    } else {
        alpha = 0xFF;
    }

    y1Reg = (s16)y1;
    arg0 = mode * 4;
    x0 <<= 0x10;
    y1 = y0Call << 0x10;
    x1 <<= 0x10;
    otPtr = (s32)otBase + arg0;
    asm("" : "=r"(otPtr), "=r"(x0) : "0"(otPtr), "1"(x0));
    GameDrawLineWide(
        (void *)otPtr,
        x0 >> 0x10,
        y1 >> 0x10,
        x1 >> 0x10,
        y1Reg,
        style[0],
        style[1],
        style[2],
        alpha);
}

void DrawScriptedTriangle(s32 time, u8 *styleArg, u8 *recordArg) {
    u8 *style;
    u8 *record;
    void *ot;
    s32 limit;
    register s32 packedSpeed asm("$3");
    s32 product;
    s32 x;
    s32 y0;
    s32 y;
    s32 y1;
    s32 mode;
    s32 semiTrans;
    s32 flags;
    s32 productResult;

    style = styleArg;
    record = recordArg;
    /* The barrier is load-bearing: without it the scheduler sinks the
     * scratchpad load past the second record load. */
    limit = *(s32 *)record;
    ot = *(void **)0x1F800004;
    asm volatile("");
    packedSpeed = *(s32 *)(record + 8);
    if (limit < time) {
        time = limit;
    }

    limit = *(s16 *)(record + 4);
    if (packedSpeed & 0x8000) {
        product = packedSpeed | 0xFFFF0000;
    } else {
        product = packedSpeed & 0x7FFF;
    }
    productResult = time * product;
    product = productResult;
    product = (u32)product / 32;
    productResult = limit + product;
    product = productResult;
    asm("" : "=r"(product), "=r"(record) : "0"(product), "1"(record));

    y = *(s16 *)(record + 6);
    x = product;
    if (packedSpeed < 0) {
        product = packedSpeed >> 16;
        packedSpeed = 0xFFFF0000;
        product |= packedSpeed;
    } else {
        product = packedSpeed >> 16;
        product &= 0x7FFF;
    }
    productResult = time * product;
    product = productResult;
    asm("" : "=r"(product), "=r"(style) : "0"(product), "1"(style));
    product = (u32)product / 32;
    y += product;

    product = *(u16 *)(style + 2);
    packedSpeed = *(u16 *)(style + 6);
    y0 = product + y;
    y1 = packedSpeed + y;
    product = *(u16 *)style;
    packedSpeed = *(u16 *)(style + 4);
    productResult = x + product;
    product = productResult;
    asm("" : "=r"(product) : "0"(product));
    limit = product;
    packedSpeed = x + packedSpeed;

    switch (style[0xB] & 3) {
    case 0:
        mode = 0;
        break;
    case 1:
        mode = 3;
        break;
    case 2:
        mode = 5;
        break;
    case 3:
        mode = 0x2BE;
        break;
    }

    {
        s32 alpha;

        alpha = style[0xB];
        semiTrans = alpha & 4;
        if (semiTrans != 0) {
            alpha &= 0x60;
            asm("" : "=r"(alpha) : "0"(alpha));
            flags = (u8)alpha;
        } else {
            flags = 0x80;
        }
    }

    asm("" : : "r"(packedSpeed));
    DrawFlatTriangleSigned(
        (u8 *)ot + (mode * 4),
        x,
        y,
        limit,
        y0,
        packedSpeed,
        y1,
        style[8],
        style[9],
        style[10],
        semiTrans,
        flags);
}

void GameDrawTexturedQuadWide(u8 *arg0, s16 x0, s16 y0, s16 x1a, s16 y0b, s16 x0b,
                   s16 y1a, s16 x1b, s16 y1b, s32 d0, s32 d1, s32 d2,
                   s32 d3, s32 d4, s32 d5, s32 d6, s32 d7, s32 dA,
                   s32 dB, s32 dC, s32 h8, s32 f8, s32 f4, s32 dE) asm("GameDrawTexturedQuad");

void DrawScriptedQuad(s32 time, u8 *desc, s32 *ctx) {
    s32 duration;
    u8 *table;
    u8 *entry;
    s32 velocity0;
    s32 velocity1;
    s32 x;
    s32 y;
    s32 dx;
    s32 dy;
    register s32 index asm("$10");
    s32 posX;
    s32 posY;
    s32 posX2;
    register s32 posY2 asm("$6");
    u32 velocityX;
    u32 velocityY;
    u32 velocityX2;
    u32 velocityY2;
    register s32 value asm("$2");
    s32 flags;

    duration = ctx[0];
    table = *(u8 **)0x1F800004;
    velocity0 = ctx[3];
    velocity1 = ctx[4];
    entry = desc;
    if (duration < time) {
        time = duration;
    }

    posX = *(s16 *)((u8 *)ctx + 4);
    if (velocity0 & 0x8000) {
        velocityX = velocity0 | 0xFFFF0000;
    } else {
        velocityX = velocity0 & 0x7FFF;
    }
    posX += (time * velocityX) >> 5;
    asm("" : "=r"(posX) : "0"(posX));
    x = posX;

    posY = *(s16 *)((u8 *)ctx + 6);
    if (velocity0 < 0) {
        value = velocity0 >> 16;
        velocityY = value | 0xFFFF0000;
    } else {
        value = velocity0 / 65536;
        velocityY = value & 0x7FFF;
    }
    value = posY + ((time * velocityY) >> 5);
    y = value;
    asm("" : "=r"(value), "=r"(y) : "0"(value), "1"(y) : "memory");

    posX2 = *(s16 *)((u8 *)ctx + 8);
    if (velocity1 & 0x8000) {
        velocityX2 = velocity1 | 0xFFFF0000;
    } else {
        velocityX2 = velocity1 & 0x7FFF;
    }
    value = posX2 + ((time * velocityX2) >> 5);
    asm("" : "=r"(value) : "0"(value) : "memory");
    dx = value;

    posY2 = *(s16 *)((u8 *)ctx + 0xA);
    if (velocity1 < 0) {
        value = velocity1 >> 16;
        velocityY2 = value | 0xFFFF0000;
    } else {
        value = velocity1 / 65536;
        velocityY2 = value & 0x7FFF;
    }
    posY2 += (time * velocityY2) >> 5;
    asm("" : "=r"(posY2) : "0"(posY2));
    dy = posY2;

    switch (entry[0xD] & 3) {
    case 0:
        asm(".globl func_800486C0\nfunc_800486C0 = . + 4");
        index = 0;
        break;
    case 1:
        index = 3;
        break;
    case 2:
        index = 5;
        break;
    case 3:
        index = 0x2BE;
        break;
    }

    flags = entry[0xD];
    GameDrawTexturedQuadWide(table + index * 4, x, y, x + dx, y, x, y + dy,
                  x + dx, y + dy, entry[0], entry[1], entry[2], entry[3],
                  entry[4], entry[5], entry[6], entry[7], entry[0xA],
                  entry[0xB], entry[0xC], *(u16 *)(entry + 8), flags & 8,
                  flags & 4, entry[0xE]);
}

s32 RunTimedDrawScript(void *commands, s32 *progress, s32 step) {
    TimedDrawCommand *base = commands;
    register s32 *progressPtr asm("$18") = progress;
    register s32 stepReg asm("$19") = step;
    TimedDrawCommand *cmd;
    TimedDrawCommand *cmdTmp;
    s32 index = 0;
    s32 remaining;
    s32 type;
    s32 nextProgress;
    register s32 updatedProgress asm("$6");
    s32 limit;

    asm("" : "=r"(base), "=r"(progressPtr), "=r"(stepReg) : "0"(base), "1"(progressPtr), "2"(stepReg));
    if (stepReg < 0) {
        nextProgress = *progressPtr + stepReg;
        if (nextProgress > 0) {
            *progressPtr = nextProgress;
        } else {
            *progressPtr = 0;
        }
    }

    nextProgress = (index * 3) << 2;
    cmdTmp = (TimedDrawCommand *)(nextProgress + (s32)base);
    switch (0) { default:
    if (cmdTmp->time < 0) {
        break;
    }
    cmd = cmdTmp;
loop_body:
    remaining = *progressPtr - cmd->time;
    if (remaining >= 0) {
        type = cmd->type;
        if ((u32)type < 40) {
            switch (type) {
            case 9:
                if (g_MenuAltLayout != 0) {
                    break;
                }
                DrawScriptedSprite(
                    remaining, (u8 *)cmd->arg0, (u8 *)cmd->arg1, type);
                break;
            case 0:
            case 1:
                DrawScriptedSprite(
                    remaining, (u8 *)cmd->arg0, (u8 *)cmd->arg1, type);
                break;
            case 19:
                if (g_MenuAltLayout != 0) {
                    break;
                }
                DrawScriptedLine(
                    remaining, (u8 *)cmd->arg0, (u8 *)cmd->arg1);
                break;
            case 10:
                DrawScriptedLine(
                    remaining, (u8 *)cmd->arg0, (u8 *)cmd->arg1);
                break;
            case 29:
                if (g_MenuAltLayout != 0) {
                    break;
                }
                DrawScriptedTriangle(
                    remaining, (u8 *)cmd->arg0, (u8 *)cmd->arg1);
                break;
            case 20:
                DrawScriptedTriangle(
                    remaining, (u8 *)cmd->arg0, (u8 *)cmd->arg1);
                break;
            case 39:
                if (g_MenuAltLayout != 0) {
                    break;
                }
                DrawScriptedQuad(
                    remaining, (u8 *)cmd->arg0, (s32 *)cmd->arg1);
                break;
            case 30:
                DrawScriptedQuad(
                    remaining, (u8 *)cmd->arg0, (s32 *)cmd->arg1);
                break;
            default:
                break;
            }
        }
    }
    cmd++;
    index++;
    if (cmd->time >= 0) {
        goto loop_body;
    }

}
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

void GameDrawSpriteWide();

void DrawFadingMenuSprites(s32 progress, s32 count, s32 slot) {
    u8 *arg0Ptr;
    register u8 *arg1Ptr asm("$9");
    void *ot;
    register s32 countReg asm("$21");
    register s32 i asm("$18");
    TimedDrawCommand *cmd;
    s32 *timer;
    s32 xOffset;
    s32 yOffset;
    s32 nextTimer;
    register s32 value asm("$2");
    s32 temporary;
    void *basePtr;
    s32 offset;
    s32 done;
    s32 timerValue;
    u32 fade;
    s32 drawX;
    s32 drawY;
    s32 drawW;
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
    xOffset = (u32)value / 32;

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
    yOffset = (u32)value / 32;

    if (countReg < i) {
        return;
    }

    offset = i * 2;
    basePtr = D_80082520;
    offset = (offset + i) << 2;
    cmd = (TimedDrawCommand *)((u8 *)basePtr + offset);

loop:
    basePtr = D_8009B250;
    offset = i * 4;
    timer = (s32 *)((u8 *)basePtr + offset);

    fade = *timer & 0x1FF;
    *timer = fade;
    fade >>= 2;

    value = *(s16 *)(arg0Ptr + 2);
    drawX = *(u16 *)(arg1Ptr + 4);
    drawY = *(u16 *)(arg1Ptr + 6);
    drawW = *(s16 *)arg0Ptr;
    drawX = drawX + xOffset;
    drawX <<= 0x10;
    drawY = drawY + yOffset;
    drawY = (u32)drawY << 16;
    drawX >>= 16;
    drawY >>= 16;

    GameDrawSpriteWide((u8 *)ot + 8,
                  drawX,
                  drawY,
                  drawW,
                  value,
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

void DrawLargeTextWide(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7) asm("DrawLargeText");
void drawSmallText(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7) asm("DrawSmallText");
void GameDrawSolidRectWide(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, s32 a5, s32 a6, s32 a7, s32 a8) asm("DrawSolidRect");

void GameDrawMenuButton(s32 arg0, s32 arg1, s32 arg2, s32 arg3,
                   u8 arg4, u8 arg5, u8 arg6,
                   s32 flags, s32 arg8, s32 arg9, s32 arg10);
void GameDrawMenuButton(s32 arg0, s32 arg1, s32 arg2, s32 arg3,
                   u8 arg4, u8 arg5, u8 arg6,
                   s32 flags, s32 arg8, s32 arg9, s32 arg10) {
    register s32 f asm("$16") = flags;
    register s32 p0 asm("$18") = arg0;
    register void *ot = *(void **)0x1F800004;
    register s32 p1 asm("$20") = arg1;
    s32 p2 = arg2;
    s32 p3 = arg3;

    if (flags & 0x10) {
        if (flags % 2) {
            DrawLargeTextWide((s16)(arg0 + arg8), (s16)(arg1 + arg9), arg10,
                          0x7f, 0x7f, 0x7f, 0x244, (flags & 8) ? 0x20 : 0x40);
        } else {
            drawSmallText((s16)(arg0 + arg8), (s16)(arg1 + arg9), arg10,
                          0x7f, 0x7f, 0x7f, 0x244, (flags & 8) ? 0x20 : 0x40);
        }
    }
    DrawRectOutline(ot, (s16)p0, (s16)p1, (s16)p2, (s16)p3,
                    0xb4, 0xb4, 0xb4, (f & 4) ? (f & 0x60) : 0xff);
    GameDrawSolidRectWide(ot, (s16)p0, (s16)p1, (s16)p2, (s16)p3,
                  arg4, arg5, arg6, (f & 2) ? (f & 0x60) : 0xff);
    /* The second p3 use keeps it ahead of ot in global-alloc priority. */
    __asm__("" : : "r"(p0), "r"(p1), "r"(p2), "r"(p3), "r"(p3), "r"(f), "r"(ot));
}

s32 rsin(s32 arg0);

void DrawMenuCursorBox(s32 x0, s32 y0, s32 x1, s32 y1, s32 useFlash) {
    void *ot;
    s32 savedX0;
    s32 savedY0;
    s32 savedX1;
    s32 savedY1;
    s32 color;
    s32 white;
    s32 counter;

    ot = *(void **)0x1F800004;
    savedX0 = x0;
    savedY0 = y0;
    savedX1 = x1;
    savedY1 = y1;
    if (useFlash != 0) {
        if (g_AnimTimer & 2) {
            color = 0xFF;
        } else {
            color = 0x60;
        }
    } else {
        counter = D_8009B264;
        counter = rsin(counter % 4096);
        color = (counter / 64) - 0x41;
    }

    white = 0xFF;
    DrawRectOutline(
        ot,
        (s16)(savedX0 - 1),
        (s16)(savedY0 - 2),
        (s16)(savedX1 + 2),
        (s16)(savedY1 + 4),
        0,
        (u8)color,
        0,
        white);
    DrawRectOutline(
        ot, (s16)savedX0, (s16)savedY0, (s16)savedX1, (s16)(savedY1 + 0), 0, (u8)color, 0, white);
    D_8009B264 += 0x60;
}
