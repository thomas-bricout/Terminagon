#include "assets.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <assert.h>

void ASSETS_Init(AssetManager *assetManager, SDL_Renderer *renderer) {
    SDL_Texture *debugTexture = IMG_LoadTexture(renderer, "assets/debug.png");
    for ( int i = 0; i < ASSET_ARRAY_LENGTH; i++ ) {
        assetManager->asset_array[i] = debugTexture;
    }
    assetManager->asset_table = debugTexture;
    assetManager->debug_font = NULL;
}

void ASSETS_Load(AssetManager *assetManager, SDL_Renderer *renderer) {
    assetManager->asset_table = IMG_LoadTexture(renderer, "assets/assets.png");
    assetManager->debug_font = TTF_OpenFont("assets/WebPlusIBMMDA/web_ibm_mda.ttf", 16);
    if ( assetManager->debug_font == NULL ) {
        fprintf(stderr, "Erreur TTF_OpenFont: %s\n", SDL_GetError());
    }
}

void ASSETS_Destroy(AssetManager *assetManager) {
    for ( int i = 0; i < ASSET_ARRAY_LENGTH; i++) {
        SDL_DestroyTexture(assetManager->asset_array[i]);
    }
    SDL_DestroyTexture(assetManager->asset_table);
    TTF_CloseFont(assetManager->debug_font);
}

void ASSETS_AccessTexture(SDL_Texture **texture, AssetManager *assetManager, TextureLocation textureLocation) {
    assert(textureLocation < ASSET_ARRAY_LENGTH);
    *texture = assetManager->asset_array[textureLocation];
}