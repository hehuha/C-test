#include <stdio.h>
#include <stdlib.h>
#include "io_raw.h"

int main(int argc, char **argv)
{
    if (argc != 5) {
        fprintf(stderr,
                "Usage: %s <raw12> <width> <height> <out.pgm>\n"
                "Example: raw-hdr test.raw 512 512 out.pgm\n",
                argv[0]);
        return 1;
    }

    const char *raw_path = argv[1];
    int w = atoi(argv[2]);
    int h = atoi(argv[3]);
    const char *pgm_path = argv[4];

    uint16_t *img = raw12_read(raw_path, w, h);
    if (!img) return 1;

    if (pgm16_write(pgm_path, img, w, h) == 0)
        printf("Converted -> %s\n", pgm_path);

    free(img);
    return 0;
}