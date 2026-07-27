#include "psyq/spu.h"


void SsSetMVol(short left, short right) {
    long left_s = left;
    long right_s = right;
    SpuCommonAttr attr;

    attr.mask = 3;
    attr.mvol.volume.left = (left_s << 7) + left_s;
    attr.mvol.volume.right = (right_s << 7) + right_s;
    SpuSetCommonAttr(&attr);
}
