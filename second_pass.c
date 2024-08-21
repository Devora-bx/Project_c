#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "globals.h"
#include "first_pass.h"
#include "second_pass.h"
#include "pre_assembler.h"

int implement_second_pass(char file_name[],label** label_head,instruction_memory **list_head,data_image **data_image_head){
    char str[MAX_LINE_LENGTH] = {0};
    char first_word[MAX_LINE_LENGTH] = {0};
    char second_word[MAX_LINE_LENGTH] = {0};
    char rest_of_line[MAX_LINE_LENGTH] = {0};
    char operands[MAX_LINE_LENGTH] = {0};
    char *ent_file, *ext_file;
    FILE *fp, *ent_p, *ext_p;
    
    int line = 0;
    int num_of_section_in_line;
    fp=fopen(file_name, "r");
    if(fp == NULL){
        printf("Failed to open file: %s\n", file_name);
        return 1;
    }
    ent_file = add_new_file(file_name, ".ent");
    ent_p = fopen(ent_file, "w");
    if (ent_p == NULL) {
        printf("Failed to open file: %s\n", ent_file);
        fclose(fp);  
        return 0;
    }

    ext_file = add_new_file(file_name, ".ext");
    ext_p = fopen(ext_file, "w");
    if (ext_p == NULL) {
        printf("Failed to open file: %s\n", ext_file);
        fclose(fp);  
        free(ext_file);  
        return 0;
    }
    fseek(fp, 0, SEEK_SET);
    while (fgets(str,MAX_LINE_LENGTH, fp))
    {
        line++;
        memset(first_word, 0, MAX_LINE_LENGTH);
        memset(second_word, 0, MAX_LINE_LENGTH);
        memset(rest_of_line, 0, MAX_LINE_LENGTH);
        
         sscanf(str, " %s %s %[^\n]", first_word, second_word, rest_of_line);
        /*printf("Parsed line %d: first_word='%s', second_word='%s', rest_of_line='%s'\n", line, first_word, second_word, rest_of_line);*/
        printf("%s, %d\n",first_word,line);
        if(is_empty_line(str)){
            line++;
             skip_to_next_line(fp);
        }
        if(endsWithColon(first_word)){
            if (is_instruction(second_word)){/*becous it's can't be .entry or .extern*/
                skip_to_next_line(fp);
            }
            else{
                
                chek_for_label_argument(ext_p,rest_of_line,line,label_head,list_head,data_image_head);
            }
        }
        else if(is_instruction(first_word)){
            skip_to_next_line(fp);
        }
        else {
            chek_for_label_argument(ext_p,second_word,line,label_head,list_head,data_image_head);
        }
    }    
    fclose(fp);
    fclose(ext_p);
    /* שחרור זיכרון שהוקצה לשם הקובץ החדש */
    free(ext_file);

    printf("File second pass closed: %s\n", file_name);
    return 0; 

}
