#include "common.h"
#include <string.h>
#include "psyq/cd.h"

long CD_namecmp(char *lhs, char *rhs) {
    return strncmp((u8 *)lhs, (u8 *)rhs, 0xC) == 0;
}
