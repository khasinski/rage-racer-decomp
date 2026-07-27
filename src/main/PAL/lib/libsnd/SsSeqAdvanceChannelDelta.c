#include "psyq/snd.h"

extern u_char *g_SndSeqTable[] asm("D_801E79CC");

void SsSeqDispatchMidiEvent(long arg0, long arg1) asm("func_8006F1E0");

void SsSeqAdvanceChannelDelta(long seq, long channel) asm("func_8006F0D0");

void SsSeqAdvanceChannelDelta(long seq, long channel) {
    register long raw0 asm("$7");
    register long raw1 asm("$5");
    register long table_offset asm("$4");
    register long chan_index asm("$3");
    register long chan asm("$2");
    SeqStruct *state;
    register long remaining asm("$4");
    register long period asm("$2");
    register long period_copy asm("$6");
    register long delay asm("$3");
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
            goto store_delay;
        } else {
            state->delta_value = remaining;
        }
    } else if (delay <= period) {
        register long saved0 asm("$19");
        register long saved1 asm("$18");
        register long total asm("$16");
        long loop_delay;

        total = delay;
        saved0 = raw0;
        saved1 = raw1;
loop_call:
        do {
            SsSeqDispatchMidiEvent(saved0 >> 16, saved1 >> 16);
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
