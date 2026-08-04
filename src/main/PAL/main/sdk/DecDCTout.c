#include "common.h"

void MDEC_out(void) asm("func_80064264");
void MDEC_in_sync(void) asm("func_800642F4");
void MDEC_out_sync(void) asm("func_8006438C");
void DMACallback(long arg0, long arg1) asm("func_8006DF94");

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
