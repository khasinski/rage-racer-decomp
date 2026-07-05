#include "psyq/kernel.h"

typedef struct {
    u8 pad0[0xC];
    void (*callback)(void);
} CallbackTable;

extern CallbackTable *D_8009A4B8;

void KernelCallbackSlot3(void) {
    D_8009A4B8->callback();
}
