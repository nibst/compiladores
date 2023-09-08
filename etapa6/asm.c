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
             "printfloatstr: .asciz \"%f\" \n"
             ".text\n";
char *BEGIN_FUN = "##TAC_BEGINFUNC\n"
                   "\t.globl	%s\n"
                   "%s:\n"
                   "\tpushq	%%rbp\n"
                   "\tmovq	%%rsp, %%rbp\n";
char *END_FUN = "##TAC_ENDFUN\n"
	"\tpopq %rbp\n"
	"\tret\n";
char *PRINT_INT = "##TAC_OUTPUT\n"
	"\tmovl	_%s(%%rip), %%esi\n"
	"\tleaq	printintstr(%%rip), %%rax\n"
	"\tmovq	%%rax, %%rdi\n"
	"\tmovl	$0, %%eax\n"
	"\tcall	printf@PLT\n";
char *PRINT_REAL = "##TAC_OUTPUT\n"
	"\tmovss	_%s(%%rip), %%xmm0\n"
	"\tpxor	%%xmm1, %%xmm1\n"
	"\tcvtss2sd	%%xmm0, %%xmm1\n"
	"\tmovq	%%xmm1, %%rax\n"
	"\tmovq	%%rax, %%xmm0\n"
	"\tleaq	printfloatstr(%%rip), %%rax\n"
	"\tmovq	%%rax, %%rdi\n"
	"\tmovl	$1, %%eax\n"
	"\tcall	printf@PLT\n";
char *PRINT_STRING = "##TAC_OUTPUT\n"
	"\tleaq	_%s(%%rip), %%rax\n"
	"\tmovq	%%rax, %%rsi\n"
	"\tleaq	printstringstr(%%rip), %%rax\n"
	"\tmovq	%%rax, %%rdi\n"
    "\tmovl	$0, %%eax\n"
	"\tcall	printf@PLT\n";

char *PRINT_CHAR = "##TAC_OUTPUT\n"
	"\tmovl	_%s(%%rip), %%esi\n"
	"\tleaq	printcharstr(%%rip), %%rax\n"
	"\tmovq	%%rax, %%rdi\n"
	"\tmovl	$0, %%eax\n"
	"\tcall	printf@PLT\n";
char* LONG_VARIABLE = "_%s:\t.long\t%s\n";
char* FLOAT_VARIABLE = "_%s:\t.float\t%s\n";
char* STRING = "_%s:\t.string\t%s\n";
char* DATA_SECTION = "#DATA SECTION\n"
                    ".data\n\n";
char* REAL_DIV = "##TAC_DIV - REAL\n"
    "\tmovss	_%s(%%rip), %%xmm0\n"
	"\tmovss	_%s(%%rip), %%xmm1\n"
	"\tdivss   %%xmm1, %%xmm0\n"
	"\tmovss	%%xmm0, _%s(%%rip)\n";
char* INT_DIV = "##TAC_DIV\n"
    "\tmovl	_%s(%%rip), %%eax\n"
	"\tmovl	_%s(%%rip), %%ecx\n"
	"\tcltd\n"
	"\tidivl	%%ecx\n"
    "\tmovl	%%eax, _%s(%%rip)\n";
char *COMPARE = "##COMPARE\n"
    "\tmovl	_%s(%%rip), %%edx\n"
	"\tmovl	_%s(%%rip), %%eax\n"
	"\tcmpl	%%eax, %%edx\n";
char* READ_VEC = "##READ_VEC\n" 	
    "\tmovl	_%s(%%rip), %%eax\n"
	"\tcltq\n"
	"\tleaq	0(,%%rax,4), %%rdx\n"    //offset
	"\tleaq	_%s(%%rip), %%rax\n"	 //base
	"\tmovl	(%%rdx,%%rax), %%eax\n" //get value of base+offset
	"\tmovl	%%eax, _%s(%%rip)\n";	 //store in temp
char* WRITE_VEC = "##WRITE_VEC\n"
    "\tmovl	_%s(%%rip), %%edx\n" //offset
	"\tmovl	_%s(%%rip), %%eax\n" //value to write
	"\tmovslq	%%edx, %%rdx\n"
	"\tleaq	0(,%%rdx,4), %%rcx\n" //load offset
	"\tleaq	_%s(%%rip), %%rdx\n" //load base
	"\tmovl	%%eax, (%%rcx,%%rdx)\n";
