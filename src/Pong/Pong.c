/* Created by angel on 29/03/24 */

#include "Pong.h"

static Player *AI_Player = NULL; /* Play Against AI */
static Player *P1 = NULL; /* Main player */
static Player *P2 = NULL; /* Second player */
static Ball *ball = NULL; /* ball */

static void InitGame(void) { /* Initializes all the game */
    /* for the moment it's only against AI */
    InitWindow(1200, 800, "Pong");
    SetTargetFPS(45);
    AI_Player = InitPlayer((Vector2){(float)GetScreenWidth()/2 - 25, 30});
    P1 = InitPlayer((Vector2){(float)GetScreenWidth()/2 - 25, (float)GetScreenHeight() - 30});
    ball = InitBall();

}

static void CloseGame() { /* Free's memory used and Closes all game instances */
    DeletePlayer(P1);
    DeletePlayer(P2);
    DeletePlayer(AI_Player);
    DeleteBall(ball);
    CloseWindow();
}

static void UpdateGame(void) {

    if ( (ball->speed.x == 0 && ball->speed.y == 0) && IsKeyPressed(KEY_ENTER) ) {
        int random = GetRandomValue(0, 1);
        if (random == 0) random = -1;
        ball->speed = (Vector2) {0, (float)random*20};
    }

    MovePlayer(P1);
    MoveBall(ball);

}

static void DrawGame(void) {
    BeginDrawing();
    ClearBackground(BLACK);
    if (P1 != NULL) DrawRectangleRec(P1->box, P1->c);
    if (P2 != NULL) DrawRectangleRec(P2->box, P2->c);
    if (AI_Player != NULL) DrawRectangleRec(AI_Player->box, AI_Player->c);
    DrawRectangleRec(ball->box, ball->c);
    EndDrawing();
}

void MainPong(void) { /* Main function for the game */
    InitGame();
    while (!WindowShouldClose()) {
        UpdateGame();
        DrawGame();
    }
    CloseGame();
}

/* ------------ Player Implementations ------------ */

static float ControlPlayer(void) {
    if (IsKeyDown(KEY_RIGHT)) return +30.0f;
    else if (IsKeyDown(KEY_LEFT)) return -30.0f;
    return 0.0f;
}

Player *InitPlayer(Vector2 pos) {
    Player *p = (Player *) malloc(sizeof(Player));
    p->box = (Rectangle){pos.x, pos.y, 50, 10};
    p->c = RAYWHITE;
    return p;
}

void MovePlayer(Player *p) {
    float dir = ControlPlayer();
    if (p->box.x + dir >= 0 && p->box.x + dir <= (float)GetScreenWidth() - p->box.width)
        p->box.x += dir;
}

void DeletePlayer(Player *p) {
    if (p != NULL) {
        p->box = (Rectangle) {0, 0, 0, 0};
        p->c = (Color) {0, 0, 0, 0};
        free((void *) p);
        p = NULL;
    }
}

/* ------------------------------------------------ */

/* ------------ Ball Implementations ---------- */

Ball *InitBall(void) {
    Ball *b = (Ball *) malloc(sizeof(Ball));
    b->box = (Rectangle) {
            (float)GetScreenWidth()/2 - 10,
            (float)GetScreenHeight()/2 - 10,
            10,
            10
    };
    b->c = RAYWHITE;
    b->speed = (Vector2) {0, 0};
    return b;
}

static void NormalVector(Ball *b) {
    b->speed = (Vector2){b->speed.y, -b->speed.x};
}

static void RedirectVector(Ball *b) { /* Rotates by 45° degrees ball->speed vector */

}

void MoveBall(Ball *b) {
    /* Check ball-wall collisions */
    if ( b->box.x == 0 || b->box.x == (float)GetScreenWidth() )
        NormalVector(b);

    /* Check ball-players collisions */
    if (CheckCollisionRecs(b->box, P1->box))
        NormalVector(b);

    /* Update ball position */
    b->box.x += b->speed.x;
    b->box.y += b->speed.y;
}

void DeleteBall(Ball *b) {
    b->speed = (Vector2){0, 0};
    b->c = (Color){0, 0, 0, 0};
    b->box = (Rectangle){0, 0, 0, 0};
    free((void *)b);
    b = NULL;
}

/* -------------------------------------------- */