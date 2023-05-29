//hash linked list
#ifndef HASH_H
#define HASH_H
#define HASH_SIZE 997

typedef struct hash_node{
    int type;
    char* text;
    struct hash_node* next; 
}Hash;

extern Hash* g_hash_table[HASH_SIZE];

Hash* insert(char* text, int type, Hash* hash_table[], int hash_size);
Hash* search(char* text, Hash* hash_table[], int hash_size);
int computeAddress(char* text,int hash_size);
void init(Hash* hash_table[], int hash_size);
#endif /* HASH_H */