#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "globals.h"
#include "first_pass.h"
#include "pre_assembler.h"

int label_process(char* first_word, int* p_address, label** label_head,char *type_of_label) {
	size_t len = strlen(first_word);
	int found;
	if (len > 0 && first_word[len - 1] == ':') {
		first_word[len - 1] = '\0';  /* Remove the column */
	}
	if (len > MAX_LABEL_LENGTH) {
		printf("The label is too long\n");
		return 0;
	}

	search_label_on_list(*label_head, first_word, &found);

	if (!found) {
		if (is_valid_label(first_word)) {
			add_label_to_list(first_word, p_address, label_head,type_of_label);
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

/* Function to add a new label to the list */
void add_label_to_list(char* name_of_label, int* address_of_label, label** head,char *type_of_label) {
	label* new_label = (label*)malloc(sizeof(label));
	if (new_label == NULL) {
		perror("Failed to allocate memory for new label");
		exit(EXIT_FAILURE);
	}

	new_label->name_of_label = duplicate(name_of_label);  /* Copy the string */
	if (address_of_label != NULL)
	{
		new_label->address_of_label = *address_of_label;
	}
	else
	{
		new_label->address_of_label = 0;
	}
	new_label->type_of_label = NULL;  /* Initialize or set appropriately */
	new_label->next = NULL;
	update_label_type(new_label,type_of_label);

	if (*head == NULL) {
		*head = new_label;
	}
	else {
		label* current = *head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = new_label;
	}

}


/* Function to search for a label in the list */
label* search_label_on_list(label* head, char* name_of_label, int* found) {
	label* current = head;
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

/* Function to check if a label is valid */
int is_valid_label(char* name_of_label) {
	return (!instr_detection(name_of_label) &&
		!opcode_detection(name_of_label) &&
		!reg_detection(name_of_label) &&
		!extra_char_detection(name_of_label) &&
		isalpha(*name_of_label));
}


int instruction_data_process(char* first_word, char* rest_of_line, int* DC, int line, data_image** data_image_head) {
	if (!instr_data_detection(first_word, rest_of_line, DC, line, data_image_head)) {
		printf("undefinde instruchion");
		return 0;
	}
	return 1;
}
/*
הפונקציה גם בודקת שאין פסיקים רצופים
פונקציה שבודקת אם המספרים הם מספרים שלמים חוקיים , אפשר - או + לפני*/
void check_valid_data(char* rest_of_line, int* DC, int line, data_image** data_image_head) {
	char* token;
	
	if (check_valid_data_comma(rest_of_line)) {
		/*גיבוי המחרוזת לפני השימוש ב-strtok*/
		char rest_of_line_copy[MAX_LINE_LENGTH];
		strncpy(rest_of_line_copy, rest_of_line, MAX_LINE_LENGTH);
		token = strtok(rest_of_line_copy, ",");
		while (token != NULL) {
			if (is_valid_integer(token)) {
				
				add_to_data_image(atoi(token),*DC, data_image_head,line);
				(*DC)++;
			}
			else {
				
				printf("One or more numbers are invalid");
				break;
			}
			token = strtok(NULL, ",");
		}
		
	}
	else {
		printf("Invalid data format in line: %d\n", line);
	}
}
/*פונקציה זו בודקת עם המחרוזת שנשלחה תקינה כלומר בודקת אם המרוזת מתיחה ומסתיימת בתו כפול*/
void check_valid_string(char* rest_of_line, int* DC, int line, data_image** data_image_head) {
	int i;
	if (starts_and_ends_with_quote(rest_of_line)) {
		
		/*הוספת תווים לרשימה המקושרת*/
		for (i = 1; i < (int)strlen(rest_of_line) - 1; i++) { /*מתחילים מהתו הראשון לאחר הציטוט הראשון ועד לתו לפני הציטוט האחרון*/
			
			add_to_data_image((int)rest_of_line[i],*DC, data_image_head,line);
			(*DC)++;
		}
		
		add_to_data_image((int)'\0',*DC, data_image_head,line); /*הוספת תו סוף המחרוזת*/
		(*DC)++;
	}
	else {
		printf("Invalid string in line: %d\n", line);
	}
}

int check_valid_entry(char* rest_of_line, int* DC, label* label_head) {
	label* lbl = find_label_by_name(rest_of_line, label_head);
	char* label;
	if (lbl != NULL) {
		printf("should update lable: %s to entry type", rest_of_line);
		update_label_type(lbl, ".entry");
		printf("the label after update is: %s\n", lbl->type_of_label);
		label = malloc(strlen(rest_of_line) + 20); /* 20 is a safe over-estimation for the integer conversion */
		if (!label) {
			fprintf(stderr, "Memory allocation failed\n");
			return 0;
		}

		/* Create the new string */
		sprintf(label, "%s %d\n", rest_of_line, *DC);

		return 1;
	}
	else {
		printf("Label %s not found in current file and can't defined as entry\n", rest_of_line);
		return 0;
	}
}
void update_data_label(label *label_head,int IC){
	label* current = label_head;
	while (current != NULL) {
		if (strcmp(current->type_of_label,".data") == 0) {
			current->address_of_label += IC;
		}
		current = current->next;
	}
	
}





void add_to_data_image(int value, int address, data_image** data_image_head, int line) {
    

    // הקצאת זיכרון למחרוזת הבינארית (16 ביטים + '\0' לסיום)
    char binary_value[17]; 
    binary_value[16] = '\0';

    // המרה לבינארית
    for (int i = 15; i >= 0; i--) {
        binary_value[i] = (value & 1) ? '1' : '0';
        value >>= 1;
    }
   

    // יצירת צומת חדש ל-data_image
    data_image* new_node = (data_image*)malloc(sizeof(data_image));
    if (new_node == NULL) {
        printf("Memory allocation failed\n");
        return; // יציאה אם הקצאת הזיכרון נכשלה
    }
    
    new_node->binary_value = strdup(binary_value); // שכפול המחרוזת
    new_node->address = address;
    new_node->line = line;
    new_node->next = NULL;
    

    // הוספת הצומת לרשימה המקושרת
    if (*data_image_head == NULL) {
        *data_image_head = new_node;
    } else {
        data_image* current = *data_image_head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
	
}




void add_to_instruction_memory(int line,int address,char* binary_str , instruction_memory** instruction_memory_head) {
	instruction_memory* new_instruction = (instruction_memory*)malloc(sizeof(instruction_memory));
	new_instruction->address = address;
	new_instruction->line = line;
	new_instruction->binary_str = duplicate(binary_str);
	new_instruction->next = NULL;

	if (*instruction_memory_head == NULL) {
		*instruction_memory_head = new_instruction;
	}
	else {
		instruction_memory* current = *instruction_memory_head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = new_instruction;
	}
}
label* find_label_by_name(const char* name_of_label, label* label_head) {
	label* current = label_head;
	while (current != NULL) {
		if (strcmp(current->name_of_label, name_of_label) == 0) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}
void update_label_type(label* lbl, const char* new_type) {
	/* printf("in update type!!\n"); */
	if (lbl->type_of_label != NULL) {
		free(lbl->type_of_label);
	}
	lbl->type_of_label = duplicate(new_type);
}
void create_entry_file(const char* filename, const char* rest_of_line, label* label_head) {
	FILE* file;
	label* lbl = find_label_by_name(rest_of_line, label_head);
	if (lbl == NULL) {
		printf("Label %s not found\n", rest_of_line);
		return;
	}

	file = fopen(filename, "w");
	if (file == NULL) {
		perror("Error opening file");
		return;
	}

	fprintf(file, "%s %d\n", rest_of_line, lbl->address_of_label);

	fclose(file);
}

void convert_str_to_binary(int length, char *str, char *ARE) {
    char *token;
    char binary[16];
    char result[30] = "";
	if(str[0]=='\0' ||!strcmp(str,"NULL")){/*if its label or empty argument*/
		return;
	}
	else{
		token = strtok(str, ",");
		while (token != NULL) {
			int num = atoi(token);
			decimal_to_binary(length, num, binary);
			strcat(result, binary);
			token = strtok(NULL, ",");
		}

		strcat(result, ARE);
		printf("Converted to Binary: %s\n", result);
		strcpy(str, result);
	}
		
}


void convert_first_word_to_binary(int length, char* str, char* ARE) {

	char* token;
	char binary[16];
	int first_number = 1;
	char result[30] = "";
	
	token = strtok(str, ",");
	while (token != NULL) {
		int num = atoi(token);
		if (first_number && (num >= 0)) {
			decimal_to_binary(length, num, binary);
			first_number = 0;
		} else {
			convert_to_binary(length, num, binary);
		}
		strcat(result, binary);
		token = strtok(NULL, ",");
	}
	
	strcat(result, ARE);
	printf("Concatenated Binary: %s\n", result);
	strcpy(str, result);
}

void decimal_to_binary(int length, int decimal, char binary[])
{
	int i;
	int is_negative = 0;
	char temp;
	/* Handle negative numbers */
	if (decimal < 0)
	{
		is_negative = 1;
		decimal = -decimal;
	}
	/* Handle special case for 0 */
	if (decimal == 0)
	{
		memset(binary, '0', length);
		binary[length] = '\0';
		/*  printf(" Binary number is: %s\n", binary); */
		return;
	}
	/* Convert the decimal number to binary */
	i = 0;
	while (decimal > 0)
	{
		binary[i++] = (decimal % 2) ? '1' : '0';
		decimal /= 2;
	}
	/* Pad the binary representation with '0's to ensure it has the correct length */
	while (i < length)
		binary[i++] = '0';

	/* Null-terminate the string */
	binary[i] = '\0';
	/* Reverse the binary array to get the correct representation */
	for (i = 0; i < length / 2; i++)
	{
		temp = binary[i];
		binary[i] = binary[length - i - 1];
		binary[length - i - 1] = temp;
	}
	/* If number was negative, compute two's complement */
	if (is_negative)
	{
		for (i = 0; i < length; i++)
		{
			binary[i] = (binary[i] == '0') ? '1' : '0'; /*Invert bits */
		}
		/* Add 1 to the binary number */
		for (i = length - 1; i >= 0; i--)
		{
			if (binary[i] == '0')
			{
				binary[i] = '1';
				break;
			}
			else
			{
				binary[i] = '0';
			}
		}
	}
}

void convert_to_binary(int length, int decimal, char binary[]) {

	memset(binary, '0', length);

	if (decimal >= 0 && decimal < length) {
		binary[length - decimal - 1] = '1';
	}

	binary[length] = '\0';
}



int opcode_process(char* first_word, char* rest_of_line, int* IC,int line,instruction_memory **instruction_memory_head) {
	
	int detected_label_on_first_pass = 0;
	int num_of_opcode = 0;
	int fieldBitSize1 = 0, fieldBitSize2 = 0, fieldBitSize3 = 0;
	char* first_word_to_binary = (char*)calloc(16, sizeof(char)); /* Allocate memory for the binary string */
	char* second_word_to_binary = (char*)calloc(16, sizeof(char)); /* Allocate memory for the binary string */
	char* third_word_to_binary = (char*)calloc(16, sizeof(char)); /* Allocate memory for the binary string */
	
	if (valid_num_argument(first_word, rest_of_line, &num_of_opcode)) {
		if(!parsing_arg(first_word, rest_of_line, first_word_to_binary, second_word_to_binary, third_word_to_binary,
		&fieldBitSize1, &fieldBitSize2, &fieldBitSize3, &detected_label_on_first_pass))
		{
			printf("invalid argument in line: %d\n",line);
			return 0;
		}
		
		if (first_word_to_binary != NULL) {
			convert_first_word_to_binary(fieldBitSize1, first_word_to_binary, "100");
			convert_str_to_binary(fieldBitSize2, second_word_to_binary, "100");
			convert_str_to_binary(fieldBitSize3, third_word_to_binary, "100");
		
		}
		
			add_to_instruction_memory(line,*IC,first_word_to_binary,instruction_memory_head);
			(*IC)++;

			
			if (strcmp(second_word_to_binary,"NULL") != 0) {/*if its not empty*/
				add_to_instruction_memory(line,*IC,second_word_to_binary,instruction_memory_head);
				(*IC)++;
			}
			
			
			if (strcmp(third_word_to_binary,"NULL") != 0) {
				add_to_instruction_memory(line,*IC,third_word_to_binary,instruction_memory_head);
				(*IC)++;
			}
		
			free(first_word_to_binary);
			free(second_word_to_binary);
			free(third_word_to_binary);
			return 1;
		}
	
	free(first_word_to_binary);
	free(second_word_to_binary);
	free(third_word_to_binary);
	return 0;
}


void free_label_list(label* head) {
	label* current = head;
	label* next;

	while (current != NULL) {
		next = current->next;
		free(current->name_of_label);
		free(current->type_of_label);
		free(current);
		current = next;
	}
}

void free_instruction_memory(instruction_memory* head) {
	instruction_memory* current = head;
	instruction_memory* next;

	while (current != NULL) {
		next = current->next;
		free(current->binary_str);
		free(current);
		current = next;
	}
}

void free_data_image(data_image* data_image_head) {
	data_image* current = data_image_head;
	data_image* next_node;

	while (current != NULL) {
		next_node = current->next;
		free(current);
		current = next_node;
	}

	data_image_head = NULL;
}

