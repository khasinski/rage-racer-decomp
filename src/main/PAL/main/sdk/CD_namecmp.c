#include "common.h"

long LibcStrncmp(long arg0, long arg1, long arg2);

long CD_namecmp(long arg0, long arg1) {
    return LibcStrncmp(arg0, arg1, 0xC) == 0;
}
