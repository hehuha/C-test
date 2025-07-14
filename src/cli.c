#include <stdio.h>
#include <string.h>
#include "cli.h"

static void usage(const char *prog)
{
    fprintf(stderr,
            "Usage:\n"
            "  %s denoise <raw> -w <w> -h <h> -o <out>\n"
            "  %s hdr <raw1> <raw2> ... -o <out>\n", prog);
}

int cli_parse(int argc, char **argv)//cli_parse() 函数的作用是解析命令行参数
{
    if (argc < 2) {usage(argv[0]);return 1;}//usage() 作用是打印出命令行的使用方法
    if (strcmp(argv[1], "denoise") == 0) // strcmp() is used to compare two strings, returns 0 if they are equal
    {
        if(argc < 8) { usage(argv[0]);return 1;}
        printf("denoise mode:%s %s %s %s %s %s\n", argv[2], argv[3], argv[4], argv[5], argv[6], argv[7]);      
    }else if (strcmp(argv[1], "hdr") == 0){
        printf("HDR mode:%d frames\n ",argc-3); 
    }else{usage(argv[0]);return 1;}
    return 0;
}