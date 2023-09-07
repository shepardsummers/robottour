#include "robot.h"

void handleevent(SDL_Context *ctx, State *state, SDL_Event *event);
void updatemousepos(SDL_Context *ctx, SDL_Event *event, SDL_Point *mousepos);

void handleevents(SDL_Context *ctx, State *state) {
    SDL_Event *event = malloc(sizeof(SDL_Event));
    while (SDL_PollEvent(event)) {
        handleevent(ctx, state, event);
    }
    free(event);
}

void handleevent(SDL_Context *ctx, State *state, SDL_Event *event) {
        switch (event->type) {
            case SDL_QUIT:
                cleanup(ctx);
                break;
            case SDL_MOUSEMOTION:
                updatemousepos(ctx, event, state->mousepos);
                break;
            case SDL_KEYDOWN:
                switch (event->key.keysym.sym) {
                    case SDLK_ESCAPE:
                        state->mode = NORMAL;
                        break;
                    case SDLK_c:
                        if (event->key.keysym.mod & KMOD_CTRL) {
                            state->mode = NORMAL;
                        }
                        break;
                    case SDLK_b:
                        state-> mode = BARRIER;
                        break;
                    case SDLK_g:
                        state->mode = GATE;
                        break;
                    case SDLK_t:
                        state->mode = TARGET;
                        break;
                    case SDLK_s:
                        state->mode = START;
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                switch (state->mode) {
                    case BARRIER:
                        barrierclick(ctx, state);
                        break;
                    case GATE:
                        gateclick(ctx, state);
                        break;
                    case TARGET:
                        targetclick(ctx, state);
                        break;
                    case START:
                        startclick(ctx, state);
                        break;
                    default:
                        break;
                }
                break;
        }
}

void updatemousepos(SDL_Context *ctx, SDL_Event *event, SDL_Point *mousepos) {
    int w, h;
    SDL_GetWindowSize(ctx->window, &w, &h);
    mousepos->x = ((event->motion.x - ((w - ctx->dstrect->w) / 2)) * WINWIDTH) / ctx->dstrect->w;
    mousepos->y = ((event->motion.y - ((h - ctx->dstrect->h) / 2)) * WINHEIGHT) / ctx->dstrect->h;
}

