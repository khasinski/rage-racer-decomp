#include "common.h"

void SsSeqClose(short arg0);

void SsSeqCloseWrapper(short arg0);

void SsSeqCloseWrapper(short arg0) {
    SsSeqClose(arg0);
}

void SsSepCloseWrapper(short arg0);

void SsSepCloseWrapper(short arg0) {
    SsSeqClose(arg0);
}
