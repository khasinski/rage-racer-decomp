#include "common.h"
#include "game/render.h"
#include "psyq/gte.h"

extern Matrix g_SceneLightMatrix asm("D_8009E6AC");

/* Loads the GTE light matrix with g_SceneLightMatrix * `view`, working on a
 * local copy so the caller's view matrix is left alone. */
void GameSetGteLightMatrix(Matrix *view) {
    Matrix m;

    m = *view;
    MulMatrix2(&g_SceneLightMatrix, &m);
    SetLightMatrix(&m);
}
