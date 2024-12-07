#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "main.h"

int main()
{
    // Use the current second its compiled for uses of rand() (below)
    srand(time(NULL));

    // Initialize all parameters
    Node startNode = {2, 2, 0, 2};
    Node endNode = {5, 9, 0, 0};
    List *pHeadOpen = NULL;  // Empty linked list
    List *pHeadClose = NULL; // Empty linked list
    int isGoalReached = 0;

    // Update the pHeadOpen to the startNode
    pHeadOpen = addToHead(pHeadOpen, &startNode);
    printf("Initial openList: \n");
    display(pHeadOpen);
    printf("Initial closeList: \n");
    display(pHeadClose);

    // Initialize and display the board with random obstacle.
    Node board[WIDTH][HEIGHT];
    initializeBoard(board);
    printBoard(board, &startNode, &endNode, pHeadClose);

    // A* algorithm loop
    List *currentOpenItem = pHeadOpen;
    while (currentOpenItem != NULL)
    {
        Node *lowestFNode = findLowestFNode(currentOpenItem);

        // Check if goal is reached
        if (lowestFNode->i == endNode.i && lowestFNode->j == endNode.j)
        {
            printf("GOAL REACHED!\n");
            isGoalReached = 1;
            break;
        }

        // Remove lowest F node from open list
        pHeadOpen = deleteItem(pHeadOpen, lowestFNode);

        // Generate neighbors
        List headNeighbourList = generateNeighbours(lowestFNode, board);

        List *current = &headNeighbourList;
        // Process each neighbor
        while (current != NULL)
        {
            // Calculate g, h, and f scores
            current->node.g = lowestFNode->g + 1;
            int dx = abs(current->node.i - endNode.i);
            int dy = abs(current->node.j - endNode.j);
            current->node.h = (dx + dy) + (sqrt(2) - 2) * min(&dx, &dy);
            current->node.f = current->node.g + current->node.h;

            // Check if neighbor is already in open or closed list with better path
            List *existingInOpen = findItem(pHeadOpen, &current->node);
            List *existingInClosed = findItem(pHeadClose, &current->node);

            if ((existingInOpen && existingInOpen->node.f <= current->node.f) ||
                (existingInClosed && existingInClosed->node.f <= current->node.f))
            {
                current = current->next;
                continue;
            }

            // Add neighbor to open list
            pHeadOpen = addToHead(pHeadOpen, &current->node);

            current = current->next;
        }

        // Add current node to closed list
        pHeadClose = addToHead(pHeadClose, lowestFNode);

        // Reset for next iteration
        currentOpenItem = pHeadOpen;
    }

    // Final board drawing and processing
    pHeadClose = addToHead(pHeadClose, &endNode);

    // If we did not find any path.
    if (!isGoalReached)
    {
        printf("No path found\n");
    }
    // Print the path list (close list).
    display(pHeadClose);

    // Print the resulting board
    printBoard(board, &startNode, &endNode, pHeadClose);

    return 0;
}

List generateNeighbours(Node *lowestFNode, Node board[WIDTH][HEIGHT])
{
    List *headNeighbourList = NULL;

    for (int i = 0; i < WIDTH; i++)
    {
        for (int j = 0; j < HEIGHT; j++)
        {
            // Check if the node is a valid neighbor (adjacent or diagonal)
            int dx = abs(board[i][j].i - lowestFNode->i);
            int dy = abs(board[i][j].j - lowestFNode->j);

            if ((dx <= 1 && dy <= 1) && (dx + dy > 0))
            {
                // Check if the node is not an obstacle before adding
                if (!board[i][j].isObstacle)
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
    Node *lowestNode = &headOpenList->node; // Initialize the lowestFNode as the node of the first element of the linked List
    List *currentItem = headOpenList;       // Initialize the currentItem as the first element of the linked List
    while (currentItem != NULL)
    {
        if (currentItem->node.f < lowestNode->f)
        {
            lowestNode = &currentItem->node;
        }
        currentItem = currentItem->next;
    }
    return lowestNode;
}

int min(const int *a, const int *b)
{
    return (*a < *b) ? *a : *b;
}

int display(const List *list)
{
    const List *curent = list;
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
            board[i][j].isObstacle = rand() % 2; // Generate either an obstacle or a free node.
            board[i][j].f = 0;
        }
    }

    return 0;
}

int printBoard(Node board[WIDTH][HEIGHT], const Node *startNode, const Node *endNode, List *closedList)
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
                printf("START ");
            }
            else if (i == endNode->i && j == endNode->j)
            {
                printf("END  ");
            }
            else if (findItem(closedList, &board[i][j]))
            {
                printf("-     ");
            }
            else
            {
                printf("%d     ", board[i][j].isObstacle);
            }
        }
        printf("\n");
    }
    printf("\n");

    return 0;
}