char* tac_to_asm[] = {
    [TAC_ADD] = "##TAC_ADD\n" 
    "\tmovl	_%s(%%rip), %%edx\n"
	"\tmovl	_%s(%%rip), %%eax\n"
	"\taddl	%%edx, %%eax\n"
	"\tmovl	%%eax, _%s(%%rip)\n",
    [TAC_SUB] = "##TAC_SUB\n" 
    "\tmovl	_%s(%%rip), %%eax\n"
	"\tmovl	_%s(%%rip), %%edx\n"
	"\tsubl	%%edx, %%eax\n"
	"\tmovl	%%eax, _%s(%%rip)\n",
    [TAC_MUL] = "##TAC_MUL\n"
    "\tmovl	_%s(%%rip), %%edx\n"
	"\tmovl	_%s(%%rip), %%eax\n"
	"\timull	%%edx, %%eax\n"
	"\tmovl	%%eax, _%s(%%rip)\n",
    [TAC_COPY] = "##TAC_COPY\n"
    "\tmovl	_%s(%%rip), %%eax\n"
	"\tmovl	%%eax, _%s(%%rip)\n",
    [TAC_RETURN] = "##TAC_RETURN\n"
    "\tmovl	_%s(%%rip), %%eax\n"
	"\tpopq	%%rbp\n"
	"\tret\n",
    [TAC_FUNC_CALL] = "##TAC_FUNC_CALL\n"
    "\tcall\t%s\n"
	"\tmovl	%%eax, _%s(%%rip) ##FUNC RETURN\n",
    [TAC_INPUT] = "##TAC_INPUT\n"	
    "\tleaq	_%s(%%rip), %%rax\n"
	"\tmovq	%%rax, %%rsi\n"
	"\tleaq	%s(%%rip), %%rax\n"
	"\tmovq	%%rax, %%rdi\n"
	"\tmovl	$0, %%eax\n"
	"\tcall	__isoc99_scanf@PLT\n"
};

    
void printVariables(FILE *fout, HashNode* hash_table[], int hash_size);
void printOneVariable(FILE *fout, HashNode* node);
void printConstants(FILE *fout, HashNode* hash_table[], int hash_size);
void printStringConstant(FILE *fout,char* string_text);
void printOutput(FILE *fout);
void printCharConstant(FILE *fout,char* char_text);
void printDataSection(FILE *fout, HashNode* hash_table[], int hash_size){
    fprintf(fout,"%s",DATA_SECTION);
    printVariables(fout, hash_table, hash_size);
    printConstants(fout, hash_table, hash_size);
}
void printOneVariable(FILE *fout, HashNode* node){
    if(node->type == SYMBOL_VARIABLE){
        if(node->datatype == DATATYPE_REAL)
            fprintf(fout,FLOAT_VARIABLE,node->text,node->declared_values ? (node->declared_values->string ? node->declared_values->string :"0.0") : "0.0");
        else
            fprintf(fout,LONG_VARIABLE,node->text,node->declared_values ? (node->declared_values->string ? node->declared_values->string :"0") : "0");        
    }
       
    if (node->type == SYMBOL_VECTOR){
        fprintf(fout,"_%s:\n",node->text);
        if(node->declared_values != NULL){
            for(StringList *list_node = node->declared_values; list_node; list_node = list_node->next){
                if(node->datatype == DATATYPE_REAL)
                    fprintf(fout,".float %s\n",list_node->string);
                else
                    fprintf(fout,".long %s\n",list_node->string);
                   
            }

        }
    }
}
void printVariables(FILE *fout, HashNode* hash_table[], int hash_size){
    HashNode *node;
    for(int i=0; i<hash_size;++i){
        for (node=hash_table[i]; node; node = node->next){
            printOneVariable(fout, node);

        }
    }
}
void printConstants(FILE *fout, HashNode* hash_table[], int hash_size){
    HashNode *node;
    
    for(int i=0; i<hash_size;++i){
        for (node=hash_table[i]; node; node = node->next){
            switch (node->type)
            {
            case SYMBOL_LIT_INT:
                fprintf(fout,LONG_VARIABLE,node->text,node->text); 
                break;
            case SYMBOL_LIT_REAL:
                fprintf(fout,FLOAT_VARIABLE,node->text,node->text); 
                break;
            case SYMBOL_LIT_CHAR:
                printCharConstant(fout,node->text);
                break;
            case SYMBOL_LIT_STRING:
                printStringConstant(fout,node->text);
                break;
            default:
                break;
            }
        }
    }
}
void printCharConstant(FILE *fout,char* char_text){
    if (char_text != NULL){
        // Extract the character between single quotes and convert it to an integer.
        int int_value = (int)char_text[1];
        
        // Convert the integer to a string and allocate memory for it.
        int str_size = snprintf(NULL, 0, "%d", int_value) + 1; // Calculate the size of the string.
        char* string_of_char_ascii_decimal = (char*)malloc(str_size);

        // Convert the integer to a string and store it in declared_value.
        snprintf(string_of_char_ascii_decimal, str_size, "%d", int_value);
        fprintf(fout,LONG_VARIABLE,char_text,string_of_char_ascii_decimal); 
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
        char* str_name = (char*)malloc(str_size*sizeof(char));
        strcpy(str_name,prefix);
        if (str_name != NULL) {
            char* str_temp = (char*)malloc(str_size*sizeof(char));
            // Convert the uintptr_t to a string and store it in str_name.
            snprintf(str_temp, str_size, "%lx", ptr_value);
            strcat(str_name,str_temp);
            free(str_temp);
            // Free the allocated memory for str_name.
            return str_name;
        }
    }
}
void printStringConstant(FILE *fout,char* string_text){
    
    char* str_name = generateStringVariableName(string_text);
    fprintf(fout, STRING, str_name, string_text);
    // Free the allocated memory for str_name.
    free(str_name);
}
 
