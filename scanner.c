#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "globals.h"
char *INSTRUCTIONS[] = {".data", ".string", ".extern", ".entry"};
Register REGISTERS[] = {
    {"r0",1},
    {"r1",2},
    {"r2",3},
    {"r3",4},
    {"r4",5},
    {"r5",6},
    {"r6",7},
    {"r7",8}
};

op_code OPCODES[] = {
        {"mov",  2},
        {"cmp",  2},
        {"add",  2},
        {"sub",  2},
        {"not",  1},
        {"clr",  1},
        {"lea",  2},
        {"inc",  1},
        {"dec",  1},
        {"jmp",  1},
        {"bne",  1},
        {"red",  1},
        {"prn",  1},
        {"jsr",  1},
        {"rts",  0},
        {"stop", 0}
};

int instr_detection(char *str) {
    int i;

    /* Return 0 if the string is NULL */
    if (str == NULL) {
        return 0;
    }

    /* Iterate through the list of known instructions and compare the string with each instruction in the list */
    for (i = 0; i < INSTRUCTIONS_COUNT; i++) {
        if (strcmp(str, INSTRUCTIONS[i]) == 0) {
            return 1; /* Return 1 if the string matches an instruction */
        }
    }
    return 0; /* Return 0 if the string is not a valid instruction */
}

int opcode_detection(char *str){
    int i;

    /* Return 0 if the string is NULL */
    if (str == NULL) {
        return 0;
    }

    /* Iterate through the list of known opcode and compare the string with each opcode in the list */
    for (i = 0; i < OPCODES_COUNT; i++) {
        if (strcmp(str, OPCODES[i].opcode) == 0) {
            return 1; /* Return 1 if the string matches an opcode */
        }
    }
    return 0; /* Return 0 if the string is not a valid opcode */
}

int reg_detection(char *str){
	int i;
    /* Return 0 if the string is NULL */
    if (str == NULL) {
        return 0;
    }

    /* Iterate through the list of known register and compare the string with each register in the list */
    for (i = 0; i < REG_COUNT; i++) {
        if (!strcmp(str, REGISTERS[i].name_of_register)) {
            return REGISTERS[i].reg_num; /* Return the number of reg (but +1) */
        }
    }
    return 0; /* Return 0 if the string is not a valid register */
}

bool endsWithColon(const char *str) {/*check if the string in the begining of the line is label*/
    int len = strlen(str);
    if (len == 0) {
        return false; /* String is empty*/
    }
    return str[len - 1] == ':';
}

int extra_char_detection(char *str) {
    /* Allocate memory for the copy of the string*/
    char *rest_of_line = (char *)malloc((strlen(str) + 1) * sizeof(char));
    if (rest_of_line == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    /* Copy the original string to avoid modifying it*/
    strcpy(rest_of_line, str);

    /*Use strtok to tokenize the string*/
    char *token = strtok(rest_of_line, " ");
    if (token == NULL) {
        free(rest_of_line);
        return 0; // No tokens found
    }

    /* Check if there's another token after the first*/
    token = strtok(NULL, " ");
    if (token != NULL) {
        free(rest_of_line);
        return 1; /* Extra character found*/
    }

    free(rest_of_line);
    return 0; /*No extra character*/
}
// if the string start with point its should be instruction
int is_instruction(char * str){
    return(*str == '.');
}