#include "common.h"

void SsSeqAdvanceChannelDelta(s16 seq, s16 channel) asm("func_8006F0D0");

void SsSeqAdvanceChannelTick(s32 arg0, s32 arg1) asm("func_800718B4");

void SsSeqAdvanceChannelTick(s32 arg0, s32 arg1) {
    SsSeqAdvanceChannelDelta(arg0, arg1);
}
