#include "psyq/snd.h"

extern u8 *D_801E79CC[];

s32 SsSeqReadDeltaTime(s32 arg0, s32 arg1) asm("func_80070D70");

s32 SsSeqReadDeltaTime(s32 arg0, s32 arg1) {
    register SeqStruct *state asm("$5");
    u8 *stream;
    s32 value;
    s32 byte;

    asm volatile(
        ".set noat\n\t"
        "sll $4, $4, 16\n\t"
        "sra $4, $4, 14\n\t"
        "sll $5, $5, 16\n\t"
        "sra $5, $5, 16\n\t"
        "sll $2, $5, 1\n\t"
        "addu $2, $2, $5\n\t"
        "sll $2, $2, 2\n\t"
        "subu $2, $2, $5\n\t"
        "sll $2, $2, 2\n\t"
        "subu $2, $2, $5\n\t"
        "lui $1, %%hi(D_801E79CC)\n\t"
        "addu $1, $1, $4\n\t"
        "lw $3, %%lo(D_801E79CC)($1)\n\t"
        "sll $2, $2, 2\n\t"
        "addu $5, $2, $3\n\t"
        ".set at"
        : "=r"(state)
        :
        : "$1", "$2", "$3", "$4");

    stream = state->read_pos;
    state->read_pos = stream + 1;
    value = stream[0];

    if (value == 0) {
        return 0;
    }

    if (value & 0x80) {
        value &= 0x7F;
        do {
            stream = state->read_pos;
            state->read_pos = stream + 1;
            byte = stream[0];
            value = (value << 7) + (byte & 0x7F);
        } while (byte & 0x80);
    }

    {
        register s32 ret asm("$2");

        ret = value * 5;
        asm volatile(
            "lw $3, 0x80($5)\n\t"
            "sll $2, $2, 1\n\t"
            "addu $3, $2, $3\n\t"
            "sw $3, 0x80($5)"
            : "=r"(ret)
            : "0"(ret), "r"(state)
            : "$3", "memory");
        return ret;
    }
}
