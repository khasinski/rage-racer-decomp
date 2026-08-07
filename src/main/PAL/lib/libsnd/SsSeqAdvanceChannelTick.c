#include "common.h"

void SsSeqAdvanceChannelDelta(short seq, short channel);

void SsSeqAdvanceChannelTick(long arg0, long arg1);

void SsSeqAdvanceChannelTick(long seq, long sep) {
    SsSeqAdvanceChannelDelta(seq, sep);
}
