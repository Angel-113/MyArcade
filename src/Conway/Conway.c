#include "Conway.h"

#define SQUARE_SIZE 10

static Arena* memory_pool = NULL;
static unsigned char** grid = NULL;
static unsigned char** copy_grid = NULL;
static short int rows = 0;
static short int cols = 0;

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
    SetWindowTitle("Game of Life");

    rows = ( short int )( GetScreenHeight() / SQUARE_SIZE );
    cols = ( short int )( GetScreenWidth() / SQUARE_SIZE );

    memory_pool = CreateArena( 2 * ( rows * cols * sizeof(unsigned char) ) );
    grid = ArenaAlloc(memory_pool, rows * sizeof(unsigned char*));
    copy_grid = ArenaAlloc(memory_pool, rows * sizeof(unsigned char*));

    for ( int i = 0; i < rows; i++ ) {

        grid[i] = ArenaAlloc(memory_pool, cols * sizeof(unsigned char));
        copy_grid[i] = ArenaAlloc(memory_pool, cols * sizeof(unsigned char));

        for ( int j = 0; j < cols; j++ ) {
            grid[i][j] = ( short int ) GetRandomValue(0, 1);
            copy_grid[i][j] = grid[i][j];
        }

    }

}

static void UpdateGame ( void ) {

}

static void DrawGame ( void ) {
    BeginDrawing();
    ClearBackground(BLACK);
    for ( int i = 0; i < rows; i++ )
        for ( int j = 0; j < cols; j++ ) {
            if ( grid[i][j] ) {
                DrawRectangleRec((Rectangle){j, i, SQUARE_SIZE, SQUARE_SIZE}, RAYWHITE);
                DrawRectangleLinesEx((Rectangle){j, i, SQUARE_SIZE, SQUARE_SIZE}, 1.0f, GRAY);
            }
            else DrawRectangleLinesEx((Rectangle){j, i, SQUARE_SIZE, SQUARE_SIZE}, 1.0f, GRAY);
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