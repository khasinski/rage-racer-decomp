#include <sys/types.h>

#include "common.h"
#include "psyq/spu_internal.h"


long SpuTransferStatus(long addr, long mode) {
    long size;
    u_long status;

    switch (mode) {
    case 5:
        mode = 0;
        size = 0x20;
        break;
    case 6:
        mode = 0x100;
        addr += 0x800;
        size = 0x20;
        break;
    default:
        mode = 0;
        size = 0x40;
        break;
    }

    _spu_startDmaTransfer(addr, mode, size);
    status = g_SpuRegBase->raw[0xD7] & 0x800;
    return status > 0;
}