void printOutputFunction(FILE* fout, TAC* tac){
    switch (tac->result->datatype)
    {
    case DATATYPE_INT:
        fprintf(fout,PRINT_INT, tac->result->text);
        break;
    case DATATYPE_REAL:
        fprintf(fout,PRINT_REAL, tac->result->text);
        break;
    case DATATYPE_CHAR:
        fprintf(fout,PRINT_CHAR, tac->result->text);
        break;
    case DATATYPE_STRING:
        char *str_name =  generateStringVariableName(tac->result->text);
        fprintf(fout,PRINT_STRING, str_name);
        free(str_name);
        break;
    default:
        fprintf(fout,PRINT_INT, tac->result->text);
        break;
    }
}
void printDataSectionVectors(FILE *fout,TAC* first){
    TAC *tac;
    
    for (tac = first; tac; tac = tac->next){
        if(tac->type == TAC_VEC_DEC){
            fprintf(fout,"_%s:\n",tac->result->text);
            for (TAC *tac_prev = tac; tac_prev, tac_prev->type == TAC_VEC_INIT; tac_prev = tac_prev->prev)
                fprintf(fout,".long %s\n",tac_prev->result->text);
        }
    }
}
int isVectorWrite(TAC* tac){
    TAC* curr;
    for(curr = tac; curr->type != TAC_COPY; curr = curr->next);
    if (curr->result ==  tac->result)
        return 1;
    return 0;
}
TAC* processTacsUntilCopy(FILE* fout, TAC* first){
    TAC* tac;
    int count = 0;
    TAC* curr;
    for(tac = first->next; tac->type != TAC_COPY; tac = tac->next){
        switch (tac->type){
            case TAC_ADD:
            case TAC_SUB:
            case TAC_MUL:
                fprintf(fout,tac_to_asm[tac->type],tac->operator1->text,tac->operator2->text,tac->result->text);//potentially segmentation fault ***;
                //adjust datatype of temp result, maybe it should be on semantic.c 
                if(tac->operator1->datatype == DATATYPE_REAL || tac->operator2->datatype == DATATYPE_REAL)
                    tac->result->datatype = DATATYPE_REAL; 
                if(tac->operator1->datatype == DATATYPE_INT || tac->operator2->datatype == DATATYPE_INT)
                    tac->result->datatype = DATATYPE_INT; 
                break;
            case TAC_DIV:
                //adjust datatype of temp result, maybe it should be on semantic.c 
                if(tac->operator1->datatype == DATATYPE_REAL || tac->operator2->datatype == DATATYPE_REAL){
                    fprintf(fout,REAL_DIV,tac->operator1->text,tac->operator2->text,tac->result->text);//potentially segmentation fault ***;
                    tac->result->datatype = DATATYPE_REAL; 
                }
                if(tac->operator1->datatype == DATATYPE_INT || tac->operator2->datatype == DATATYPE_INT){
                    fprintf(fout,INT_DIV,tac->operator1->text,tac->operator2->text,tac->result->text);//potentially segmentation fault ***;
                    tac->result->datatype = DATATYPE_INT; 

                }
                break;

            case TAC_EQUAL:
            case TAC_NOT_EQUAL:
            case TAC_GREATER:
            case TAC_GREATER_EQUAL:
            case TAC_LESS:
            case TAC_LESS_EQUAL:
                fprintf(fout,COMPARE,tac->operator1->text,tac->operator2->text);
                break;
            case TAC_FUNC_ARG:
                HashNode* last_func = NULL;
                curr = tac;
                for(curr = tac; curr->type != TAC_FUNC_CALL; curr = curr->next);
                HashNode* func = search(curr->operator1->text, g_hash_table, HASH_SIZE);
                if(last_func != func)
                    count = 0;
                StringList* params = func->func_params;
                for (int i = 0; i < count + 1; i++){
                    if(params!=NULL){
                        fprintf(fout,tac_to_asm[TAC_COPY],tac->result->text,params->string);
                        params = params->next;
                    }   
                }
                break;
            case TAC_FUNC_CALL:
                fprintf(fout,tac_to_asm[tac->type],tac->operator1->text,tac->result->text);
                break;
            case TAC_INPUT:
                //go through until finding the attrib. Ex: a = input(int) + 2; It will go through tacs until find "a=" tac
                curr = tac;
                for(curr = tac; curr->type != TAC_COPY; curr = curr->next);
                //switch to apply the correct print function =-> %d,%f,%c,%s
                switch (curr->result->datatype){
                    case DATATYPE_INT:
                        fprintf(fout,tac_to_asm[tac->type],tac->result->text,"printintstr");
                        break;
                    case DATATYPE_REAL:
                        fprintf(fout,tac_to_asm[tac->type],tac->result->text,"printfloatstr");
                        break;
                    case DATATYPE_CHAR:
                        fprintf(fout,tac_to_asm[tac->type],tac->result->text,"printcharstr");
                        break;
                    case DATATYPE_STRING:
                        fprintf(fout,tac_to_asm[tac->type],tac->result->text,"printstringstr");
                        break;
                    default:
                        fprintf(fout,tac_to_asm[tac->type],tac->result->text,"printintstr");
                        break;
                }
            case TAC_VEC_IND:
                fprintf(fout,READ_VEC,tac->operator2->text,tac->operator1->text,tac->result->text);
                tac->result->datatype = tac->operator1->datatype;
                break;
            default:
                break;
        }
    }
    fprintf(fout,WRITE_VEC,first->operator2->text,tac->operator1->text,first->operator1->text);
    return tac;
}

