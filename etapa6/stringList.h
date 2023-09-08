#ifndef STRING_LIST_H
#define STRING_LIST_H
// Define a structure for a node
typedef struct node
{
    char *string;
    struct node *next;
} StringList;

// Function to create a new node
StringList *createStringListNode(char *string);

// Function to insert a new node at the end of the list
void appendStringList(StringList **head, char *string);


// Function to print the linked list
void printList(StringList *head);

// Function to free the memory allocated for the linked list
void freeList(StringList *head);
#endif