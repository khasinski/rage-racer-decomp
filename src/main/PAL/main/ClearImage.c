#include "common.h"
#include "psyq/gpu.h"

extern GpuCallbacks *g_GpuFuncs asm("D_800941E0");
extern char D_8001356C[];

void func_80065968(char *arg0, void *arg1);

void func_80065A90(void *arg0, u32 arg1, u32 arg2, u32 arg3) {
    register void *rect asm("s3") = arg0;
    register u32 b asm("s2") = arg1;
    register u32 g asm("s1") = arg2;
    register u32 r asm("s0") = arg3;

    func_80065968(D_8001356C, rect);

    {
        register void *rectArg asm("$5") = rect;

        asm volatile(
            "andi $16,$16,0x00ff # maspsx-keep\n\t"
            "sll $16,$16,16 # maspsx-keep\n\t"
            "andi $17,$17,0x00ff # maspsx-keep\n\t"
            "sll $17,$17,8 # maspsx-keep\n\t"
            "or $16,$16,$17 # maspsx-keep\n\t"
            "andi $18,$18,0x00ff # maspsx-keep"
            :
            : "r"(g), "r"(rectArg));

        g_GpuFuncs->send(g_GpuFuncs->cmd0C, rectArg, 8, r | b);
    }
}