void generateAsm(TAC *first)
{
    FILE *fout;
    fout = fopen("out.s", "w");

    // INIT
    fprintf(fout, "%s", INIT);

    TAC *tac;
    int count = 0;
    TAC* curr;
    for (tac = first; tac; tac = tac->next){
        switch (tac->type){
            case TAC_BEGIN_FUN:
                fprintf(fout, BEGIN_FUN, tac->result->text, tac->result->text);//potentially segmentation fault ***; tac->result->text can be NULL
                break;
            case TAC_END_FUN:
                fprintf(fout, "%s", END_FUN);
                break;
            case TAC_OUTPUT_ARG:
                printOutputFunction(fout,tac);
                break;
            case TAC_ADD:
            case TAC_SUB:
            case TAC_MUL:
                fprintf(fout,tac_to_asm[tac->type],tac->operator1->text,tac->operator2->text,tac->result->text);//potentially segmentation fault ***;
                //adjust datatype of temp result, maybe it should be on semantic.c 
                if(tac->operator1->datatype == DATATYPE_REAL || tac->operator2->datatype == DATATYPE_REAL)
                    tac->result->datatype = DATATYPE_REAL; 
                if(tac->operator1->datatype == DATATYPE_INT || tac->operator2->datatype == DATATYPE_INT)
                    tac->result->datatype = DATATYPE_INT; 
                break;
            case TAC_DIV:
                //adjust datatype of temp result, maybe it should be on semantic.c 
                if(tac->operator1->datatype == DATATYPE_REAL || tac->operator2->datatype == DATATYPE_REAL){
                    fprintf(fout,REAL_DIV,tac->operator1->text,tac->operator2->text,tac->result->text);//potentially segmentation fault ***;
                    tac->result->datatype = DATATYPE_REAL; 
                }
                if(tac->operator1->datatype == DATATYPE_INT || tac->operator2->datatype == DATATYPE_INT){
                    fprintf(fout,INT_DIV,tac->operator1->text,tac->operator2->text,tac->result->text);//potentially segmentation fault ***;
                    tac->result->datatype = DATATYPE_INT; 

                }
                break;
            case TAC_COPY:
                fprintf(fout,tac_to_asm[tac->type],tac->operator1->text,tac->result->text);
                break;
            case TAC_LABEL:
                fprintf(fout,"_%s:\n",tac->result->text);
                break;
            case TAC_JUMP:
                fprintf(fout,"\tjmp\t_%s\n",tac->result->text);
                break;
            case TAC_EQUAL:
            case TAC_NOT_EQUAL:
            case TAC_GREATER:
            case TAC_GREATER_EQUAL:
            case TAC_LESS:
            case TAC_LESS_EQUAL:
                fprintf(fout,COMPARE,tac->operator1->text,tac->operator2->text);
                break;
            case TAC_JFALSE:
                
                switch (tac->prev->type){
                    case TAC_EQUAL:
                        fprintf(fout,"\tjne\t_%s\n",tac->result->text);
                        break;
                    case TAC_GREATER_EQUAL:
                        fprintf(fout,"\tjl\t_%s\n",tac->result->text);
                        break;
                    case TAC_GREATER:
                        fprintf(fout,"\tjle\t_%s\n",tac->result->text);
                        break;
                    case TAC_LESS_EQUAL:
                        fprintf(fout,"\tjg\t_%s\n",tac->result->text);
                        break;
                    case TAC_LESS:
                        fprintf(fout,"\tjge\t_%s\n",tac->result->text);
                        break;
                    case TAC_NOT_EQUAL:
                        fprintf(fout,"\tje\t_%s\n",tac->result->text);
                        break;
                    default: 
                        break;
                }
                break;
            case TAC_FUNC_ARG:
                curr = tac;
                for(curr = tac; curr->type != TAC_FUNC_CALL; curr = curr->next);
                HashNode* func = search(curr->operator1->text, g_hash_table, HASH_SIZE);
   
                StringList* params = func->func_params;
                for (int i = 0; i < count; i++){
                    if(params!=NULL){
                        params = params->next;
                    }   
                }
                if(params!=NULL){
                    fprintf(fout,tac_to_asm[TAC_COPY],tac->result->text,params->string);
                }
                count++;
                break;
            case TAC_FUNC_CALL:
                fprintf(fout,tac_to_asm[tac->type],tac->operator1->text,tac->result->text);
                count = 0;
                break;
            case TAC_RETURN:
                fprintf(fout,tac_to_asm[tac->type],tac->result->text);
                break;
            case TAC_INPUT:
                //go through until finding the attrib. Ex: a = input(int) + 2; It will go through tacs until find "a=" tac
                curr = tac;
                for(curr = tac; curr->type != TAC_COPY; curr = curr->next);
                //switch to apply the correct print function =-> %d,%f,%c,%s
                switch (curr->result->datatype){
                    case DATATYPE_INT:
                        fprintf(fout,tac_to_asm[tac->type],tac->result->text,"printintstr");
                        break;
                    case DATATYPE_REAL:
                        fprintf(fout,tac_to_asm[tac->type],tac->result->text,"printfloatstr");
                        break;
                    case DATATYPE_CHAR:
                        fprintf(fout,tac_to_asm[tac->type],tac->result->text,"printcharstr");
                        break;
                    case DATATYPE_STRING:
                        fprintf(fout,tac_to_asm[tac->type],tac->result->text,"printstringstr");
                        break;
                    default:
                        fprintf(fout,tac_to_asm[tac->type],tac->result->text,"printintstr");
                        break;
                }
                break;
            case TAC_VEC_IND:
                if(isVectorWrite(tac)){
                    tac = processTacsUntilCopy(fout,tac);
                }
                else{
                    fprintf(fout,READ_VEC,tac->operator2->text,tac->operator1->text,tac->result->text);
                    tac->result->datatype = tac->operator1->datatype; //datatype will be the same as the base vector datatype, temp->datatype = v[x]->datatype
                }
                break;
            default:
                break;
        }
    }
    printDataSection(fout,g_hash_table,HASH_SIZE);
    fclose(fout);
}