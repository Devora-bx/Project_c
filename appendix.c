#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pre_assembler.h"
#include "globals.h"

//this function relate to main and add the ending to files
                                                                                                                                                                                                                    char *add_new_file(char *file_name, char *ending) {
    char *c, *new_file_name;
    new_file_name = handle_malloc(MAX_LINE_LENGTH * sizeof(char));
    strcpy(new_file_name, file_name);
    /* deleting the file name if a '.' exists and forth */
    if ((c = strchr(new_file_name, '.')) != NULL) {
        *c = '\0';
    }
    /* adds the ending of the new file name */
    strcat(new_file_name, ending);
    printf(" after add ending is: %s",new_file_name);
    return new_file_name;
}
///////////////////this functions cleaning the files from extra space//////////////////////////////
void remove_extra_spaces_str(char * str) {
   int i, j;
    char str_temp[MAX_LINE_LENGTH];
    i = j = 0;
    /* eliminating white-spaces in the beginning of the line */
    while (is_space_or_tab(*(str + i))) {
        i++;
    }
    while (*(str + i) != '\0') {
        /* copying character */
        while (!is_space_or_tab(*(str + i)) && *(str + i) != '\0') {
            *(str_temp + j) = *(str + i);
            i++;
            j++;
        }
        /* if loop stopped because end of line char */
        if (*(str + i) == '\0') {
            break;
        }
        /* if loop stopped because of a white-space skipping them until another character is encountered*/
        while (is_space_or_tab(*(str + i))) {
            i++;
        }
        /* if stopped not because of end of line char then copy one space for all the others that were skipped */
        if (!(*(str + i) == '\n' || *(str + i) == '\0')) {
            *(str_temp + j) = ' ';
            j++;
        }
    }
    *(str_temp + j) = *(str + i);
    *(str_temp + j + 1) = '\0';
    remove_spaces_next_to_comma(str_temp);
    strcpy(str, str_temp);
}

int is_space_or_tab(char c) {
    /* Check if the char is a space or a tab */
    return (isspace(c) && c != '\n');
}

void remove_spaces_next_to_comma(char *str) {
    char *ptr = str;
    /* If the line starts with ',' avoiding accessing outside the str */
    if(*ptr == ','){
        return;
    }
    while ((ptr = strchr(ptr, ',')) != NULL) {
        /* Space before the comma */
        if (*(ptr - 1) == ' ') {
            memmove(ptr - 1, ptr, strlen(ptr) + 1);
            if (*(ptr) == ' ') {
                /* Also space after the comma */
                memmove(ptr, ptr + 1, strlen(ptr + 1) + 1);
            }
        }
        else if (*(ptr + 1) == ' ') {
            /* Only space after the comma */
            memmove(ptr + 1, ptr + 2, strlen(ptr + 2) + 1);
            ptr++;
        }else{
            ptr++;
        }
    }
}
/*////////////////// all this function above cleaning the files///////////////////////////////////

/void readMacrosFromFile(const char *filename, Macro **head) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "macr", 4) == 0) {//if there a define of macro add it to the macro table//
            char name[81];
            char content[768];
            if (sscanf(line, "macr %255s %767[^\n]", name, content) == 2) {
                printf("will added");
            }
        }
    }
    
    fclose(file);
}
/////////////////////////////////////////////////////////////////////////////////////*/

void *handle_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "Error: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}
node *make_node(char *name, char *content, int line_num){
    node *temp;

    /* Check if memory allocation for the node succeeded */
    temp = handle_malloc(sizeof(node));

    temp->macro_name = name;        /* Set the name of the node */
    temp->macro_content = content;  /* Set the content string of the node */
    temp->macro_line = line_num;    /* Set the line number associated with the content */
    temp->next = NULL;        /* Initialize the next pointer to NULL */

    return temp;  /* Return a pointer to the newly created node */
}
node *search_list(node *head, char *name, int *found){
    *found = 0;

    /* If the list is empty */
    if (head == NULL) {
        return NULL;
    }

    /* fix to check if this is defination*/
    if (strcmp(name, head->macro_name) == 0) {
        *found = 1;
        printf("Node %s already exists in the list\n", name);
        return head;
    }

    /* If the end of the list is reached */
    if (head->next == NULL) {
        return head;
    }

    /* Recursively search the rest of the list */
    return search_list(head->next, name, found);
}

int is_valid_macro_name(char *name_macr){
    return(!instr_detection(name_macr) && !opcode_detection(name_macr) && !reg_detection(name_macr) && !extra_char_detection(name_macr));
 }

void add_macro_to_list(node **head, char *name, char *content, int line_num){
    int found;
    node *new_node, *temp;
    found = 0;

    /* Temp is the immediate parent of the new node in the list
     * or if the macro name already exists in the list, temp is the mcro with the same name.
     * */
    temp = search_list(*head,name,&found);

    /* If the list already has a macro with the same name */
    if(!found && strcmp(temp->macro_content,content) != 0){
        /* The content of the same node name is not the same - skipping this macro definition */
        printf("ERROR_CODE_13");
        free(name);
        free(content);
        return;
    }

    /* If the macro with the same name is not found in the list */
    if(!found){
        new_node = make_node(name,content,line_num);

        /* If the list is empty, add the new node to the head of the list */
        if(temp == NULL){
            *head = new_node;
        }

        /* If the list is not empty, add the new node down the list */
        else{
            temp->next = new_node;
        }
    }
}

void free_node(node *node1){
    /* Free memory allocated for the name, content and node */
    free(node1->macro_name);
    free(node1->macro_content);
    free(node1);
}

void free_list(node *head){
    /* Free memory for the current node and its contents while storing the current node in a temporary pointer */
    while(head != NULL) {
        node *temp = head;
        head = head->next;
        free_node(temp);
    }
}
