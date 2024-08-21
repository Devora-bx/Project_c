#include "globals.h"
#include <stdbool.h>
int implement_first_pass(char file_name[]);  
int label_process(char * first_word,int * p_address, label** label_head,char *type_of_label);
int instruction_data_process(char *first_word, char *rest_of_line,int * DC,int line,data_image **data_image_head);
int opcode_process(char *first_word, char *rest_of_line,int * IC,int line,instruction_memory **instruction_memory_head);
void convert_to_binary(int length, int decimal, char binary[]);
void decimal_to_binary(int length, int decimal, char binary[]);
int instr_data_detection(char *first_word, char *rest_of_line,int * DC,int line,data_image **data_image_head);
int instr_detection(char *str);
int opcode_detection(char *str);
int reg_detection(char *str);
int extra_char_detection(char *str);
bool endsWithColon(const char *str);
int is_instruction(char * str);
void check_valid_string(char *rest_of_line,int * DC,int line,data_image **data_image_head);
void update_data_label(label *label_head,int IC);
int check_valid_entry(char *rest_of_line,int * DC, label* label_head);
void check_valid_data(char *rest_of_line, int * DC,int line,data_image **data_image_head);
int check_valid_data_comma(char *rest_of_line);
int is_valid_integer(char *token);
int starts_and_ends_with_quote(char *rest_of_line);
char *trim_whitespace(char *str);
int is_empty_line(char *line);
label* find_label_by_name(const char *name_of_label, label* label_head);
void update_label_type(label *lbl, const char *new_type);
void create_entry_file(const char *filename, const char *content, label* label_head);
void add_to_data_image(int value,int address, data_image** data_image_head,int line);
int is_valid_label(char *name_of_label);
void add_label_to_list(char *name_of_label, int *address_of_label, label **head,char *type_of_label);
label* search_label_on_list(label *head, char *name_of_label, int *found);

void save_data_line(char* name_of_file,short num_of_line,char* content_of_line,line_data ** line_data_head);
char* duplicate(const char* str);

void free_label_list(label *head);
void convert_str_to_binary(int length,char *str, char *ARE);

int valid_num_argument(char * str ,char * rest_of_line,int * num_of_opcode);

int parsing_arg(char *name_of_opcode, char *arguments,char *first_word_to_binary,char *second_word_to_binary,char *third_word_to_binary,int * fieldBitSize1,int * fieldBitSize2,int * fieldBitSize3, int *detected_label_on_first_pass);
int detection_argument(char *argument);

// int first_word_process();
// int second_word_process();
// int third_word_process();
void identifyAddressingMode(char *target_arg,char *source_arg,int *type_of_target_arg,int *type_of_source_arg);
int validateParameters(char *name_of_opcode,int *num_of_opcode,int type_of_target_arg,int type_of_source_arg);
void add_to_instruction_memory(int line,int address,char* binary_str , instruction_memory** instruction_memory_head);
void free_instruction_memory(instruction_memory *head);

void convert_first_word_to_binary(int length, char* str, char* ARE);
void printlist_label(label *head);

void print_memory(instruction_memory *instruction_head,data_image *data_head,int IC) ;
