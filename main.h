#ifndef MAIN_H
#define MAIN_H

#define WIDTH 10
#define HEIGHT 10
#define SCORE_UNSET -1

#include "linkedList.h"

int printBoard(Node board[WIDTH][HEIGHT], const Node *startNode, const Node *endNode, List *closedList);
int initializeBoard(Node board[WIDTH][HEIGHT]);
Node *findLowestFNode(List *openList);
List generateNeighbours(Node *lowestFNode, Node board[WIDTH][HEIGHT]);
int min(const int *a, const int *b);

#endif