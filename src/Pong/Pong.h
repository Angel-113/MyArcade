// Created by angel on 29/03/24.

#ifndef ARCADECONSOLE_PONG_H
#define ARCADECONSOLE_PONG_H

#include "raylib.h"
#include "raymath.h"
#include "../Arena/Arena.h"
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

typedef struct Ball {
    Vector2 pos;
    Vector2 speed;
    Color c;
} Ball;

typedef struct Player {
    Rectangle box;
    Color c;
} Player;

extern void MainPong(void);

#endif //ARCADECONSOLE_PONG_H
