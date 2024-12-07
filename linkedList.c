#include <stdio.h>
#include <stdlib.h>

#include "linkedList.h"

List *addToHead(List *head, Node *nodeToAdd)
{
    if (nodeToAdd == NULL)
        return head;

    List *newNode = malloc(sizeof(List));
    if (newNode == NULL)
    {
        // Handle memory allocation failure
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    newNode->node = *nodeToAdd; // Copy the node
    newNode->next = head;

    return newNode;
}

List *deleteItem(List *head, Node *itemToDelete)
{
    if (head == NULL)
    {
        return NULL;
    }

    List *current = head;
    List *prev = NULL;

    // Special case: deleting the first node
    if (current->node.i == itemToDelete->i &&
        current->node.j == itemToDelete->j)
    {
        head = current->next;
        // DO NOT free itemToDelete here, as it might be a node from the original list
        return head;
    }

    // Traverse the list to find the node
    while (current != NULL)
    {
        if (current->node.i == itemToDelete->i &&
            current->node.j == itemToDelete->j)
        {
            if (prev != NULL)
            {
                prev->next = current->next;
            }
            return head;
        }
        prev = current;
        current = current->next;
    }
    // Node not found
    return head;
}

List *findItem(List *head, Node *itemToFind)
{
    if (head == NULL || itemToFind == NULL)
    {
        return NULL;
    }

    List *current = head;
    while (current != NULL)
    {
        if (current->node.i == itemToFind->i && current->node.j == itemToFind->j)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

List create(Node *startNode)
{
    List list;
    list.next = NULL;

    if (startNode != NULL)
    {
        // Optional: add the start node if provided
        List *newNode = malloc(sizeof(List));
        newNode->node = *startNode;
        newNode->next = NULL;
        list.next = newNode;
    }

    return list;
}

int getSize(const List *head)
{
    if (head == NULL)
    {
        exit(EXIT_FAILURE);
    }

    int size = 1;
    const List *current = head;
    while (current->next != NULL)
    {
        size++;
        current = current->next;
    }
    return size;
};
