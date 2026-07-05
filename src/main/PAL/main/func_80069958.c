#include "common.h"

void SetRGBfifo(void *arg0, void *arg1, void *arg2) asm("func_80069958");

void SetRGBfifo(void *arg0, void *arg1, void *arg2) {
    asm("lwc2 $20,0($4)");
    asm("lwc2 $21,0($5)");
    asm("lwc2 $22,0($6)");
}
