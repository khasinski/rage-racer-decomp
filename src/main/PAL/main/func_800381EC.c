#include "common.h"
extern s32 D_801E40D8;
extern s32 D_801E408C;
extern s32 D_8007E054;
extern s32 D_8007E058;

void func_8005B190();

void func_800381EC(s32 arg0) {
    register s32 result asm("$6");
    register s32 i asm("$5");
    register s32 offset asm("$3");
    register s32 arg asm("$4");
    register s32 scene asm("$2");

    result = 0;
    if (arg0 < 0) {
        arg0 += D_801E40D8;
    }

    scene = D_801E408C;
    i = 0;
    offset = scene << 4;
loop:
    if (*(s32 *)((u8 *)&D_8007E054 + offset) >= arg0) {
        goto next;
    }

    if (arg0 >= *(s32 *)((u8 *)&D_8007E058 + offset)) {
        i++;
        goto check;
    }

    result = 0x46;
    goto done;

next:
    i++;

check:
    if (i < 2) {
        offset += 8;
        goto loop;
    }

done:
    arg = result;
    asm("" : "=r"(arg) : "0"(arg));
    func_8005B190(arg, arg);
}
