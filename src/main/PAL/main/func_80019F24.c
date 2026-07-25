#include "common.h"
#include "psyq/gpu.h"

extern s16 D_8007C70A;
extern s32 D_8019CA6C;
extern u8 D_801E4BF8[];
extern s32 D_801E6F10;
extern u8 *D_801E42D0;

void StoreImage(Rect *rect, void *data) asm("func_80065B88");
void LoadImage(Rect *rect, void *data) asm("func_80065B24");
void DrawSync(s32 mode) asm("func_800658FC");

void func_80019F24(void) {
    s32 buffer[0xE0];
    register s16 *rectY asm("$16");
    register s32 value asm("$17");
    register s32 one asm("$4");
    register s32 *dst asm("$3");
    register s32 *src asm("$5");
    register s32 count asm("$4");
    register u8 **basePtr asm("$16");
    register Rect *rect asm("$4");
    register s32 copyIndex asm("$3");
    register s32 copyOffset asm("$2");
    s32 index;

    rectY = &D_8007C70A;
    *rectY = (u16)D_8019CA6C + 0x100;
    one = 1;
    value = one - D_801E4BF8[D_8019CA6C];
    if (D_801E4BF8[D_8019CA6C] == D_801E6F10) {
        rectY = (s16 *)((u8 *)rectY - 2);
        rect = (Rect *)rectY;
        StoreImage(rect, buffer);
        DrawSync(0);

        index = D_8019CA6C;
        rect = (Rect *)rectY;
        basePtr = &D_801E42D0;
        LoadImage(rect, *basePtr + ((((index * 8) - index) << 7)));
        DrawSync(0);

        src = buffer;
        copyIndex = D_8019CA6C;
        count = 0;
        copyOffset = (copyIndex * 8) - copyIndex;
        dst = (s32 *)(*basePtr + (copyOffset << 7));
        do {
            *dst++ = *src++;
            count++;
        } while (count < 0xE0);

        D_801E4BF8[D_8019CA6C] = value;
    }
}
