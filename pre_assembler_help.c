#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "pre_assembler.h"
/////////////////////////////////////////////////////////////
char* remove_extra_spaces_file(char *file_name) {
    FILE *orig_file, *temp_file;
    char *new_file_name;
    char str[BIG_NUMBER_CONST];
    int num_line = 0;

    orig_file = fopen(file_name, "r");
    if (orig_file == NULL){
        printf("Error opening original file\n");
        return NULL;
    }

    new_file_name = add_new_file(file_name, ".t01");
    if (new_file_name == NULL) {
        fclose(orig_file);
        printf("Error creating new file name\n");
        return NULL;
    }

    temp_file = fopen(new_file_name, "w");
    if (temp_file == NULL){
        fclose(orig_file);
        printf("Error opening temporary file\n");
        return NULL;
    }

    while (fgets(str, BIG_NUMBER_CONST, orig_file)) {
        num_line++;
        if (strlen(str) > MAX_LINE_LENGTH) {
            printf("Line %d too long\n", num_line);
            fclose(orig_file);
            fclose(temp_file);
            return NULL;
        } else if (*str == ';') {
            *str = '\n';
            *(str + 1) = '\0';
        } else {
            remove_extra_spaces_str(str);
        }
        fputs(str, temp_file);
    }

    fclose(orig_file);
    fclose(temp_file);
    return new_file_name;
}
///////////////////////////////////////////////////////////////
int add_macro(char *file_name, node **head) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    char *macro_name = NULL;
    char macro_content[MAX_LINE_LENGTH * 100] = ""; // מקום לשמירת תוכן המאקרו
    int is_macro = 0; // דגל לזיהוי אם אנחנו בתוך מאקרו
    int line_number = 0; // מספר השורה

    while (fgets(line, sizeof(line), file)) {
        line_number++;
        if (strncmp(line, "macr ", 5) == 0) {
            int mcro_line=line_number;
            char temp_name[MAX_LINE_LENGTH];
            sscanf(line + 5, "%s", temp_name);
            if (is_valid_macro_name(temp_name)) {
                is_macro = 1;
                macro_content[0] = '\0'; // אתחול תוכן המאקרו
                macro_name = handle_malloc((strlen(temp_name) + 1) * sizeof(char));
                strcpy(macro_name, temp_name);
            } else {
                // שם המאקרו לא תקין
                fprintf(stderr, "Invalid macro name at line %d: %s\n", line_number, temp_name);
                continue;
            }
        } else if (is_macro && strncmp(line, "endmacr", 7) == 0) {
            // מצאנו סיום מאקרו
            add_macro_to_list(head, macro_name, macro_content,mcro_line);
            is_macro = 0;
            free(macro_name);
            macro_name = NULL;
        } else if (is_macro) {
            // אנחנו בתוך מאקרו, שמור את השורה בתוכן המאקרו
            strcat(macro_content, line);
        }
    }

    if (macro_name != NULL) {
        free(macro_name);
    }

    fclose(file);
    return 0;
}