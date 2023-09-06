#define NUM_BARRIERS    8
#define NUM_GATES   3

typedef struct Barrier {
    int x;
    int y;
} Barrier;

typedef struct Gate {
    int x;
    int y;
} Gate;

typedef struct Board {
    Barrier *barriers;
    Gate *gates;
} Board;

void printboard(Board *board);
