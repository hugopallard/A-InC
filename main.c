#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "main.h"

int main()
{

    // Initialize all parameters
    Node startNode = {2, 2, 0, 2};
    Node endNode = {5, 9, 0, 0};
    float g = 0;
    float h = 0;
    float f = 0;            // score: f = sum of g + h;
    List *pHeadOpen = NULL; // Empty list
    List *pHeadClose = NULL;

    // Update the pHeadOpen to the startNode
    pHeadOpen = addToHead(pHeadOpen, &startNode);
    printf("Initial headOpenList: \n");
    display(pHeadOpen);
    printf("Initial headCloseList: \n");
    display(pHeadClose);

    // Initialize and display the board with random obstacle.
    Node board[WIDTH][HEIGHT];
    initializeBoard(board);
    drawBoard(board, &startNode, &endNode);

    // A* algorithm loop
    // // Traverse to the last item of openList;
    List *currentOpenItem = pHeadOpen;
    while (currentOpenItem != NULL)
    {
        Node *lowestFNode = findLowestFNode(currentOpenItem);
        printf("The lowest f node: (%d,%d) with f of: %d\n\n", lowestFNode->i, lowestFNode->j, lowestFNode->f);

        // Check if we've reached the goal before processing neighbors
        if (lowestFNode->i == endNode.i && lowestFNode->j == endNode.j)
        {
            printf("GOAL REACHED!\n");
            break;
        }

        printf("Open list after delete: \n");
        pHeadOpen = deleteItem(pHeadOpen, lowestFNode);
        display(pHeadOpen);

        List headNeighbourList = generateNeighbours(lowestFNode, board);
        // Debug print
        printf("Neighbours found: \n");
        display(&headNeighbourList);

        // Traverse the neighbourList;
        List *current = &headNeighbourList;
        while (current != NULL)
        {
            // if successor is the goal, stop search
            if (lowestFNode->i == endNode.i && lowestFNode->j == endNode.j)
            {
                printf("ARRIVED !");
                break;
            }
            // Else compute the scores
            else
            {
                current->node.g = lowestFNode->g + 1;

                int dx = abs(current->node.i - endNode.i);
                int dy = abs(current->node.j - endNode.j);
                current->node.h = (dx + dy) + (sqrt(2) - 2) * min(&dx, &dy);

                current->node.f = current->node.g + current->node.h;
            }

            Node *potentialNode = NULL;
            printf("Current node (%d,%d) and f score: %d\n", current->node.i, current->node.j, current->node.f);
            potentialNode = findItem(pHeadOpen, &current->node);

            if (potentialNode != NULL && potentialNode->f < current->node.f)
            {
                printf("Found in open with better F\n");
                current = current->next;
                continue;
            }
            potentialNode = findItem(pHeadClose, &current->node);
            if (potentialNode != NULL && potentialNode->f < current->node.f)
            {
                printf("Found in close with better F\n");
                current = current->next;
                continue;
            }
            printf("Not found, added to openList\n");
            pHeadOpen = addToHead(pHeadOpen, &current->node);
            current = current->next;
        }
        pHeadClose = addToHead(pHeadClose, lowestFNode);
        currentOpenItem = pHeadOpen;
        free(lowestFNode);
        printf("\nCurrent Open list: \n");
        display(pHeadOpen);

        // Break the loop if the open list becomes empty
        if (currentOpenItem == NULL)
            break;
    }
    display(pHeadClose);
    return 0;
}

List generateNeighbours(Node *lowestFNode, Node board[WIDTH][HEIGHT])
{
    List *headNeighbourList = NULL;

    for (int i = 0; i < WIDTH; i++)
    {
        for (int j = 0; j < HEIGHT; j++)
        {
            // Skip the node itself
            if (board[i][j].i == lowestFNode->i && board[i][j].j == lowestFNode->j)
                continue;

            // Check if the node is a valid neighbor (adjacent or diagonal)
            int dx = abs(board[i][j].i - lowestFNode->i);
            int dy = abs(board[i][j].j - lowestFNode->j);

            if ((dx <= 1 && dy <= 1) && (dx + dy > 0))
            {
                // Check if the node is not an obstacle before adding
                if (board[i][j].isObstacle == 0)
                {
                    headNeighbourList = addToHead(headNeighbourList, &board[i][j]);
                }
            }
        }
    }

    return *headNeighbourList;
};

