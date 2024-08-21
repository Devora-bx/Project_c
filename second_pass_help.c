#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "globals.h"
#include "first_pass.h"
#include "second_pass.h"
#include "pre_assembler.h"
void skip_to_next_line(FILE *fp) {
    int ch;
    // לקרוא תווים אחד אחרי השני עד שמגיעים לסוף השורה או לסוף הקובץ
    while ((ch = fgetc(fp)) != '\n' && ch != EOF) {
        // לא עושים כלום, רק ממשיכים לקרוא את התווים
    }
}
void chek_for_label_argument(FILE *ext_p, char * rest_of_line, int line, label** label_head,instruction_memory **list_head,data_image **data_image_head ) {
    label  *label;
    
    int address;
    int found;
    
    char address_label[MAX_LINE_LENGTH];  // String to store the converted value
    char address_label_binary[MAX_LINE_LENGTH];
    char rest_of_line_copy[MAX_LINE_LENGTH] = {0};
    char label_entry[MAX_LINE_LENGTH];

    strncpy(rest_of_line_copy, rest_of_line, MAX_LINE_LENGTH);
    
    char *token = strtok(rest_of_line_copy, ",");
    
    while (token != NULL) {
        
        // Check each operand individually
        if (!check_operand_start(token)) { // Check if the operand is a label
            label = search_label_on_list(*label_head, token, &found);
            if (label) {
                printf("debug the address: %d\n",label->address_of_label);
                sprintf(address_label, "%d", label->address_of_label);
                sprintf(address_label_binary, "%d", label->address_of_label);
               
                if (strcmp(".extern", label->type_of_label) == 0) {
                    
                    convert_str_to_binary(12, address_label_binary, "001");
                    
                    insert_label_address(*list_head,*data_image_head, address_label_binary,line);
                    
                    // if (address != -1) {
                    //     sprintf(label_entry, "%s %d", label->name_of_label, address);
                    //     fprintf(ext_p, "%s\n", label_entry);
                    // } else {
                    //     printf("Label '%s' not found\n", label->name_of_label);
                    
                    // } 
                } else {
                    
                    convert_str_to_binary(12, address_label_binary, "010");
                    
                    insert_label_address(*list_head,*data_image_head, address_label_binary,line);
                }
                
            // } else {
            //     printf("Label '%s' not found\n", token);
            // }
        }
        
    }
    token = strtok(NULL, ","); // Move to the next operand
}
}

void insert_label_address(instruction_memory *head,data_image *data_image_head,char *address_label_binary,int line) {
    
    instruction_memory *current1 = head;
    data_image *current2 = data_image_head;
    
    while (current1 != NULL) {
        
       if (current1->line == line && current1->binary_str[0]=='\0'){
        
            current1->binary_str = duplicate(address_label_binary);
            return;
       }
        current1 = current1->next;
    }  

}