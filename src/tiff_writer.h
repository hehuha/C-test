#ifndef TIFF_WRITER_H
#define TIFF_WRITER_H
int tiff32_write(const char *path,
                 const float *img,
                 int w, int h);
#endif