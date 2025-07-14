#include "denoise_bm3d.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

static inline float hard_thr(float x, float T)
{
    return fabsf(x) > T ? x : 0.0f;
}

void bm3d_lite(const uint16_t *src,
               uint16_t       *dst,
               int w, int h,
               float sigma)
{
    const int block = 8;
    const int B = w * h;
    float   *tmp = (float *)malloc(B * sizeof(float));
    float    thr = 2.7f * sigma;           // 经验阈值

    /* 1. 转 float & 减去均值（简化） */
    float mean = 0.0f;
    for (int i = 0; i < B; ++i) mean += src[i];
    mean /= B;
    for (int i = 0; i < B; ++i) tmp[i] = src[i] - mean;

    /* 2. 8×8 DCT（可换 SSE）→ 硬阈值 → 逆 DCT（省略，用空域近似） */
    /* 简化：直接空域硬阈值 */
    for (int i = 0; i < B; ++i)
        tmp[i] = hard_thr(tmp[i], thr);

    /* 3. 加回均值并转回 16-bit */
    for (int i = 0; i < B; ++i) {
        float v = tmp[i] + mean;
        if (v < 0)   v = 0;
        if (v > 4095) v = 4095;
        dst[i] = (uint16_t)(v + 0.5f);
    }
    free(tmp);
}