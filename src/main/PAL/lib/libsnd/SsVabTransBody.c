#include "psyq/snd.h"
#include "psyq/spu.h"

extern u8 D_801E4CFC[];
extern u32 D_801E8AB8[];
extern u32 D_801F17BC[];

s16 SsVabTransBody(u8 *addr, s16 vab_id) {
    s16 id;
    s32 index;
    u32 spu_addr;

    if ((u16)vab_id < 17) {
        id = vab_id;
        if (D_801E4CFC[id] == 2) {
            index = id * 4;
            spu_addr = *(u32 *)((u8 *)D_801F17BC + index);
            SpuSetTransferMode(0);
            SpuSetTransferStartAddr(spu_addr);
            Spu_ReadFromSpu((s32)addr, *(u32 *)((u8 *)D_801E8AB8 + index));
            D_801E4CFC[id] = 1;
            return id;
        }
    }

    _spu_setTransferCompletionFlag(0);
    return -1;
}

s16 SsVabTransCompleted(s16 immediate_flag) {
    return SpuIsTransferCompleted(immediate_flag);
}
