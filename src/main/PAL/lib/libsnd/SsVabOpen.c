#include "psyq/snd.h"
#include "psyq/spu.h"

extern u_char D_801E4CFC[];
extern u_short D_801F17AC;
extern VabHdr *D_8019CA20[];
extern short D_801E40D0;
extern ProgAtr *D_8019C9B0[];
extern VagAtr *D_8019CA70[];
extern u_long D_801F17BC[];
extern u_short *D_801F180C[];
extern long D_801E8AB8[];

extern long func_8007B2C0(void);
extern void func_8007B294(long value);

short func_80072BC0(u_char *addr, VabHdr *header) {
    short vabId;

    vabId = SsVabOpenHead(addr, -1);
    if (vabId != -1) {
        vabId = SsVabTransBody((u_char *)D_801F180C[vabId], vabId);
    }
    return vabId;
}

short func_80072C18(u_char *addr, short vabId) {
    return SsVabOpenHeadWithMode(addr, vabId, 0, 0);
}

short func_80072C4C(u_char *addr, short vabId, u_long spuAddr) {
    return SsVabOpenHeadWithMode(addr, vabId, 1, spuAddr);
}

short func_80072C80(u_char *addr, short vabId, u_long spuAddr) {
    return SsVabOpenHeadWithMode(addr, vabId, 1, spuAddr);
}

short func_80072CB4(u_char *addr, short vabid, short mode, u_long spuAddr) {
    long vagLengths[256];
    long i;
    long totalSize;
    short vabId;
    u_short length;
    u_short *vagOffsetTable;
    u_long magic;
    u_long allocation;
    u_char vagCount;
    ProgAtr *programTable;
    u_char *cursor;
    VabHdr *header;

    vabId = 16;
    if (func_8007B2C0() == 1) {
        return -1;
    }
    func_8007B294(1);
    if (vabid >= 16) {
        func_8007B294(0);
        return -1;
    }
    if (vabid == -1) {
        for (i = 0; i < 16; i++) {
            if (D_801E4CFC[i] == 0) {
                D_801E4CFC[i] = 1;
                vabId = i;
                D_801F17AC++;
                break;
            }
        }
    } else {
        cursor = D_801E4CFC;
        if (cursor[vabid] == 0) {
            D_801E4CFC[vabid] = 1;
            vabId = vabid;
            D_801F17AC++;
        }
    }
    if (vabId >= 16) {
        func_8007B294(0);
        return -1;
    }

    cursor = addr;
    D_8019CA20[vabId] = (VabHdr *)cursor;
    cursor += sizeof(VabHdr);
    header = (VabHdr *)addr;
    magic = header->form;
    if ((magic >> 8) != (('V' << 16) | ('A' << 8) | 'B')) {
        D_801E4CFC[vabId] = 0;
        func_8007B294(0);
        D_801F17AC--;
        return -1;
    }
    if ((magic & 0xFF) == 'p') {
        if (header->ver >= 5) {
            D_801E40D0 = 0x80;
        } else {
            D_801E40D0 = 0x40;
        }
    } else {
        D_801E40D0 = 0x40;
    }
    if (header->ps > D_801E40D0) {
        D_801E4CFC[vabId] = 0;
        func_8007B294(0);
        D_801F17AC--;
        return -1;
    }

    D_8019C9B0[vabId] = (ProgAtr *)cursor;
    programTable = (ProgAtr *)cursor;
    cursor += D_801E40D0 * sizeof(ProgAtr);
    totalSize = 0;
    for (i = 0; i < D_801E40D0; i++) {
        programTable[i].reserved1 = totalSize;
        if (programTable[i].tones != 0) {
            totalSize++;
        }
    }

    totalSize = 0;
    D_8019CA70[vabId] = (VagAtr *)cursor;
    vagOffsetTable = (u_short *)(cursor + (header->ps << 9));
    vagCount = header->vs;
    for (i = 0; i < 256; i++) {
        if (vagCount >= i) {
            length = *vagOffsetTable;
            if (header->ver >= 5) {
                vagLengths[i] = length * 8;
            } else {
                vagLengths[i] = length * 4;
            }
            totalSize += vagLengths[i];
        }
        vagOffsetTable++;
    }

    D_801F180C[vabId] = vagOffsetTable;
    allocation = spuAddr;
    if (mode == 0) {
        allocation = SpuMalloc(totalSize);
        if (allocation == -1) {
            D_801E4CFC[vabId] = 0;
            func_8007B294(0);
            D_801F17AC--;
            return -1;
        }
    }
    if ((allocation + totalSize) > 0x80000U) {
        D_801E4CFC[vabId] = 0;
        func_8007B294(0);
        D_801F17AC--;
        return -1;
    }

    D_801F17BC[vabId] = allocation;
    totalSize = 0;
    for (i = 0; i <= vagCount; i++) {
        totalSize += vagLengths[i];
        if (!(i & 1)) {
            ((short *)&programTable[i / 2].reserved2)[0] =
                (allocation + totalSize) >> 3;
        } else {
            ((short *)&programTable[i / 2].reserved2)[1] =
                (allocation + totalSize) >> 3;
        }
    }

    D_801E8AB8[vabId] = totalSize;
    D_801E4CFC[vabId] = 2;
    return vabId;
}
