#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "appendix.c"
#include "pre_assembler.c"
#include "globals.h"

int implement_macro(char * file_name){
    FILE *file;
    Macro *head = NULL;
    /* Open the file for reading */
    file = fopen(file_name, "r");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(1);
    }
    remove_extra_spaces_str(file_name);
    remove_spaces_next_to_comma(file_name);
    readMacrosFromFile(file_name,head);
    return 0;
}
 