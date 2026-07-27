#include "common.h"

void SsSeqClose(short arg0) asm("func_80071948");

void SsSeqCloseWrapper(short arg0) asm("func_80071AC4");

void SsSeqCloseWrapper(short arg0) {
    SsSeqClose(arg0);
}

void SsSepCloseWrapper(short arg0) asm("func_80071AE8");

void SsSepCloseWrapper(short arg0) {
    SsSeqClose(arg0);
}
