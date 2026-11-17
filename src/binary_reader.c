// See documentation in .h file
#include "binary_reader.h"
#define ILLEGAL_FILE_SIZE -1L
bool is_valid_size(size_t num) {
    switch (num) {
        case BITS_16:
        case BITS_32:
        case BITS_64:
            return true;
        default:
            return false;
    }
}

bool read_exact_size(const char* filename, size_t* out_size,unsigned char **buffer) {
    if(NULL == filename || NULL == out_size || NULL == buffer){
        return false;
    }
    *out_size = 0;
    *buffer = NULL;
    FILE *file = fopen(filename, "rb");
    if (!file) return false;

    // Jump to the end of the file
    if(fseek(file, 0, SEEK_END) !=0){
        fclose(file);
        return false;
    }
    // Get the current byte offset (the file size)
    long file_size = ftell(file);
    if(ILLEGAL_FILE_SIZE == file_size){
        fclose(file);
        return false;
    }
    // Rewind back to the beginning
    if (fseek(file, 0, SEEK_SET) != 0){
        fclose(file);
        return false;
    }

    if (file_size <= 0) {
        fclose(file);
        return false;
    }

    *buffer = malloc(file_size);
    if (!*(buffer)) {
        fclose(file);
        return false;
    }

    // Read the entire file in one go
    size_t bytes_read = fread(*buffer, 1, file_size, file);
    fclose(file);
    if(bytes_read != (size_t)file_size){
        free(*buffer);
        *buffer=NULL;
        return false;
    }

    *out_size = bytes_read;
    return true;
}

bool reading_by_offset(const unsigned char *buffer, const size_t out_size,size_t offset,unsigned char * bit){
    if(offset < out_size && NULL != buffer && NULL != bit)
    {
        *bit = buffer[offset];
        return true;
    }
    return false;
}

bool read_integer(const unsigned char *buffer, const size_t out_size, size_t offset, size_t bit_width, uint64_t *out_value) {
   
    if (buffer == NULL || out_value == NULL) {
        return false;
    }
    if (!is_valid_size(bit_width)) {
        return false;
    }
    size_t num_bytes = bit_width / 8;

    
    if (offset <= out_size && num_bytes <= (out_size - offset)) {
        
        *out_value = 0; 
        for (size_t i = 0; i < num_bytes; i++) {
            *out_value |= ((uint64_t)buffer[offset + i] << (8 * i));
        }
        
        return true;
    }

    return false;
}
