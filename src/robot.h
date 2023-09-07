#include <SDL2/SDL.h>

#define NUMBARRIERS    8
#define NUMGATES   3

#define SQUARESIZE 256
#define BORDERSIZE 8
#define WIDTH 4
#define HEIGHT 4
#define WINWIDTH ((WIDTH * SQUARESIZE) + (BORDERSIZE * 2))
#define WINHEIGHT (HEIGHT * SQUARESIZE + (BORDERSIZE * 2))
#define BOARDWIDTH WIDTH * SQUARESIZE
#define BOARDHEIGHT HEIGHT * SQUARESIZE

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    Uint32 lastupdate;
    SDL_Rect *dstrect;
} SDL_Context;

typedef struct {
    long long *barriers;
    short *gates;
    int target;
    int start;
} Board;

typedef enum {
    NORMAL,
    BARRIER,
    GATE,
    TARGET,
    START,
} Mode;

typedef struct {
    Board *board;
    Mode mode;
    SDL_Point *mousepos;
} State;


Board *newboard();
void addgate(short *gates, int i);
void addgates(short *gates, int *gateslist, int n);
void removegate(short *gates, int i);
int gateexists(short *gates, int i);
void addbarrier(long long *barriers, int i);
void addbarriers(long long *barriers, int *barrierlist, int n);
void removebarrier(long long *barriers, int i);
int barrierexists(long long *barriers, int i);
int getclosestbarrier(SDL_Point *mousepos, Board *board);
int getclosestsquare(SDL_Point *mousepos, Board *board);
int getclosestedge(SDL_Point *mousepos, Board *board);

SDL_Context *SDL_InitContext();
void cleanup(SDL_Context *ctx);
void render(SDL_Context *ctx, State *state);
int needsupdate(Uint32 lastupdate);
void renderbarrier(SDL_Context *ctx, int i);
void rendergate(SDL_Context *ctx, int i);
void rendertarget(SDL_Context *ctx, int target);
void renderstart(SDL_Context *ctx, int start);

void handleevents(SDL_Context *ctx, State *state);

void handlemode(SDL_Context *ctx, State *state);
void barrierclick(SDL_Context *ctx, State *state);
void gateclick(SDL_Context *ctx, State *state);
void targetclick(SDL_Context *ctx, State *state);
void startclick(SDL_Context *ctx, State *state);

