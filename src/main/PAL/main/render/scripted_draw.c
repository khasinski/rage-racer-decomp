#include "common.h"
#include "game/menu.h"
#include "game/render.h"
#include "game/scratchpad.h"
#include "game/state.h"


void DrawScriptedSprite(s32 elapsed, u8 *shape, u8 *motion, s32 type) {
    register u8 *motionReg asm("$10") = motion;
    register u8 *shapeReg asm("$9");
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

    /* Match note: materialize motionReg in $t2 before the first load. */
    limit = *(s32 *)motionReg;
    otBase = SCRATCH_OT_BASE_AS(void);
    packed = *(s32 *)(motionReg + 0x10);
    shapeReg = shape;
    if (limit < elapsed) {
        elapsed = limit;
    }

    x = *(s16 *)(motionReg + 4);
    if (packed & 0x8000) {
        temp = packed | 0xFFFF0000;
    } else {
        temp = packed & 0x7FFF;
    }
    interp = (u32)(elapsed * temp) / 32;

    asm volatile("");
    y = *(s16 *)(motionReg + 6);
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
    interp = (u32)(elapsed * temp) / 32;
    y += interp;
    asm("" : "=r"(y) : "0"(y));

    switch (shapeReg[6] & 3) {
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

    flagByte = shapeReg[6];
    flags8 = flagByte & 8;
    flags4 = flagByte & 4;
    if (type != 0) {
        temp = shapeReg[7] & 0x7F;
        asm("" : "=r"(temp) : "0"(temp));
        alpha = (u8)temp;
    } else {
        alpha = 0x80;
    }

    DrawSprite(
        (u8 *)otBase + (mode * 4),
        (s16)x,
        (s16)y,
        *(s16 *)(shapeReg + 0),
        *(s16 *)(shapeReg + 2),
        shapeReg[4],
        shapeReg[5],
        motionReg[0xA],
        motionReg[0xB],
        motionReg[0xC],
        *(u16 *)(motionReg + 8),
        flags8,
        flags4,
        alpha);
}

void DrawScriptedLine(s32 elapsed, u8 *shape, u8 *motion) {
    register u8 *motionReg asm("$8") = motion;
    u8 *shapeReg;
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

    /* Match note: materialize motionReg in $t0 before the first load. */
    limit = *(s32 *)motionReg;
    otBase = SCRATCH_OT_BASE_AS(void);
    xPacked = *(s32 *)(motionReg + 0xC);
    yPacked = *(s32 *)(motionReg + 0x10);
    shapeReg = shape;
    if (limit < elapsed) {
        elapsed = limit;
    }

    y1 = *(s16 *)(motionReg + 4);
    if (xPacked & 0x8000) {
        temp = xPacked | 0xFFFF0000;
    } else {
        temp = xPacked & 0x7FFF;
    }
    interp = (u32)(elapsed * temp) / 32;
    x0 = y1 + interp;

    y1 = *(s16 *)(motionReg + 6);
    if (xPacked < 0) {
        s32 mask;

        y1Reg = xPacked >> 16;
        mask = 0xFFFF0000;
        temp = y1Reg | mask;
    } else {
        temp = (xPacked >> 16) & 0x7FFF;
    }
    interp = (u32)(elapsed * temp) / 32;
    y1 += interp;

    asm volatile("");
    x1Base = *(s16 *)(motionReg + 8);
    if (yPacked & 0x8000) {
        y0Call = y1;
        temp = yPacked | 0xFFFF0000;
    } else {
        y0Call = y1;
        temp = yPacked & 0x7FFF;
    }
    interp = (u32)(elapsed * temp) / 32;

    asm volatile("");
    y1 = *(s16 *)(motionReg + 0xA);
    x1 = x1Base + interp;
    if (yPacked < 0) {
        y1Reg = yPacked >> 16;
        x1Base = 0xFFFF0000;
        temp = y1Reg | x1Base;
    } else {
        temp = (yPacked >> 16) & 0x7FFF;
    }
    interp = (u32)(elapsed * temp) / 32;
    y1 += interp;
    asm("" : "=r"(y1) : "0"(y1));

    switch (shapeReg[3] & 3) {
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

    if (shapeReg[3] & 4) {
        alpha = shapeReg[3] & 0x60;
    } else {
        alpha = 0xFF;
    }

    y1Reg = (s16)y1;
    elapsed = mode * 4;
    x0 <<= 0x10;
    y1 = y0Call << 0x10;
    x1 <<= 0x10;
    otPtr = (s32)otBase + elapsed;
    asm("" : "=r"(otPtr), "=r"(x0) : "0"(otPtr), "1"(x0));
    DrawLine(
        (void *)otPtr,
        x0 >> 0x10,
        y1 >> 0x10,
        x1 >> 0x10,
        y1Reg,
        (u8)shapeReg[0],
        (u8)shapeReg[1],
        (u8)shapeReg[2],
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
    ot = SCRATCH_OT_BASE_AS(void);
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
    DrawFlatTriangle(
        (u8 *)ot + (mode * 4),
        (s16)x,
        (s16)y,
        (s16)limit,
        (s16)y0,
        (s16)packedSpeed,
        (s16)y1,
        style[8],
        style[9],
        style[10],
        semiTrans,
        flags);
}

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
    table = SCRATCH_OT_BASE_AS(u8);
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
    GameDrawTexturedQuad((s32)(table + index * 4), (s16)x, (s16)y,
                  (s16)(x + dx), (s16)y, (s16)x, (s16)(y + dy),
                  (s16)(x + dx), (s16)(y + dy), entry[0], entry[1], entry[2], entry[3],
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
                    remaining, (u8 *)cmd->shape, (u8 *)cmd->motion, type);
                break;
            case 0:
            case 1:
                DrawScriptedSprite(
                    remaining, (u8 *)cmd->shape, (u8 *)cmd->motion, type);
                break;
            case 19:
                if (g_MenuAltLayout != 0) {
                    break;
                }
                DrawScriptedLine(
                    remaining, (u8 *)cmd->shape, (u8 *)cmd->motion);
                break;
            case 10:
                DrawScriptedLine(
                    remaining, (u8 *)cmd->shape, (u8 *)cmd->motion);
                break;
            case 29:
                if (g_MenuAltLayout != 0) {
                    break;
                }
                DrawScriptedTriangle(
                    remaining, (u8 *)cmd->shape, (u8 *)cmd->motion);
                break;
            case 20:
                DrawScriptedTriangle(
                    remaining, (u8 *)cmd->shape, (u8 *)cmd->motion);
                break;
            case 39:
                if (g_MenuAltLayout != 0) {
                    break;
                }
                DrawScriptedQuad(
                    remaining, (u8 *)cmd->shape, (s32 *)cmd->motion);
                break;
            case 30:
                DrawScriptedQuad(
                    remaining, (u8 *)cmd->shape, (s32 *)cmd->motion);
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
        limit = base[index].motion;
        if (updatedProgress < limit) {
            *progressPtr = updatedProgress;
        } else {
            *progressPtr = limit;
            return 1;
        }
    }

    return 0;
}


void DrawFadingMenuSprites(s32 progress, s32 count, s32 slot) {
    u8 *shapePtr;
    register u8 *motionPtr asm("$9");
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

    shapePtr = (u8 *)g_MenuRowScript[0].shape;
    elapsed = progress - g_MenuRowScript[0].time;
    motionPtr = (u8 *)g_MenuRowScript[0].motion;
    ot = SCRATCH_OT_BASE_AS(void);
    countReg = count;
    packed = *(s32 *)(motionPtr + 0x10);
    i = 0;

    if (elapsed < 0) {
        return;
    }

    limit = *(s32 *)motionPtr;
    if (limit < elapsed) {
        elapsed = limit;
    }

    g_MenuRowFlashLevels[slot] = 0x1FC;

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
    basePtr = g_MenuRowScript;
    offset = (offset + i) << 2;
    cmd = (TimedDrawCommand *)((u8 *)basePtr + offset);

loop:
    basePtr = g_MenuRowFlashLevels;
    offset = i * 4;
    timer = (s32 *)((u8 *)basePtr + offset);

    fade = *timer & 0x1FF;
    *timer = fade;
    fade >>= 2;

    value = *(s16 *)(shapePtr + 2);
    drawX = *(u16 *)(motionPtr + 4);
    drawY = *(u16 *)(motionPtr + 6);
    drawW = *(s16 *)shapePtr;
    drawX = drawX + xOffset;
    drawX <<= 0x10;
    drawY = drawY + yOffset;
    drawY = (u32)drawY << 16;
    drawX >>= 16;
    drawY >>= 16;

    DrawSprite((u8 *)ot + 8,
                  drawX,
                  drawY,
                  drawW,
                  value,
                  shapePtr[4],
                  shapePtr[5],
                  fade,
                  fade,
                  fade,
                  *(u16 *)(motionPtr + 8),
                  0,
                  1,
                  shapePtr[7]);

    timerValue = *timer;
    nextTimer = 0;
    if (timerValue >= 60) {
        nextTimer = timerValue - 60;
    }
    cmd++;
    i++;
    done = countReg < i;
    *timer = nextTimer;
    shapePtr = (u8 *)cmd->shape;
    motionPtr = (u8 *)cmd->motion;
    if (!done) {
        goto loop;
    }
}


void GameDrawMenuButton(s32 x0, s32 y0, s32 x1, s32 y1,
                   u8 r, u8 g, u8 b,
                   s32 flags, s32 textX, s32 textY, s32 caption) {
    register s32 f asm("$16") = flags;
    register s32 p0 asm("$18") = x0;
    register void *ot = SCRATCH_OT_BASE_AS(void);
    register s32 p1 asm("$20") = y0;
    s32 p2 = x1;
    s32 p3 = y1;

    if (flags & 0x10) {
        if (flags % 2) {
            DrawLargeText((s16)(x0 + textX), (s16)(y0 + textY), (u8 *)caption,
                          0x7f, 0x7f, 0x7f, 0x244, (flags & 8) ? 0x20 : 0x40);
        } else {
            DrawSmallText((s16)(x0 + textX), (s16)(y0 + textY), (u8 *)caption,
                          0x7f, 0x7f, 0x7f, 0x244, (flags & 8) ? 0x20 : 0x40);
        }
    }
    DrawRectOutline(ot, (s16)p0, (s16)p1, (s16)p2, (s16)p3,
                    0xb4, 0xb4, 0xb4, (f & 4) ? (f & 0x60) : 0xff);
    DrawSolidRect(ot, (s16)p0, (s16)p1, (s16)p2, (s16)p3,
                  r, g, b, (f & 2) ? (f & 0x60) : 0xff);
    /* The second p3 use keeps it ahead of ot in global-alloc priority. */
    __asm__("" : : "r"(p0), "r"(p1), "r"(p2), "r"(p3), "r"(p3), "r"(f), "r"(ot));
}


void DrawMenuCursorBox(s32 x0, s32 y0, s32 x1, s32 y1, s32 useFlash) {
    void *ot;
    s32 savedX0;
    s32 savedY0;
    s32 savedX1;
    s32 savedY1;
    s32 color;
    s32 white;
    s32 counter;

    ot = SCRATCH_OT_BASE_AS(void);
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
        counter = g_MenuCursorPulsePhase;
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
    g_MenuCursorPulsePhase += 0x60;
}
