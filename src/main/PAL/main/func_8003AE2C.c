#include "common.h"
#include "game/car.h"

typedef struct {
    s16 count;
} UnkFunc8003AE2CCount;

extern s32 D_8009E73C;
extern s32 D_8009E740;
extern s16 D_8009E834;
extern UnkFunc8003AE2CCount D_8009E83C;
extern s32 D_801E4364;
extern GameCarRuntimeProgressWindow D_801F18C0[];

void func_8003AE2C(void) {
    register s32 active asm("$6");
    register s32 i asm("$5");
    register s32 sentinel asm("$8");
    register GameCarRuntimeProgressWindow *entry asm("$4");
    s32 total;

    active = 1;
    if (D_801E4364 >= D_8009E83C.count) {
        i = 0;
        sentinel = -1;
        total = D_8009E73C + D_8009E740;
        entry = D_801F18C0;

        do {
            if (entry->activeFlag != sentinel) {
                if ((((s32 *)entry)[-1] + entry->field_6C) - total > 0) {
                    active++;
                }
            }
            i++;
            entry++;
        } while (i < 0xB);

        D_8009E834 = active;
    }
}
