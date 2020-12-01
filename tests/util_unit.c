/*
 * util_unit.c
 *
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <roaring/bitset_util.h>

#ifdef __cplusplus  // stronger type checking errors if C built in C++ mode
using namespace roaring::internal;
#endif

#include "test.h"

DEFINE_TEST(setandextract_uint16) {
    const unsigned int bitset_size = 1 << 16;
    const unsigned int bitset_size_in_words =
        bitset_size / (sizeof(uint64_t) * 8);

    for (unsigned int offset = 1; offset < bitset_size; offset++) {
        const unsigned int valsize = bitset_size / offset;
        uint16_t* vals = (uint16_t*)malloc(valsize * sizeof(uint16_t));
        uint64_t* bitset =
            (uint64_t*)calloc(bitset_size_in_words, sizeof(uint64_t));
        for (unsigned int k = 0; k < valsize; ++k) {
            vals[k] = (uint16_t)(k * offset);
        }

        bitset_set_list(bitset, vals, valsize);
        uint16_t* newvals = (uint16_t*)malloc(valsize * sizeof(uint16_t));
        bitset_extract_setbits_uint16(bitset, bitset_size_in_words, newvals, 0);

        for (unsigned int k = 0; k < valsize; ++k) {
            assert_int_equal(newvals[k], vals[k]);
        }

        free(vals);
        free(newvals);
        free(bitset);
    }
}

#ifdef IS_X64
DEFINE_TEST(setandextract_sse_uint16) {
    const unsigned int bitset_size = 1 << 16;
    const unsigned int bitset_size_in_words =
        bitset_size / (sizeof(uint64_t) * 8);

    for (unsigned int offset = 1; offset < bitset_size; offset++) {
        const unsigned int valsize = bitset_size / offset;
        uint16_t* vals = (uint16_t*)malloc(valsize * sizeof(uint16_t));
        uint64_t* bitset =
            (uint64_t*)calloc(bitset_size_in_words, sizeof(uint64_t));
        for (unsigned int k = 0; k < valsize; ++k) {
            vals[k] = (uint16_t)(k * offset);
        }

        bitset_set_list(bitset, vals, valsize);
        uint16_t* newvals = (uint16_t*)malloc(valsize * sizeof(uint16_t) + 64);
        bitset_extract_setbits_sse_uint16(bitset, bitset_size_in_words, newvals,
                                          valsize, 0);

        for (unsigned int k = 0; k < valsize; ++k) {
            assert_int_equal(newvals[k], vals[k]);
        }

        free(vals);
        free(newvals);
        free(bitset);
    }
}
#endif

DEFINE_TEST(setandextract_uint32) {
    const unsigned int bitset_size = 1 << 16;
    const unsigned int bitset_size_in_words =
        bitset_size / (sizeof(uint64_t) * 8);

    for (unsigned int offset = 1; offset < bitset_size; offset++) {
        const unsigned int valsize = bitset_size / offset;
        uint16_t* vals = (uint16_t*)malloc(valsize * sizeof(uint16_t));
        uint64_t* bitset =
            (uint64_t*)calloc(bitset_size_in_words, sizeof(uint64_t));

        for (unsigned int k = 0; k < valsize; ++k) {
            vals[k] = (uint16_t)(k * offset);
        }

        bitset_set_list(bitset, vals, valsize);
        uint32_t* newvals = (uint32_t*)malloc(valsize * sizeof(uint32_t));
        bitset_extract_setbits(bitset, bitset_size_in_words, newvals, 0);

        for (unsigned int k = 0; k < valsize; ++k) {
            assert_int_equal(newvals[k], vals[k]);
        }

        free(vals);
        free(newvals);
        free(bitset);
    }
}

#ifdef USE_AVX
// returns 1 when ok
void setandextract_avx2_uint32() {
    const unsigned int bitset_size = 1 << 16;
    const unsigned int bitset_size_in_words =
        bitset_size / (sizeof(uint64_t) * 8);

    for (unsigned int offset = 1; offset < bitset_size; offset++) {
        const unsigned int valsize = bitset_size / offset;
        uint16_t* vals = malloc(valsize * sizeof(uint16_t));
        uint64_t* bitset = calloc(bitset_size_in_words, sizeof(uint64_t));

        for (unsigned int k = 0; k < valsize; ++k) {
            vals[k] = (uint16_t)(k * offset);
        }

        bitset_set_list(bitset, vals, valsize);
        uint32_t* newvals = malloc(valsize * sizeof(uint32_t) + 64);
        bitset_extract_setbits_avx2(bitset, bitset_size_in_words, newvals,
                                    valsize, 0);

        for (unsigned int k = 0; k < valsize; ++k) {
            assert_int_equal(newvals[k], vals[k]);
        }

        free(vals);
        free(newvals);
        free(bitset);
    }
}
#endif

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(setandextract_uint16),
#ifdef IS_X64
        cmocka_unit_test(setandextract_sse_uint16),
#endif
        cmocka_unit_test(setandextract_uint32),
#ifdef USE_AVX
        cmocka_unit_test(setandextract_avx2_uint32),
#endif
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
