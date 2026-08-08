#include "psyq/spu.h"

void _SpuDataCallback(void *callback) {
    DMACallback(4, callback);
}
