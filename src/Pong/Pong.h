// Created by angel on 29/03/24.

#ifndef ARCADECONSOLE_PONG_H
#define ARCADECONSOLE_PONG_H

#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
#include <string.h>

typedef struct Ball {
    Rectangle box;
    Vector2 speed;
    Color c;
} Ball;

typedef struct Player {
    Rectangle box;
    Color c;
} Player;

extern void MainPong(void);
extern void Hit(Ball *b, Player *p);

extern Ball *InitBall(void);
extern void ChangeDirection(Ball *b);
extern void MoveBall(Ball* b);
extern void DeleteBall(Ball *b);

extern Player *InitPlayer(Vector2 pos);
extern void MovePlayer(Player *p);
extern void DeletePlayer(Player *p);

#endif //ARCADECONSOLE_PONG_H
