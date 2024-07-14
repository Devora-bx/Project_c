#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
char *INSTUCTIONS[] = {".data", ".string", ".extern", ".entry"};

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
        if (strcmp(str, INSTUCTIONS[i]) == 0) {
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
        if (strcmp(str, OPCODES[i]->opcode) == 0) {
            return 1; /* Return 1 if the string matches an opcode */
        }
    }
    return 0; /* Return 0 if the string is not a valid opcode */
}
bool endsWithColon(const char *str) {//check if the string in the begining of the line is label
    int len = strlen(str);
    if (len == 0) {
        return false; // String is empty
    }
    return str[len - 1] == ':';
}