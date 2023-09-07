#include <stdio.h>
#include "robot.h"

Board *newboard() {
    Board *board = malloc(sizeof(Board));
    board->barriers = malloc(sizeof(long long));
    board->gates = malloc(sizeof(short));
    *board->barriers &= 0;
    *board->gates = 0;
    return board;
}

void addbarrier(long long *barriers, int i) {
    *barriers = *barriers | ((long long) 1 << i);
}

void addgate(short *gates, int i) {
    *gates = *gates | (1 << i);
}

void addbarriers(long long *barriers, int *barrierlist, int n) {
    for (int i = 0; i < n; i++) {
        addbarrier(barriers, barrierlist[i]);
    }
}

void addgates(short *gates, int *gateslist, int n) {
    for (int i = 0; i < n; i++) {
        addgate(gates, gateslist[i]);
    }
}

