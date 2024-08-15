/* Created by angel on 29/03/24 */

#include "Pong.h"

#define BALL_SIZE 10

static Arena* memory_pool = NULL;
static Player* P1 = NULL;
static Player* AI = NULL;
static Ball* B = NULL;

static unsigned char points_p1 = 0;
static unsigned char points_ai = 0;

static void InitGame ( void );
static void UpdateGame ( void );
static void DrawGame ( void );
static void CloseGame ( void );

static void ReallocBall ( void );
static void MoveBall ( void );
static void BallCollision ( void );

static void MovePlayer ( void );
static void MoveAI ( void );

static void PlayerBallCollision ( Player *p );

void MainPong ( void ) {
    InitGame();
    while ( !WindowShouldClose() ) {
        DrawGame();
        UpdateGame();
    }
    CloseGame();
}

static void InitGame ( void ) {

    SetTargetFPS(60);
    SetWindowTitle("Pong");
    memory_pool = CreateArena( 2*sizeof(Player) + sizeof(Ball) );

    P1 = ArenaAlloc(memory_pool, sizeof(Player));
    AI = ArenaAlloc(memory_pool, sizeof(Player));
    B = ArenaAlloc(memory_pool, sizeof(Ball));

    *P1 = (Player) {
            (Rectangle) {
                    20,
                    (float)GetScreenHeight()/2 - 40,
                    10,
                    80
            },
            RAYWHITE
    };

    *AI = (Player) {
            (Rectangle) {
                    (float)GetScreenWidth() - 20,
                    (float)GetScreenHeight()/2 - 40,
                    10,
                    80
            },
            RAYWHITE
    };

    *B = (Ball) {
            (Vector2) { (float) GetScreenWidth()/2 , (float) GetScreenHeight()/2  },
            (Vector2) { GetRandomValue(0, 1) == 0 ? -10 : 10, 0 },
            RAYWHITE
    };

}

static void UpdateGame ( void ) {
    MovePlayer();
    MoveAI();
    MoveBall();
    BallCollision();
}

static void DrawGame ( void ) {
    char txt1[4], txt2[4];
    sprintf(txt1, "%u", points_p1);
    sprintf(txt2, "%u", points_ai);

    BeginDrawing();

    ClearBackground(BLACK);

    DrawText(txt1, MeasureText(txt1, 30), 40, 30, GRAY);
    DrawText(txt2, GetScreenWidth() - 2*MeasureText(txt2, 30), 40, 30, GRAY);
    DrawLine(GetScreenWidth()/2, 0, GetScreenWidth()/2, GetScreenHeight(), GRAY);
    DrawCircle((int)B->pos.x, (int)B->pos.y, BALL_SIZE, B->c);
    DrawRectangleRec(P1->box, P1->c);
    DrawRectangleRec(AI->box, AI->c);

    EndDrawing();
}

static void CloseGame ( void ) {
    DestroyArena(memory_pool);
    memory_pool = NULL;
    P1 = NULL;
    AI = NULL;
    B = NULL;
    points_ai = 0;
    points_p1 = 0;
}

static void ReallocBall ( void ) {
    *B = (Ball) {
            (Vector2) { (float) GetScreenWidth()/2 , (float) GetScreenHeight()/2  },
            (Vector2) { GetRandomValue(0, 1) == 0 ? -10 : 10, 0 },
            RAYWHITE
    };
}

static void MoveBall ( void ) {
    B->pos.x += B->speed.x;
    B->pos.y += B->speed.y;
}

/*
 * The physics in pong game are simple:
 *
 * - if a player hits the ball at the top corner (or near it)
 * then it should bounce off towards our top border
 *
 * - if a player hits the ball at the bottom corner (or near it)
 * then it should bounce off towards our bottom border
 *
 * - if a player hits the ball at the center
 * then it should bounce off towards the right and not up or
 * down at all.
 *
*/

static void BallCollision ( void ) {

    Rectangle top, bottom;
    top = (Rectangle) { 0, 1, (float)GetScreenWidth(), 1 };
    bottom = (Rectangle) { 0, (float)GetScreenHeight(), (float)GetScreenWidth(), 1 };

    B->speed =  CheckCollisionCircleRec(B->pos, BALL_SIZE, top) || CheckCollisionCircleRec(B->pos, BALL_SIZE, bottom) ? Vector2Rotate(B->speed, -PI/4) : B->speed;

    PlayerBallCollision(P1);
    PlayerBallCollision(AI);

    bool point_p1, point_ai;
    point_p1 = B->pos.x > (float) GetScreenWidth() ? true : false;
    point_ai = B->pos.x < 0 ? true : false;
    if ( point_ai || point_p1 ) {
        points_ai += points_ai < UINT8_MAX ? point_ai : 0;
        points_p1 += points_p1 < UINT8_MAX ? point_p1 : 0;
        ReallocBall();
    }

}

static void PlayerBallCollision ( Player* p ) {

    unsigned char top_bottom, mid_bottom;
    top_bottom = 20;
    mid_bottom = 60;

    Rectangle top, mid, bottom;

    top = (Rectangle) { p->box.x, p->box.y, 10, 20 };
    mid = (Rectangle) { p->box.x, p->box.y + (float)top_bottom, 10, 40 };
    bottom = (Rectangle) { p->box.x, p->box.y + (float)mid_bottom, 10, 20 };

    if ( CheckCollisionCircleRec( B->pos, BALL_SIZE, top ) ) /* Ball goes up */
        B->speed = (Vector2) { -B->speed.x, -2 };
    else if ( CheckCollisionCircleRec( B->pos, BALL_SIZE, mid ) ) /* Ball goes straight */
        B->speed.x = -B->speed.x;
    else if (CheckCollisionCircleRec( B->pos, BALL_SIZE, bottom ) ) /* Ball goes down */
        B->speed = (Vector2) { -B->speed.x, +2 };

}

static void MovePlayer ( void ) {
    if ( IsKeyDown(KEY_DOWN) && P1->box.y + 80 < (float)GetScreenHeight() )
        P1->box.y += 10;
    else if ( IsKeyDown(KEY_UP) && P1->box.y > 0 )
        P1->box.y -= 10;
}

static void MoveAI ( void ) { AI->box.y = 0.9f * B->pos.y - 0.9f * 40; }