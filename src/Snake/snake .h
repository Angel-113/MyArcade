//
// Created by angel on 26/03/24.
//

#ifndef ARCADECONSOLE_SNAKE_H
#define ARCADECONSOLE_SNAKE_H

#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_SIZE 100

typedef struct Apple {
    Rectangle box;
    Color c;
} Apple;

typedef struct NodeSnake {
    Rectangle box;
    Color c;
    struct NodeSnake *next;
} NodeSnake;

typedef struct Snake {
    NodeSnake *head;
    Vector2 speed;
    unsigned int size;
} Snake;

extern void MainSnake(void);

extern NodeSnake *InitNode(float x, float y, float width, float height); /* Initializes a node for snake */
extern void DeleteNode(NodeSnake *n); /* Delete a node for snake */

extern Snake *InitSnake(void); /* Initializes Snake */
extern void Eat(Snake *s); /* If Snake's head pos == apple pos, then grow */
extern void SnakeDeath(Snake *s); /* If Snake's head hit Snake's body or Wall then Snake die */
extern void Move(Snake *S); /* Moves snake head */

extern Apple *InitApple(void); /* Initializes object Apple */
extern void ReposApple(Apple *A); /* Changes the position of Apple */
extern void DeleteApple(Apple *A); /* De-allocates Object Apple */

#endif //ARCADECONSOLE_SNAKE_H
