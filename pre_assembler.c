#include <stdio.h>
int is_valid_macro(char * file_name){
    FILE *file;
    /* Open the file for reading */
    file = fopen(file_name, "r");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
  
}
 