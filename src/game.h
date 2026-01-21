#ifndef GAME_H
#define GAME_H

typedef struct Game Game;

#include "assets.h"
#include "entity_pool.h"
#include "input.h"

struct Game {
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_FPoint camera_pos;

    InState *inState;
    AssetManager *asset_manager;
    EntityPool *pool;
};

void GAME_Init(Game *game, SDL_Renderer *renderer, SDL_Window *window, AssetManager *asset_manager, EntityPool *pool, InState *inState);
void GAME_Run(Game *game);

#endif