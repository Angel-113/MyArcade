//
// Created by angel on 5/08/24.
//

#ifndef ARCADECONSOLE_ARENA_H
#define ARCADECONSOLE_ARENA_H

#include <stdlib.h>
#include <stdio.h>

typedef struct Arena  {
    char* arena;
    size_t size;
    size_t offset;
} Arena ;

extern Arena* CreateArena ( size_t size );
extern void* ArenaAlloc ( Arena* A, size_t size );
extern void DestroyArena ( Arena* A );

#endif //ARCADECONSOLE_ARENA_H
