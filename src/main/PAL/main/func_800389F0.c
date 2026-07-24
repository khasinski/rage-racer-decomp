#include "common.h"
#include "game/car.h"

extern GameCarRuntime D_801F1854[];

void func_80017A10(s32 arg0);
void func_8001DFC0(GameCarRuntime *arg0);

void func_800389F0(void) {
    GameCarRuntime *base;
    s32 i;
    u8 *entry;
    s32 one;
    s32 minus_one;

    base = D_801F1854;
    func_80017A10(1);

    i = 0;
    minus_one = -1;
    one = 1;
    entry = (u8 *)&base->field_BC;
    do {
        if (*(s16 *)(entry - 0x10) != (i++, minus_one)) {
            if (*(s32 *)entry == one) {
                func_8001DFC0(base);
            }
        }
        entry += 0x19C;
        base++;
    } while (i < 11);
}
