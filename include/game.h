#ifndef GAME_H
#define GAME_H

typedef struct Game Game;

#include "assets.h"
#include "entity_pool.h"
#include "input.h"
#include "tilemap.h"

#define SCREEN_W 1280
#define SCREEN_H 720


struct Game {
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_FPoint camera_pos;

    InState *inState;
    AssetManager *asset_manager;
    EntityPool *pool;

    SDL_GameController *controller0;
    SDL_GameController *controller1;
    Tile map[HAUTEUR][LARGEUR];
};

void GAME_Init(Game *game, SDL_Renderer *renderer, SDL_Window *window, AssetManager *asset_manager, EntityPool *pool, InState *inState,SDL_GameController *controller0,SDL_GameController *controller1);
void GAME_Run(Game *game);
void MoveCameraIJKL(Game *game, float deltaTime);

#endif