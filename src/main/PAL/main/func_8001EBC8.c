#include "common.h"
#include "psyq/gpu.h"

extern volatile u32 *D_8009AF2C[];
extern Rect D_8009AF38[];
extern volatile s32 D_8009AF34;
extern volatile s32 D_8009AF34_value asm("D_8009AF34");
extern volatile s32 D_8009AF48;
extern Rect D_8009AF4C;
extern volatile s16 D_8009AF4C_x asm("D_8009AF4C");
extern volatile s16 D_8009AF4E_y asm("D_8009AF4E");
extern s16 D_8009AF50;
extern s16 D_8009AF52;
extern volatile s32 D_8009AF54;
extern s32 D_8019CA00;

void func_8006402C(volatile u32 *arg0, s32 arg1);
void func_80065B24(Rect *rect, void *data);
void func_8006D1D0(void);

void func_8001EBC8(void) {
    Rect rect;
    register volatile s32 *bufferPtr asm("$6");
    register s32 oldBuffer asm("$16");
    register s32 state asm("$3");
    s32 bufferIndex;
    register s32 bufferAddr asm("$2");
    s32 pixelCount;
    register s32 next asm("$2");
    register s32 oldOffset asm("$2");
    s32 index;
    register u16 x asm("$2");
    register u16 step asm("$7");
    register s32 signedStep asm("$2");

    if (D_8019CA00 != 0) {
        func_8006D1D0();
        D_8019CA00 = 0;
    }

    rect = D_8009AF4C;

    bufferPtr = &D_8009AF34;
    oldBuffer = *bufferPtr;
    state = *bufferPtr;
    x = D_8009AF4C_x;
    step = D_8009AF50;
    *bufferPtr = state == 0;

    index = D_8009AF48;
    x += step;
    D_8009AF4C_x = x;

    if ((s16)x < (D_8009AF38[index].x + D_8009AF38[index].w)) {
        signedStep = (s16)step;
        pixelCount = signedStep * D_8009AF52;
        bufferIndex = D_8009AF34_value;
        bufferAddr = bufferIndex << 2;
        asm("addu %0,%1,%0" : "=r"(bufferAddr) : "r"(bufferPtr), "0"(bufferAddr));
        func_8006402C(*(volatile u32 **)(bufferAddr - 8), pixelCount / 2);
    } else {
        D_8009AF54 = 1;
        asm("" : : : "memory");
        next = index == 0;
        D_8009AF48 = next;
        asm("" : : : "memory");
        D_8009AF4C_x = D_8009AF38[next].x;
        asm("" : : : "memory");
        D_8009AF4E_y = D_8009AF38[next].y;
    }

    oldOffset = oldBuffer << 2;
    func_80065B24(&rect, (void *)*(volatile u32 **)((s32)D_8009AF2C + oldOffset));
}
