#include "common.h"

void SsSeqAdvanceChannelTick(long seq, long sep) {
    SsSeqAdvanceChannelDelta((short)seq, (short)sep);
}
