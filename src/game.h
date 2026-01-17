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

void GAME_init(Game *game, SDL_Renderer *renderer, SDL_Window *window, AssetManager *asset_manager, EntityPool *pool);
void GAME_run(Game *game);

#endif