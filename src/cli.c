/* src/cli.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io_raw.h"
#include "denoise_bm3d.h"
#include "tiff_writer.h"
#include "hdr_fusion.h"

/* ---------- 帮助 ---------- */
static void usage(const char *prog)
{
    fprintf(stderr,
            "Usage:\n"
            "  %s denoise <raw> -w <w> -h <h> -o <out.tiff> [-s <sigma>]\n"
            "  %s hdr <raw1> <raw2> ... -o <out.tiff>\n"
            "Example:\n"
            "  raw-hdr denoise sample.raw -w 512 -h 512 -o out.tiff -s 50\n",
            prog, prog);
}

/* ---------- denoise ---------- */
static int cmd_denoise(int argc, char **argv)
{
    if (argc < 9) { usage(argv[0]); return 1; }

    const char *in  = argv[2];
    int w = atoi(argv[4]);
    int h = atoi(argv[6]);
    const char *out = argv[8];
    float sigma = (argc > 9 && !strcmp(argv[9], "-s")) ? (float)atof(argv[10]) : 50.0f;

    uint16_t *src = raw12_read(in, w, h);
    if (!src) return 1;

    uint16_t *dst16 = malloc((size_t)w * h * sizeof(uint16_t));
    if (!dst16) { free(src); return 1; }

    bm3d_lite(src, dst16, w, h, sigma);

    /* 转 32-bit float 再写 TIFF */
    float *dst32 = malloc((size_t)w * h * sizeof(float));
    if (!dst32) { free(src); free(dst16); return 1; }
    for (size_t i = 0; i < (size_t)w * h; ++i)
        dst32[i] = dst16[i] / 4095.0f;   // 归一化到 0-1

    if (tiff32_write(out, dst32, w, h) == 0)
        printf("Denoised -> %s\n", out);

    free(src); free(dst16); free(dst32);
    return 0;
}

/* ---------- hdr ---------- */
static int cmd_hdr(int argc, char **argv)
{
    if (argc < 4) { usage(argv[0]); return 1; }

    /* 找到 -o 位置，前面都是 RAW 文件 */
    int n = 0;
    while (n < argc - 3 && strcmp(argv[2 + n], "-o") != 0) ++n;
    if (n < 2) { usage(argv[0]); return 1; }

    const char **files = (const char **)(argv + 2);
    const char *out    = argv[2 + n + 1];
    int w = 512, h = 512;                // 示例尺寸，可扩展读 meta
    float expo[] = {1.0f, 0.5f, 0.25f}; // 示例曝光比

    if (hdr_merge(files, n, w, h, expo, out) == 0)
        printf("HDR merged -> %s\n", out);
    return 0;
}

/* ---------- 统一入口 ---------- */
int cli_parse(int argc, char **argv)
{
    if (argc < 2) { usage(argv[0]); return 1; }

    if (strcmp(argv[1], "denoise") == 0)
        return cmd_denoise(argc, argv);
    else if (strcmp(argv[1], "hdr") == 0)
        return cmd_hdr(argc, argv);
    else
    { usage(argv[0]); return 1; }
}