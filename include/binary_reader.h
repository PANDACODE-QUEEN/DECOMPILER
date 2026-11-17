#ifndef BINARY_READER_H
#define BINARY_READER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

enum bits {
  BITS_16 = 16,
  BITS_32 = 32,
  BITS_64 = 64
};

bool is_valid_size(size_t num);

bool read_exact_size(const char* filename, size_t* out_size, unsigned char **buffer);

bool reading_by_offset(const unsigned char *buffer, const size_t out_size,size_t offset,unsigned char * bit);

bool read_integer(const unsigned char *buffer, const size_t out_size, size_t offset, size_t bit_width, uint64_t *out_value);

#endif // BINARY_READER_H