//
// Created by angel on 8/05/24.
//

#include "Conway.h"

static Cell **M = (Cell **) NULL;
static Cell **M1 = (Cell **) NULL;
static short int height = 0;
static short int width = 0;

static void InitGame( void ); /* Initialization of game */
static void UpdateGame( void ); /* Updates variable state */
static void DrawGame( void ); /* Draw game variables */
static void CloseGame( void ); /* Close game instances */

static int CountNeighbors( int i, int j ); /* Count the amount of alive neighbors around M[i][j] */
static void GameOfLife( int n, int i, int j ); /* Applies the game of life rules to M[i][j] and the result is M1[i][j] */
static void ChangeGrid ( void ); /* Changes the grids */

void MainConway( void ) { /* Main function for the game */
    InitGame();
    while ( !WindowShouldClose() ) {
        UpdateGame();
        DrawGame();
    }
    CloseGame();
}

void InitGame( void ) {

    SetWindowTitle("Game of Life");
    SetTargetFPS(60);

    M = (Cell **) malloc( height * sizeof(Cell) );
    M1 = (Cell **) malloc( height * sizeof(Cell) );

    for ( int i = 0; i < height; i++ ) {
        M[i] = (Cell *) malloc( width * sizeof(Cell) );
        M1[i] = (Cell *) malloc( width * sizeof(Cell) );
        for ( int j = 0; j < width; j++ ) {
            M[i][j] = (Cell) {
                    (Rectangle) {
                        10*(float)i,
                        10*(float)j,
                        10,
                        10
                        },
                    BLACK,
                    false
            };
            M1[i][j] = M[i][j];
        }
    }

}

void UpdateGame( void ) {
    if ( M != NULL && M1 != NULL ) {
        /* Here update game variables */

    }
    else {
        perror("Error when updating game variables !!! \n");
        exit(EXIT_FAILURE);
    }
}

void DrawGame( void ) {
    if (M != NULL && M1 != NULL) {
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++) {
                DrawRectangleRec(M[i][j].box, M[i][j].c);
                DrawRectangleLinesEx(M[i][j].box, 1.0f, GRAY);
            }
    } else {
        perror("Error when drawing the game !!! \n");
        exit(EXIT_FAILURE);
    }
}

void CloseGame( void ) {
    for ( int i = 0; i < height; i++ ) {
        for ( int j = 0; j < width; j++ ) {
            M[i][j] = (Cell) { 0 };
            M1[i][j] = (Cell) { 0 };
        }
        free( (void *) M[i] );
        free( (void *) M1[i] );
    }
    free( (void *) M );
    free( (void *) M1 );
}

void ChangeGrid( void ) {
    static Cell **Aux = (Cell **) NULL;
    Aux = M;
    M = M1;
    M1 = Aux;
}

void GameOfLife(int n, int i, int j) {

    if ( M[i][j].state && ( n < 2 || n > 3 ) ) {
        M1[i][j].state = false;
        M1[i][j].c = BLACK;
    }
    else if ( !M[i][j].state && ( n == 3 ) ) {
        M1[i][j].state = true;
        M1[i][j].c = RAYWHITE;
    }

}

int CountNeighbors(int i, int j) {
    int neighbors = 0;

    for ( int ci = 0; ci < i + 1; ci++ ) {
        int current_i = (int) (ci + i) % height;

        for ( int cj = 0; cj < j + 1; cj++ ) {
            if ( ci == i && cj == j ) continue;
            int current_j = (int) (cj + j) % width;
            if ( M[current_i][current_j].state ) neighbors++;
        }
    }

    return neighbors;
}