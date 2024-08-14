//
// Created by angel on 26/03/24.
//

#ifndef ARCADECONSOLE_SNAKE_H
#define ARCADECONSOLE_SNAKE_H

#include "raylib.h"
#include "../Arena/Arena.h"
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_SIZE 100
#define MAX_POINTS ULLONG_MAX

typedef struct Apple {
    Rectangle box;
    Color c;
} Apple;

typedef struct NodeSnake {
    Rectangle box;
    Color c;
    struct NodeSnake *next;
} NodeSnake;

extern void MainSnake ( void );

#endif //ARCADECONSOLE_SNAKE_H
