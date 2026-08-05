#include "common.h"

void MDEC_out(void);
void MDEC_in_sync(void);
void MDEC_out_sync(void);
void DMACallback(long arg0, long arg1);

void DecDCTout(void) {
    MDEC_out();
}

void DecDCTinSync(void) {
    MDEC_in_sync();
}

void DecDCToutSync(void) {
    MDEC_out_sync();
}

void DecDCTinCallback(long arg0) {
    DMACallback(0, arg0);
}

void DecDCToutCallback(long arg0) {
    DMACallback(1, arg0);
}
