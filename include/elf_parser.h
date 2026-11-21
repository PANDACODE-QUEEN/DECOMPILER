#ifndef ELF_PARSER_H
#define ELF_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "binary_reader.h"

bool is_elf64(const unsigned char *buffer, size_t file_size);

#endif // BINARY_READER_H