#ifndef GAME_H
#define GAME_H

#include "assets.h"
#include "entity_pool.h"

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;

    AssetManager *asset_manager;
    EntityPool *pool;
} Game;

void GAME_Init(Game *game, SDL_Renderer *renderer, SDL_Window *window, AssetManager *asset_manager, EntityPool *pool);
void GAME_Run(Game *game);

#endif