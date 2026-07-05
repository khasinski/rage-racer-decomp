#include "psyq/spu.h"
#include "psyq/kernel.h"

extern s32 D_8009AB70;
extern s32 D_8009A710;
extern s32 D_8009A768;

void SpuStart(void) {
    s32 temp;

    if (D_8009AB70 == 0) {
        D_8009AB70 = 1;
        EnterCriticalSection();
        D_8009A710 = 0;
        _SpuDataCallback(_spu_transferCallback);
        temp = OpenEvent(0xF0000009, 0x20, 0x2000, 0);
        D_8009A768 = temp;
        EnableEvent(temp);
        ExitCriticalSection();
    }
}
