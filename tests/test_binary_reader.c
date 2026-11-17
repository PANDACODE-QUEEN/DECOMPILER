#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "binary_reader.h"

int main(void) {
    unsigned char data[] = {
        0x34, 0x12,
        0x78, 0x56, 0x34, 0x12,
        0xEF, 0xCD, 0xAB, 0x89,
        0x67, 0x45, 0x23, 0x01
    };
    
    size_t data_size = sizeof(data);
    uint64_t val = 0;
    bool res;
    unsigned char bit = 0;
    
    int failed_tests = 0;

    printf("--- Running binary_reader Unit Tests ---\n\n");

    val = 0;
    res = read_integer(data, data_size, 0, 16, &val);
    if (res == true && val == 0x1234) {
        printf("[PASS] Test  1: bit-16                 | Expected: 0x1234, Got: 0x%llX\n", (unsigned long long)val);
    } else {
        printf("[FAIL] Test  1: bit-16\n");
        failed_tests++;
    }

    val = 0;
    res = read_integer(data, data_size, 2, 32, &val);
    if (res == true && val == 0x12345678) {
        printf("[PASS] Test  2: bit-32                 | Expected: 0x12345678, Got: 0x%llX\n", (unsigned long long)val);
    } else {
        printf("[FAIL] Test  2: bit-32\n");
        failed_tests++;
    }

    val = 0;
    res = read_integer(data, data_size, 6, 64, &val);
    if (res == true && val == 0x0123456789ABCDEF) {
        printf("[PASS] Test  3: bit-64                 | Expected: 0x0123456789ABCDEF, Got: 0x%016llX\n", (unsigned long long)val);
    } else {
        printf("[FAIL] Test  3: bit-64\n");
        failed_tests++;
    }

    val = 0;
    res = read_integer(data, data_size, 0, 24, &val);
    if (res == false) {
        printf("[PASS] Test  4: Invalid width          | Expected: false, Got: false\n");
    } else {
        printf("[FAIL] Test  4: Invalid width\n");
        failed_tests++;
    }

    val = 0;
    res = read_integer(data, data_size, 10, 64, &val);
    if (res == false) {
        printf("[PASS] Test  5: Out of bounds          | Expected: false, Got: false\n");
    } else {
        printf("[FAIL] Test  5: Out of bounds\n");
        failed_tests++;
    }

    val = 0;
    res = read_integer(data, data_size, 6, 64, &val);
    if (res == true && val == 0x0123456789ABCDEF) {
        printf("[PASS] Test  6: Exact boundary         | Expected: 0x0123456789ABCDEF, Got: 0x%016llX\n", (unsigned long long)val);
    } else {
        printf("[FAIL] Test  6: Exact boundary\n");
        failed_tests++;
    }

    res = read_integer(NULL, data_size, 0, 32, &val);
    if (res == false) {
        printf("[PASS] Test  7: buffer == NULL         | Expected: false, Got: false\n");
    } else {
        printf("[FAIL] Test  7: buffer == NULL\n");
        failed_tests++;
    }

    res = read_integer(data, data_size, 0, 32, NULL);
    if (res == false) {
        printf("[PASS] Test  8: out_value == NULL      | Expected: false, Got: false\n");
    } else {
        printf("[FAIL] Test  8: out_value == NULL\n");
        failed_tests++;
    }

    bool res_first = reading_by_offset(data, data_size, 0, &bit);
    unsigned char first_byte = bit;
    bool res_last = reading_by_offset(data, data_size, data_size - 1, &bit);
    unsigned char last_byte = bit;
    if (res_first && res_last && first_byte == 0x34 && last_byte == 0x01) {
        printf("[PASS] Test  9: Offset first/last byte | Matched 0x34 and 0x01\n");
    } else {
        printf("[FAIL] Test  9: Offset first/last byte\n");
        failed_tests++;
    }

    res = reading_by_offset(data, data_size, data_size + 5, &bit);
    if (res == false) {
        printf("[PASS] Test 10: Offset out of bounds   | Expected: false, Got: false\n");
    } else {
        printf("[FAIL] Test 10: Offset out of bounds\n");
        failed_tests++;
    }

    bool res_null_buf = reading_by_offset(NULL, data_size, 0, &bit);
    bool res_null_bit = reading_by_offset(data, data_size, 0, NULL);
    if (res_null_buf == false && res_null_bit == false) {
        printf("[PASS] Test 11: Offset NULL pointers   | Expected: false, Got: false\n");
    } else {
        printf("[FAIL] Test 11: Offset NULL pointers\n");
        failed_tests++;
    }

    const char* real_file = "test_real.bin";
    const char* empty_file = "test_empty.bin";
    const char* missing_file = "does_not_exist.bin";
    
    FILE *f_real = fopen(real_file, "wb");
    if (!f_real) {
        printf("[FATAL] Could not create %s. Aborting file tests.\n", real_file);
        return EXIT_FAILURE;
    }
    if (fwrite(data, 1, data_size, f_real) != data_size) {
        printf("[FATAL] Could not write all data to %s. Aborting file tests.\n", real_file);
        fclose(f_real);
        remove(real_file);
        return EXIT_FAILURE;
    }
    if (fclose(f_real) != 0) {
        printf("[FATAL] Error closing %s. Aborting file tests.\n", real_file);
        remove(real_file);
        return EXIT_FAILURE;
    }
    
    FILE *f_empty = fopen(empty_file, "wb");
    if (!f_empty) {
        printf("[FATAL] Could not create %s. Aborting file tests.\n", empty_file);
        remove(real_file);
        return EXIT_FAILURE;
    }
    if (fclose(f_empty) != 0) {
        printf("[FATAL] Error closing %s. Aborting file tests.\n", empty_file);
        remove(real_file);
        remove(empty_file);
        return EXIT_FAILURE;
    }

    size_t out_file_size = 0;
    unsigned char *file_buffer = NULL;
    res = read_exact_size(real_file, &out_file_size, &file_buffer);
    if (res == true && out_file_size == data_size && memcmp(data, file_buffer, data_size) == 0) {
        printf("[PASS] Test 12: Read real file         | Content matched perfectly\n");
    } else {
        printf("[FAIL] Test 12: Read real file\n");
        failed_tests++;
    }
    if (file_buffer) free(file_buffer);

    out_file_size = 0; file_buffer = NULL;
    res = read_exact_size(missing_file, &out_file_size, &file_buffer);
    if (res == false && file_buffer == NULL) {
        printf("[PASS] Test 13: Missing file         | Expected: false, Got: false\n");
    } else {
        printf("[FAIL] Test 13: Missing file\n");
        failed_tests++;
    }

    out_file_size = 0; file_buffer = NULL;
    res = read_exact_size(empty_file, &out_file_size, &file_buffer);
    if (res == false && file_buffer == NULL) {
        printf("[PASS] Test 14: Empty file           | Expected: false, Got: false\n");
    } else {
        printf("[FAIL] Test 14: Empty file\n");
        failed_tests++;
    }

    remove(real_file);
    remove(empty_file);

    if (failed_tests == 0) {
        printf("\n=> ALL 14 TESTS PASSED.\n");
    } else {
        printf("\n=> %d TESTS FAILED.\n", failed_tests);
    }
    
    return failed_tests == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}