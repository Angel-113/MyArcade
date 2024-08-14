// Created by angel on 5/08/24.

#include "Arena.h"

Arena* CreateArena ( size_t size ) {
    Arena* A = (Arena*) malloc(sizeof(Arena));
    if ( A != NULL ) {
        A->arena = (char*) malloc(size);
        if ( A->arena == NULL ) {
            perror("Error when creating memory pool in Arena pointer ( A->arena == NULL ) \n");
            return NULL;
        }
        A->size = size;
        A->offset = 0;
    }
    return A;
}

void* ArenaAlloc ( Arena* A, size_t size ) {
    if ( A->offset + size > A->size ) {
        perror("This arena is not big enough\n");
        return NULL;
    }

    void* ptr = A->arena + A->offset;
    A->offset += size;
    return ptr;
}

void DestroyArena ( Arena* A ) {
    free( (void*)A->arena );
    free( (void*)A );
}