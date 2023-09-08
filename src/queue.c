#include <limits.h>
#include <stdlib.h>
#include "lib.h"

Queue* newqueue(unsigned capacity) {
    Queue* queue = malloc(sizeof(Queue));
    queue->capacity = capacity;
    queue->size = 0;
    queue->front = 0;
    queue->rear = capacity - 1;
    queue->array = malloc(sizeof(int) * capacity);
    return queue;
}

int isfull(Queue* queue) {
    return queue->size == queue->capacity;
}

int isempty(Queue* queue) {
    return queue->size == 0;
}

void enqueue(Queue* queue, Node *node) {
    if (isfull(queue)) {
        return;
    }
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = node;
    queue->size += 1;
}

Node *dequeue(Queue* queue) {
    if (isempty(queue)) {
        return NULL;
    }
    Node *item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size -= 1;
    return item;
}

Node *front(Queue* queue) {
    if (isempty(queue)) {
        return NULL;
    }
    return queue->array[queue->front];
}

Node *rear(Queue* queue) {
    if (isempty(queue)) {
        return NULL;
    }
    return queue->array[queue->rear];
}

void freequeue(Queue* queue) {
    while (!isempty(queue)) {
        free(dequeue(queue));
    }
    free(queue->array);
    free(queue);
}
