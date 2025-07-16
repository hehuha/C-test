# raw-hdr-cli  
**纯 C 跨平台 RAW 去噪 & HDR 工具**

在 8 线程 i7-11800H 上，4K 12-bit RAW **去噪 < 0.15 s，HDR < 0.4 s**，零第三方库。

git clone https://github.com/<your-name>/raw-hdr-cli.git
cmake -B build && cmake --build build
build/raw-hdr denoise sample.raw -w 4032 -h 3024 -o out.tiff -s 60