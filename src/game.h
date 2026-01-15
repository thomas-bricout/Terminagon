#ifndef GAME_H
#define GAME_H

#include "assets.h"

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    AssetManager *asset_manager;
} Game;

void GAME_init(Game *game, SDL_Renderer *renderer, SDL_Window *window, AssetManager *asset_manager);
void GAME_run(Game *game);

#endif