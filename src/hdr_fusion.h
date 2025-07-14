#ifndef HDR_FUSION_H
#define HDR_FUSION_H
#include <stdint.h>

/* 融合 n 张 12-bit RAW → 32-bit HDR
 * files : 文件路径数组
 * w,h   : 尺寸（所有图相同）
 * expo  : 曝光时间数组（秒）
 * out   : 输出 32-bit float TIFF
 */
int hdr_merge(const char **files,
              int n,
              int w, int h,
              const float *expo,
              const char *out_tiff);
#endif