#include "common.h"
#include "psyq/kernel.h"
#include "psyq/snd.h"
#include "psyq/spu.h"

void ssinit(void) {
    KernelCallbackSlot3();
    SpuInit();
    _SsInitTables();
}
