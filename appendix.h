#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *add_new_file(char *name_file, char *ending){
    size_t length = strlen(name_file);
     char* copy= (char*)malloc((length + 1) * sizeof(char));
    if (copy == NULL) {
        // Handle memory allocation failure
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

}
strcpy(copy, name_file);
    
strcat(copy,ending);

return copy;
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

void remove_spaces_next_to_comma(char *str) 
{
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

    