#include "globals.h"
#include <stdbool.h>
int implement_first_pass(char file_name[]);  
int label_process(char * first_word,int * p_address);
int instruction_process(char *first_word, char *rest_of_line);
int opcode_process(char *first_word, char *rest_of_line);
int file_process();
int convert_to_binary();
int instr_detection_and_execute(char *first_word, char *rest_of_line);
int instr_detection(char *str);
int opcode_detection(char *str);
int reg_detection(char *str);
int extra_char_detection(char *str);
bool endsWithColon(const char *str);
int is_instruction(char * str);
label * search_label_on_list(label * head, char * name_of_label,int * found);
int is_valid_label(char * name_of_label);
void add_label_to_list(char * name_of_label, int * address_of_label,label **head,label *temp );
void check_valid_data(char *rest_of_line);
void check_valid_string(char *rest_of_line);
void check_valid_extern(char *rest_of_line);
void check_valid_entry(char *rest_of_line);
int check_valid_data_comma(char *rest_of_line);
int is_valid_integer(char *token);
int starts_and_ends_with_quote(char *rest_of_line);
char *trim_whitespace(char *str);
int is_empty_line(char *line);
