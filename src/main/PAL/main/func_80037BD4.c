#include "common.h"

typedef struct {
    s32 value;
    u8 pad[0x34];
} Unk80037BD4;

extern Unk80037BD4 D_801E4DF4[];

s32 func_80037BD4(void) {
    Unk80037BD4 *ptr = D_801E4DF4;
    s32 count = 0;
    s32 i = 5;

    do {
        i--;
        count += ptr->value != 0;
        ptr++;
    } while (i >= 0);

    return count;
}
