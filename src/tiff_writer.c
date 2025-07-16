/* src/tiff_writer.c
 * 手动写 BigTIFF 32-bit float 单页，足够让 Photoshop/RawTherapee 识别
 */
#include "tiff_writer.h"
#include <stdio.h>
#include <stdint.h>

int tiff32_write(const char *path, const float *img, int w, int h)
{
    FILE *fp = fopen(path, "wb");
    if (!fp) return -1;

    /* --------------- TIFF 头 --------------- */
    uint16_t header[] = {
        0x4949, 42,              // little-endian + 42
        8, 0,                    // 8-byte offset to IFD
    };
    fwrite(header, sizeof(header), 1, fp);

    /* --------------- IFD 目录（8 个条目） --------------- */
    typedef struct {
        uint16_t tag, type;
        uint32_t count, value;
    } TIFF_ENTRY;

    uint32_t strip_offset = ftell(fp) + 8 + 8 * 12 + 4;
    TIFF_ENTRY ifd[] = {
        {256, 4, 1, w},               // ImageWidth
        {257, 4, 1, h},               // ImageLength
        {258, 4, 1, 32},              // BitsPerSample = 32
        {259, 3, 1, 1},               // Compression = None
        {262, 3, 1, 1},               // Photometric = BlackIsZero
        {273, 4, 1, strip_offset},    // StripOffsets
        {278, 4, 1, h},               // RowsPerStrip = h
        {279, 4, 1, w * h * 4}        // StripByteCounts
    };
    fwrite(ifd, sizeof(ifd), 1, fp);

    /* 目录结束符 */
    uint32_t zero = 0;
    fwrite(&zero, 4, 1, fp);

    /* --------------- 像素数据 --------------- */
    fwrite(img, sizeof(float), (size_t)w * h, fp);
    fclose(fp);
    return 0;
}