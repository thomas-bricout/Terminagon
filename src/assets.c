#include "assets.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void ASSETS_Load(AssetManager *assetManager, SDL_Renderer *renderer) {
    assetManager->asset_table = IMG_LoadTexture(renderer, "assets/assets.png");
}

void ASSETS_Destroy(AssetManager *assetManager) {
    SDL_DestroyTexture(assetManager->asset_table);
}