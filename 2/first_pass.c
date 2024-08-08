#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "globals.h"
#include "first_pass.h"
#include "pre_assembler.h"
void printlist_label(label *head);
int implement_first_pass(char file_name[]){
    char str[MAX_LINE_LENGTH] = {0};
    char first_word[MAX_LINE_LENGTH] = {0};
    char second_word[MAX_LINE_LENGTH] = {0};
    char rest_of_line[MAX_LINE_LENGTH] = {0};
    char *line_to_ent_file;
    char *ent_file;
    FILE *fp, *ent_p;
    int IC = 0;
    int DC = 0;
    int line = 0;

    fp = fopen(file_name, "r");
    if(fp == NULL){
        printf("Failed to open file: %s\n", file_name);
        return 1;
    }

    ent_file = add_new_file(file_name, ".ent");
    ent_p = fopen(ent_file, "w");
    if(ent_p == NULL){
        printf("Failed to open file: %s\n", ent_file);
        fclose(fp);  // Close the opened file before returning
        return 1;
    }

    printf("File opened successfully: %s\n", file_name);

    while(fgets(str, MAX_LINE_LENGTH, fp)){
        line++;
        // printf("Reading line %d: %s\n", line, str);

        memset(first_word, 0, MAX_LINE_LENGTH);
        memset(second_word, 0, MAX_LINE_LENGTH);
        memset(rest_of_line, 0, MAX_LINE_LENGTH);

        sscanf(str, "%s %s %s", first_word, second_word, rest_of_line);
        // printf("Parsed line %d: first_word='%s', second_word='%s', rest_of_line='%s'\n", line, first_word, second_word, rest_of_line);

        if(endsWithColon(first_word)){ // optional label
            // printf("Optional label detected in line %d: %s\n", line, first_word);
            
            if(is_instruction(second_word)){
                // printf("Instruction after label detected in line %d: %s\n", line, second_word);
                if(!strcmp(first_word,".entry") || !strcmp(first_word,".extern")){
                    continue;
                }
                label_process(first_word, &DC);
                instruction_data_process(second_word, rest_of_line, &DC);
                convert_to_binary(second_word);
            }
            else if(opcode_detection(second_word)){
                // printf("Opcode after label detected in line %d: %s\n", line, second_word);
                label_process(first_word, &IC);
                // opcode_process(second_word, rest_of_line);
            }
            else{
                printf("Invalid after label");
            }
        }
        else if(is_instruction(first_word)){
            if(!strcmp(first_word,".entry") || !strcmp(first_word,".extern")){
                continue;
            }
            // printf("Instruction detected in line %d: %s\n", line, first_word);
            instruction_data_process(first_word, second_word, &DC);
        }
        else if(opcode_detection(first_word)){
            // printf("Opcode detected in line %d: %s\n", line, first_word);
            // opcode_process(first_word, "");
        }
        else{
            if(is_empty_line(str)){
                // printf("Empty line detected in line %d\n", line);
            } else {
                printf("Error: Unrecognized line format in line %d: %s\n", line, str);
            }
        }
    }
    
    printlist_label(label_head);
    fseek(fp, 0, SEEK_SET); // Move the file pointer to the start for the second pass
    line = 0;
    while(fgets(str, MAX_LINE_LENGTH, fp)){
        memset(first_word, 0, MAX_LINE_LENGTH);
        memset(second_word, 0, MAX_LINE_LENGTH);

        sscanf(str, "%s %s", first_word, second_word);
        if(!strcmp(first_word,".entry")){
            line_to_ent_file = check_valid_entry(second_word, &DC);
            if(line_to_ent_file != NULL){
                fprintf(ent_p, "%s", line_to_ent_file);
            }
        }
    }

    fseek(ent_p, 0, SEEK_END); // Move to the end of the file
    long size = ftell(ent_p);  // Get the current file pointer position
    if(size == 0){
        remove(ent_file);
    }

    fclose(ent_p);
    fclose(fp);
    printf("File closed: %s\n", file_name);
    return 0;
}
void printlist_label(label *head){
    while (head)
    {
        
         printf("the name of label isss: %s\n",head->name_of_label);
         printf("the address of label: %d\n",head->address_of_label);
         printf("the type of label: %s\n",head->type_of_label);
        head = head->next;
    }
    
}


