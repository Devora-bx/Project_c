#ifndef LABRATORY_C_FINAL_PROJECT_GLOBALS_H
#define LABRATORY_C_FINAL_PROJECT_GLOBALS_H
/*The File contain all the global values in the program*/

/* Maximum length of a label in command line  */
#define MAX_LABEL_LENGTH 31

/* Maximum length of a single command line  */
#define MAX_LINE_LENGTH 81

/* Default IC value */
#define IC_INIT_VALUE 100

#define OPCODES_COUNT 16

#define REG_COUNT 8

#define WORD_LEN 15

#define ARE_BITS 3

#define RELOCATABLE_VALUE 2

#define EXTERNAL_VALUE 1

#define ABSOLUTE_VALUE 4

typedef struct location {
    char *file_name;
    int line_num;
} location;


/* Define a line struct*/
typedef struct line_data {
    /*The file name will help us to print relevant errors*/
    char *file_name;
    /*The line number will helps us to track the current line number and to print errors*/
    short number;
    /*The content of the line*/
    char *data;

} line_data;

typedef struct op_code {
    char *opcode;    /* The opcode corresponding to the operation */
    int arg_num;     /* The number of arguments for the operation */
} op_code;

typedef struct Macro{
    char *name_of_macro;
    char *content;
    struct Macro *next; 
}Macro;

#endif