#include "common.h"

void SsSeqAdvanceChannelDelta(short seq, short channel);

void SsSeqAdvanceChannelTick(long arg0, long arg1);

void SsSeqAdvanceChannelTick(long arg0, long arg1) {
    SsSeqAdvanceChannelDelta(arg0, arg1);
}
