#include "Conway.h"

static int **M = NULL;
static int **M1 = NULL;
static int height = 0;
static int width = 0;
static const int square_size = 9;

static void InitGame ( void );
static void UpdateGame ( void );
static void DrawGame ( void );
static void CloseGame ( void );

void MainConway ( void ) {
    InitGame();
    while (!WindowShouldClose()) {
        UpdateGame();
        DrawGame();
    }
    CloseGame();
}

static void InitGame ( void ) {
    SetTargetFPS(60);

    height = GetScreenHeight() / square_size;
    width = GetScreenWidth() / square_size;

    M = (int **) malloc(height * sizeof(int *));
    M1 = (int **) malloc(height * sizeof(int *));

    if (!M || !M1) {
        CloseWindow();
        exit(1);
    }

    for (int i = 0; i < height; i++) {
        M[i] = (int *)malloc(width * sizeof(int));
        M1[i] = (int *)malloc(width * sizeof(int));
        if (!M[i] || !M1[i]) {
            CloseWindow();
            exit(1);
        }
        for (int j = 0; j < width; j++) {
            M[i][j] = GetRandomValue(0, 1);
            M1[i][j] = M[i][j];
        }
    }
}

void CopyGrid ( void ) { for ( int i = 0; i < height; i++ ) strncpy((void *)M[i], (void *)M1[i], sizeof(int)); }

int CountNeighbors ( int i, int j ) {
    int alive_neighbors = 0;
    if (i == 0) i = height - 1;
    if (j == 0) j = width - 1;

    for (int l = i - 1; l <= i + 1; l++) {
        for (int k = j - 1; k <= j + 1; k++) {
            if (l == i && k == j) continue;
            int current_i = (l + i) % height;
            int current_j = (k + j) % width;
            if (M[current_i][current_j]) alive_neighbors++;
        }
    }
    return alive_neighbors;
}

static void UpdateGame ( void ) {
    for ( int i = 0; i < height; i++ ) {
        for ( int j = 0; j < width; j++ ) {
            int alive_neighbors = CountNeighbors(i, j);
            if (M[i][j] && (alive_neighbors > 3 || alive_neighbors < 2)) M1[i][j] = 0;
            if (!M[i][j] && (alive_neighbors == 3)) M1[i][j] = 1;
        }
    }
    CopyGrid();
}

static void DrawGame ( void ) {
    BeginDrawing();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            Color c = (M[i][j] == 0) ? BLACK : RAYWHITE;
            Rectangle R = (Rectangle) {
                (float) (square_size * j),
                (float) (square_size * i),
                (float) square_size,
                (float) square_size
            };
            DrawRectangleRec(R, c);
            DrawRectangleLinesEx(R, 1, GRAY);
        }
    }
    EndDrawing();
    WaitTime(0.5);
}

static void CloseGame ( void ) {
    for ( int i = 0; i < height; i++ ) {
            free(M[i]);
            free(M1[i]);
    }
    free(M);
    free(M1);
    M = NULL;
    M1 = NULL;
}