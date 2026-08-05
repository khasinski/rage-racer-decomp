#include "common.h"
#include "psyq/kernel.h"


void SpuInit(void);
void _SsInitTables(void);

void ssinit(void) {
    KernelCallbackSlot3();
    SpuInit();
    _SsInitTables();
}
