#include "common.h"


void SsQuit(void) asm("func_80071C24");
void SsQuit(void) {
    void SpuQuit(void) asm("func_80079AE4");

    SpuQuit();
}
