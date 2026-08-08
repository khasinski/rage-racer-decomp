#include "psyq/spu.h"
#include "psyq/spu_internal.h"

void _SpuDataCallback(void *callback) {
    DMACallback(4, callback);
}
