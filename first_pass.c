#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "globals.h"
#include "first_pass.h"

int implement_first_pass(char file_name[]){
    char str[MAX_LINE_LENGTH] = {0};
    char first_word[MAX_LINE_LENGTH] = {0};
    char second_word[MAX_LINE_LENGTH] = {0};
    char rest_of_line[MAX_LINE_LENGTH] = {0};
    FILE *fp33;
    int IC = 0;
    int DC = 0;
    int num_of_section_in_line;
    int line = 0;

    fp33 = fopen(file_name, "r");
    if(fp33 == NULL){
        printf("Failed to open file: %s\n", file_name);
        return 1;
    }

    printf("File opened successfully: %s\n", file_name);

    while(fgets(str, MAX_LINE_LENGTH, fp33)){
        line++;
        printf("Reading line %d: %s\n", line, str);

        memset(first_word, 0, MAX_LINE_LENGTH);
        memset(second_word, 0, MAX_LINE_LENGTH);
        memset(rest_of_line, 0, MAX_LINE_LENGTH);

        num_of_section_in_line = sscanf(str, "%s %s %s", first_word, second_word, rest_of_line);
        printf("Parsed line %d: first_word='%s', second_word='%s', rest_of_line='%s'\n", line, first_word, second_word, rest_of_line);

        if(endsWithColon(first_word)){ // optional label
            printf("Optional label detected in line %d: %s\n", line, first_word);
            if(num_of_section_in_line == 3){
                // Check if it's entry/extern and if so, ignore
                if(is_instruction(second_word)){
                    printf("Instruction after label detected in line %d: %s\n", line, second_word);
                    label_process(first_word, &DC);
                    instruction_process(second_word, rest_of_line);
                }
                else if(opcode_detection(second_word)){
                    printf("Opcode after label detected in line %d: %s\n", line, second_word);
                    label_process(first_word, &IC);
                    // opcode_process(second_word, rest_of_line);
                }
            }
            else if(num_of_section_in_line == 2){
                if(opcode_detection(second_word)){
                    printf("Opcode with label detected in line %d: %s\n", line, second_word);
                    label_process(first_word, &IC); // if label on opcode, use IC
                    // opcode_process(second_word, "");
                }
            }
        }
        else if(is_instruction(first_word) && num_of_section_in_line == 2){ // no start with label, check if is instruction
            printf("Instruction detected in line %d: %s\n", line, first_word);
            instruction_process(first_word, second_word);
        }
        else if(opcode_detection(first_word)){
            printf("Opcode detected in line %d: %s\n", line, first_word);
            // opcode_process(first_word, "");
        }
        else{
            if(is_empty_line(str)){
                printf("Empty line detected in line %d\n", line);
            } else {
                printf("Error: Unrecognized line format in line %d: %s\n", line, str);
            }
        }
    }

    fclose(fp33);
    printf("File closed: %s\n", file_name);
    return 0;
}
