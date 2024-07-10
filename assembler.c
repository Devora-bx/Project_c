
#include <stdio.h>
int main(int argc, char *argv[]) {
    char *as_file, *am_file;
    while(--argc){
        /* Generate a new file with the ".as" extension by adding it to the input filename.*/
        printf("Start pre_assembler\n");
        as_file = add_new_file(argv[argc], ".as");

        /*Execute the macro preprocessor on the ".as" file.*/
        if (!is_valid_macro(as_file)) {
            /*If it failed, move to the next file.*/
            continue;


    }
    printf("Start first pass\n");
        /* Generate a new file with the ".am" extension by adding it to the input filename.*/
        am_file = add_new_file(argv[argc], ".am");
        /*Execute the first pass, and then the second on the ".am" file.*/
        if (execute_first_pass(am_file)) {
            /*If it failed, move to the next file.*/
            continue;
        }

        /*Free allocated memory*/
        free(as_file);
        free(am_file);
    }
    printf("end\n");
    return 0;
}

