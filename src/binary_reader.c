// See documentation in .h file
#include <stdio.h>
#include <stdlib.h>

unsigned char* read_exact_size(const char* filename, size_t* out_size) {
    FILE *file = fopen(filename, "rb");
    if (!file) return NULL;

    // Jump to the end of the file
    fseek(file, 0, SEEK_END);
    // Get the current byte offset (the file size)
    long file_size = ftell(file);
    // Rewind back to the beginning
    rewind(file); 

    if (file_size <= 0) {
        fclose(file);
        return NULL;
    }

    unsigned char *buffer = malloc(file_size);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    // Read the entire file in one go
    size_t bytes_read = fread(buffer, 1, file_size, file);
    fclose(file);

    *out_size = bytes_read;
    return buffer;
}

unsigned char* reading_by_offest(const unsigned char *buffer, const size_t out_size,size_t offset){
    if(offset< out_size)
    {
        return buffer[offset];
    }
    return NULL;
}
