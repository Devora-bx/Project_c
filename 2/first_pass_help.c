#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "first_pass.h"
#include "pre_assembler.h"

data_image *data_image_head = NULL;
label *label_head = NULL;

// Add the declarations if they are not included in any header files
// int check_valid_data_comma(char *rest_of_line);
// int is_valid_integer(char *token);
// int starts_and_ends_with_quote(char *rest_of_line);

int label_process(char *first_word, int *p_address) {
    size_t len = strlen(first_word);
    if (len > 0 && first_word[len - 1] == ':') {
        first_word[len - 1] = '\0';  // Remove the colon
    }

    int found;
    label *temp = search_label_on_list(label_head, first_word, &found);

    if (!found) {
        if (is_valid_label(first_word)) {
            add_label_to_list(first_word, p_address, &label_head);
        }
    }

    return 1;
}

char* duplicate(const char* str)
{
    char* new_str = (char*)malloc((strlen(str) + 1) * sizeof(char));
    strcpy(new_str, str);
    return new_str;
}

// Function to add a new label to the list
void add_label_to_list(char *name_of_label, int *address_of_label, label **head) {
    label *new_label = (label *)malloc(sizeof(label));
    if (new_label == NULL) {
        perror("Failed to allocate memory for new label");
        exit(EXIT_FAILURE);
    }

    new_label->name_of_label = duplicate(name_of_label);  // Copy the string
    new_label->address_of_label = *address_of_label;
    new_label->type_of_label = NULL;  // Initialize or set appropriately
    new_label->next = NULL;

    if (*head == NULL) {
        *head = new_label;
    } else {
        label *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_label;
    }
}


// Function to search for a label in the list
label* search_label_on_list(label *head, char *name_of_label, int *found) {
    label *current = head;
    *found = 0;

    while (current != NULL) {
        if (strcmp(name_of_label, current->name_of_label) == 0) {
            *found = 1;
            printf("label %s already defined\n", name_of_label);
            return current;
        }
        current = current->next;
    }

    return NULL;
}

// Function to check if a label is valid
int is_valid_label(char *name_of_label) {
    return (!instr_detection(name_of_label) &&
            !opcode_detection(name_of_label) &&
            !reg_detection(name_of_label) &&
            !extra_char_detection(name_of_label));
}

// Function to add a new label to the list
// void add_label_to_list(char *name_of_label, int *address_of_label, label **head) {
//     label *new_label = (label *)malloc(sizeof(label));
//     if (new_label == NULL) {
//         perror("Failed to allocate memory for new label");
//         exit(EXIT_FAILURE);
//     }

//     new_label->name_of_label = strdup(name_of_label);  // Copy the string
//     new_label->address_of_label = *address_of_label;
//     new_label->next = NULL;

//     if (*head == NULL) {
//         *head = new_label;
//     } else {
//         label *current = *head;
//         while (current->next != NULL) {
//             current = current->next;
//         }
//         current->next = new_label;
//     }
//      label_head= new_label;
// }
// Other functions...
/*פונקציה שמקבל מילה שמתחילה בנקודה 
שולחת אותה לפונקציה שבודקת אם זה שם הנחיה או הוראה במידה וכן כבר ישלח לפונקציה 
המתאימה במידה ולא תודפס הודעת שגיאה*/
int instruction_data_process(char *first_word,char *rest_of_line,int * DC){
    if(!instr_data_detection(first_word,rest_of_line,DC)){
        printf("undefinde instruchion");
        return 0;
    }
    return 1;
}
/*
הפונקציה גם בודקת שאין פסיקים רצופים
פונקציה שבודקת אם המספרים הם מספרים שלמים חוקיים , אפשר - או + לפני*/
void check_valid_data(char *rest_of_line ,int *DC) {
    if (check_valid_data_comma(rest_of_line)) {
        /*גיבוי המחרוזת לפני השימוש ב-strtok*/
        char rest_of_line_copy[MAX_LINE_LENGTH];
        strncpy(rest_of_line_copy, rest_of_line, MAX_LINE_LENGTH);
        char *token = strtok(rest_of_line_copy, ",");
        int valid = 1;
        int count = 0;
        while (token != NULL) {
            if (is_valid_integer(token)) {
                 add_to_data_image(atoi(token));
                 count++;
            }
            else{
                valid = 0;
                break;
            }
            token = strtok(NULL, ",");
        }
        if (valid) {
              *DC += count;
        }
        else{
            printf("One or more numbers are invalid");
        }
    } else {
        printf("Invalid data format\n");
    }
}
/*פונקציה זו בודקת עם המחרוזת שנשלחה תקינה כלומר בודקת אם המרוזת מתיחה ומסתיימת בתו כפול*/
void check_valid_string(char *rest_of_line, int *DC) {
    if (starts_and_ends_with_quote(rest_of_line)) {
        /*עדכון המתשנה DC*/
        int length = strlen(rest_of_line) - 2; /*מורידים 2 כדי לא לכלול את הציטוטים*/
        *DC += length + 1; /*אורך המחרוזת ועוד 1*/

        /*הוספת תווים לרשימה המקושרת*/
        for (int i = 1; i < (int)strlen(rest_of_line) - 1; i++) { /*מתחילים מהתו הראשון לאחר הציטוט הראשון ועד לתו לפני הציטוט האחרון*/
            add_to_data_image((int)rest_of_line[i]);
        }
        add_to_data_image((int)'\0'); /*הוספת תו סוף המחרוזת*/

       
    } else {
        printf("Invalid string\n");
    }
}

char * check_valid_entry(char *rest_of_line, int *DC) {
    label *lbl = find_label_by_name(rest_of_line);
    if (lbl != NULL) {
        update_label_type(lbl, ".entry");
        char *label = malloc(strlen(rest_of_line) + 20); // 20 is a safe over-estimation for the integer conversion
        if (!label) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }

        // Create the new string
        sprintf(label, "%s %d", rest_of_line, *DC);

        return label;
    } else {
        printf("Label %s not found\n", rest_of_line);
         return NULL;
    }
}

void check_valid_extern( char *rest_of_line,int *DC) {
    printf("Checking valid extern...\n");
}


void add_to_data_image(int value) {
    data_image *new_node = (data_image *)malloc(sizeof(data_image));
    new_node->value = value;
    new_node->next = NULL;

    if (data_image_head == NULL) {
        data_image_head = new_node;
    } else {
        data_image *current = data_image_head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}
label* find_label_by_name(const char *name_of_label) {
    label *current = label_head;
    while (current != NULL) {
        if (strcmp(current->name_of_label, name_of_label) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}
void update_label_type(label *lbl, const char *new_type) {
    if (lbl->type_of_label != NULL) {
        free(lbl->type_of_label);
    }
    lbl->type_of_label = duplicate(new_type);
}
void create_entry_file(const char *filename, const char *rest_of_line) {
    label *lbl = find_label_by_name(rest_of_line);
    if (lbl == NULL) {
        printf("Label %s not found\n", rest_of_line);
        return;
    }

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(file, "%s %d\n", rest_of_line, lbl->address_of_label);

    fclose(file);
}

int convert_to_binary(char *str){
    ///not implemented???
    return 0;
}