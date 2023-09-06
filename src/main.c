#include <stdio.h>
#include "board.h"

int main() {
    Barrier barriers[] = {
        {1, 2},
        {3, 1},
        {3, 3},
        {4, 0},
        {4, 2},
        {5, 2},
        {6, 0},
        {7, 3},
    };
    Gate gates[] = {
        {0, 1},
        {2, 3},
        {3, 1},
    };
    Board board;
    board.barriers = barriers;
    board.gates = gates;
    printboard(&board);
}

