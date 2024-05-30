//
// Created by angel on 8/05/24.
//

#include "Conway.h"

static Cell **M = (Cell **) NULL;
static int width = 0;
static int height = 0;

/* Some initialization definitions */
static void InitGrid( void ); /* Initializes a random grid of Cell */
static void InitGame( void ); /* Initializes everything necessary for the game  */

/* Functions to do the game work */
static void CopyGrid( Cell **M_c ); /* Copies original grid to work with it */
static int CountNeighbors( int i , int j ); /* Count the amount of alive neighbors */
static void DrawGGrid( void ); /* Draws the grid */
static void DrawGame( void );
static void GameOfLife( Cell **M_c ); /* Applies game of life rules to the copy grid */
static void CloseGame(void);

void MainConway( void ) {
    InitGame();
    while (!WindowShouldClose())
        DrawGame();
    CloseGame();
}

void DrawGame(void) {
    BeginDrawing();
    DrawGGrid();
    EndDrawing();
}

void InitCell( void ) {
    width = GetScreenWidth(); /* Get the screen width */
    height = GetScreenHeight(); /* Get the screen height */

    /*
     * We are working with boxes of 10 by 10, so we're going to divide by 10 both sides.
     * I mean, on a screen of x*y pixels, there's going to be x*y/100 - 80 boxes.
     */

    M = (Cell **) calloc(width, sizeof(Cell));

    int i = 0;
    while (i < width) {
        M[i] = (Cell *) calloc(height, sizeof(Cell));
        int j = 0;
        while (j < height) {
            M[i][j] = (Cell) {
                    (Rectangle){10*(float)i, 10*(float)j, 10, 10},
                    BLACK,
                    false
            };
            j++;
        }
        i++;
    }
}

void InitGame( void ) { /* Initializes window, set target frames per second and initializes cell */
    SetWindowTitle("Conway");
    SetTargetFPS(60);
    InitCell();
}

void DrawGGrid( void ) { /* Draws the grid drawing each box in the grid on the screen */
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            DrawRectangleRec(M[i][j].box, M[i][j].c);
}

void CloseGame(void) {
    for (int i = 0; i < width; i++) free((void *)M[i]);
    free((void *)M);
}
