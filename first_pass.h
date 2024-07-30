#include "globals.h"
#include <stdbool.h>
int implement_first_pass();  
int label_process();
int instruction_process();
int opcode_process();
int file_process();
int convert_to_binary();
int instr_detection(char *str);
int opcode_detection(char *str);
int reg_detection(char *str);
int extra_char_detection(char *str);
bool endsWithColon(const char *str);
int is_instruction(char * str);
