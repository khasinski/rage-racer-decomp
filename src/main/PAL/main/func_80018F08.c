#include "common.h"

extern u32 D_8009E698;
extern u32 D_8019C904;
extern u32 D_8019CAFC;

void func_80017948(void *, s32);
void func_800179B4(void *, s32);
void func_80017B94(void *, s32);
void func_80017BAC(s32);

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
