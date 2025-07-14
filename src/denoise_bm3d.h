#ifndef DENOISE_BM3D_H
#define DENOISE_BM3D_H
#include <stdint.h>

/* 8×8 分块硬阈值 BM3D-Lite
 * src   : 16-bit 灰度  (w×h)
 * dst   : 输出 16-bit
 * sigma : 噪声标准差（0–4095）
 */
void bm3d_lite(const uint16_t *src,
               uint16_t       *dst,
               int w, int h,
               float sigma);
#endif