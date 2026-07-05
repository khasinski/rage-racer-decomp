#include "common.h"

extern u32 D_8009E698;
extern u32 D_8019C904;
extern u32 D_8019CAFC;

void func_80017948(void *, s32);
void func_800179B4(void *, s32);
void func_80017B94(void *, s32);
void func_80017BAC(s32);

void func_80018F08(void) {
    register u32 temp asm("$2");
    register u32 *src asm("$5");
    register u32 dst asm("$4");
    register u32 count asm("$3");

    temp = D_8009E698;
    __asm__ volatile("" : "=r"(temp) : "0"(temp));
    src = (u32 *)temp;
    temp = D_8019C904;
    __asm__ volatile("" : "=r"(temp) : "0"(temp));
    dst = temp;
    count = src[6] + 0x28;
    temp = count + dst;
    count >>= 2;
    D_8019CAFC = temp;

    while (count != 0) {
        *(u32 *)dst = *src;
        src++;
        count--;
        dst += 4;
    }

    func_80017B94((void *)D_8019C904, 0);
    func_800179B4((void *)(D_8019C904 + 0x28), *(s32 *)(D_8009E698 + 0x20));
    func_80017BAC(0);
    *(u32 *)(D_8009E698 + 0x20) = D_8019C904 + 0x28;
    func_80017948((void *)(D_8019C904 + 0x28), 0);
}
