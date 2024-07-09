/* Created by angel on 29/03/24 */

#include "Pong.h"

static Player *AI_Player = NULL; /* Play Against AI */
static Player *P1 = NULL; /* Main player */
static Ball *ball = NULL; /* ball */
static Vector2 InitialVelocity = { 0 };

/* Game Definitions */

static void InitGame(void); /* Initializes game */
static void UpdateGame(void); /* Update game variables */
static void DrawGame(void); /* Draw game variables state (one frame) */
static void CloseGame(void); /* Free game variables and closes games */
static void AllToInitialState (void);

/* --------------------- */

/* Player Definitions */

static Player *InitPlayer(Vector2 pos); /* Malloc player instance with position pos */
static void DeletePlayer(Player *p); /* Free's player instance */
static void MovePlayer(Player *p); /* Move player->box */
static void DrawPlayer(Player *p); /* Draw player->box with color player->c */

/* ----------------------- */

/* Ball Definitions */

static Ball *InitBall(void);
static void MoveBall(void);
static void DrawBall(void);
static void ChangeDirection(void);
static void ReturnToCenter(void);
static void DeleteBall(void);

void MainPong(void) { /* Main Function for Pong */
    InitGame(); 
    while (!WindowShouldClose()) {
        UpdateGame();
        DrawGame();
    }
    CloseGame(); 
}


/* ----------------- */

void InitGame(void) {
    SetWindowTitle("Pong");
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
    ClearBackground(GRAY);
    DrawBall();
    DrawPlayer(P1);
    DrawPlayer(AI_Player);
    EndDrawing();
}

void CloseGame(void) {
    DeletePlayer(P1);
    DeletePlayer(AI_Player);
    DeleteBall();
}

void AllToInitialState (void) {
    P1->box.x = (float)GetScreenWidth()/2;
    AI_Player->box.x = (float)GetScreenWidth()/2;
    ReturnToCenter();
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
    if (GetRandomValue(0, 1)) b->speed = (Vector2){0, +10};
    else b->speed = (Vector2){0, -10};

    InitialVelocity = b->speed;
    return b;
}

void ChangeDirection(void) {

    Vector2 current_speed = (Vector2)ball->speed;

    /* When ball pos is at screen borders */
    if ( ball->box.x + 10 == (float)GetScreenWidth() ) ball->speed = Vector2Rotate(current_speed, -PI);
    else if ( ball->box.x == 0 ) ball->speed = Vector2Rotate(ball->speed, -PI);

    /* When ball collisions with any player */
    if ( CheckCollisionRecs(ball->box, P1->box) || CheckCollisionRecs(ball->box, AI_Player->box) ) {
        ball->speed = Vector2Rotate( ball->speed, - PI );
    }

    /* When ball goes beyond 0 or ScreenHeight */
    if ( ball->box.y < 0 || ball->box.y > (float)GetScreenHeight() ) {
        WaitTime(2);
        AllToInitialState();
    }

}

void DrawBall(void) { if (ball != (Ball *)NULL) DrawRectangleRec(ball->box, ball->c); }

void MoveBall(void) {
    ChangeDirection();
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

void ReturnToCenter (void) {
    ball->box.x = (float)(GetScreenWidth() + 10) / 2;
    ball->box.y = (float)(GetScreenHeight() + 10)/2;
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
        if ( IsKeyDown(KEY_RIGHT) && p->box.x + 50 < (float) GetScreenWidth() ) p->box.x += 20;
    }
}

void DrawPlayer(Player *p) { if (p != NULL) DrawRectangleRec(p->box, p->c); }

void DeletePlayer(Player *p) {
    if (p != NULL) {
        p->c = (Color){};
        p->box = (Rectangle){};
        free((void *)p);
    }
}
/* -------------------------- */