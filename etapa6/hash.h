//Níkolas Padão Schuster


#ifndef HASH_H
#define HASH_H
#define HASH_SIZE 997
#define SYMBOL_LIT_INT 1 
#define SYMBOL_LIT_CHAR 2
#define SYMBOL_LIT_REAL 3
#define SYMBOL_LIT_STRING 4
#define SYMBOL_LABEL 5
#define SYMBOL_IDENTIFIER 7 //just an id, not yet differentiated into types of ids (vector,func,var,...)
#define SYMBOL_VARIABLE 8
#define SYMBOL_FUNCTION 9
#define SYMBOL_VECTOR 10

//for setting types of variables
#include "types.h"
#include "stringList.h"
#include <stdio.h>
#include <stdlib.h>


//hash linked list
typedef struct hash_node{
    int type;
    DataType datatype;
    char* text;
    StringList* func_params; //store function parameters, if is a function
    StringList* declared_values; //for generating ASM, just need to be printable (thats why is string list). this is a list of strings just for the vector initialization
    struct hash_node* next; 
}HashNode;
extern HashNode* g_hash_table[HASH_SIZE];

HashNode* insert(char* text, int type, HashNode* hash_table[], int hash_size);
HashNode* search(char* text, HashNode* hash_table[], int hash_size);
int computeAddress(char* text,int hash_size);
void init(HashNode* hash_table[], int hash_size);
void printHash(HashNode* hash_table[],int hash_size);
int hashCheckUndeclared(HashNode* hash_table[],int hash_size);

HashNode* makeTemp(); //pass hash_table?
HashNode* makeLabel();
#endif /* HASH_H */