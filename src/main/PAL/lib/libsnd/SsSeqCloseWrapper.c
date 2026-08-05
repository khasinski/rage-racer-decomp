#include "common.h"
#include "psyq/snd.h"

void SsSeqCloseWrapper(short arg0);

void SsSeqCloseWrapper(short arg0) {
    SsSeqClose(arg0);
}

void SsSepCloseWrapper(short arg0);

void SsSepCloseWrapper(short arg0) {
    SsSeqClose(arg0);
}
