#include "common.h"
#include "game/menu.h"

extern s32 D_8009E67C;
extern s32 D_801E428C;
extern s32 D_801E4D0C;
extern s16 D_8009E834;
extern s32 D_801E40D4;
extern s32 D_8009E6A4;
extern s32 D_801E4B94;
extern s16 D_8019CABC;
extern s16 D_8019CB58;
extern s16 D_8019CB54;
extern GameScoreRecord D_8019CB40[];
extern GameScoreRecord D_8019CB42[];
extern s32 D_8019C7C4;
extern s32 D_8019C8EC;
extern s16 D_801E4034;
extern s16 D_8019CAC0;
extern s32 D_801E4FAC;
extern s32 D_801E419C;

s32 func_8001785C(s32 arg0);
s32 func_800214B8(void);
void func_80021540(void);

void func_800207E0(void) {
    s32 score_index;
    u8 *slots;
    s32 slot_count;
    s32 filled;
    s32 i;
    s32 done;
    register s32 value asm("$2");
    s32 *state;

    slots = (u8 *)(D_8009E67C + D_801E428C);
    D_801E4D0C = 0;

    if (*slots == 0 || D_8009E834 < *slots) {
        *slots = D_8009E834;
    }

    value = func_8001785C(D_801E40D4);
    slot_count = 4;
    if (D_8009E6A4 < value) {
        *(s16 *)(D_8009E67C + 4) = 1;
    }

    if (D_8009E6A4 < 2) {
        slot_count = 3;
    }

    filled = 0;
    for (i = 0; i < slot_count; i++) {
        if (((u8 *)D_8009E67C)[i] != 0) {
            filled++;
        }
    }

    done = ((slot_count ^ filled) == 0);
    D_801E4B94 = done;

    if (done != 0) {
        register s16 *record asm("$4");

        score_index = (D_8019CABC * 6) + D_8009E6A4;

        if (score_index == 4) {
            record = &D_8019CB58;
            goto check_record;
        }
        if (score_index != 10) {
            goto not_special_record;
        }
        record = &D_8019CB54;

check_record:
        if (*record == -1) {
            *record = 0;
        }

        goto after_record_check;

not_special_record:
        if (score_index != 5) {
            if (D_8019CB40[score_index + 1].value == -1) {
                D_8019CB40[score_index + 1].value = 0;
            }
        }

after_record_check:
        value = func_800214B8();
        D_8019C7C4 = value;
        if (value != 0) {
            register s32 offset asm("$4");

            offset = score_index * 4;
            if (D_8019CB40[score_index].value == 0 || value < D_8019CB40[score_index].value) {
                D_8019CB40[score_index].value = (u16)D_8019C7C4;
            }
            D_801E4D0C = 0xD2;
        }

        func_80021540();
        if (D_8019C7C4 == 1) {
            register s32 offset asm("$4");

            offset = score_index * 4;
            if (*(s16 *)((char *)D_8019CB42 + offset) < 99) {
                (*(s16 *)((char *)D_8019CB42 + offset))++;
            }
        }
    } else {
        D_8019C7C4 = 0;
    }

    D_8019C8EC = 0;
    if (D_801E4B94 != 0) {
        if ((D_801E4034 == 0 && D_8009E6A4 == 4) || (D_801E4034 == 1 && D_8009E6A4 == 5)) {
            D_8019C8EC = 1;
            D_8019CAC0 = 1;
        }
    }

    D_801E419C = 0;
    if (D_801E4B94 != 0 && D_8019C8EC == 0) {
        state = (s32 *)D_801E4FAC;
        if (state[3] < D_8009E6A4 + 1) {
            D_801E419C = 1;
        }
    }
}
