//
// Created by angel on 8/05/24.
//

#ifndef ARCADECONSOLE_CONWAY_H
#define ARCADECONSOLE_CONWAY_H

#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Cell {
    Rectangle box;
    Color c;
    bool state;
} Cell;

extern void MainConway(void); /* Main function for conway's game of life */

#endif //ARCADECONSOLE_CONWAY_H
