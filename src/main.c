#include "Pong/Pong.h"
#include "Snake/snake .h"
#include "Conway/Conway.h"

Rectangle *Buttons = NULL;
int n = 3;

const char *string_1 = "Snake";
const char *string_2 = "Pong";
const char *string_3 = "Game of Life";  

static void InitMainGame(void);
static void DrawMainGame(void);
static void ChooseGame(void);
static void CloseMainGame(void);

int main(void) {
    InitMainGame();
    while (!WindowShouldClose()) {
        ChooseGame();
        DrawMainGame();
    }
    CloseMainGame();
    return 0;
}

static void InitMainGame(void) {
    int x = 1280;
    int y = 720;
    InitWindow(x, y, "Arcade Console");
    SetTargetFPS(60);

    Buttons = (Rectangle *) malloc(n * sizeof(Rectangle));
    for (int i = 0; i < n; i++)
        Buttons[i] = (Rectangle) {
        (float)GetScreenWidth()/2 - 100,
        (float)(150 * (i + 1)),
        200,
        50
    };
}

static void ChooseGame(void) {
    Vector2 Mouse = (Vector2) GetMousePosition();
    if (CheckCollisionPointRec(Mouse, Buttons[0]) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) MainSnake();
    else if (CheckCollisionPointRec(Mouse, Buttons[1]) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) MainPong();
    else if (CheckCollisionPointRec(Mouse, Buttons[2]) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) MainConway();
}

static void DrawMainGame(void) {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawRectangleRec(Buttons[0], GREEN);
    DrawRectangleRec(Buttons[1], RED);
    DrawRectangleRec(Buttons[2], YELLOW);

    DrawText(string_1, Buttons[0].x, Buttons[0].y, 20, WHITE);
    DrawText(string_2, Buttons[1].x, Buttons[1].y, 20, WHITE);
    DrawText(string_3, Buttons[2].x, Buttons[2].y, 20, WHITE);

    EndDrawing();
}

static void CloseMainGame(void) { /* Unload all the things that are used in the game */
    for (int i = 0; i < n; i++)
        Buttons[i] = (Rectangle){};
    free((void *)Buttons);
    Buttons = NULL;
    CloseWindow();
}