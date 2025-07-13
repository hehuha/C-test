#ifndef IO_RAW_H
#define IO_RAW_H

#include <stdint.h>

/* 读取 12-bit packed RAW (16-bit container, big-endian) */
uint16_t *raw12_read(const char *path, int width, int height);

/* 保存 16-bit PGM (P5) */
int pgm16_write(const char *path, const uint16_t *img, int width, int height);

#endif /* IO_RAW_H */