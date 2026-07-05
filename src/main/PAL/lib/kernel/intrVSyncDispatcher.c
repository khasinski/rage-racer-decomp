#include "psyq/kernel.h"

extern volatile s32 D_8009A4EC;
extern void (*D_8009A4CC[])(void);

void intrVSyncDispatcher(void) {
    register s32 i asm("$17");
    register void (**callback)(void) asm("$16");
    void (*func)(void);
    s32 count;

    count = D_8009A4EC;
    i = 0;
    callback = D_8009A4CC;
    D_8009A4EC = count + 1;
    count = D_8009A4EC;
    for (; i < 8; i++) {
        func = *callback++;
        if (func != 0) {
            func();
        }
    }
}
