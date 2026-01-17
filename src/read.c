#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "const.h"
#include "read.h"

long file_size(FILE *f) {
    long size;

    fseek(f, 0, SEEK_END);
    size = ftell(f);
    rewind(f);   // or fseek(f, 0, SEEK_SET)

    return size;
}

long* offsets(const char* file_path) {
    FILE * f  = fopen(file_path, "r");

    long* offsets = malloc( sizeof(long) * (MAX_LEN + 2) );
    for (int i = 0; i <= MAX_LEN; i++)
        offsets[i] = -1;
    offsets[MAX_LEN + 1] = file_size(f);

    char buffer[MAX_LEN + 2]; // Holds \n and \0
    int curr_len = 0;

    while (1) {
        long pos = ftell(f);

        if (fgets(buffer, sizeof(buffer), f) == NULL) {
            break;
        }

        int len = strlen(buffer);
        if (buffer[len - 1] == '\n')
            len--;
        
        if (len > curr_len && offsets[len] == -1) {
            offsets[len] = pos; // store byte offset
            curr_len = len;
        }
    }

    fclose(f);
    return offsets;
}