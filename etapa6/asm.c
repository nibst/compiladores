#include "asm.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

char *INIT = "##FIXED INIT\n"
             ".section	.rodata\n"
             "printintstr: .asciz	\"%d\" \n"
             "printstringstr: .asciz \"%s\" \n"
             "printcharstr: .asciz \"%c\" \n"
             ".text\n";
char *BEGIN_FUN = "##TAC_BEGINFUNC\n"
                   "\t.globl	_%s\n"
                   "_%s:\n"
                   "\tpushq	%%rbp\n"
                   "\tmovq	%%rsp, %%rbp\n";
char *END_FUN = "##TAC_ENDFUN\n"
	"\tpopq %rbp\n"
	"\tret\n";
char *PRINT_NUMBER = "##TAC_OUTPUT\n"
	"movl	_%s(%%rip), %%esi\n"
	"leaq	printintstr(%%rip), %%rax\n"
	"movq	%%rax, %%rdi\n"
	"movl	$0, %%eax\n"
	"call	printf@PLT\n";
char *PRINT_STRING = "##TAC_OUTPUT\n"
	"leaq	_%s(%%rip), %%rax\n"
	"movq	%%rax, %%rsi\n"
	"leaq	printstringstr(%%rip), %%rax\n"
	"movq	%%rax, %%rdi\n"
    "movl	$0, %%eax\n"
	"call	printf@PLT\n";

char *PRINT_CHAR = "##TAC_OUTPUT\n"
	"movl	_%s(%%rip), %%esi\n"
	"leaq	printcharstr(%%rip), %%rax\n"
	"movq	%%rax, %%rdi\n"
	"movl	$0, %%eax\n"
	"call	printf@PLT\n";
char* VARIABLE = "_%s:\t.long\t%s\n";
char* STRING = "_%s:\t.string\t%s\n";
char* DATA_SECTION = "#DATA SECTION\n"
                    ".data\n\n";

void printVariables(FILE *fout, HashNode* hash_table[], int hash_size);
void printConstants(FILE *fout, HashNode* hash_table[], int hash_size);
void printString(FILE *fout,char* string_text);
void printOutput(FILE *fout);
void printChar(FILE *fout,char* char_text);
void printDataSection(FILE *fout, HashNode* hash_table[], int hash_size){
    fprintf(fout,"%s",DATA_SECTION);
    printVariables(fout, hash_table, hash_size);
    printConstants(fout, hash_table, hash_size);
}
void printVariables(FILE *fout, HashNode* hash_table[], int hash_size){
    HashNode *node;
    for(int i=0; i<hash_size;++i){
        for (node=hash_table[i]; node; node = node->next){
            if(node->type == SYMBOL_VARIABLE)
                fprintf(fout,VARIABLE,node->text,node->declared_value ? node->declared_value : "0"); 
        }
    }
}
void printConstants(FILE *fout, HashNode* hash_table[], int hash_size){
    HashNode *node;
    
    for(int i=0; i<hash_size;++i){
        for (node=hash_table[i]; node; node = node->next){
            if(node->type == SYMBOL_LIT_INT || node->type == SYMBOL_LIT_REAL){
                fprintf(fout,VARIABLE,node->text,node->text); 
            }
            if(node->type == SYMBOL_LIT_CHAR){
                printChar(fout,node->text);
            }
            if(node->type == SYMBOL_LIT_STRING){
                printString(fout,node->text);
            }
        }
    }
}
void printChar(FILE *fout,char* char_text){
    if (char_text != NULL){
        // Extract the character between single quotes and convert it to an integer.
        int int_value = (int)char_text[1];
        
        // Convert the integer to a string and allocate memory for it.
        int str_size = snprintf(NULL, 0, "%d", int_value) + 1; // Calculate the size of the string.
        char* string_of_char_ascii_decimal = (char*)malloc(str_size);

        // Convert the integer to a string and store it in declared_value.
        snprintf(string_of_char_ascii_decimal, str_size, "%d", int_value);
        fprintf(fout,VARIABLE,char_text,string_of_char_ascii_decimal); 
        free(string_of_char_ascii_decimal);
    }
}
char* generateStringVariableName(char* string_text){
    //creates name like : str<address_of_string>
    if (string_text != NULL) {
        uintptr_t ptr_value = (uintptr_t)string_text;
        char* prefix = "str";
        // Convert the uintptr_t to a string and allocate memory for it.
        int str_size = snprintf(NULL, 0, "%lx", ptr_value) + 1 + strlen(prefix); // Use "%lx" for hexadecimal output. +1 for \0 ; 
        char* str_name = (char*)malloc(str_size);
        strcpy(str_name,prefix);
        if (str_name != NULL) {
            char* str_temp = (char*)malloc(str_size);
            // Convert the uintptr_t to a string and store it in str_name.
            snprintf(str_temp, str_size, "%lx", ptr_value);
            strcat(str_name,str_temp);
            free(str_temp);
            // Free the allocated memory for str_name.
            return str_name;
        }
    }
}
void printString(FILE *fout,char* string_text){
    
    char* str_name = generateStringVariableName(string_text);
    fprintf(fout, STRING, str_name, string_text);
    // Free the allocated memory for str_name.
    free(str_name);
}
 
void printOutputFunction(FILE* fout, TAC* tac_output){
    TAC* tac;
    if(tac_output->prev == NULL) return;

    for(tac = tac_output->prev; doesTacExpectArguments(tac) == 0 && tac != NULL; tac = tac->prev){
        if (tac != NULL && tac->result != NULL) {
            if(tac->result->type == SYMBOL_LIT_STRING){
                char* str_name = generateStringVariableName(tac->result->text);
                fprintf(fout,PRINT_STRING,str_name);
                free(str_name);
            }
            

        }
    }

}
void generateAsm(TAC *first)
{
    FILE *fout;
    fout = fopen("out.s", "w");

    // INIT
    fprintf(fout, "%s", INIT);

    TAC *tac;
    for (tac = first; tac; tac = tac->next){
        switch (tac->type){
            case TAC_BEGIN_FUN:
                fprintf(fout, BEGIN_FUN, tac->result->text, tac->result->text);//potentially segmentation fault ***; tac->result->text can be NULL
                break;
            case TAC_END_FUN:
                fprintf(fout, "%s", END_FUN);
                break;
            case TAC_OUTPUT:
                printOutputFunction(fout,tac);
                break;
            case TAC_ADD:

            default:
                break;
        }
    }
    printDataSection(fout,g_hash_table,HASH_SIZE);
    fclose(fout);
}