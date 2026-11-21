#include "elf_parser.h"
#define ELF_IDENTIFICATION_SIZE 64
bool is_elf64(const unsigned char *buffer, size_t file_size)
{
    if(NULL == buffer || file_size < ELF_IDENTIFICATION_SIZE){
        return false;
    }
    return (0x7f== buffer[0]&&'E'==buffer[1]&&'L'==buffer[2]&&'F'==buffer[3]&& 0x02 == buffer[4]&& 0x01 == buffer[5]&& 0x01 == buffer[6]);

}