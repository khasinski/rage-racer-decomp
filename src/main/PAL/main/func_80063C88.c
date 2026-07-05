#include "common.h"
#include "psyq/kernel.h"

extern u8 D_80082FD9[];
extern s32 D_8009B9AC;

void func_80063C88(s32 arg0) {
    u8 c;
    register s32 value asm("$3") = arg0;

    c = value;
    value = c;
    if (value == 9) {
        goto tab;
    }
    if (value != 10) {
        goto other;
    }

    func_80063C88(13);
    D_8009B9AC = 0;
    goto output;

tab:
    do {
        func_80063C88(0x20);
    } while ((D_8009B9AC & 7) != 0);
    return;

other:
    if (D_80082FD9[value] & 0x97) {
        D_8009B9AC++;
    }

output:
    BiosFileWrite(1, &c, 1);
}
