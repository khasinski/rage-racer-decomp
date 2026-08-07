#include "common.h"

void SsSeqAdvanceChannelDelta(short seq, short channel);

void SsSeqAdvanceChannelTick(long seq, long sep);

void SsSeqAdvanceChannelTick(long seq, long sep) {
    SsSeqAdvanceChannelDelta(seq, sep);
}
