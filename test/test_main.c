#include "unity.h"
#include "../src/io_raw.h"
#include <stdlib.h> 

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_raw_read(void) {
    uint16_t *img = raw12_read("../test_imgs/sample_512x512_12bit.raw",512,512);
    TEST_ASSERT_NOT_NULL(img);
    TEST_ASSERT_EQUAL_UINT16(0,img[0]);
    free(img);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_raw_read);
    return UNITY_END();
}