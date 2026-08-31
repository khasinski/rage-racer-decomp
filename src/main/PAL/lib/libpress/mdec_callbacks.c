#include <sys/types.h>

#include "common.h"
#include "psyq/kernel.h"
#include "psyq/press_internal.h"

void DecDCTout(volatile u_long *buf, long size) {
    MDEC_out(buf, size);
}

void DecDCTinSync(void) {
    MDEC_in_sync();
}

void DecDCToutSync(void) {
    MDEC_out_sync();
}

void DecDCTinCallback(long callback) {
    DMACallback(0, callback);
}

void DecDCToutCallback(long callback) {
    DMACallback(1, callback);
}
