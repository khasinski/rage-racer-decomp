#include "common.h"
#include "psyq/gpu.h"

extern s16 D_8007C70A;
extern u8 D_801E4BF8[];
extern u8 *D_801E42D0;
extern s32 D_801E6F10;

void StoreImage(Rect *rect, void *data) asm("func_80065B88");
void LoadImage(Rect *rect, void *data) asm("func_80065B24");
void DrawSync(s32 mode) asm("func_800658FC");

void func_80019D7C(void) {
    s32 buffer[0xE0];
    register s32 page asm("$18") = 0;
    register s32 rectValue asm("$2");
    register s16 *rectY asm("$22") = &D_8007C70A;
    register Rect *rect asm("$21") = (Rect *)((s32)&D_8007C70A - 2);
    register u8 **basePtr asm("$20") = &D_801E42D0;
    register u8 *state asm("$17") = D_801E4BF8;
    register s32 offset asm("$16") = 0;
    register s32 value asm("$19");
    register s32 one asm("$4");
    register s32 *src asm("$5");
    register s32 *dst asm("$3");
    register u8 *base asm("$2");
    register s32 count asm("$4");

    do {
        asm volatile("addiu %0,%1,0x100" : "=r"(rectValue) : "r"(page));
        one = 1;
        *rectY = rectValue;
        value = one - *state;
        if (*state == D_801E6F10) {
            StoreImage(rect, buffer);
            DrawSync(0);
            LoadImage(rect, *basePtr + offset);
            DrawSync(0);

            src = buffer;
            base = *basePtr;
            dst = (s32 *)(offset + (s32)base);
            count = 0;
            do {
                *dst++ = *src++;
                count++;
            } while (count < 0xE0);

            *state = value;
        }
        state++;
        page++;
        offset += 0x380;
    } while (page < 0x100);
}

asm(".globl func_80019E5C\nfunc_80019E5C = func_80019D7C + 0xE0");
