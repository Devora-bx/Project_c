#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pre_assembler.h"
#include "globals.h"

//this function relate to main and add the ending to files
char * add_new_file(char * name_file,char * ending){
    size_t length = strlen(name_file);
    char * copy = (char*)malloc((length + 1) * sizeof(char));
    if (copy == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    strcpy(copy, name_file);
    strcat(copy, ending);
    return copy;
}



///////////////////this functions cleaning the files from extra space//////////////////////////////
void remove_extra_spaces_str(char str[]) {
    /* i for original string, j for modified string */
    int i, j;
    char str_temp[MAX_LINE_LENGTH];
    i = j = 0;
    /* eliminating white-spaces in the beginning of the line */
    while (is_space_or_tab(*(str + i))) {
        i++;
    }
    while (*(str + i) != '\0') {
        /* copying character */
        while (!is_space_or_tab(*(str + i)) && *(str + i) != '\0') {
            *(str_temp + j) = *(str + i);
            i++;
            j++;
        }
        /* if loop stopped because end of line char */
        if (*(str + i) == '\0') {
            break;
        }
        /* if loop stopped because of a white-space skipping them until another character is encountered*/
        while (is_space_or_tab(*(str + i))) {
            i++;
        }
        /* if stopped not because of end of line char then copy one space for all the others that were skipped */
        if (!(*(str + i) == '\n' || *(str + i) == '\0')) {
            *(str_temp + j) = ' ';
            j++;
        }
    }
    *(str_temp + j) = *(str + i);
    *(str_temp + j + 1) = '\0';
    remove_spaces_next_to_comma(str_temp);
    strcpy(str, str_temp);
}

int is_space_or_tab(char c) {
    /* Check if the char is a space or a tab */
    return (isspace(c) && c != '\n');
}

void remove_spaces_next_to_comma(char *str) {
    char *ptr = str;
    /* If the line starts with ',' avoiding accessing outside the str */
    if(*ptr == ','){
        return;
    }
    while ((ptr = strchr(ptr, ',')) != NULL) {
        /* Space before the comma */
        if (*(ptr - 1) == ' ') {
            memmove(ptr - 1, ptr, strlen(ptr) + 1);
            if (*(ptr) == ' ') {
                /* Also space after the comma */
                memmove(ptr, ptr + 1, strlen(ptr + 1) + 1);
            }
        }
        else if (*(ptr + 1) == ' ') {
            /* Only space after the comma */
            memmove(ptr + 1, ptr + 2, strlen(ptr + 2) + 1);
            ptr++;
        }else{
            ptr++;
        }
    }
}
/////////////////// all this function above cleaning the files///////////////////////////////////

void readMacrosFromFile(const char *filename, Macro **head) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "macr", 4) == 0) {//if there a define of macro add it to the macro table//
            char name[81];
            char content[768];
            if (sscanf(line, "macr %255s %767[^\n]", name, content) == 2) {
                printf("will added");
            }
        }
    }
    
    fclose(file);
}
int is_valid_macro(char *name_file){
        return 0;
 }

