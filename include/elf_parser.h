#ifndef ELF_PARSER_H
#define ELF_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "binary_reader.h"
typedef struct {
    unsigned char e_ident[16];  
    uint16_t      e_type;       
    uint16_t      e_machine;    
    uint32_t      e_version;    
    uint64_t      e_entry;      
    uint64_t      e_phoff;     
    uint64_t      e_shoff;      
    uint32_t      e_flags;     
    uint16_t      e_ehsize;    
    uint16_t      e_phentsize;  
    uint16_t      e_phnum;      
    uint16_t      e_shentsize;  
    uint16_t      e_shnum;      
    uint16_t      e_shstrndx;   
} Elf64_headers;
bool is_elf64(const unsigned char *buffer, size_t file_size);

#endif // BINARY_READER_H