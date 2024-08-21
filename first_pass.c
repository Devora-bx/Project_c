#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "globals.h"
#include "first_pass.h"
#include "pre_assembler.h"
#include "second_pass.h"
#define stop_with_error() do { \
    fclose(ent_p); \
    fclose(fp); \
    free_label_list(label_head); \
    free_instruction_memory(instruction_memory_head);\
    return 0; \
} while(0)




int implement_first_pass(char file_name[]) {
    label* label_head = NULL; 
    long size;
    /* string to save the current line */
    char str[MAX_LINE_LENGTH] = {0};
    /* strings to divide the line into sections */
    char first_word[MAX_LINE_LENGTH] = {0};
    char second_word[MAX_LINE_LENGTH] = {0};
    char rest_of_line[MAX_LINE_LENGTH] = {0};
    /* strings to save the lines to write into entry and extern files */
    char *line_to_ent_file, *line_to_ext_file;
    /* string to handle the name of files */
    char *ent_file, *ext_file;

    FILE *fp, *ent_p, *ext_p;
    
    /* Initialize IC and DC pointers */
    int IC = IC_INIT_VALUE;
    int IC_CURRENT = IC_INIT_VALUE;
    int extern_address = 0;
    int DC = 0;
    int DC_CURRENT = 0;
    int is_instruction_code;
    instruction_memory *instruction_memory_head = NULL;
    data_image *data_image_head = NULL;

    int line = 0;

    /* opening the am file to read */
    fp = fopen(file_name, "r");
    if (fp == NULL) {
        printf("Failed to open file: %s\n", file_name);
        return 0;
    }
    /* opening the ent file to write */
    ent_file = add_new_file(file_name, ".ent");
    ent_p = fopen(ent_file, "w");
    if (ent_p == NULL) {
        printf("Failed to open file: %s\n", ent_file);
        fclose(fp);  /* Close the opened file before returning */
        return 0;
    }
    /* opening the ext file to write */
    ext_file = add_new_file(file_name, ".ext");
    ext_p = fopen(ext_file, "w");
    if (ext_p == NULL) {
        printf("Failed to open file: %s\n", ext_file);
        fclose(fp);  /* Close the opened file before returning */
        return 0;
    }

    printf("File opened successfully: %s\n", file_name);
    /* read line from file and parsing it */
    while (fgets(str, MAX_LINE_LENGTH, fp)) {
        line++;
        is_instruction_code = 0;
        IC_CURRENT = IC;/*save the current ic before change*/
        DC_CURRENT = DC;
        memset(first_word, 0, MAX_LINE_LENGTH);
        memset(second_word, 0, MAX_LINE_LENGTH);
        memset(rest_of_line, 0, MAX_LINE_LENGTH);

        sscanf(str, "%s %s %s", first_word, second_word, rest_of_line);
        if(*str == ';'){/* if its a comment, ignor. */
            continue;
        }
        if (endsWithColon(first_word)) { /* optional label */
            if (is_instruction(second_word)) {
                if (!strcmp(first_word, ".entry") || !strcmp(first_word, ".extern")) {
                    continue; /*ignor from defination label on .entry/.extern */
                }
                else if(instruction_data_process(second_word, rest_of_line, &DC,line,&data_image_head)){
                    if(!label_process(first_word, &DC_CURRENT, &label_head,".data")){
                    stop_with_error();/*or label or instruction wrong*/
                    }
                }
                else{
                    printf("invalid instruction in line: %d\n",line);
                    stop_with_error();/*or label or instruction wrong*/
                }
                
            }        
            else if (opcode_detection(second_word)) {
                if(opcode_process(second_word, rest_of_line,&IC,line,&instruction_memory_head)){
                    is_instruction_code = 1;
                    if(!label_process(first_word, &IC_CURRENT, &label_head,".code")){
                    printf("invalid label in line: %d\n",line);
                    stop_with_error();/*invalid label */
                    }
                }
                else{
                    printf("invalid code line: %d\n",line);
                    stop_with_error();/*invalid opcode*/
                }           
            } 
        }
        else if (is_instruction(first_word)) {
            if (!strcmp(first_word, ".entry")) {
                printf("ignor its entry and ic is: %d\n",IC_CURRENT);
                continue;
            }
            else if(!strcmp(first_word, ".extern")){
                if(!label_process(second_word, &extern_address, &label_head,".external"))
                stop_with_error();
            }
            else if(!instruction_data_process(first_word, second_word, &DC,line,&data_image_head)) {
                stop_with_error();
           }
        } 
        else if (opcode_detection(first_word)) {
            if(!opcode_process(first_word, second_word,&IC,line,&instruction_memory_head)){
                stop_with_error();
            }
            is_instruction_code = 1;
        } 
        else {
            if (is_empty_line(str)) {
                continue;
            } else {
                printf("Error: Unrecognized line format in line %d: %s\n", line, str);
                stop_with_error();
            }
        }

        
        printf("the line is: %d, and ic is: %d, and dc is: %d\n",line,IC_CURRENT,DC);
    
    
    }
    update_data_label(label_head,IC);
    /* end first pass and parsing the line without entry  */
    printf("done ?\n");
    
    
    if(implement_second_pass(file_name,&label_head,&instruction_memory_head,&data_image_head)){
        printf("cool");
    }
    printf("File closed: %s\n", file_name);
    print_memory(instruction_memory_head,data_image_head,IC);
    printf("\n\n");
    printlist_label(label_head);
    fclose(fp);
    free_label_list(label_head);
    free_instruction_memory(instruction_memory_head);
    return 0;
}

void printlist_label(label *head) {
    while (head) {
        printf("The name of label is: %s\n", head->name_of_label);
        printf("The address of label: %d\n", head->address_of_label);
        printf("The type of label: %s\n", head->type_of_label);
        head = head->next;
    }
}

void print_memory(instruction_memory *instruction_head,data_image *data_head,int IC) {
    while (instruction_head) {
        printf("%d        %d        %s\n",instruction_head->line,instruction_head->address,instruction_head->binary_str);
        instruction_head = instruction_head->next;
    }
    while (data_head)
    {
    data_head->address += IC;
    printf("%d        %d           %s\n", data_head->line,data_head->address, data_head->binary_value);
    data_head = data_head->next; 
    }

    
}
