#include "psyq/spu.h"


void SsSetSpuInputAttr(u8 source, u8 field, u8 value) {
    SpuCommonAttr attr;

    if (source == 0) {
        if (field == 0) {
            attr.mask = 0x200;
            attr.cd.mix = value;
        }
        if (field == 1) {
            attr.mask = 0x100;
            attr.cd.reverb = value;
        }
    }

    if (source == 1) {
        if (field == 0) {
            attr.mask = 0x2000;
            attr.ext.mix = value;
        }
        if (field == 1) {
            attr.mask = 0x1000;
            attr.ext.reverb = value;
        }
    }

    SpuSetCommonAttr(&attr);
}
