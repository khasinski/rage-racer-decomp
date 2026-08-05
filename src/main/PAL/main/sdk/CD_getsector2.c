#include "common.h"
#include "psyq/cd.h"

extern volatile u_char *g_CdReg0 asm("D_80099300");
extern volatile u_char *g_CdReg3 asm("D_8009930C");
extern volatile u_long *g_ComDelayReg asm("D_80099310");
extern volatile u_long *g_CdromDelayReg asm("D_80099334");
extern volatile u_long *g_CdDpcr asm("D_80099338");
extern volatile u_long *g_CdDmaMadr asm("D_8009933C");
extern volatile u_long *g_CdDmaBcr asm("D_80099340");
extern volatile u_long *g_CdDmaChcr asm("D_80099344");
extern long D_800992E4;


extern CdCallback g_CdSyncCallback asm("D_8009903C");
extern CdCallback g_CdReadyCallback asm("D_80099040");
extern u_char g_CdSyncStatus asm("D_80099318");
extern u_char g_CdReadyStatus asm("D_80099319");
extern u_char g_CdSyncResult[] asm("D_8009BAF0");
extern u_char g_CdReadyResult[] asm("D_8009BAF8");

long CdReadInterruptStatus(void);

long CD_getsector2(long arg0, u_long arg1) {
    volatile u_char *status;
    volatile u_long *cdDpcr;
    volatile u_long *cdDmaChcr;

    *g_CdReg0 = 0;
    *g_CdReg3 = 0x80;
    *g_CdromDelayReg = 0x20943;
    *g_ComDelayReg = 0x1323;

    cdDpcr = g_CdDpcr;
    *cdDpcr |= 0x8000;

    *g_CdDmaMadr = arg0;
    *g_CdDmaBcr = arg1 | 0x10000;

    status = g_CdReg0;
    while ((*status & 0x40) == 0) {
    }

    *g_CdDmaChcr = 0x11000000;

    cdDmaChcr = g_CdDmaChcr;
    if ((*cdDmaChcr & 0x1000000) != 0) {
        cdDpcr = cdDmaChcr;
        do {
        } while ((*cdDpcr & 0x1000000) != 0);
    }

    *g_ComDelayReg = 0x1325;
    return 0;
}

void CdSetSectorParam(long arg0);
void CdSetSectorParam(long arg0) {
    D_800992E4 = arg0;
}

void CdDispatchInterrupts(void);
void CdDispatchInterrupts(void) {
    u_char *statusByte;
    long status;
    long saved;

    saved = *g_CdReg0 % 4;
    statusByte = &g_CdReadyStatus;

    while ((status = CdReadInterruptStatus()) != 0) {
        if ((status & 4) != 0) {
            if (g_CdReadyCallback != 0) {
                g_CdReadyCallback(*statusByte, g_CdReadyResult);
            }
        }

        if ((status & 2) != 0) {
            CdCallback doneCallback;
            u_char *resultByte;

            doneCallback = g_CdSyncCallback;
            if (doneCallback != 0) {
                resultByte = &g_CdSyncStatus;
                asm volatile("" : "=r"(resultByte) : "0"(resultByte));
                doneCallback(*resultByte, g_CdSyncResult);
            }
        }
    }

    *g_CdReg0 = saved;
}
