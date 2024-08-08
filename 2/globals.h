#ifndef LABRATORY_C_FINAL_PROJECT_GLOBALS_H
#define LABRATORY_C_FINAL_PROJECT_GLOBALS_H
/*The File contain all the global values in the program*/

/* Maximum length of a label in command line  */
#define MAX_LABEL_LENGTH 31

/* Maximum length of a single command line  */
#define MAX_LINE_LENGTH 81
/* Default IC value */
#define IC_INIT_VALUE 100
/* arbitrary very big number for line length */
#define BIG_NUMBER_CONST 1000

#define INSTRUCTIONS_COUNT 4

#define OPCODES_COUNT 16

#define REG_COUNT 8

#define WORD_LEN 15

#define ARE_BITS 3

#define ABSOLUTE VALUE 4

#define RELOCATABLE_VALUE 2

#define EXTERNAL_VALUE 1

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

typedef struct node {
    char *macro_name;
    char *macro_content;
    int macro_line;
    struct node *next;
} node;

typedef struct Register{
    char *name_of_register;
    int reg_num;
}Register;

typedef struct data_image {
    int value;
    struct data_image *next;
} data_image;
typedef struct instr{
    char * name_of_instr;
    void (*func)(char *,int *);
}instr;
 extern data_image *data_image_head;
 typedef struct label{
    char *name_of_label;
    char * type_of_label;
    int address_of_label;
    struct label *next;
}label;
extern label *label_head; /*רשימה מקושרת של תוויות*/
extern int DC;
#endif