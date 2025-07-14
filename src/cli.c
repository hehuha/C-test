/* src/cli.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io_raw.h"
#include "denoise_bm3d.h"
#include "hdr_fusion.h"

/* ---------- 帮助 ---------- */
static void usage(const char *prog)
{
    fprintf(stderr,
            "Usage:\n"
            "  %s denoise <raw> -w <w> -h <h> -o <out.pgm> [-s <sigma>]\n"
            "  %s hdr <raw1> <raw2> ... -o <out.tiff>\n"
            "Example:\n"
            "  raw-hdr denoise sample.raw -w 512 -h 512 -o out.pgm -s 50\n",
            prog, prog);
}

/*--------------路由表---------------------*/

/* ---------- denoise ---------- */
static int cmd_denoise(int argc, char **argv)
{
    /* 解析参数：顺序固定 raw -w w -h h -o out [-s sigma] */
    if (argc < 9) { usage(argv[0]); return 1; }

    const char *in  = argv[2];
    int w = atoi(argv[4]);//atoi用于将字符串转换为整数
    int h = atoi(argv[6]);
    const char *out = argv[8];//const用于声明指针变量，指针变量的值不能被修改
    float sigma = (argc > 9 && !strcmp(argv[9], "-s")) ? (float)atof(argv[10]) : 50.0f;

    /* 读取 → 去噪 → 写出 */
    uint16_t *src = raw12_read(in, w, h);
    if (!src) return 1;

    uint16_t *dst = malloc((size_t)w * h * sizeof(uint16_t));
    if (!dst) { free(src); return 1; }

    bm3d_lite(src, dst, w, h, sigma);

    if (pgm16_write(out, dst, w, h) == 0)
        printf("Denoised -> %s\n", out);

    free(src);
    free(dst);
    return 0;
}

/* ----------------- hdr ---------------- */
static int cmd_hdr(int argc, char **argv)
{
    if (argc < 4) { usage(argv[0]); return 1; }

    int n = 0;
    while (n < argc - 3 && strcmp(argv[2 + n], "-o") != 0) ++n;
    if (n < 2) { usage(argv[0]); return 1; }

    /* 简易：固定曝光 1, 0.5, 0.25... */
    float expo[8];
    for (int i = 0; i < n; ++i) expo[i] = 1.0f / (1 << i);

    const char **files = (const char **)(argv + 2);
    const char *out    = argv[2 + n + 1];

    int w = 512, h = 512;   /* 实际应从第一帧读 meta */
    hdr_merge(files, n, w, h, expo, out);
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