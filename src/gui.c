#include "robot.h"

#define FPS 60

#define BARRIERTHICKNESS (SQUARESIZE / 16)
#define BARRIEROFFSET (SQUARESIZE / 16)
#define BARRIERLENGTH (SQUARESIZE - (BARRIEROFFSET * 2))
#define GATETHICKNESS (BARRIERTHICKNESS / 2 + (SQUARESIZE / 64))
#define POINTSIZE (SQUARESIZE / 16)

void updatetexture(SDL_Context *ctx, State *state);
void rendergrid(SDL_Context *ctx);
void renderbarriers(SDL_Context *ctx, long long *barriers);
void rendergates(SDL_Context *ctx, unsigned short *gates);
void rendercircle(SDL_Renderer *renderer, int x0, int y0, int radius);
void rendertarget(SDL_Context *ctx, int target);
void renderstart(SDL_Context *ctx, int start);
void renderpath(SDL_Context *ctx, State *state, Node *path);

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
    SDL_Rect *dstrect = malloc(sizeof(SDL_Rect));
    dstrect->x = BORDERSIZE;
    dstrect->y = BORDERSIZE;
    dstrect->w = BOARDWIDTH;
    dstrect->h = BOARDHEIGHT;

    ctx->window = window;
    ctx->renderer = renderer;
    ctx->texture = texture;
    ctx->lastupdate = lastupdate;
    ctx->dstrect = dstrect;
  
    return ctx;
}

void render(SDL_Context *ctx, State *state) {
    ctx->lastupdate = SDL_GetTicks();
    SDL_SetRenderTarget(ctx->renderer, NULL);
    SDL_SetRenderDrawColor(ctx->renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(ctx->renderer);

    updatetexture(ctx, state);
    SDL_SetRenderTarget(ctx->renderer, NULL);
    SDL_RenderCopy(ctx->renderer, ctx->texture, NULL, ctx->dstrect);

    SDL_RenderPresent(ctx->renderer);
}

void updatetexture(SDL_Context *ctx, State *state) {
    SDL_SetRenderTarget(ctx->renderer, ctx->texture);
    SDL_SetRenderDrawColor(ctx->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(ctx->renderer);

    rendergrid(ctx);
    rendergates(ctx, state->board->gates);
    renderbarriers(ctx, state->board->barriers);
    rendertarget(ctx, state->board->target);
    renderstart(ctx, state->board->start);
    if (state->pathactive)
        renderpath(ctx, state, state->path);
    handlemode(ctx, state);
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

void rendergates(SDL_Context *ctx, unsigned short *gates) {
    SDL_SetRenderTarget(ctx->renderer, ctx->texture);
    int i;
    for (i = 0; i < 16; i++) {
        SDL_SetRenderDrawColor(ctx->renderer, 0xFF, 0x00, 0x00, 0xFF);
        if (*gates & (1 << i)) {
            rendergate(ctx, i);
        }
    }
}

void rendergate(SDL_Context *ctx, int i) {
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

void renderbarriers(SDL_Context *ctx, long long *barriers) {
    SDL_SetRenderTarget(ctx->renderer, ctx->texture);
    SDL_SetRenderDrawColor(ctx->renderer, 0x00, 0x00, 0x00, 0xFF);
    int i;
    for (i = 0; i < 40; i++) {
        if (*barriers & ((long long)1 << i)) {
            renderbarrier(ctx, i);
        }
    }
}

void renderbarrier(SDL_Context *ctx, int i) {
    int horiz = (i % 9) < 4;
    SDL_Rect rect;
    rect.x = horiz ? (i % 9) * SQUARESIZE + BARRIEROFFSET : (((i % 9) - 4) % 5) * SQUARESIZE - (BARRIERTHICKNESS / 2);
    rect.y = horiz ? (i / 9) * SQUARESIZE - (BARRIERTHICKNESS / 2) : (i / 9) * SQUARESIZE + BARRIEROFFSET;
    rect.w = horiz ? BARRIERLENGTH : BARRIERTHICKNESS;
    rect.h = horiz ? BARRIERTHICKNESS : BARRIERLENGTH;

    SDL_RenderFillRect(ctx->renderer, &rect);
}

void rendertarget(SDL_Context *ctx, int target) {
    if (target < 0)
        return;
    SDL_SetRenderDrawColor(ctx->renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_SetRenderTarget(ctx->renderer, ctx->texture);
    rendercircle(ctx->renderer,
                 (target % WIDTH) * SQUARESIZE + (SQUARESIZE / 2),
                 (target / WIDTH) * SQUARESIZE + (SQUARESIZE / 2), POINTSIZE);
}

void renderstart(SDL_Context *ctx, int start) {
    if (start < 0)
        return;
    SDL_SetRenderDrawColor(ctx->renderer, 0x00, 0xFF, 0x00, 0xFF);
    SDL_SetRenderTarget(ctx->renderer, ctx->texture);
    SDL_Point p = getstartcoords(start);
    rendercircle(ctx->renderer, p.x, p.y, POINTSIZE);
}

void renderpath(SDL_Context *ctx, State *state, Node *path) {
    if (!path)
        return;

    SDL_SetRenderDrawColor(ctx->renderer, 0x00, 0xFF, 0x00, 0xFF);
    SDL_SetRenderTarget(ctx->renderer, ctx->texture);

    Node *node = path;
    int x1, y1, x2, y2;
    x2 = (node->data % WIDTH) * SQUARESIZE + (SQUARESIZE / 2);
    y2 = (node->data / WIDTH) * SQUARESIZE + (SQUARESIZE / 2);
    
    node = node->parent;
    while (node) {
        x1 = x2;
        y1 = y2;
        x2 = (node->data % WIDTH) * SQUARESIZE + (SQUARESIZE / 2);
        y2 = (node->data / WIDTH) * SQUARESIZE + (SQUARESIZE / 2);
        SDL_RenderDrawLine(ctx->renderer, x1, y1, x2, y2);
        node = node->parent;
    }
    SDL_Point p = getstartcoords(state->board->start);
    SDL_RenderDrawLine(ctx->renderer, x2, y2, p.x, p.y);
}

void togglepath(State *state) {
    state->pathactive = !state->pathactive;
}

void rendercircle(SDL_Renderer *renderer, int x0, int y0, int radius) {
    int x = radius -1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y) {
        SDL_RenderDrawLine(renderer, x0 + y, y0 + x, x0 - y, y0 + x);
        SDL_RenderDrawLine(renderer, x0 + x, y0 + y, x0 - x, y0 + y);
        SDL_RenderDrawLine(renderer, x0 - y, y0 - x, x0 + y, y0 - x);
        SDL_RenderDrawLine(renderer, x0 - x, y0 - y, x0 + x, y0 - y);

        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        }

        if (err > 0) {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}


int needsupdate(Uint32 lastupdate) {
    return (SDL_GetTicks() - lastupdate) > (1000 / FPS);
}

void cleanup(SDL_Context *ctx) {
    SDL_DestroyWindow(ctx->window);
    free(ctx);
    SDL_Quit();
    exit(0);
}

