#include "common.h"

void func_8001A2E0(void *arg0);

void func_8001A3C0(void *arg0) {
    union {
        s32 offset;
        u8 *next;
    } state;
    u8 *ptr;

    ptr = (u8 *)arg0 + 4;
    goto test;

loop:
    state.offset = (u32)state.offset >> 2;
    state.offset <<= 2;
    state.next = ptr + state.offset;
    func_8001A2E0(ptr);
    ptr = state.next;

test:
    state.offset = *(s32 *)ptr;
    if (state.offset > 0) {
        ptr += 4;
        goto loop;
    }
}
