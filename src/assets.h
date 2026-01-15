#ifndef ASSETS_H
#define ASSETS_H

#include <SDL2/SDL.h>

typedef struct {
    SDL_Texture *asset_table;
} AssetManager;

void ASSETS_Load(AssetManager *assetManager, SDL_Renderer *renderer);
void ASSETS_Destroy(AssetManager *assetManager);

#endif