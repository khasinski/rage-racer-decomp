#include "common.h"

void MDEC_out(void);
void MDEC_in_sync(void);
void MDEC_out_sync(void);
void DMACallback(long arg0, long arg1);

void DecDCTout(void) asm("func_8006402C");
void DecDCTout(void) {
    MDEC_out();
}

void DecDCTinSync(void) asm("func_8006404C");
void DecDCTinSync(void) {
    MDEC_in_sync();
}

void DecDCToutSync(void) asm("func_8006406C");
void DecDCToutSync(void) {
    MDEC_out_sync();
}

void DecDCTinCallback(long arg0) asm("func_8006408C");
void DecDCTinCallback(long arg0) {
    DMACallback(0, arg0);
}

void DecDCToutCallback(long arg0) asm("func_800640B0");
void DecDCToutCallback(long arg0) {
    DMACallback(1, arg0);
}
