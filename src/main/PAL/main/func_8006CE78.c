#include "common.h"

typedef void (*Callback)(void);

typedef struct {
    s16 state;
    u8 pad2[6];
    s32 value;
    u8 padC[0x10];
    u8 tail[4];
} Entry;

extern s32 D_801E6C84;
extern s32 D_801E6C74;
extern Entry *D_801E8AAC;
extern s32 D_8009DF18;
extern Callback D_8019C994;
extern s32 D_801E42C8;

void data_ready_callback(void) asm("func_8006CE78");

void data_ready_callback(void) {
    register s32 index asm("$2") = D_801E6C84;
    register Entry *base asm("$3") = D_801E8AAC;
    register Entry *entry asm("$2");

    entry = (Entry *)((index << 5) + (s32)base);
    entry->state = 2;
    __asm__ volatile(
        "lui    $6,%%hi(D_8009DF14)\n"
        "addiu  $6,$6,%%lo(D_8009DF14)\n"
        "lwl    $3,0x1F(%0)\n"
        "lwr    $3,0x1C(%0)\n"
        "nop\n"
        "swl    $3,0x3($6)\n"
        "swr    $3,0x0($6)"
        :
        : "r"(entry)
        : "$3", "$6", "memory");
    D_8009DF18 = entry->value;
    D_801E6C84 = D_801E6C74;
    if (D_8019C994 != 0) {
        D_8019C994();
    }
    D_801E42C8 = 0;
}
