#include "psyq/snd.h"
#include "psyq/spu.h"

extern u_char g_SndVabStatus[] asm("D_801E4CFC");
extern u_short g_SndVabOpenCount asm("D_801F17AC");
extern VabHdr *g_SndVabHeader[] asm("D_8019CA20");
extern short g_SndVabProgMax asm("D_801E40D0");
extern ProgAtr *g_SndVabProgTable[] asm("D_8019C9B0");
extern VagAtr *g_SndVabToneTable[] asm("D_8019CA70");
extern u_long g_SndVabSpuAddr[] asm("D_801F17BC");
extern u_short *g_SndVabBodyAddr[] asm("D_801F180C");
extern long g_SndVabBodySize[] asm("D_801E8AB8");

extern long func_8007B2C0(void);
extern void func_8007B294(long value);

short SsVabOpen(u_char *addr, VabHdr *header);
short SsVabOpen(u_char *addr, VabHdr *header) {
    short vabId;

    vabId = SsVabOpenHead(addr, -1);
    if (vabId != -1) {
        vabId = SsVabTransBody((u_char *)g_SndVabBodyAddr[vabId], vabId);
    }
    return vabId;
}

short SsVabOpenHead(u_char *addr, short vabId) {
    return SsVabOpenHeadWithMode(addr, vabId, 0, 0);
}

short SsVabOpenHeadSticky(u_char *addr, short vabId, u_long spuAddr) {
    return SsVabOpenHeadWithMode(addr, vabId, 1, spuAddr);
}

short SsVabFakeHead(u_char *addr, short vabId, u_long spuAddr);
short SsVabFakeHead(u_char *addr, short vabId, u_long spuAddr) {
    return SsVabOpenHeadWithMode(addr, vabId, 1, spuAddr);
}

short SsVabOpenHeadWithMode(u_char *addr, short vabid, short mode, u_long spuAddr) {
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
            if (g_SndVabStatus[i] == 0) {
                g_SndVabStatus[i] = 1;
                vabId = i;
                g_SndVabOpenCount++;
                break;
            }
        }
    } else {
        cursor = g_SndVabStatus;
        if (cursor[vabid] == 0) {
            g_SndVabStatus[vabid] = 1;
            vabId = vabid;
            g_SndVabOpenCount++;
        }
    }
    if (vabId >= 16) {
        func_8007B294(0);
        return -1;
    }

    cursor = addr;
    g_SndVabHeader[vabId] = (VabHdr *)cursor;
    cursor += sizeof(VabHdr);
    header = (VabHdr *)addr;
    magic = header->form;
    if ((magic >> 8) != (('V' << 16) | ('A' << 8) | 'B')) {
        g_SndVabStatus[vabId] = 0;
        func_8007B294(0);
        g_SndVabOpenCount--;
        return -1;
    }
    if ((magic & 0xFF) == 'p') {
        if (header->ver >= 5) {
            g_SndVabProgMax = 0x80;
        } else {
            g_SndVabProgMax = 0x40;
        }
    } else {
        g_SndVabProgMax = 0x40;
    }
    if (header->ps > g_SndVabProgMax) {
        g_SndVabStatus[vabId] = 0;
        func_8007B294(0);
        g_SndVabOpenCount--;
        return -1;
    }

    g_SndVabProgTable[vabId] = (ProgAtr *)cursor;
    programTable = (ProgAtr *)cursor;
    cursor += g_SndVabProgMax * sizeof(ProgAtr);
    totalSize = 0;
    for (i = 0; i < g_SndVabProgMax; i++) {
        programTable[i].reserved1 = totalSize;
        if (programTable[i].tones != 0) {
            totalSize++;
        }
    }

    totalSize = 0;
    g_SndVabToneTable[vabId] = (VagAtr *)cursor;
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

    g_SndVabBodyAddr[vabId] = vagOffsetTable;
    allocation = spuAddr;
    if (mode == 0) {
        allocation = SpuMalloc(totalSize);
        if (allocation == -1) {
            g_SndVabStatus[vabId] = 0;
            func_8007B294(0);
            g_SndVabOpenCount--;
            return -1;
        }
    }
    if ((allocation + totalSize) > 0x80000U) {
        g_SndVabStatus[vabId] = 0;
        func_8007B294(0);
        g_SndVabOpenCount--;
        return -1;
    }

    g_SndVabSpuAddr[vabId] = allocation;
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

    g_SndVabBodySize[vabId] = totalSize;
    g_SndVabStatus[vabId] = 2;
    return vabId;
}
