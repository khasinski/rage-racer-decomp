#include "common.h"

extern u32 D_8007C484;
extern s32 D_8019CACC;
extern u8 D_8019C86C;
extern u8 D_8009EC94;
extern void *D_8009E698;
extern void *D_801E6828;
extern void *D_801E4BC8;
extern void *D_801E41A8[];
extern s32 D_801E4168;
extern s32 D_801E40E4;
extern void *D_801E5020;
extern void *D_801E4B98;
extern s32 D_801E4144;
extern void *D_801E424C[];
extern void *D_801E8A54[];

void LoadImage(void *rect, void *data) asm("func_80065B24");

void func_80017884(s32 arg0) {
    register s32 value asm("$2");
    register s32 ptr asm("$2");
    register s32 tmp asm("$3");

    tmp = D_8019CACC;
    value = 0xA;
    *(s32 *)0x1F80006C = value;
    value = 0x80;
    *(u8 *)0x1F800072 = value;
    *(u8 *)0x1F800071 = value;
    *(u8 *)0x1F800070 = value;
    value = 0x2C;
    *(u8 *)0x1F800073 = value;
    value = 0xFF;
    *(u8 *)0x1F800076 = value;
    *(u8 *)0x1F800075 = value;
    *(u8 *)0x1F800074 = value;
    value = 0x3C;
    *(u8 *)0x1F800077 = value;
    value = 0x140;
    *(u16 *)0x1F80007C = value;
    value = 0xF0;
    *(u16 *)0x1F80007E = value;
    ptr = (s32)&D_8019C86C;
    D_801E6828 = (void *)ptr;
    ptr = (s32)&D_8009EC94;
    *(s32 *)0x1F800064 = arg0;
    *(u16 *)0x1F800078 = 0;
    *(u16 *)0x1F80007A = 0;
    D_801E4BC8 = (void *)ptr;
    *(s32 *)0x1F800068 = tmp;
}

asm(".globl func_80017920\nfunc_80017920 = func_80017884 + 0x9C");

void func_80017948(s32 *base, s32 index) {
    register s32 *ptr asm("$6");
    register s32 i asm("$5");
    register s32 value asm("$2");
    register s32 count asm("$3");

    asm volatile("addiu $sp,$sp,-8");
    ptr = base + 3;
    D_801E41A8[index] = base;
    value = base[1];
    i = 0;
    value = (s32)base + value;
    base[1] = value;
    value = base[2];
    count = base[0];
    value = (s32)base + value;
    base[2] = value;
    if (count != 0) {
        do {
            value = *ptr;
            i++;
            value = (s32)base + value;
            *ptr = value;
            value = base[0];
            ptr++;
        } while ((u32)i < (u32)value);
    }
    asm volatile("addiu $sp,$sp,8");
}

void func_800179B4(s32 *base, s32 offset) {
    register s32 *ptr asm("$6");
    register s32 i asm("$7");
    register s32 value asm("$2");
    register s32 count asm("$3");

    asm volatile("addiu $sp,$sp,-8");
    ptr = base + 3;
    value = base[1];
    i = 0;
    value -= offset;
    base[1] = value;
    value = base[2];
    count = base[0];
    value -= offset;
    base[2] = value;
    if (count != 0) {
        do {
            value = *ptr;
            i++;
            value -= offset;
            *ptr = value;
            value = base[0];
            ptr++;
        } while ((u32)i < (u32)value);
    }
    asm volatile("addiu $sp,$sp,8");
}

void func_80017A10(s32 index) {
    register s32 *entry asm("$4");
    register s32 ptr asm("$2");
    register s32 value asm("$3");

    ptr = (s32)D_801E41A8;
    entry = (s32 *)((index << 2) + ptr);
    ptr = *entry;
    value = *(volatile s32 *)entry;
    ptr = *(s32 *)(ptr + 4);
    value = *(s32 *)value;
    *(s32 *)0x1F800054 = ptr;
    ptr = *entry;
    asm volatile("" : "=r"(ptr) : "0"(ptr));
    ptr = *(s32 *)(ptr + 8);
    *(s32 *)0x1F800058 = ptr;
    ptr = *entry;
    D_801E4168 = value;
    ptr += 0xC;
    *(s32 *)0x1F800050 = ptr;
}

void func_80017A6C(s32 *base) {
    register s32 *ptr asm("$5");
    register s32 count asm("$2");
    register s32 i asm("$6");
    register s32 limit asm("$7");
    register s32 *item asm("$3");
    register s32 value asm("$2");

    asm volatile("addiu $sp,$sp,-8");
    ptr = base + 1;
    count = base[0];
    *(s32 *)0x1F800048 = (s32)ptr;
    D_801E40E4 = count;
    i = 0;
    if (count > 0) {
        limit = count;
        item = base + 3;
        do {
            value = *ptr;
            i++;
            value = (s32)base + value;
            *ptr = value;
            value = *item;
            ptr += 3;
            value = (s32)base + value;
            *item = value;
            item += 3;
        } while (i < limit);
    }
    asm volatile("addiu $sp,$sp,8");
}

void func_80017AD0(s32 *base) {
    register s32 *ptr asm("$3");
    register s32 count asm("$6");
    register s32 i asm("$5");
    register s32 value asm("$2");

    asm volatile("addiu $sp,$sp,-8");
    D_801E5020 = base;
    base = (s32 *)((s32)base + 0x800);
    D_801E4B98 = base;
    base = (s32 *)((s32)base + 0x1000);
    ptr = base + 2;
    count = base[0];
    asm volatile("" : "=r"(count) : "0"(count));
    value = base[1];
    *(s32 *)0x1F80005C = (s32)ptr;
    value = (s32)base + value;
    asm volatile("" : "=r"(value) : "0"(value));
    D_801E4144 = count;
    *(s32 *)0x1F800060 = value;
    i = 0;
    if (count > 0) {
        do {
            value = *ptr;
            i++;
            value = (s32)base + value;
            *ptr = value;
            ptr++;
        } while (i < count);
    }
    asm volatile("addiu $sp,$sp,8");
}

void func_80017B44(void *asset, s32 index) {
    D_801E424C[index] = asset;
}

void func_80017B5C(s32 index) {
    LoadImage(&D_8007C484, D_801E424C[index]);
}

void func_80017B94(void *asset, s32 index) {
    D_801E8A54[index] = asset;
}

void func_80017BAC(s32 index) {
    D_8009E698 = D_801E8A54[index];
}

asm(".globl func_80017BBC\nfunc_80017BBC = func_80017BAC + 0x10");
asm(".word 0x03E00008\n.word 0x00000000");
