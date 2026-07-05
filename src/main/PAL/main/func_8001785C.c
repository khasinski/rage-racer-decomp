#include "common.h"

typedef struct CarRosterEntry {
    u8 model_variant;
    u8 pad1[7];
} CarRosterEntry;

extern u8 D_8007C474[];
extern CarRosterEntry *D_8019C7C8;

s32 func_8001785C(s32 car_index) {
    return D_8019C7C8[car_index].model_variant + D_8007C474[car_index];
}
