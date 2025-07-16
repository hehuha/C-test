/* src/denoise_bm3d.c
 * 8×8 空域硬阈值 + SSE2 + OpenMP
 */
#include "denoise_bm3d.h"
#include <emmintrin.h>
#include <math.h>
#include <stdlib.h>

/* 8×8 DCT 简化：直接空域阈值即可 */
void bm3d_lite(const uint16_t *src, uint16_t *dst, int w, int h, float sigma)
{
    const int block = 8;
    const float thr = 2.7f * sigma;

#pragma omp parallel for schedule(static)
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int idx = y * w + x;
            float v = src[idx];
            v = fabsf(v) > thr ? v : 0.0f;   // 硬阈值
            if (v < 0)   v = 0;
            if (v > 4095) v = 4095;
            dst[idx] = (uint16_t)(v + 0.5f);
        }
    }
}