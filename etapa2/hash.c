//Níkolas Padão Schuster
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Hash* insert(char* text, int type, Hash* hash_table[], int hash_size){
    //first of all, check if text already exists in the table
    Hash* new_node = search(text,hash_table,hash_size);
    if (new_node != NULL)
        return new_node;
    int address = computeAddress(text,hash_size);
    Hash* first_node = hash_table[address];

    new_node = (Hash*)calloc(1,sizeof(Hash));
    char* text_cpy = (char*)calloc(strlen(text)+1,sizeof(char));
    strcpy(text_cpy,text);
    new_node->type = type;
    new_node->text = text_cpy;
    new_node->next = first_node;
    hash_table[address] = new_node;
    return new_node;
}

Hash* search(char* text, Hash* hash_table[], int hash_size){
    int address = computeAddress(text,hash_size);
    Hash* node = hash_table[address];
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
void init(Hash* hash_table[],int hash_size){
    for (int i = 0; i < hash_size; i++){
        hash_table[i] = NULL;   
    }
}
