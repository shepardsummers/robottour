struct Node {
    int data;
    struct Node *parent;
};
typedef struct Node Node;

typedef struct {
    int front, rear, size;
    unsigned capacity;
    Node **array;
} Queue;

Queue* newqueue(unsigned capacity);
int isfull(Queue* queue);
int isempty(Queue* queue);
void enqueue(Queue* queue, Node *node);
Node *dequeue(Queue* queue);
Node *front(Queue* queue);
Node *rear(Queue* queue);
void freequeue(Queue* queue);
