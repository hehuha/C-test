cmake_minimum_required(VERSION 3.15)
project(C-test C)#设置项目名称为C-test，语言为C

#强制c11标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

#声明raw-hdr最终可执行文件，将三个源文件编译成一个可执行文件
add_executable(raw-hdr
src/io_raw.c
src/cli.c
src/denoise_bm3d.c
src/main.c
src/hdr_fusion.c
src/tiff_writer.c)

#-O3优化，-march=native优化，-fopenmp使用openmp
find_package(OpenMP REQUIRED)
target_compile_options(raw-hdr PRIVATE  $<$<CONFIG:Release>:-O3 -march=native -fopenmp>)
target_link_libraries(raw-hdr  PRIVATE OpenMP::OpenMP_C)#使用openmp

#单独测试可执行文件
add_executable(test_raw
test/test_main.c
src/io_raw.c
test/unity.c
test/test_denoise.c
src/denoise_bm3d.c
src/hdr_fusion.c)
