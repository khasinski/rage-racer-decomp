#include "common.h"
#include "game/asset.h"
#include "game/car.h"
#include "game/state.h"

extern u32 D_8009E87C;
extern GameCarEntry *D_8019C7C8;
extern s32 D_801E40D4;
extern u8 *D_801E4090;
s32 func_80017848(s32 arg0, s32 arg1);
s32 func_80017C78(s32 arg0, void *arg1);
void func_80017B94(void *arg0, s32 arg1);
void func_80017948(void *arg0, s32 arg1);
void func_80017B44(void *arg0, s32 arg1);
void func_8001D748(s32 arg0, s32 arg1);
void func_8001D900(s32 arg0, s32 arg1);
void func_80042C94(void);
extern s32 D_8019C904;
extern s32 D_801E4B30;
void func_80017A10(s32);
extern s32 D_8009E6A4;
extern s16 D_8019CABC;
extern s32 D_801E428C;
extern s16 D_801E4DAC;
extern s32 D_801E772C[];
s32 func_800632B0(void);
void func_80017BE4(void);
extern s32 D_8019C754;
extern s32 D_801E4D70;
extern s32 D_801E8AB0;
extern s32 D_801F17A8;
extern u32 D_8009E698;
extern u32 D_8019CAFC;
void func_800179B4(void *, s32);
void func_80017BAC(s32);

void func_80018A70(s32 arg0) {
    register u8 *ptr asm("$16");
    register s32 index asm("$17");
    GameCarEntry *entry;
    register s32 offset asm("$2");
    register s32 temp asm("$4");
    register u32 mode asm("$3");
    s32 flag;

    if (g_AssetLoadState == 1) {
        index = arg0 << 3;
        entry = (GameCarEntry *)(index + (s32)D_8019C7C8);
        offset = func_80017848(arg0, entry->modelVariant + 1) << 1;
        mode = D_8009E87C;
        ptr = D_801E4090;

        temp = offset + 0xA;
        if (mode == 0) {
            ptr += 0x20000;
        }

        if (func_80017C78(temp, ptr) != 0) {
            func_80017B94(ptr, D_8009E87C < 1);

            temp = *(s32 *)(ptr + 0x20);
            flag = D_8009E87C < 1;
            temp = (s32)ptr + temp;
            *(s32 *)(ptr + 0x20) = temp;
            func_80017948((void *)temp, flag);

            temp = *(s32 *)(ptr + 0x24);
            flag = D_8009E87C < 1;
            temp = (s32)ptr + temp;
            *(s32 *)(ptr + 0x24) = temp;
            func_80017B44((void *)temp, flag);

            if (D_801E40D4 < 10) {
                entry = (GameCarEntry *)(index + (s32)D_8019C7C8);
                func_8001D748(entry->shapeIndex, *(s32 *)(ptr + 0x24));
                entry = (GameCarEntry *)(index + (s32)D_8019C7C8);
                func_8001D900(entry->textureIndex, *(s32 *)(ptr + 0x24));
            }

            g_AssetLoadState = 0;
        }
    }
}

s32 func_80018B98(void) {
    s32 state;

    if (g_AssetLoadState != 0) {
        return 1;
    }

    state = 7;
    if (g_MainState == state) {
        g_MainState = 0;
        return 0;
    }

    func_80042C94();
    g_MainState = state;
    g_AssetLoadState = 1;
    return 1;
}

void func_80018C0C(void) {
    s32 ptr;
    s32 offset;

    if (g_AssetLoadState == 1) {
        if (func_80017C78(9, (void *)D_8019C904) != 0) {
            func_80017948((void *)(D_8019C904 + 4), 0);
            func_80017A10(0);

            ptr = D_8019C904;
            offset = *(s32 *)ptr;
            g_AssetLoadState = 0;
            D_801E4B30 = ptr + offset;
        }
    }
}

s32 func_80018C88(void) {
    s32 value;

    if (g_AssetLoadState != 0) {
        func_80017BE4();
    }

    if (D_801E4DAC == 0) {
        value = (func_800632B0() & 0xFFF) % (D_801E772C[D_8019CABC] + 1);
        D_8009E6A4 = value;
        if (((D_801E428C & 3) == 3) && (value < 2)) {
            D_8009E6A4 = ((func_800632B0() & 0xFFF) % (D_801E772C[D_8019CABC] - 1)) + 2;
        }
    }

    if (g_MainState == 8) {
        g_MainState = 0;
        return 0;
    }

    g_MainState = 8;
    g_AssetLoadState = 1;
    return 1;
}

void func_80018DF8(void) {
    s32 state;
    s32 kind;
    s32 result;

    state = g_AssetLoadState;
    switch (state) {
    case 1:
        kind = 0x55;
        if (D_801E4DAC != 0) {
            register s32 index asm("$2") = D_8019CABC;
            register s32 scaled asm("$3");
            register s32 base asm("$2");

            __asm__ volatile("");
            scaled = index << 1;
            scaled += index;
            base = D_8009E6A4;
            scaled <<= 1;
            base += 0x4A;
            kind = scaled + base;
        }

        result = func_80017C78((s32)kind, (void *)D_801E4B30);
        if (result != 0) {
            g_AssetLoadState = 2;
            D_8019C754 = result + D_801E4B30;
        }
        break;
    case 2:
        if (func_80017C78(0x56, (void *)D_8019C754) != 0) {
            register s32 ptr asm("$4") = D_8019C754;
            register s32 first asm("$2");
            register s32 second asm("$3");
            register s32 third asm("$2");

            first = *(s32 *)(ptr + 4);
            second = *(s32 *)(ptr + 8);
            first = ptr + first;
            second = ptr + second;
            D_801F17A8 = first;
            D_801E8AB0 = second;
            third = *(s32 *)ptr;
            g_AssetLoadState = 0;
            D_801E4D70 = third;
        }
        break;
    }
}

void func_80018F08(void) {
    u32 temp;
    u32 *dst;
    u32 *src;
    u32 count;

    temp = D_8009E698;
    if (temp != 0) {
        src = (u32 *)temp;
    } else {
        src = (u32 *)temp;
    }
    count = src[6];
    temp = D_8019C904;
    if (count != 0) {
        dst = (u32 *)temp;
    } else {
        dst = (u32 *)temp;
    }
    count = count + 0x28;
    temp = count + (u32)dst;
    count >>= 2;
    D_8019CAFC = temp;

    while (count != 0) {
        *dst = *src;
        src++;
        count--;
        dst++;
    }

    func_80017B94((void *)D_8019C904, 0);
    temp = *(s32 *)(D_8009E698 + 0x20);
    func_800179B4((void *)(D_8019C904 + 0x28), temp);
    func_80017BAC(0);
    *(u32 *)(D_8009E698 + 0x20) = D_8019C904 + 0x28;
    func_80017948((void *)(D_8019C904 + 0x28), 0);
}
