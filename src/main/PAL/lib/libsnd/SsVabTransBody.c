#include "psyq/snd.h"
#include "psyq/spu.h"

extern u_char g_SndVabStatus[] asm("D_801E4CFC");
extern u_long g_SndVabBodySize[] asm("D_801E8AB8");
extern u_long g_SndVabSpuAddr[] asm("D_801F17BC");

short SsVabTransBody(u_char *addr, short vab_id) {
    short id;
    long index;
    u_long spu_addr;

    if ((u_short)vab_id < 17) {
        id = vab_id;
        if (g_SndVabStatus[id] == 2) {
            index = id * 4;
            spu_addr = *(u_long *)((u_char *)g_SndVabSpuAddr + index);
            SpuSetTransferMode(0);
            SpuSetTransferStartAddr(spu_addr);
            Spu_ReadFromSpu((long)addr, *(u_long *)((u_char *)g_SndVabBodySize + index));
            g_SndVabStatus[id] = 1;
            return id;
        }
    }

    _spu_setTransferCompletionFlag(0);
    return -1;
}

short SsVabTransCompleted(short immediate_flag) {
    return SpuIsTransferCompleted(immediate_flag);
}