Node *findLowestFNode(List *headOpenList)
{
    List *currentItem = headOpenList;       // Initialize the currentItem as the first element of the linkedList
    Node *lowestNode = &headOpenList->node; // Initialize the lowestFNode as the node of the first element of the linkedList
    while (currentItem != NULL)
    {
        if (currentItem->node.f < lowestNode->f)
        {
            lowestNode = &currentItem->node;
        }
        currentItem = currentItem->next;
    }
    free(currentItem);
    return lowestNode;
}

List create(Node *startNode)
{
    List list;
    list.next = NULL; // Important: initialize next to NULL

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

int getSize(List *list)
{
    List *current = list;
    int size = 1;
    while (current->next != NULL)
    {
        size++;
        current = current->next;
    }
    free(current);
    return size;
};

List *addToHead(List *head, Node *itemToAdd)
{

    // If list is empty
    if (head == NULL)
    {
        // Create the first node
        head = malloc(sizeof(struct Node));
        if (head == NULL)
        {
            return NULL; // malloc failed
        }
        head->node = *itemToAdd;
        head->next = NULL;
        // return the pointer to the new first item.
        return head;
    }

    // Create a new node for the end of the list
    struct List *newItem = malloc(sizeof(struct List));
    if (newItem == NULL)
    {
        return head; // malloc failed, return original head
    }

    // Traverse to the last node
    struct List *current = head;
    while (current->next != NULL)
    {
        current = current->next;
    }

    // Add the new node
    newItem->node = *itemToAdd;
    newItem->next = NULL;
    current->next = newItem;

    // Can be ignored, as we add item from the head.
    return head;
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
            // Again, do not free itemToDelete
            return head;
        }
        prev = current;
        current = current->next;
    }

    // Node not found
    return head;
}

Node *findItem(List *head, Node *itemToFind)
{
    if (head == NULL || itemToFind)
    {
        return NULL;
    }
    List *current = head;
    while (current != NULL)
    {
        if (current->node.i == itemToFind->i && current->node.j == itemToFind->j)
        {
            return &current->node;
        }
        current = current->next;
    }

    return NULL;
}

int min(const int *a, const int *b)
{
    return (*a < *b) ? *a : *b;
}

int display(List *list)
{
    List *curent = list;
    while (curent != NULL)
    {
        printf("(%d,%d | score: %d, obs: %d) -> \n", curent->node.i, curent->node.j, curent->node.f, curent->node.isObstacle);
        curent = curent->next;
    }
    printf("NULL\n\n");
    return 0;
};

int initializeBoard(Node board[WIDTH][HEIGHT])
{
    int i = 0;
    int j = 0;
    for (i = 0; i < WIDTH; i++)
    {
        for (j = 0; j < HEIGHT; j++)
        {
            board[i][j].i = i;
            board[i][j].j = j;
            board[i][j].isObstacle = 0; // Generate either an obstacle or a free node rand() % 2.
            board[i][j].f = 0;
        }
    }

    return 0;
}

int drawBoard(const Node board[WIDTH][HEIGHT], const Node *startNode, const Node *endNode)
{
    int i = 0;
    int j = 0;

    if (board == NULL)
    {
        printf("Please provide a non-null board");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < WIDTH; i++)
    {
        for (j = 0; j < HEIGHT; j++)
        {
            if (i == startNode->i && j == startNode->j)
            {
                printf("START  ");
            }
            else if (i == endNode->i && j == endNode->j)
            {
                printf("END   ");
            }
            else
            {
                printf("%d,%d   ", board[i][j].isObstacle, board[i][j].f);
            }
        }
        printf("\n");
    }

    return 0;
}