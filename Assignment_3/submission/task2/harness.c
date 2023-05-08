#include <unistd.h>
#include <string.h>
#include <stdio.h>  
#include <stdlib.h>
#include <assert.h>

#include "library.h"

#define SIZE 100
#define INT_SIZE 4

int main(int argc, char* argv[]) {
    if(argc < 3) {
        printf("Usage: %s mul|echo <input_filename>\n", argv[0]);
    } else {
        char buffer[SIZE] = {0};
        // Open given file and store contents into our buffer
        FILE *file = fopen(argv[2], "r");

        // Figure out target
        if(strcmp(argv[1], "echo") == 0) {
            ssize_t l;

            // Read the input from a file and feed to lib_echo
            char c = getc(file);
            int idx = 0;

            // Go char by char and stop either at EOF or the buffer size
            while (c != EOF || idx < SIZE) {
                buffer[idx++] = c;
                c = getc(file);
            }

            lib_echo(buffer, (ssize_t)idx /* I like being clear */);
        } else if(strcmp(argv[1], "mul") == 0) {
            int a, b = 0; 

            // Read from the file 
            fgets(buffer, INT_SIZE+1, file);
            a = atoi(buffer);

            // Clear buffer
            memset(buffer, 0, SIZE);

            // Read b
            fgets(buffer, INT_SIZE+1, file); 
            b = atoi(buffer);

            printf("a = %d, b = %d, a * b = %d\n", a, b, lib_mul(a, b));
        } else {
            printf("Usage: %s mul|echo <input_filename>\n", argv[0]);
        }
        
        fclose(file);
    }

    return 0;
}