#include "psyq/snd.h"

extern u8 *D_801E79CC[];

void func_8006F1E0(s32 arg0, s32 arg1);

void SsSeqAdvanceChannelDelta(s32 seq, s32 channel) asm("func_8006F0D0");

void SsSeqAdvanceChannelDelta(s32 seq, s32 channel) {
    register s32 raw0 asm("$7");
    register s32 raw1 asm("$5");
    register s32 table_offset asm("$4");
    register s32 chan_index asm("$3");
    register s32 chan asm("$2");
    SeqStruct *state;
    register s32 remaining asm("$4");
    register s32 period asm("$2");
    register s32 period_copy asm("$6");
    register s32 delay asm("$3");
    register s32 store_value asm("$2");
    s32 count;
    volatile s32 pad[4];

    raw0 = seq << 16;
    table_offset = raw0 >> 14;
    raw1 = channel << 16;
    chan_index = raw1 >> 16;
    chan = ((((chan_index * 3) * 4) - chan_index) * 4) - chan_index;
    state = (SeqStruct *)((chan * 4) + (s32)*(u8 **)((u8 *)D_801E79CC + table_offset));

    period = state->tick_period;
    delay = state->delta_value;
    remaining = delay - period;
    period_copy = period;

    if (remaining > 0) {
        count = state->unk6E;
        store_value = count;
        if (count > 0) {
            store_value--;
            state->unk6E = store_value;
        } else if (count == 0) {
            state->unk6E = period_copy;
            store_value = state->delta_value - 1;
            goto store_delay;
        } else {
            state->delta_value = remaining;
        }
    } else if (delay <= period) {
        register s32 saved0 asm("$19");
        register s32 saved1 asm("$18");
        register s32 total asm("$16");
        s32 loop_delay;

        total = delay;
        saved0 = raw0;
        saved1 = raw1;
loop_call:
        do {
            func_8006F1E0(saved0 >> 16, saved1 >> 16);
            loop_delay = state->delta_value;
        } while (loop_delay == 0);

        count = state->tick_period;
        total += loop_delay;
        if (total < count) {
            store_value = total - count;
            goto loop_call;
        }
        store_value = total - count;
store_delay:
        state->delta_value = store_value;
    }
}
