#include "psyq/snd.h"
#include "psyq/spu.h"

extern u8 D_801E4CFC[];
extern u16 D_801F17AC;
extern VabHdr *D_8019CA20[];
extern s16 D_801E40D0;
extern ProgAtr *D_8019C9B0[];
extern VagAtr *D_8019CA70[];
extern u32 D_801F17BC[];
extern u16 *D_801F180C[];
extern s32 D_801E8AB8[];

extern s32 func_8007B2C0(void);
extern void func_8007B294(s32 value);

s16 func_80072BC0(u8 *addr, VabHdr *header) {
    s16 vabId;

    vabId = SsVabOpenHead(addr, -1);
    if (vabId != -1) {
        vabId = SsVabTransBody((u8 *)D_801F180C[vabId], vabId);
    }
    return vabId;
}

s16 func_80072C18(u8 *addr, s16 vabId) {
    return SsVabOpenHeadWithMode(addr, vabId, 0, 0);
}

s16 func_80072C4C(u8 *addr, s16 vabId, u32 spuAddr) {
    return SsVabOpenHeadWithMode(addr, vabId, 1, spuAddr);
}

s16 func_80072C80(u8 *addr, s16 vabId, u32 spuAddr) {
    return SsVabOpenHeadWithMode(addr, vabId, 1, spuAddr);
}

s16 func_80072CB4(u8 *addr, s16 vabid, s16 mode, u32 spuAddr) {
    s32 vagLengths[256];
    s32 i;
    s32 totalSize;
    s16 vabId;
    u16 length;
    u16 *vagOffsetTable;
    u32 magic;
    u32 allocation;
    u8 vagCount;
    ProgAtr *programTable;
    u8 *cursor;
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
    vagOffsetTable = (u16 *)(cursor + (header->ps << 9));
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
            ((s16 *)&programTable[i / 2].reserved2)[0] =
                (allocation + totalSize) >> 3;
        } else {
            ((s16 *)&programTable[i / 2].reserved2)[1] =
                (allocation + totalSize) >> 3;
        }
    }

    D_801E8AB8[vabId] = totalSize;
    D_801E4CFC[vabId] = 2;
    return vabId;
}
