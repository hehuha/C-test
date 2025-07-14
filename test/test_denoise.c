#include "unity.h"
#include "../src/denoise_bm3d.h"

void test_bm3d_zero_noise(void)
{
    uint16_t in[64]  = {0};
    uint16_t out[64];
    bm3d_lite(in, out, 8, 8, 0.0f);
    TEST_ASSERT_EQUAL_UINT16(0, out[0]);
}
