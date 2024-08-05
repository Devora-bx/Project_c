#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "first_pass.h"
#include "pre_assembler.h"

// Add the declarations if they are not included in any header files
// int check_valid_data_comma(char *rest_of_line);
// int is_valid_integer(char *token);
// int starts_and_ends_with_quote(char *rest_of_line);

int label_process(char *first_word, int *p_address) {
    label *head, *temp;
    int found;

    if (strlen(first_word) > MAX_LABEL_LENGTH) { // if the label is too long
        printf("the label is too long");
    }

    head = NULL;
    temp = search_label_on_list(head, first_word, &found); // search if label already defined

    if (!found) {
        if (is_valid_label(first_word)) { // if the label not already defined, check if it's valid
            add_label_to_list(first_word, p_address, &head, temp); // if it's valid, add it
        }
    }

    return 1;
}

label* search_label_on_list(label *head, char *name_of_label, int *found) {
    *found = 0;

    // If the list is empty
    if (head == NULL) {
        return NULL;
    }

    // Check if this is a definition
    if (strcmp(name_of_label, head->name_of_label) == 0) {
        *found = 1;
        printf("label %s already defined\n", name_of_label);
        return head;
    }

    // If the end of the list is reached
    if (head->next == NULL) {
        return head;
    }

    // Recursively search the rest of the list
    return search_label_on_list(head->next, name_of_label, found);
}

int is_valid_label(char *name_of_label) {
    return (!instr_detection(name_of_label) && !opcode_detection(name_of_label) && !reg_detection(name_of_label) && !extra_char_detection(name_of_label));
}

void add_label_to_list(char *name_of_label, int *address_of_label, label **head, label *temp) {
    label *new_label = handle_malloc(sizeof(label));
    new_label->name_of_label = name_of_label;
    new_label->address_of_label = *address_of_label; // Correct the assignment

    if (temp == NULL) {
        *head = new_label;
    } else {
        temp->next = new_label;
    }
}

// Other functions...
/*פונקציה שמקבל מילה שמתחילה בנקודה 
שולחת אותה לפונקציה שבודקת אם זה שם הנחיה או הוראה במידה וכן כבר ישלח לפונקציה 
המתאימה במידה ולא תודפס הודעת שגיאה*/
int instruction_process(char *first_word,char *rest_of_line){
    if(!instr_detection_and_execute(first_word,rest_of_line)){
        printf("undefinde instruchion");
    }
}
/*
הפונקציה גם בודקת שאין פסיקים רצופים
פונקציה שבודקת אם המספרים הם מספרים שלמים חוקיים , אפשר - או + לפני*/
void check_valid_data(char *rest_of_line) {
   
    if (check_valid_data_comma(rest_of_line)) {
        // גיבוי המחרוזת לפני השימוש ב-strtok
        char rest_of_line_copy[MAX_LINE_LENGTH];
        strncpy(rest_of_line_copy, rest_of_line, MAX_LINE_LENGTH);

        char *token = strtok(rest_of_line_copy, ",");
        int valid = 1;
        while (token != NULL) {
            if (!is_valid_integer(token)) {
                valid = 0;
                break;
            }
            token = strtok(NULL, ",");
        }
        if (valid) {
            printf("Valid data\n");
        } else {
            printf("One or more numbers are invalid\n");
        }
    } else {
        printf("Invalid data format\n");
    }
}
/*פונקציה זו בודקת עם המחרוזת שנשלחה תקינה כלומר בודקת אם המרוזת מתיחה ומסתיימת בתו כפול*/
void check_valid_string( char *rest_of_line) {
    if(starts_and_ends_with_quote(rest_of_line))
    {
        printf(" validdd");
    }
    else{
         printf("not valid");
    }
}

void check_valid_extern( char *rest_of_line) {
    printf("Checking valid extern...\n");
}

void check_valid_entry( char *rest_of_line) {
    printf("Checking valid entry...\n");
}
//////////////////////////////////


// void check_valid_data(char *rest_of_line) {
//     if (check_valid_data_comma(rest_of_line)) {
//         // Backup the string before using strtok
//         char rest_of_line_copy[MAX_LINE_LENGTH];
//         strncpy(rest_of_line_copy, rest_of_line, MAX_LINE_LENGTH);

//         char *token = strtok(rest_of_line_copy, ",");
//         int valid = 1;
//         while (token != NULL) {
//             if (!is_valid_integer(token)) {
//                 valid = 0;
//                 break;
//             }
//             token = strtok(NULL, ",");
//         }
//         if (valid) {
//             printf("Valid data\n");
//         } else {
//             printf("One or more numbers are invalid\n");
//         }
//     } else {
//         printf("Invalid data format\n");
//     }
// }

// void check_valid_string(char *rest_of_line) {
//     if (starts_and_ends_with_quote(rest_of_line)) {
//         printf(" validdd");
//     } else {
//         printf("not valid");
//     }
// }

// void check_valid_extern(char *rest_of_line) {
//     printf("Checking valid extern...\n");
// }

// void check_valid_entry(char *rest_of_line) {
//     printf("Checking valid entry...\n");
// }
