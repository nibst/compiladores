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
}Hash;

Hash* insert(char* text, int type, Hash* hash_table[], int hash_size);
Hash* search(char* text, Hash* hash_table[], int hash_size);
int computeAddress(char* text,int hash_size);
void init(Hash* hash_table[], int hash_size);
#endif /* HASH_H */