#include <stdio.h>
#include <stdlib.h>
#include "io_raw.h"
#include "cli.h"

//main函数是程序的入口，argc是命令行参数的个数，argv是命令行参数的数组
int main(int argc, char **argv)
{
   return cli_parse(argc, argv);
}
