#include "psyq/kernel.h"
#include "psyq/spu.h"

extern s32 D_8009A714;
extern s32 D_8009A718;
extern s32 D_8009A71C;
extern s32 D_8009A720;
extern s32 D_8009A728;
extern s16 D_8009A72C;
extern s16 D_8009A72E;
extern s32 D_8009A730;
extern s32 D_8009A734;
extern u16 D_8009A766;
extern s32 D_8009A76C;
extern s32 D_8009AB94;
extern s32 D_8009ABE0;

void _SpuInit(s32 arg0) {
    s32 temp;

    KernelCallbackSlot3();
    _spu_init(arg0);

    if (arg0 == 0) {
        register u16 *ptr asm("$2");
        register s32 i asm("$3");
        register s32 value asm("$4");

        value = 0xC000;
        i = 0x17;
        ptr = &D_8009A766;
        do {
            *ptr-- = value;
            i--;
        } while (i >= 0);
    }

    SpuStart();

    temp = D_8009ABE0;
    D_8009A718 = 0;
    D_8009A71C = 0;
    D_8009A728 = 0;
    D_8009A72C = 0;
    D_8009A72E = 0;
    D_8009A730 = 0;
    D_8009A734 = 0;
    D_8009A720 = temp;

    _spu_FsetRXX(0xD1, temp, 0);

    D_8009A714 = 0;
    D_8009AB94 = 0;
    D_8009A76C = 0;
}
