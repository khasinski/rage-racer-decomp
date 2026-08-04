#include "psyq/snd.h"

extern u_char *g_SndSeqTable[] asm("D_801E79CC");

void SsSeqDispatchMidiEvent(long arg0, long arg1) asm("func_8006F1E0");

void SsSeqAdvanceChannelDelta(long seq, long channel);

void SsSeqAdvanceChannelDelta(long seq, long channel) {
    long raw0;
    long raw1;
    long table_offset;
    long chan_index;
    long chan;
    SeqStruct *state;
    long remaining;
    long period;
    register long period_copy asm("$6");
    long delay;
    register long store_value asm("$2");
    long count;
    volatile long pad[4];

    raw0 = seq << 16;
    table_offset = raw0 >> 14;
    raw1 = channel << 16;
    chan_index = raw1 >> 16;
    chan = ((((chan_index * 3) * 4) - chan_index) * 4) - chan_index;
    state = (SeqStruct *)((chan * 4) + (long)*(u_char **)((u_char *)g_SndSeqTable + table_offset));

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
        state->delta_value = store_value;
                return;
        } else {
            state->delta_value = remaining;
        }
    } else if (delay <= period) {
        long saved0;
        long saved1;
        long total;
        long loop_delay;

        total = delay;
        saved0 = raw0;
        saved1 = raw1;
        for (;;) {
        do {
            SsSeqDispatchMidiEvent(saved0 >> 16, saved1 >> 16);
            loop_delay = state->delta_value;
        } while (loop_delay == 0);

        count = state->tick_period;
        total += loop_delay;
        if (total < count) {
            store_value = total - count;
            continue;
        }
        store_value = total - count;
        break;
        }
        state->delta_value = store_value;
    }
}
