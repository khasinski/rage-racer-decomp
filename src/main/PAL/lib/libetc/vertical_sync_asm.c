#include "psyq/kernel.h"
#include <stdio.h>

typedef void (*Callback2)(long, long);

typedef struct CallbackTable {
    u_char pad0[0xC];
    void (*callback)(void);
} CallbackTable;

HANDWRITTEN_ASM("src/main/PAL/lib/libetc", vertical_sync);
