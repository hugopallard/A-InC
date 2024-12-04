#define WIDTH 10
#define HEIGHT 10
#define SCORE_UNSET -1

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

// Utilities
int drawBoard(const Node board[WIDTH][HEIGHT], const Node *startNode, const Node *endNode);
int initializeBoard(Node board[WIDTH][HEIGHT]);
Node *findLowestFNode(List *openList);
List generateNeighbours(Node *lowestFNode, Node board[WIDTH][HEIGHT]);
int min(const int *a, const int *b);

// LinkedNodeList
List createLinkedList();
List *addToHead(List *head, Node *itemToAdd);
List *deleteItem(List *list, Node *itemToDelete);
Node *findItem(List *head, Node *itemToFind);
int display(List *list);
int getSize(List *list);
