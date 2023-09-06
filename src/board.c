#include <stdio.h>
#include "board.h"

int hasbarrier(Barrier *barriers, int x, int y);
int hasgate(Board *board, int x, int y);
void printbarrier(Barrier *barriers, int x, int y);
void printgate(Barrier *gates, int x, int y);

void printboard(Board *board) {
    int x, y;
    for (y = 0; y < 9; y++) {
        for (x = 0; x < 9; x++) {
            if (y % 2 && x % 2)
                printf("   ");
            else
                printbarrier(board->barriers, x, y);

        }
        putchar('\n');
    }
}

void printbarrier(Barrier *barriers, int x, int y) {
    if (y % 2) {
        // vertical
        if (hasbarrier(barriers, x, (y - 1)/2)) {
            printf("┃");
        } else {
            printf("│");
        }
    } else {
        // horizontal
        switch(x) {
            case 0:
                // left
                switch(y) {
                    case 0:
                        // top left
                        printf("┌");
                        break;
                    case 8:
                        // bottom left
                        printf("└");
                        break;
                    default:
                        printf("├");
                        break;
                }
                break;
            case 8:
                // right
                switch(y) {
                    case 0:
                        // top right
                        printf("┐");
                        break;
                    case 8:
                        // bottom right
                        printf("┘");
                        break;
                    default:
                        printf("┤");
                        break;
                };
                break;
            default:
                if (x % 2) {
                    if (hasbarrier(barriers, x, y/2)) {
                        printf("━━━");
                    } else {
                        printf("───");
                    }
                } else {
                    switch(y) {
                        case 0:
                            // top
                            printf("┬");
                            break;
                        case 8:
                            // bottom
                            printf("┴");
                            break;
                        default:
                            printf("┼");
                            break;
                    };
                }
                break;
        };
    }
}

int hasbarrier(Barrier *barriers, int x, int y) {
    int i;
    for (i = 0; i < NUM_BARRIERS; i++) {
        if ((barriers[i].x == x) && (barriers[i].y == y)) {
            return 1;
        }
    }
    return 0;
}

int hasgate(Board *board, int x, int y) {
    int i;
    for (i = 0; i < NUM_GATES; i++) {
        if (board->gates[i].x == x && board->gates[i].y == y) {
            return 1;
        }
    }
    return 0;
}
