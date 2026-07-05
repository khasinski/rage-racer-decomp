#include "psyq/snd.h"

void SsSeqStop(s16 arg0) {
    _SsSndStop(arg0, 0);
}

void SsSepStop(s16 arg0, s16 arg1) {
    _SsSndStop(arg0, arg1);
}
