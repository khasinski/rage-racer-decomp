#include "common.h"
void *func_8001EDC4(s32 *arg0);
void func_80064588(void *arg0, s32 arg1);
s32 func_8006CFF0(void *arg0);
s32 func_8001ED3C(s32 *arg0) {
    void *p;
    s32 retry;
    for (retry = 1; retry != 0; retry--) {
        p = func_8001EDC4(arg0);
        if (p != 0) {
            arg0[2] = (arg0[2] == 0);
            func_80064588(p, arg0[arg0[2]]);
            return func_8006CFF0(p);
        }
    }
    return -1;
}
