#include "io_raw.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
 
/* 把大端 16-bit 转主机字节序 */
static uint16_t be16_to_cpu(uint16_t v)
{
    unsigned char *p = (unsigned char *)&v;//&v 取变量v的首地址，即变量v的指针Address
    return ((uint16_t)p[0] << 8) | p[1];//高位字节在前，低位字节在后，所以用位移操作符 << 8 和 | 组合起来
}
 
/* 从指定路径读取 raw12 格式的图像数据 */
uint16_t *raw12_read(const char *path, int w, int h)
{
    FILE *fp = fopen(path, "rb");/* 以二进制读模式(rb)打开文件 */
    if (!fp) { perror("open"); return NULL; }//perror()函数用来打印出系统错误信息
 
    size_t n = (size_t)w * h;
    uint16_t *buf = malloc(n * sizeof(uint16_t));
    if (!buf) { fclose(fp); return NULL; }
 
    uint16_t tmp;
    for (size_t i = 0; i < n; ++i) {
        fread(&tmp, sizeof(tmp), 1, fp);
        buf[i] = be16_to_cpu(tmp) >> 4;   /* 高 12 位 >>4 得 0-4095 */
    }
    fclose(fp);
    return buf;
}
 
/* 将 16-bit 图像数据写入指定路径的 PGM 文件 */
int pgm16_write(const char *path, const uint16_t *img, int w, int h)
{
    FILE *fp = fopen(path, "wb");
    if (!fp) { perror("open"); return -1; }
 
    fprintf(fp, "P5\n%d %d\n4095\n", w, h);  /* 最大值为 4095 */
    fwrite(img, sizeof(uint16_t), (size_t)w * h, fp);
    fclose(fp);
    return 0;
}