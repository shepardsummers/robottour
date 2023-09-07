#include <stdio.h>
#include "robot.h"

typedef enum {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT
} Direction;

Board *newboard() {
    Board *board = malloc(sizeof(Board));
    board->barriers = malloc(sizeof(long long));
    board->gates = malloc(sizeof(short));
    board->target = 1;

    *board->barriers &= 0;
    *board->gates &= 0;
    return board;
}

void addgate(short *gates, int i) {
    *gates = *gates | (1 << i);
}

void addgates(short *gates, int *gateslist, int n) {
    for (int i = 0; i < n; i++) {
        addgate(gates, gateslist[i]);
    }
}

void removegate(short *gates, int i) {
    *gates = *gates & ~(1 << i);
}

int gateexists(short *gates, int i) {
    return *gates & (1 << i);
}

void addbarrier(long long *barriers, int i) {
    *barriers = *barriers | ((long long) 1 << i);
}

void addbarriers(long long *barriers, int *barrierlist, int n) {
    for (int i = 0; i < n; i++) {
        addbarrier(barriers, barrierlist[i]);
    }
}

void removebarrier(long long *barriers, int i) {
    *barriers = *barriers & ~((long long) 1 << i);
}

int barrierexists(long long *barriers, int i) {
    return *barriers & ((long long) 1 << i) ? 1 : 0;
}

int getclosestbarrier(SDL_Point *mousepos, Board *board) {
    int s = getclosestsquare(mousepos, board);
    int x = mousepos->x;
    int y = mousepos->y;
    x -= (s % WIDTH) * SQUARESIZE;
    y -= (s / WIDTH) * SQUARESIZE;

    int distances[] = {y, SQUARESIZE - y, x, SQUARESIZE - x};
    int min = distances[0];
    Direction dir = TOP;
    int i;
    for (i = 1; i < 4; i++) {
        if (distances[i] < min) {
            min = distances[i];
            dir = i;
        }
    }

    int b;
    b = s + (5 * (s / 4));
    if (dir == LEFT) {
        b += 4;
    } else if (dir == RIGHT) {
        b += 5;
    } else if (dir == BOTTOM) {
        b += 9;
    }
    return b;
}

int getclosestsquare(SDL_Point *mousepos, Board *board) {
    int gate = 0;
    gate += mousepos->x / SQUARESIZE;
    gate += (mousepos->y / SQUARESIZE) * WIDTH;
    return gate;
}
