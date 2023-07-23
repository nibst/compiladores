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
