#include <stdio.h>
#include "robot.h"

int main() {
    State *state = malloc(sizeof(State));
    state->board = newboard();
    state->mode = NORMAL;
    state->mousepos = malloc(sizeof(SDL_Point));
    state->path = malloc(sizeof(Node));

    SDL_Context *ctx = SDL_InitContext();

    int barrierlist[] = {6, 7, 10, 18, 20, 24, 28, 30};
    addbarriers(state->board->barriers, barrierlist, NUMBARRIERS);
    int gateslist[] = {2, 8, 11};
    addgates(state->board->gates, gateslist, NUMGATES);

    render(ctx, state);
    SDL_Event event;
    while (1) {
        if (!needsupdate(ctx->lastupdate)) {
            SDL_Delay(1);
            continue;
        }
        handleevents(ctx, state);
        render(ctx, state);

        updatepath(state);
    }
}

