#include <stdio.h>
int implement_macro(char * file_name){
    FILE *file;
    /* Open the file for reading */
    file = fopen(file_name, "r");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    remove_extra_spaces_str(file_name);
    remove_spaces_next_to_comma(file_name);
    readMacrosFromFile(file_name,Macro)
}
 