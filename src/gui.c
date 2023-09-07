#include "robot.h"

#define SQUARESIZE 256
#define BORDERSIZE 8
#define WIDTH 4
#define HEIGHT 4
#define WINWIDTH ((WIDTH * SQUARESIZE) + (BORDERSIZE * 2))
#define WINHEIGHT (HEIGHT * SQUARESIZE + (BORDERSIZE * 2))

#define BOARDWIDTH WIDTH * SQUARESIZE
#define BOARDHEIGHT HEIGHT * SQUARESIZE

#define BARRIERTHICKNESS (SQUARESIZE / 16)
#define BARRIEROFFSET (SQUARESIZE / 16)
#define BARRIERLENGTH (SQUARESIZE - (BARRIEROFFSET * 2))
#define GATETHICKNESS (BARRIERTHICKNESS / 2 + (SQUARESIZE / 64))

void updatetexture(SDL_Context *ctx, Board *board);
void rendergrid(SDL_Context *ctx);
void renderbarriers(SDL_Context *ctx, long long *barriers);
void rendergates(SDL_Context *ctx, short *gates);

SDL_Context *SDL_InitContext() {
    SDL_Context *ctx = malloc(sizeof(SDL_Context));
    SDL_Window *window = SDL_CreateWindow("Robot Tour", SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED, WINWIDTH,
                                     WINHEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_TARGET,
                                             BOARDWIDTH, BOARDHEIGHT);
    Uint32 lastupdate = SDL_GetTicks();

    ctx->window = window;
    ctx->renderer = renderer;
    ctx->texture = texture;
    ctx->lastupdate = lastupdate;
  
    return ctx;
}

void render(SDL_Context *ctx, Board *board) {
    ctx->lastupdate = SDL_GetTicks();
    SDL_SetRenderTarget(ctx->renderer, NULL);
    SDL_SetRenderDrawColor(ctx->renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(ctx->renderer);

    SDL_Rect dstrect = {BORDERSIZE, BORDERSIZE, BOARDWIDTH, BOARDHEIGHT};
    updatetexture(ctx, board);
    SDL_SetRenderTarget(ctx->renderer, NULL);
    SDL_RenderCopy(ctx->renderer, ctx->texture, NULL, &dstrect);

    SDL_RenderPresent(ctx->renderer);
}

void updatetexture(SDL_Context *ctx, Board *board) {
    SDL_SetRenderTarget(ctx->renderer, ctx->texture);
    SDL_SetRenderDrawColor(ctx->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(ctx->renderer);

    rendergrid(ctx);
    rendergates(ctx, board->gates);
    renderbarriers(ctx, board->barriers);
}

void rendergrid(SDL_Context *ctx) {
    SDL_SetRenderTarget(ctx->renderer, ctx->texture);
    SDL_SetRenderDrawColor(ctx->renderer, 0x00, 0x00, 0x00, 0xFF);
    int i;
    for (i = 0; i < HEIGHT; i++) {
        SDL_RenderDrawLine(ctx->renderer, 0, i * SQUARESIZE, BOARDWIDTH, i * SQUARESIZE);
        SDL_RenderDrawLine(ctx->renderer, i * SQUARESIZE, 0, i * SQUARESIZE, BOARDHEIGHT);
    }
}

void rendergates(SDL_Context *ctx, short *gates) {
    SDL_SetRenderTarget(ctx->renderer, ctx->texture);
    int i;
    for (i = 0; i < 16; i++) {
        SDL_SetRenderDrawColor(ctx->renderer, 0xFF, 0x00, 0x00, 0xFF);
        if (*gates & (1 << i)) {
            SDL_Rect rect = {
                (i % 4) * SQUARESIZE,
                (i / 4) * SQUARESIZE,
                SQUARESIZE,
                SQUARESIZE
            };
            SDL_RenderFillRect(ctx->renderer, &rect);
            rect.x += GATETHICKNESS;
            rect.y += GATETHICKNESS;
            rect.w -= GATETHICKNESS * 2;
            rect.h -= GATETHICKNESS * 2;
            SDL_SetRenderDrawColor(ctx->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderFillRect(ctx->renderer, &rect);
        }
    }
}

void renderbarriers(SDL_Context *ctx, long long *barriers) {
    SDL_SetRenderTarget(ctx->renderer, ctx->texture);
    SDL_SetRenderDrawColor(ctx->renderer, 0x00, 0x00, 0x00, 0xFF);
    int i;
    for (i = 0; i < 40; i++) {
        if (*barriers & ((long long)1 << i)) {
            int horiz = (i % 9) < 4;
            SDL_Rect rect;
            rect.x = horiz ? (i % 9) * SQUARESIZE + BARRIEROFFSET : (((i % 9) - 4) % 5) * SQUARESIZE - (BARRIERTHICKNESS / 2);
            rect.y = horiz ? (i / 9) * SQUARESIZE - (BARRIERTHICKNESS / 2) : (i / 9) * SQUARESIZE + BARRIEROFFSET;
            rect.w = horiz ? BARRIERLENGTH : BARRIERTHICKNESS;
            rect.h = horiz ? BARRIERTHICKNESS : BARRIERLENGTH;

            SDL_RenderFillRect(ctx->renderer, &rect);
        }
    }
}

void cleanup(SDL_Context *ctx) {
    SDL_DestroyWindow(ctx->window);
    free(ctx);
    SDL_Quit();
    exit(0);
}

