#include "common.h"
#include "psyq/kernel.h"

void DecDCTout(void) {
    MDEC_out();
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
