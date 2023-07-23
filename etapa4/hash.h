//Níkolas Padão Schuster


#ifndef HASH_H
#define HASH_H
#define HASH_SIZE 997
#define SYMBOL_LIT_INT 1 
#define SYMBOL_LIT_CHAR 2
#define SYMBOL_LIT_REAL 3
#define SYMBOL_LIT_STRING 4

#define SYMBOL_IDENTIFIER 7 //just an id, not yet differentiated into types of ids (vector,func,var,...)
#define SYMBOL_VARIABLE 8
#define SYMBOL_FUNCTION 9
#define SYMBOL_VECTOR 10

//for setting types of variables
#define DATATYPE_INT 1
#define DATATYPE_CHAR 2
#define DATATYPE_REAL 3
#define DATATYPE_BOOL 4

//hash linked list
typedef struct hash_node{
    int type;
    int datatype;
    char* text;
    struct hash_node* next; 
}HashNode;
extern HashNode* g_hash_table[HASH_SIZE];

HashNode* insert(char* text, int type, HashNode* hash_table[], int hash_size);
HashNode* search(char* text, HashNode* hash_table[], int hash_size);
int computeAddress(char* text,int hash_size);
void init(HashNode* hash_table[], int hash_size);
void printHash(HashNode* hash_table[],int hash_size);
int hashCheckUndeclared(HashNode* hash_table[],int hash_size);

#endif /* HASH_H */