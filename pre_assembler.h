#include "globals.h"
#include <stdbool.h>
char* remove_extra_spaces_file(char file_name[]);
void remove_extra_spaces_str(char *str);
int is_space_or_tab(char c);
void remove_spaces_next_to_comma(char *str);
void readMacrosFromFile(const char *filename, node **head);
int implement_macro(char file_name[]);
char * add_new_file(char * name_file,char * ending);
int is_valid_macro_name(char *as_file);
int instr_detection(char *str);
int opcode_detection(char *str);
int reg_detection(char *str);
int extra_char_detection(char *str);
bool endsWithColon(const char *str);
void *handle_malloc(size_t size);
node *make_node(char *name, char *content, int line_num);
node *search_list(node *head, char *name,char *line, int *found);
void add_macro_to_list(node **head, char *name, char *content, int line_num,node *temp);
void free_node(node *node1);
void free_list(node *head);
int add_macro(char *file_name, node **head);