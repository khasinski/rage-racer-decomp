#include "common.h"
#include "psyq/snd.h"
void SsSeqCloseWrapper(short seq) {
    SsSeqClose(seq);
}
void SsSepCloseWrapper(short seq) {
    SsSeqClose(seq);
}
