#include "common.h"

void func_8001A2E0(void *arg0);

void func_8001A3C0(void *arg0) {
    register u8 *ptr asm("$4");
    register s32 offset asm("$16");
    register u8 *next asm("$16");

    ptr = (u8 *)arg0 + 4;
    goto test;

loop:
    offset = (u32)offset >> 2;
    offset <<= 2;
    next = ptr + offset;
    func_8001A2E0(ptr);
    ptr = next;

test:
    offset = *(s32 *)ptr;
    if (offset > 0) {
        ptr += 4;
        goto loop;
    }
}
