//
// Created by angel on 8/05/24.
//

#include "Conway.h"

static Cell **M = (Cell **) NULL;
static int width = 0;
static int height = 0;

/* Some initialization definitions */
static void InitGrid(void); /* Initializes a random grid of Cell */
static void InitGame(void); /* Initializes everything necessary for the game  */

/* Functions to do the game work */

static void CopyGrid(Cell **M_c); /* Copies original grid to work with it */
static int CountNeighbors(Cell **M, Cell **M_c, int i, int j); /* Count the amount of alive neighbors */
static void GameOfLife(Cell **M_c); /* Applies game of life rules to the copy grid */

void MainConway(void) {
    
}

void InitCell(void) {
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
                    (Rectangle){(float)i, (float)j, 10, 10},
                    BLACK,
                    false
            };
            j += 10;
        }
        i += 10;
    }
}

void InitGame(void) {
    InitWindow(1280, 720, "Conway");
    SetTargetFPS(60);
    InitCell();
}

void CopyGrid(Cell **M_c) {
    if (M_c == (Cell **)NULL) M_c = (Cell **) calloc(width, sizeof(Cell));
    for(int i = 0; i < width; i++) {
        M_c[i] = (Cell *) calloc(height, sizeof(Cell));
        for (int j = 0; j < height; j++) M_c[i][j] = M[i][j]; /* Copy grid cell by cell */
    }
}

int CountNeighbors(Cell **M, Cell **M_c, int i, int j) {
    int neighbors = 0;
    if ( i  > 0 && j > 0 || i < width - 1 && j < height - 1) {
        for ( int k = i - 1; k < i + 1; k++ ) {
            for ( int t = k - 1; t < j + 1; t++ ) {

            }
        }
    }
    return neighbors;
}