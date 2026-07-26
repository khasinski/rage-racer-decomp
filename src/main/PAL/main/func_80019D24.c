#include "common.h"
#include "psyq/gpu.h"
#include "game/state.h"

extern s32 D_801E4284;
extern s32 D_801E4288;
extern s32 D_801E6F10;

/*
 * Old-style definition on purpose: func_80019E84 and func_80019EFC call this
 * with no argument at all (the original relied on whatever was left in $4), so
 * the unit must not expose a prototype for it.
 */
s32 func_80019D24(arg0)
s32 arg0;
{
    s32 ret;
    s32 one;

    ret = 0;
    if (arg0 < D_801E4284) {
        goto low;
    }

    ret = 0x100;
    if (arg0 >= D_801E4288) {
        goto high;
    }

    one = 1;
    D_801E6F10 = one;
    goto done;

high:
    ret = 0;
low:
    D_801E6F10 = 0;

done:
    return ret;
}

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
        rectValue = page + 0x100;
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

extern s32 D_801E8AF8;
extern s32 D_8019CA6C;

void func_80019D7C(void);

void func_80019E84(void) {
    s32 temp;

    temp = func_80019D24();
    D_801E8AF8 = temp;
    D_8019CA6C = temp;
    func_80019D7C();
}

extern u8 D_801E4CF7;
extern s32 D_801E6F10;
extern s32 D_801E8AF8;
extern s32 D_8019CA6C;

void func_80019EBC(void) {
    s32 value = 1;
    s32 i = 0xFF;
    u8 *ptr = &D_801E4CF7;

    do {
        *ptr = value;
        i--;
        ptr--;
    } while (i >= 0);

    D_801E6F10 = 0;
    D_801E8AF8 = 0;
    D_8019CA6C = 0;
}

extern s32 D_801E8AF8;


void func_80019EFC(void) {
    D_801E8AF8 = func_80019D24();
}

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

extern s32 D_8019CA6C;
extern s32 D_801E8AF8;

void func_80019F24(void);
s32 VSync(s32 mode) asm("func_8006DD30");

void func_8001A030(void) {
    while (D_8019CA6C != D_801E8AF8) {
        if (VSync(1) >= 471) {
            break;
        }

        if (D_8019CA6C < D_801E8AF8) {
            func_80019F24();
            D_8019CA6C++;
        } else {
            D_8019CA6C--;
            func_80019F24();
        }
    }
}

extern s32 D_8019CA6C;
extern u8 D_801F18CC[];

s32 GameRandom15(void) asm("func_800632B0");

s32 func_8001A0E4(s32 mask, s32 current) {
    s32 random;
    s32 candidate;
    s32 offset;
    s32 first;

    if (mask & g_SceneTimer) {
        return current;
    }
    if ((D_8019CA6C == 0) || (D_8019CA6C == 0x100)) {
        random = GameRandom15() & 0x7FFF;
        candidate = random % 11;

        offset = (((((current * 3) * 4) + current) * 8) - current) * 4;
        first = func_80019D24(*(s16 *)&D_801F18CC[offset]);

        offset = (((((candidate * 3) * 4) + candidate) * 8) - candidate) * 4;
        if (first == func_80019D24(*(s16 *)&D_801F18CC[offset])) {
            return candidate;
        }
    }
    return current;
}

extern s32 D_8019CA6C;
extern u8 D_801F18CC[];

s32 GameRandom15(void) asm("func_800632B0");

s32 func_8001A1F0(s32 mask, s32 current) {
    s32 random;
    s32 candidate;
    s32 offset;
    s32 first;

    if (mask & g_SceneTimer) {
        return current;
    }
    if ((D_8019CA6C == 0) || (D_8019CA6C == 0x100)) {
        random = GameRandom15() & 0x7FFF;
        candidate = random % 4;

        offset = (((((current * 3) * 4) + current) * 8) - current) * 4;
        first = func_80019D24(*(s16 *)&D_801F18CC[offset]);

        offset = (((((candidate * 3) * 4) + candidate) * 8) - candidate) * 4;
        if (first == func_80019D24(*(s16 *)&D_801F18CC[offset])) {
            return candidate;
        }
    }
    return current;
}
