#include "robot.h"

void markexplored(unsigned short *explored, int i);
int isexplored(unsigned short *explored, int i);
void getneighbors(int i, int *neighbors);
int pathlength(Node *path);
Node *getshortestpath(State *state, Node *current, unsigned short explored, int numgates, int numgatesvisited);


void updatepath(State *state) {
    int start = getstartsquare(state->board->start);
    if (start < 0)
        return;
    if (!state->path)
        state->path = malloc(sizeof(Node));
    state->path->data = start;
    state->path->parent = NULL;

    int numgates, i;
    numgates = 0;
    for (i = 0; i < 16; i++) {
        if (gateexists(state->board->gates, i))
            numgates++;
    }

    state->path = getshortestpath(state, state->path, 0, numgates, 0);
}

Node *getshortestpath(State *state, Node *current, unsigned short explored, int numgates, int numgatesvisited) {
    Node *path = malloc(sizeof(Node));
    Node *subpath = malloc(sizeof(Node));
    if (numgatesvisited == numgates) {
        path = bfs(state->board, current, state->board->target);
        if (!path)
            return NULL;
        return path;
    }
    int gateidx, i, mindist;
    Node *minpath = NULL;
    unsigned short gate;
    i = 0;
    mindist = 1000;
    for (gateidx = 0; gateidx < (WIDTH * HEIGHT); gateidx++) {
        gate = 1 << gateidx;
        if ((gate & *state->board->gates) && !(gate & explored)) {
            subpath = bfs(state->board, current, gateidx);
            if (!subpath)
                continue;
            path = getshortestpath(state, subpath, explored | gate, numgates, numgatesvisited + 1);
            if (pathlength(path) < mindist) {
                mindist = pathlength(path);
                if (minpath)
                    free(minpath);
                minpath = path;
            } else {
                free(path);
                free(subpath);
            }
            i++;
        }
    }

    return minpath;
}

int pathlength(Node *path) {
    Node *node = path;
    int i;
    for (i = 0; node; i++)
        node = node->parent;
    return i;
}

/* bfs: do a breadth first search of the grid; returns a node with value end
 * whose parent chain links back to start */
Node *bfs(Board *board, Node *start, int end) {
    Queue *queue = newqueue(1000);
    unsigned short explored;

    markexplored(&explored, start->data);
    enqueue(queue, start);
    while (!isempty(queue)) {
        Node *current = dequeue(queue);
        if (current->data == end) {
            freequeue(queue);
            return current;
        }
        int neighbors[4];
        getneighbors(current->data, neighbors);
        for (int i = 0; i < 4; i++) {
            if (!isexplored(&explored, neighbors[i]) && traversable(board, current->data, neighbors[i])) {
                markexplored(&explored, neighbors[i]);
                Node *neighbor = malloc(sizeof(Node));
                neighbor->data = neighbors[i];
                neighbor->parent = current;
                enqueue(queue, neighbor);
            }
        }

    }
    return NULL;
}

void markexplored(unsigned short *explored, int i) {
    *explored = *explored | (2 << i);
}

int isexplored(unsigned short *explored, int i) {
    return *explored & (2 << i);
}

void getneighbors(int i, int *neighbors) {
    neighbors[0] = i - 1;
    neighbors[1] = i + 1;
    neighbors[2] = i - 4;
    neighbors[3] = i + 4;
}

