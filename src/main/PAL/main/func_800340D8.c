#include "common.h"

extern u8 *D_8019C90C[2];
extern u8 D_801EB9DC[];

void func_800658FC(s32 mode);
void func_80064FF8(u8 *prim);
void func_80064DDC(void *ot, void *prim);

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
                    func_80064DDC((void *)(prevOffset + (s32)addPrimBase), (void *)((s32)offset + (s32)addPrimBase));
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
