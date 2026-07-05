#include "common.h"

typedef struct {
    s32 unk0;
    u8 pad[0x1C];
} UnkEventRecord;

extern UnkEventRecord * volatile D_801E8AAC;

void func_8006D0AC(s32 arg0, u32 arg1) {
    u32 i;

    for (i = 0; i < arg1; i++) {
        asm("" ::: "memory");
        D_801E8AAC[i + arg0].unk0 = 0;
    }
}
