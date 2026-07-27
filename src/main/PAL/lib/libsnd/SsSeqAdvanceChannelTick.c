#include "common.h"

void SsSeqAdvanceChannelDelta(short seq, short channel) asm("func_8006F0D0");

void SsSeqAdvanceChannelTick(long arg0, long arg1) asm("func_800718B4");

void SsSeqAdvanceChannelTick(long arg0, long arg1) {
    SsSeqAdvanceChannelDelta(arg0, arg1);
}
