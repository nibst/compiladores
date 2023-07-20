//Níkolas Padão Schuster


//hash linked list
#ifndef HASH_H
#define HASH_H
#define HASH_SIZE 997
#define SYMBOL_LIT_INT 1 
#define SYMBOL_LIT_CHAR 2
#define SYMBOL_LIT_REAL 3
#define SYMBOL_LIT_STRING 4
#define SYMBOL_IDENTIFIER 7
typedef struct hash_node{
    int type;
    char* text;
    struct hash_node* next; 
}HashNode;

HashNode* insert(char* text, int type, HashNode* hash_table[], int hash_size);
HashNode* search(char* text, HashNode* hash_table[], int hash_size);
int computeAddress(char* text,int hash_size);
void init(HashNode* hash_table[], int hash_size);
void printHash(HashNode* hash_table[],int hash_size);
#endif /* HASH_H */