//Níkolas Padão Schuster
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HashNode* insert(char* text, int type, HashNode* hash_table[], int hash_size){
    //first of all, check if text already exists in the table
    HashNode* new_node = search(text,hash_table,hash_size);
    if (new_node != NULL)
        return new_node;
    int address = computeAddress(text,hash_size);
    HashNode* first_node = hash_table[address];

    new_node = (HashNode*)calloc(1,sizeof(HashNode));
    char* text_cpy = (char*)calloc(strlen(text)+1,sizeof(char));
    strcpy(text_cpy,text);
    new_node->type = type;
    new_node->text = text_cpy;
    new_node->next = first_node;
    hash_table[address] = new_node;
    return new_node;
}

HashNode* search(char* text, HashNode* hash_table[], int hash_size){
    int address = computeAddress(text,hash_size);
    HashNode* node = hash_table[address];
    while (node!=NULL){
        if (strcmp(node->text,text) == 0){
            return node;
        }
        node = node->next;
    }
    return NULL;
}
int computeAddress(char* text, int hash_size){
    int address = 1;
    for (int i = 0; i <strlen(text); i++){
        address = (address * text[i]) % hash_size + 1;
    }
    return address-1;
}
void init(HashNode* hash_table[],int hash_size){
    for (int i = 0; i < hash_size; i++){
        hash_table[i] = NULL;   
    }
}
void printHash(HashNode* hash_table[],int hash_size){
    HashNode *node;
    for(int i=0; i<hash_size;++i){
        for (node=hash_table[i]; node; node = node->next){
            printf("Hash Table[%d] has %s, type %d, datatype %d\n",i,node->text,node->type,node->datatype); 
        }
    }
}
int hashCheckUndeclared(HashNode* hash_table[],int hash_size){
    int undeclared = 0;
    HashNode *node;
    for(int i=0; i<hash_size;++i){
        for (node=hash_table[i]; node; node = node->next){
            if (node->type == SYMBOL_IDENTIFIER){
                fprintf(stderr,"Semantic ERROR: identifier %s undeclared\n",node->text); 
                ++undeclared;
            }
        }
    }
    return undeclared;
}
void hashSetDeclaredValue(HashNode* node, char* value){
    if (value  == NULL){
        node->declared_value = NULL;
        return;
    }
    if(value[0] == '\''){
        // Extract the character between single quotes and convert it to an integer.
        int int_value = (int)value[1];
        
        // Convert the integer to a string and allocate memory for it.
        int str_size = snprintf(NULL, 0, "%d", int_value) + 1; // Calculate the size of the string.
        node->declared_value = (char*)malloc(str_size);

        // Convert the integer to a string and store it in declared_value.
        snprintf(node->declared_value, str_size, "%d", int_value);
    }
    else{
        int str_size = strlen(value) + 1; // Calculate the size of the string.
        node->declared_value = (char*)malloc(str_size);
        
        strcpy(node->declared_value, value);
    }
}



HashNode* makeTemp(){
    static int serial = 0;
    char buffer[256] = "";
    sprintf(buffer,"mYWeeidT_emp%d",serial++);
    //TODO not orthogonal to others functions of hash
    //need to pass hash table and size to make temp
    return insert(buffer,SYMBOL_VARIABLE,g_hash_table,HASH_SIZE);
}
HashNode* makeLabel(){
    static int serial = 0;
    char buffer[256] = "";
    sprintf(buffer,"mYWeeidL_abule%d",serial++);
    //TODO not orthogonal to others functions of hash
    //need to pass hash table and size to make temp
    return insert(buffer,SYMBOL_LABEL,g_hash_table,HASH_SIZE);
}
