#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "first_pass.h"
int implement_first_pass(char file_name[]){
    char *new_file1, *new_file2, *new_file3, *final_file, *temp_file_name1, *str,*first_word;
    FILE *fp;
    fp = fopen(file_name,"r");
    if(new_file1 ==NULL){
        printf("allocated failed");
        remove(new_file1);
        free(new_file1);
    }
   while (fgets(str, MAX_LINE_LENGTH, fp)) {
    sscanf(str,"%s",first_word);
    if(endsWithColon(first_word)){
        label_process(first_word);
    }
    else if(is_instruction(first_word)){
        instruction_process();
    }
    // int label_process();
    // int instruction_process();
    int opcode_process();
    int file_process();
    int convert_to_binary();
   }
}

