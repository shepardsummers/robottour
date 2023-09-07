#include <SDL2/SDL.h>

#define NUMBARRIERS    8
#define NUMGATES   3

typedef struct Board {
    long long *barriers;
    short *gates;
} Board;

Board *newboard();
void addbarrier(long long *barriers, int i);
void addgate(short *gates, int i);
void addbarriers(long long *barriers, int *barrierlist, int n);
void addgates(short *gates, int *gateslist, int n);

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    Uint32 lastupdate;
} SDL_Context;

SDL_Context *SDL_InitContext();
void cleanup(SDL_Context *ctx);
void render(SDL_Context *ctx, Board *board);

