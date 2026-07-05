#include "common.h"

void SsSeqClose(s16 arg0) asm("func_80071948");

void SsSepCloseWrapper(s16 arg0) asm("func_80071AE8");

void SsSepCloseWrapper(s16 arg0) {
    SsSeqClose(arg0);
}
