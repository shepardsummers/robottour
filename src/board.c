#include <stdio.h>
#include "robot.h"

int mouseinsideboard(SDL_Point *mousepos);

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
    board->start = 37;

    *board->barriers &= 0;
    *board->gates &= 0;
    return board;
}

void addgate(unsigned short *gates, int i) {
    *gates = *gates | (1 << i);
}

void addgates(unsigned short *gates, int *gateslist, int n) {
    for (int i = 0; i < n; i++) {
        addgate(gates, gateslist[i]);
    }
}

void removegate(unsigned short *gates, int i) {
    *gates = *gates & ~(1 << i);
}

int gateexists(unsigned short *gates, int i) {
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
    if (s < 0)
        return -1;
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
    if (!mouseinsideboard(mousepos))
        return -1;
    int gate = 0;
    gate += mousepos->x / SQUARESIZE;
    gate += (mousepos->y / SQUARESIZE) * WIDTH;
    return gate;
}

int getclosestedge(SDL_Point *mousepos, Board *board) {
    if (!mouseinsideboard(mousepos))
        return -1;
    int x, y;
    x = mousepos->x;
    y = mousepos->y;

    int distances[] = {y, BOARDHEIGHT - y, x, BOARDWIDTH - x};
    int min = distances[0];
    Direction dir = TOP;
    int i;
    for (i = 1; i < 4; i++) {
        if (distances[i] < min) {
            min = distances[i];
            dir = i;
        }
    }
    switch (dir) {
        case TOP:
            return x / SQUARESIZE;
            break;
        case BOTTOM:
            return (x / SQUARESIZE) + (HEIGHT * 9);
            break;
        case LEFT:
            return 4 + ((y / SQUARESIZE) * 9);
            break;
        case RIGHT:
            return 8 + ((y / SQUARESIZE) * 9);
            break;
    }
}

int getstartsquare(int start) {
    if (start < 4) {
        return start;
    } if (start > 35) {
        return start - 24;
    } else if (start % 9 == 4){
        return ((start - 4) / 9) * 4;
    } else {
        return (((start - 8) / 9 + 1) * 4) - 1;
    }
}

SDL_Point getstartcoords(int start) {
    SDL_Point p;
    int horiz;
    horiz = (start % 9) < 4;
    if (horiz) {
        p.x = (start % 9) * SQUARESIZE + (SQUARESIZE / 2);
        p.y = (start / 9) * SQUARESIZE;
    } else {
        p.x = (((start % 9) - 4) % 5) * SQUARESIZE;
        p.y = (start / 9) * SQUARESIZE + (SQUARESIZE / 2);
    }
    return p;
}

int traversable(Board *board, int s1, int s2) {
    if (s1 < 0 || s1 > 15 || s2 < 0 || s2 > 15) {
        return 0;
    } else if (s1 == s2) {
        return 0;
    } else

    if (s1 - s2 == 1) {
        if (s1 / 4 != s2 / 4)
            return 0;
        return !barrierexists(board->barriers, s1 + (5 * (s1 / 4)) + 4);
    } else if (s1 - s2 == -1) {
        if (s1 / 4 != s2 / 4)
            return 0;
        return !barrierexists(board->barriers, s1 + (5 * (s1 / 4)) + 5);
    } else if (s1 - s2 == 4) {
        return !barrierexists(board->barriers, s1 + (5 * (s1 / 4)));
    } else if (s1 - s2 == -4) {
        return !barrierexists(board->barriers, s1 + (5 * (s1 / 4)) + 9);
    } else {
        return 0;
    }
}

int mouseinsideboard(SDL_Point *mousepos) {
    return mousepos->x > 0 && mousepos->x < BOARDWIDTH && mousepos->y > 0 && mousepos->y < BOARDHEIGHT;
}
