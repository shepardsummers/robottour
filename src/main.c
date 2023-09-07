#include <stdio.h>
#include "robot.h"

int main() {
    Board *board = newboard();
    SDL_Context *ctx = SDL_InitContext();

    int barrierlist[] = {6, 7, 10, 18, 20, 24, 28, 30};
    addbarriers(board->barriers, barrierlist, NUMBARRIERS);
    int gateslist[] = {2, 8, 11};
    addgates(board->gates, gateslist, NUMGATES);

    render(ctx, board);
    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                cleanup(ctx);
                break;
        }
        render(ctx, board);
    }
}

