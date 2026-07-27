#include "common.h"

long LibcStrncmp(long arg0, long arg1, long arg2) asm("func_8006CC8C");

long CD_namecmp(long arg0, long arg1) asm("func_8006C53C");
long CD_namecmp(long arg0, long arg1) {
    return LibcStrncmp(arg0, arg1, 0xC) == 0;
}
