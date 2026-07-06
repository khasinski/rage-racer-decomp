#include "common.h"
#include "psyq/kernel.h"

extern s32 D_8009B538[];


INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005EFAC", func_8005EFAC);

void func_8005F0D4(void) {
    EnableEvent(D_8009B538[0]);
    EnableEvent(D_8009B538[1]);
    EnableEvent(D_8009B538[2]);
    EnableEvent(D_8009B538[3]);
    EnableEvent(D_8009B538[4]);
    EnableEvent(D_8009B538[5]);
    EnableEvent(D_8009B538[6]);
    EnableEvent(D_8009B538[7]);
}

void func_8005F16C(void) {
    DisableEvent(D_8009B538[0]);
    DisableEvent(D_8009B538[1]);
    DisableEvent(D_8009B538[2]);
    DisableEvent(D_8009B538[3]);
    DisableEvent(D_8009B538[4]);
    DisableEvent(D_8009B538[5]);
    DisableEvent(D_8009B538[6]);
    DisableEvent(D_8009B538[7]);
}

void func_8005F204(void) {
    EnterCriticalSection();
    CloseEvent(D_8009B538[0]);
    CloseEvent(D_8009B538[1]);
    CloseEvent(D_8009B538[2]);
    CloseEvent(D_8009B538[3]);
    CloseEvent(D_8009B538[4]);
    CloseEvent(D_8009B538[5]);
    CloseEvent(D_8009B538[6]);
    CloseEvent(D_8009B538[7]);
    ExitCriticalSection();
}
