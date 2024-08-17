#include "Conway.h"

#define SQUARE_SIZE 10

static Arena* memory_pool = NULL;
static bool** grid = NULL;
static bool** copy_grid = NULL;
static short int rows = 0;
static short int cols = 0;

static void InitGame ( void );
static void UpdateGame ( void );
static void DrawGame ( void );
static void CloseGame ( void );

static void GameOfLife ( void );
static unsigned char CountNeighbors ( short int i, short int j );
static void ChangeGrid ( void );

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
    SetWindowTitle("Game of Life");

    rows = ( short int )( GetScreenHeight() / SQUARE_SIZE );
    cols = ( short int )( GetScreenWidth() / SQUARE_SIZE );

    memory_pool = CreateArena( 2*1024*1024  ); /* Creates an arena of 2MB */
    grid = ArenaAlloc(memory_pool, rows * sizeof(bool*));
    copy_grid = ArenaAlloc(memory_pool, rows * sizeof(bool*));

    for ( short int i = 0; i < rows; i++ ) {
        grid[i] = ArenaAlloc(memory_pool, cols * sizeof(bool));
        for ( short int j = 0; j < cols; j++ ) grid[i][j] = (bool)GetRandomValue(0, 1);
        copy_grid[i] = ArenaAlloc(memory_pool, cols * sizeof(bool));
        for ( short int j = 0; j < cols; j++ ) copy_grid[i][j] = grid[i][j];
    }

}

static void UpdateGame ( void ) {
    GameOfLife();
    ChangeGrid();
    WaitTime(0.25);
}

static void DrawGame ( void ) {
    BeginDrawing();
    ClearBackground(BLACK);
    for ( short int i = 1; i < rows - 1; i++ )
        for ( short int j = 1; j < cols - 1; j++ ) {
            if ( grid[i][j] ) {
                DrawRectangleRec((Rectangle){j*SQUARE_SIZE, i*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE}, RAYWHITE);
                DrawRectangleLinesEx((Rectangle){j*SQUARE_SIZE, i*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE}, 1.0f, GRAY);
            }
            else DrawRectangleLinesEx((Rectangle){j*SQUARE_SIZE, i*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE}, 1.0f, GRAY);
        }
    EndDrawing();
}

static void CloseGame ( void ) {
    DestroyArena(memory_pool);
    grid = NULL;
    copy_grid = NULL;
    rows = 0;
    cols = 0;
}

static void GameOfLife ( void ) {
    for ( short int i = 0; i < rows; i++ )
        for ( short int j = 0; j < cols; j++ ) {
            unsigned char alive_neighbors = CountNeighbors(i, j);
            if ( grid[i][j] && (alive_neighbors > 3 || alive_neighbors < 2) ) copy_grid[i][j] = 0;
            else if ( !grid[i][j] && alive_neighbors == 3 ) copy_grid[i][j] = 1;
        }
}

static unsigned char CountNeighbors ( short int i, short int j ) {
    unsigned char alive_neighbors = 0;

    /* When i and j are not in the matrix borders  */
    alive_neighbors += i != 0 && j != 0 && i < rows && j < cols && grid[i - 1][j - 1] ? 1 : 0;
    alive_neighbors += i != 0 && j != 0 && i < rows && j < cols && grid[i - 1][j] ? 1 : 0;
    alive_neighbors += i != 0 && j != 0 && i < rows && j < cols && grid[i - 1][j + 1] ? 1 : 0;
    alive_neighbors += i != 0 && j != 0 && i < rows && j < cols && grid[i][j - 1] ? 1 : 0;
    alive_neighbors += i != 0 && j != 0 && i < rows && j < cols && grid[i][j + 1] ? 1 : 0;
    alive_neighbors += i != 0 && j != 0 && i < rows && j < cols && grid[i + 1][j - 1] ? 1 : 0;
    alive_neighbors += i != 0 && j != 0 && i < rows && j < cols && grid[i + 1][j] ? 1 : 0;
    alive_neighbors += i != 0 && j != 0 && i < rows && j < cols && grid[i + 1][j + 1] ? 1 : 0;

    /* When i and j are in the matrix borers */

    return alive_neighbors;
}

static void ChangeGrid ( void ) {
    for ( short int i = 0; i < rows; i++ )
        for ( short int j = 0; j < cols; j++ ) grid[i][j] = copy_grid[i][j];
}