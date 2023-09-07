#include "robot.h"

void barriermode(SDL_Context *ctx, State *state);
void gatemode(SDL_Context *ctx, State *state);
void targetmode(SDL_Context *ctx, State *state);
void startmode(SDL_Context *ctx, State *state);

void handlemode(SDL_Context *ctx, State *state) {
    switch (state->mode) {
        case BARRIER:
            barriermode(ctx, state);
            break;
        case GATE:
            gatemode(ctx, state);
            break;
        case TARGET:
            targetmode(ctx, state);
            break;
        case START:
            startmode(ctx, state);
            break;
        default:
            break;
    }
}

void barriermode(SDL_Context *ctx, State *state) {
    int b = getclosestbarrier(state->mousepos, state->board);
    SDL_SetRenderTarget(ctx->renderer, ctx->texture);
    SDL_SetRenderDrawColor(ctx->renderer, 0xB0, 0xB0, 0xB0, 0xFF);
    renderbarrier(ctx, b);
}

void barrierclick(SDL_Context *ctx, State *state) {
    int b = getclosestbarrier(state->mousepos, state->board);
    if (barrierexists(state->board->barriers, b)) {
        removebarrier(state->board->barriers, b);
    } else {
        addbarrier(state->board->barriers, b);
    }
}

void gatemode(SDL_Context *ctx, State *state) {
    int g = getclosestsquare(state->mousepos, state->board);
    SDL_SetRenderTarget(ctx->renderer, ctx->texture);
    SDL_SetRenderDrawColor(ctx->renderer, 0xF0, 0x80, 0x80, 0xFF);
    rendergate(ctx, g);
}

void gateclick(SDL_Context *ctx, State *state) {
    int g = getclosestsquare(state->mousepos, state->board);
    if (gateexists(state->board->gates, g)) {
        removegate(state->board->gates, g);
    } else {
        addgate(state->board->gates, g);
    }
}

void targetmode(SDL_Context *ctx, State *state) {
    int g = getclosestsquare(state->mousepos, state->board);
    rendertarget(ctx, g);
}

void targetclick(SDL_Context *ctx, State *state) {
    int g = getclosestsquare(state->mousepos, state->board);
    if (state->board->target == g) {
        state->board->target = -1;
    } else {
        state->board->target = g;
    }
}

void startmode(SDL_Context *ctx, State *state) {
    int b = getclosestedge(state->mousepos, state->board);
    renderstart(ctx, b);
}

void startclick(SDL_Context *ctx, State *state) {
    int b = getclosestedge(state->mousepos, state->board);
    if (state->board->start == b) {
        state->board->start = -1;
    } else {
        state->board->start = b;
    }
}

