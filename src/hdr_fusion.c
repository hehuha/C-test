#include "hdr_fusion.h"
#include "io_raw.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

static inline float weight(float val)
{
    /* 线性权重：远离过曝/欠曝 */
    float t = val / 4095.0f;
    return fminf(t, 1.0f - t) * 4.0f;
}

int hdr_merge(const char **files, int n, int w, int h,
              const float *expo, const char *out_tiff)
{
    /* 一次性分配内存 */
    size_t N = (size_t)w * h;
    float  *hdr = calloc(N, sizeof(float));
    float  *acc = calloc(N, sizeof(float)); /* 权重累加 */

    for (int i = 0; i < n; ++i) {
        uint16_t *raw = raw12_read(files[i], w, h);
        if (!raw) goto fail;

        for (size_t k = 0; k < N; ++k) {
            float v   = raw[k] / 4095.0f;      // 归一化
            float w   = weight(raw[k]);
            hdr[k]   += w * v / expo[i];       // 加权累积辐射度
            acc[k]   += w;
        }
        free(raw);
    }

    /* 归一化并写 TIFF（占位：先写 32-bit PGM） */
    for (size_t k = 0; k < N; ++k)
        hdr[k] = (acc[k] > 0.0f) ? hdr[k] / acc[k] : 0.0f;

    /* 临时用 32-bit PGM 代替 TIFF */
    FILE *fp = fopen(out_tiff, "wb");
    if (!fp) goto fail;
    fprintf(fp, "Pf\n%d %d\n-1.0\n", w, h);  // 32-bit PGM 头
    fwrite(hdr, sizeof(float), N, fp);
    fclose(fp);

    free(hdr); free(acc);
    return 0;

fail:
    free(hdr); free(acc);
    return 1;
}