#include "common.h"

extern u8 D_8007DF04[];

void func_80016754(s32 x, s32 y, void *str, s32 color);

void func_80033D50(s32 x, s32 y, s32 value, s32 color, s32 divisor) {
    register s32 savedX asm("$9");
    register s32 savedY asm("$10");
    register s32 savedColor asm("$11");
    register s32 localDivisor asm("$4");
    register s32 whole asm("$7");
    register s32 fraction asm("$5");
    register s32 minutes asm("$3");
    register s32 seconds asm("$7");
    register s32 secondTens asm("$4");
    register s32 fractionHundreds asm("$6");
    register s32 fractionTens asm("$3");
    s32 remainder;

    savedX = x;
    savedY = y;
    localDivisor = divisor;
    savedColor = color;
    if (value >= 0) {
        whole = value / localDivisor;
        remainder = value % localDivisor;

        minutes = whole / 60;
        fraction = (remainder * 1000) / localDivisor;
        seconds = whole % 60;
        D_8007DF04[0] = minutes + '0';

        secondTens = seconds / 10;
        D_8007DF04[2] = secondTens + '0';
        D_8007DF04[3] = (seconds - (secondTens * 10)) + '0';

        fractionHundreds = fraction / 100;
        D_8007DF04[5] = fractionHundreds + '0';

        fractionTens = fraction / 10;
        D_8007DF04[6] = (fractionTens - (fractionHundreds * 10)) + '0';
        D_8007DF04[7] = (fraction - (fractionTens * 10)) + '0';
    } else {
        D_8007DF04[0] = '-';
        D_8007DF04[2] = '-';
        D_8007DF04[3] = '-';
        D_8007DF04[5] = '-';
        D_8007DF04[6] = '-';
        D_8007DF04[7] = '-';
    }

    func_80016754(savedX, savedY, D_8007DF04, savedColor);
}

extern u8 D_8007DF10;
extern u8 D_8007DF11[];
extern u8 D_8007DF13;
extern u8 D_8007DF14;

void func_80033F30(s32 x, s32 y, s32 ticks, s32 color) {
    s32 savedY;
    s32 sec;
    s32 tmp;
    s32 min;
    u8 *p;
    s32 tens;
    s32 tens2;

    savedY = y;
    sec = ticks / 25;
    tmp = sec / 60;
    min = tmp;
    tmp = sec - min * 60;
    if (min < 10) {
        D_8007DF10 = ' ';
    } else {
        D_8007DF10 = min / 10 + '0';
    }
    tens = min / 10;
    p = D_8007DF11;
    tens2 = tmp / 10;
    *p = min - tens * 10 + '0';
    D_8007DF13 = tens2 + '0';
    D_8007DF14 = tmp - tens2 * 10 + '0';
    func_80016754(x, savedY, p - 1, color);
}

extern s32 D_801E4B34;

void func_80066604(void *packet, void *rect);
void AddPrim(void *ot, void *prim) asm("func_80064DDC");

void *func_80034058(void *ot, void *packet, s16 x, s16 y, s32 w, s32 h) {
    void *oldPacket;
    s16 rect[4];
    s32 offset;

    offset = ((D_801E4B34 << 4) - D_801E4B34) << 4;
    rect[0] = x;
    rect[1] = y + offset;
    rect[2] = w;
    rect[3] = h;
    func_80066604(packet, rect);
    oldPacket = packet;
    packet = (u8 *)packet + 12;
    AddPrim(ot, oldPacket);
    return packet;
}

extern u8 *D_8019C90C[2];
extern u8 D_801EB9DC[];

void func_800658FC(s32 mode);
void func_80064FF8(u8 *prim);

void func_800340D8(void) {
    register u8 **initBuffers asm("$16");
    register u8 **buffers asm("$18");
    register s32 row asm("$21");
    register s32 col asm("$19");
    register s32 linear asm("$16");
    register s32 offset asm("$17");
    register s32 y asm("$22");
    register s32 color asm("$23");
    register s32 xStep asm("$20");
    register s32 yStart;
    s32 bufferIndex;
    u8 *buffer;
    u8 *firstBuffer;
    register u8 *addPrimBase asm("$5");
    register s32 prevOffset asm("$4");
    register u8 *storeBaseV1 asm("$3");
    register u8 *storeBaseV0 asm("$2");

    initBuffers = D_8019C90C;
    firstBuffer = D_801EB9DC;
    initBuffers[0] = firstBuffer;
    D_8019C90C[1] = firstBuffer + 12000;
    func_800658FC(0);

    color = 0x20;
    buffers = initBuffers;
    bufferIndex = 0;
    do {
        row = 0;
        y = 0x5A;
        do {
            col = 0;
            yStart = y;
            xStep = 0;
            do {
                linear = (row << 5) + col;
                buffer = buffers[0];
                offset = linear << 4;
                func_80064FF8((u8 *)((s32)offset + (s32)buffer));
                storeBaseV1 = buffers[0];
                *(s16 *)((s32)offset + (s32)storeBaseV1 + 0xC) = 2;
                storeBaseV1 = buffers[0];
                *(s16 *)((s32)offset + (s32)storeBaseV1 + 0xE) = 1;
                storeBaseV1 = buffers[0];
                *(s16 *)((s32)offset + (s32)storeBaseV1 + 8) = 0xCD - xStep;
                storeBaseV0 = buffers[0];
                *(s16 *)((s32)offset + (s32)storeBaseV0 + 0xA) = yStart;
                buffers[0][offset + 4] = color;
                buffers[0][offset + 5] = color;
                buffers[0][offset + 6] = color;

                if (linear > 0) {
                    addPrimBase = buffers[0];
                    prevOffset = offset - 0x10;
                    AddPrim((void *)(prevOffset + (s32)addPrimBase), (void *)((s32)offset + (s32)addPrimBase));
                }

                col++;
                xStep += 3;
            } while (col < 0x20);

            row++;
            y += 2;
        } while (row < 0x10);

        bufferIndex = bufferIndex + 1;
        buffers++;
    } while (bufferIndex < 2);
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/race/func_80033D50", func_8003425C);

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/race/func_80033D50", func_8003479C);
