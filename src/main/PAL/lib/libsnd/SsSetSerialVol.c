#include "psyq/spu.h"


void SsSetSerialVol(u8 source, s16 left, s16 right) {
    SpuCommonAttr attr;

    if (source == 0) {
        attr.mask = 0xC0;
        if (left >= 0x80) {
            left = 0x7F;
        }
        if (right >= 0x80) {
            right = 0x7F;
        }
        attr.cd.volume.left = left * 0x102;
        attr.cd.volume.right = right * 0x102;
    }

    if (source == 1) {
        attr.mask = 0xC00;
        if (left >= 0x80) {
            left = 0x7F;
        }
        if (right >= 0x80) {
            right = 0x7F;
        }
        attr.ext.volume.left = left * 0x102;
        attr.ext.volume.right = right * 0x102;
    }

    SpuSetCommonAttr(&attr);
}
