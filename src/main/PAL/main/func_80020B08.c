#include "common.h"

extern s32 D_801E4B94;
extern s32 D_8019C8EC;
extern s32 D_801E4FAC;
extern s32 D_8019C7C8;
extern s32 D_8009E6A4;
extern s32 D_801E419C;
extern s16 D_801E4034;
extern s32 D_801E772C[];
extern s32 D_801E42E4;

void func_80021288(s32 arg0, s32 arg1);
void func_800212F0(s32 arg0);
void func_80019BB8(s32 arg0);
void func_80019B3C(s32 arg0);

void func_80020B08(void) {
    register s32 oldValue asm("s0");
    s32 *ptr;
    s32 *entry;

    if (D_801E4B94 != 0) {
        if (D_8019C8EC != 0) {
            register s32 magic asm("v1");
            register s32 *afterPtr asm("v0");

            ptr = (s32 *)D_801E4FAC;
            oldValue = ptr[3];
            func_80021288(D_8019C7C8, (s32)ptr);
            magic = 0x3B9AC9FF;
            afterPtr = (s32 *)D_801E4FAC;
            afterPtr[4] = magic;
            afterPtr[3] = oldValue;
            func_800212F0(0);
            func_80019BB8(0x21);
        } else {
            register s32 current asm("v0");
            register s32 next asm("a1");
            register s32 *menuPtr asm("a0");
            register s32 enabled asm("v1");

            func_80019B3C(7);
            current = D_8009E6A4;
            menuPtr = (s32 *)D_801E4FAC;
            enabled = D_801E419C;
            next = current + 1;
            D_8009E6A4 = next;
            menuPtr[2] = next;
            menuPtr[0] = 0;

            if (enabled != 0) {
                menuPtr[3] = next;
                entry = &D_801E772C[D_801E4034];
                if (*entry < next) {
                    *entry = next;
                }
            }

            func_800212F0(D_8009E6A4);
        }
    } else {
        D_801E42E4 = 6;
    }
}
