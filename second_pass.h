#include "globals.h"
#include <stdbool.h>
//int implement_second_pass(char file_name[]);
void skip_to_next_line(FILE *fp);
void chek_for_label_argument(FILE *ext_p, char * rest_of_line,int line,label** label_head,instruction_memory **list_head,data_image **data_image_head);
bool check_operand_start(char *operand);
int implement_second_pass(char file_name[],label** label_head,instruction_memory **list_head,data_image **data_image_head);
void insert_label_address(instruction_memory *head,data_image *data_image_head,char *address_label_binary,int line);