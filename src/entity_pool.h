#ifndef ENTITY_POOL.H
#define ENTITY_POOL.H

#include <SDL2/SDL.h>
#include "assets.h"

#define MAX_ENTITY_COUNT 500

typedef struct {
    int location;
    int id;
} EntityID;

typedef struct {
    int currentCount;

    EntityID id[MAX_ENTITY_COUNT];
    TextureLocation texture[MAX_ENTITY_COUNT];
    SDL_Rect rectangle[MAX_ENTITY_COUNT];
} EntityPool ;

void POOL_Init(EntityPool *pool);

#endif