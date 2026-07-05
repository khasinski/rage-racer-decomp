#include "common.h"

void SsSeqClose(s16 arg0) asm("func_80071948");

void SsSeqCloseWrapper(s16 arg0) asm("func_80071AC4");

void SsSeqCloseWrapper(s16 arg0) {
    SsSeqClose(arg0);
}
