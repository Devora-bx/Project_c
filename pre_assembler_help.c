#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "globals.h"
#include "pre_assembler.h"

char* remove_extra_spaces_file(char file_name[]) {
	// errno = 0;
    
    char *new_file_name;
    char str[BIG_NUMBER_CONST];
    int num_line = 0;
    FILE *orig_file, *temp_file;
    
    // printf("Opening file: %s\n", file_name);
    orig_file = fopen(file_name, "r");

    if (orig_file == NULL){
	// printf("%d",errno);
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
	    free(new_file_name);
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

int add_macro(char *file_name, node **head) {
    
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    char *macro_name = NULL;
    char macro_content[MAX_LINE_LENGTH * 100] = ""; 
    int is_macro = 0; 
    int line_number = 0;
    int mcro_line; 

    while (fgets(line, sizeof(line), file)) {
        line_number++;
        int found = 0;
        node * temp;
        if (strncmp(line, "macr ", 5) == 0) {
            //add search in the list
            mcro_line=line_number;
	        
            char temp_name[MAX_LINE_LENGTH];
            sscanf(line + 5, "%s", temp_name);
            
            temp = search_list(*head,temp_name,line,&found);
             
            if(found){
                
                continue;
            }
            if (is_valid_macro_name(temp_name)) {
                
                is_macro = 1;
                macro_content[0] = '\0'; 
                macro_name = handle_malloc((strlen(temp_name) + 1) * sizeof(char));
                strcpy(macro_name, temp_name);
            } else {
               
                fprintf(stderr, "Invalid macro name at line %d: %s\n", line_number, temp_name);
                continue;
            }
        } else if (is_macro && strncmp(line, "endmacr", 7) == 0) {
            printf(" the content of  macro is: %s\n",macro_content);
           
            add_macro_to_list(head, macro_name, macro_content,mcro_line,temp);
           
            is_macro = 0;
            free(macro_name);
            macro_name = NULL;
        } else if (is_macro) {
            
            strcat(macro_content, line);
        }
    }

    if (macro_name != NULL) {
        free(macro_name);
    }

    fclose(file);
    return 1;
}
void printlist(node *head){
    while (head)
    {
        printf("the name of macr: %s\n",head->macro_name);
        printf("the content of macr: %s\n",head->macro_content);
        head = head->next;
    }
    
}