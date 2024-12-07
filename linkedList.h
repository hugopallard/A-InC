#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct Node
{
    int i;
    int j;
    int g;
    int h;
    int isObstacle;
    int f;

} Node;

typedef struct List
{
    struct Node node;
    struct List *next;
} List;

List createLinkedList();
List *addToHead(List *head, Node *itemToAdd);
List *deleteItem(List *list, Node *itemToDelete);
List *findItem(List *head, Node *itemToFind);
int display(const List *list);
int getSize(const List *list);

#endif