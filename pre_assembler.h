void remove_extra_spaces_str(char str[]);
int is_space_or_tab(char c);
void remove_spaces_next_to_comma(char *str);
void readMacrosFromFile(const char *filename, Macro **head);
int implement_macro(char * file_name);
char * add_new_file(char * name_file,char * ending);