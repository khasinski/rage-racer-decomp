#include "common.h"
#include "psyq/kernel.h"

extern s32 D_8009B538[];


void func_8005EFAC(void) {
    EnterCriticalSection();
    D_8009B538[0] = OpenEvent(0xF4000001, 0x0004, 0x2000, 0);
    D_8009B538[1] = OpenEvent(0xF4000001, 0x8000, 0x2000, 0);
    D_8009B538[2] = OpenEvent(0xF4000001, 0x0100, 0x2000, 0);
    D_8009B538[3] = OpenEvent(0xF4000001, 0x2000, 0x2000, 0);
    D_8009B538[4] = OpenEvent(0xF0000011, 0x0004, 0x2000, 0);
    D_8009B538[5] = OpenEvent(0xF0000011, 0x8000, 0x2000, 0);
    D_8009B538[6] = OpenEvent(0xF0000011, 0x0100, 0x2000, 0);
    D_8009B538[7] = OpenEvent(0xF0000011, 0x2000, 0x2000, 0);
    ExitCriticalSection();
}

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
