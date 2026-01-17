#ifndef ASSETS_H
#define ASSETS_H

#include <SDL2/SDL.h>

#define ASSET_ARRAY_LENGTH 100

typedef enum {
    debug = 0
} TextureLocation ;

typedef struct {
    SDL_Texture *asset_table;
    SDL_Texture *asset_array[ASSET_ARRAY_LENGTH];
} AssetManager;

void ASSETS_Init(AssetManager *assetManager, SDL_Renderer *renderer);
void ASSETS_Load(AssetManager *assetManager, SDL_Renderer *renderer);
void ASSETS_Destroy(AssetManager *assetManager);
SDL_Texture *ASSETS_AccessTexture(AssetManager *assetManager, TextureLocation textureLocation);

#endif