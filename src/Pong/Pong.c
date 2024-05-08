/* Created by angel on 29/03/24 */

#include "Pong.h"

static Player *AI_Player = NULL; /* Play Against AI */
static Player *P1 = NULL; /* Main player */
static Ball *ball = NULL; /* ball */

/* Game Definitions */

static void InitGame(void); /* Initializes game */
static void UpdateGame(void); /* Update game variables */
static void DrawGame(void); /* Draw game variables state (one frame) */
static void CloseGame(void); /* Free game variables and closes games */

/* --------------------- */

/* Player Definitions */

static Player *InitPlayer(Vector2 pos); /* Malloc player instance with position pos */
static void DeletePlayer(Player *p); /* Free's player instance */
static void MovePlayer(Player *p); /* Move player->box */
static void DrawPlayer(Player *p); /* Draw player->box with color player->c */

/* ----------------------- */

/* Ball Definitions */

static Ball *InitBall(void);
static void Hit(Player *p);
static void MoveBall(void);
static void DrawBall(void);
static void ChangeDirection();
static void DeleteBall(void);

/* ----------------- */

void InitGame(void) {
    InitWindow(1200, 720, "Pong");
    SetTargetFPS(60);
    P1 = InitPlayer((Vector2){ (float)GetScreenWidth()/2, (float)GetScreenHeight() - 30 });
    AI_Player = InitPlayer((Vector2){(float)GetScreenWidth()/2, 30});
    ball = InitBall();
}

void UpdateGame(void) {
    MoveBall();
    MovePlayer(P1);
}

void DrawGame(void) {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawBall();
    DrawPlayer(P1);
    DrawPlayer(AI_Player);
    EndDrawing();
}

void CloseGame(void) {
    DeletePlayer(P1);
    DeletePlayer(AI_Player);
    DeleteBall();
    CloseWindow();
}

void MainPong(void) { /* Main Function for Pong */
    InitGame(); 
    while (!WindowShouldClose()) {
        UpdateGame();
        DrawGame();
    }
    CloseGame(); 
}

/* Ball Implementations */

Ball *InitBall(void) {
    Ball *b = (Ball *)malloc(sizeof(Ball));
    b->box = (Rectangle) {
            (float)GetScreenWidth()/2,
            (float)GetScreenHeight()/2,
            10,
            10
    };
    b->c = RAYWHITE;
    b->speed = (Vector2){0, 0};
    return b;
}

void ChangeDirection(void) {
    ball->speed.x = 20 * cosf(PI/4);
    ball->speed.y = 20 * sinf(PI/4);
}

void Hit(Player *p) {
    if (CheckCollisionRecs(p->box, ball->box))
        ChangeDirection();
}

void DrawBall(void) {
    if (ball != (Ball *)NULL) DrawRectangleRec(ball->box, ball->c);
}

void MoveBall(void) {
    ball->box.x += ball->speed.x;
    ball->box.y += ball->speed.y;
}

void DeleteBall(void) {
    if (ball != NULL) {
        ball->c = (Color){};
        ball->box = (Rectangle){};
        ball->speed = (Vector2){};
        free((void *)ball);
        ball = NULL;
    }
}

/* --------------------------- */

/* Player Implementations */
Player *InitPlayer(Vector2 pos) {
    Player *p = (Player *) malloc(sizeof(Player));
    p->box = (Rectangle){pos.x, pos.y, 50, 10};
    p->c = (Color)RAYWHITE;
    return p;
}

void MovePlayer(Player *p) {
    if (p != NULL) {
        if ( IsKeyDown(KEY_LEFT) && p->box.x > 0 ) p->box.x -= 20;
        if ( IsKeyDown(KEY_RIGHT) && p->box.width - p->box.x < (float) GetScreenWidth() ) p->box.x += 20;
    }
}

void DrawPlayer(Player *p) {
    if (p != NULL) DrawRectangleRec(p->box, p->c);
}

void DeletePlayer(Player *p) {
    if (p != NULL) {
        p->c = (Color){};
        p->box = (Rectangle){};
        free((void *)p);
    }
}
/* -------------------------- */