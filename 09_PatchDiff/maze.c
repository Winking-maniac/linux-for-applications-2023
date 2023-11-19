#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum {
    MAZE_SIZE = 6,
    FULL_MAZE_SIZE = 2 * MAZE_SIZE + 1,
    ROOMS = MAZE_SIZE * MAZE_SIZE,
    PASSES = 2 * MAZE_SIZE * (MAZE_SIZE - 1)
};

typedef enum RoomStatus {
    IN,
    BOUND,
    OUT
} RoomStatus;

typedef enum PassStatus {
    PASS,
    WALL
} PassStatus;

typedef struct Maze {
    int bounds;
    RoomStatus *rooms;
    PassStatus *passes;
} Maze;

void
connect(Maze *m) {
    int r = rand() % m->bounds;
    int cur = 0;
    int i;
    for (i = 0; i < ROOMS; ++i) {
        if (m->rooms[i] == BOUND) {
            if (cur == r) break;
            cur++;
        }
    }
    m->rooms[i] = IN;
    m->bounds--;
    int found = 0;
    int row = i / MAZE_SIZE;
    int col = i % MAZE_SIZE;
    if (col != 0) {
        if (m->rooms[i - 1] == IN) {
            m->passes[(2 * MAZE_SIZE - 1) * row + col - 1] = PASS;
            found = 1;
        } else if (m->rooms[i - 1] == OUT) {
            m->rooms[i - 1] = BOUND;
            m->bounds++;
        }
    }
    if (col != MAZE_SIZE - 1) {
        if (m->rooms[i + 1] == IN && !found) {
            m->passes[(2 * MAZE_SIZE - 1) * row + col] = PASS;
            found = 1;
        } else if (m->rooms[i + 1] == OUT) {
            m->rooms[i + 1] = BOUND;
            m->bounds++;
        }
    }
    if (row != 0) {
        if (m->rooms[i - MAZE_SIZE] == IN && !found) {
            m->passes[(2 * MAZE_SIZE - 1) * (row - 1) + col + MAZE_SIZE - 1] = PASS;
            found = 1;
        } else if (m->rooms[i - MAZE_SIZE] == OUT) {
            m->rooms[i - MAZE_SIZE] = BOUND;
            m->bounds++;
        }
    }
    if (row != MAZE_SIZE - 1) {
        if (m->rooms[i + MAZE_SIZE] == IN && !found) {
            m->passes[(2 * MAZE_SIZE - 1) * row + col + MAZE_SIZE - 1] = PASS;
            found = 1;
        } else if (m->rooms[i + MAZE_SIZE] == OUT) {
            m->rooms[i + MAZE_SIZE] = BOUND;
            m->bounds++;
        }
    }
}

Maze *
generate(void) {
    Maze *res = calloc(1, sizeof(*res));
    res->rooms = calloc(ROOMS, sizeof(*(res->rooms)));
    res->passes = calloc(PASSES, sizeof(*(res->passes)));
    for (int i = 0; i < PASSES; ++i) {
        res->passes[i] = WALL;
    }
    for (int i = 0; i < ROOMS; ++i) {
        res->rooms[i] = OUT;
    }
    res->rooms[0] = IN;
    res->rooms[1] = BOUND;
    res->rooms[MAZE_SIZE] = BOUND;
    res->bounds = 2;
    for (int i = 0; i < ROOMS - 1; ++i) {
        connect(res);
    }
    return res;
}

void
print(Maze *m, char wall, char pass) {
    for (int i = 0; i < FULL_MAZE_SIZE; ++i) {
        for (int j = 0; j < FULL_MAZE_SIZE; ++j) {
            if (j == 0 || i == 0 || j == FULL_MAZE_SIZE - 1 || i == FULL_MAZE_SIZE - 1) {
                printf("%c", wall);
            } else if (i % 2 == 0 && j % 2 == 0) {
                printf("%c", wall);
            } else if (i * j % 2 != 0) {
                printf("%c", pass);
            } else {
                if (i % 2 == 1) {
                    printf("%c", m->passes[(2 * MAZE_SIZE - 1) * (i / 2) + (j / 2) - 1] == PASS
                            ? pass : wall);
                } else {
                    printf("%c",m->passes[(2 * MAZE_SIZE - 1) * (i / 2 - 1) + MAZE_SIZE - 1 + j / 2] == PASS
                             ? pass : wall);
                }
            }
        }
        printf("\n");
    }
}

int
main(int argc, char *argv[]) {
    srand(time(NULL));
    
    Maze *m = generate();
    print(m, '#', '.');
    
    free(m);
    return 0;
}
