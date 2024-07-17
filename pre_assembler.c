#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "pre_assembler.h"

int implement_macro(char * file_name){
    node *head;
    char *new_file1, *new_file2, *final_file, *temp_file_name1, *temp_file_name2;

    /* Step 1: Removing unnecessary white spaces in the input file and saving the result in a new temp file */
    new_file1 = remove_extra_spaces_file(file_name);
    if (new_file1 == NULL) {
        return 0;
    }

    /* Initialize the macro linked list */
    head = NULL;

    /* Step 2: Scanning and saving all the macros in the input file in a linked list of macros */
    if (!add_macro(new_file1, &head)) {
        free_list(head);
        abrupt_close(2, "%s", new_file1);
        return 0;
    }

    /* Step 3: Removing the declaration of the macros from the input file and saving the result in a new temp file */
    new_file2 = remove_mcros_decl(new_file1);
    if (new_file2 == NULL) {
        free_list(head);
        abrupt_close(2, "%s", new_file1);
        print_internal_error(ERROR_CODE_15);
        return 0;
    }

    /* Free allocated memory for the name of the first temp file*/
    free(new_file1);

    /* Step 4: Replacing all macro calls with their definitions as saved in the linked list and saving the result in a new file */
    final_file = replace_all_mcros(new_file2, head);
    if (final_file == NULL) {
        free_list(head);
        abrupt_close(4, "%s", new_file2);
        print_internal_error(ERROR_CODE_15);
        return 0;
    }

    temp_file_name1 = add_new_file(file_name, ".t01");
    temp_file_name2 = add_new_file(file_name, ".t02");
    remove(temp_file_name1);
    remove(temp_file_name2);

    free(temp_file_name1);
    free(temp_file_name2);


    /* Step 5: Freeing allocated memory for the strings of the new file names */
    free(new_file2);
    free(final_file);
    free_list(head);

    return 1;
}
 