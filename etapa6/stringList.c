#include "stringList.h"
#include <stdlib.h>

// Function to create a new node
StringList *createStringListNode(char *string)
{
    StringList *newNode = (StringList *)malloc(sizeof(StringList));
    newNode->string = string;
    newNode->next = NULL;
    return newNode;
}

// Function to insert a new node at the end of the list
void appendStringList(StringList **head, char *string)
{
    StringList *newNode = createStringListNode(string);
    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        StringList *current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Function to print the linked list
void printList(StringList *head)
{
    StringList *current = head;
    while (current != NULL)
    {
        current = current->next;
    }
}

// Function to free the memory allocated for the linked list
void freeList(StringList *head)
{
    StringList *current = head;
    while (current != NULL)
    {
        StringList *temp = current;
        current = current->next;
        free(temp);
    }
}