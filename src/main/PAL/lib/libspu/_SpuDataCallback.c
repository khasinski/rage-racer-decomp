#include "psyq/spu.h"

void DMACallback(long spec, void *callback);

void _SpuDataCallback(void *callback) {
    DMACallback(4, callback);
}
