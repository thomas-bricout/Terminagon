#include "assets.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <assert.h>
#include "tilemap.h"

void ASSETS_Init(AssetManager *assetManager, SDL_Renderer *renderer) {
    SDL_Texture *debugTexture = IMG_LoadTexture(renderer, "assets/debug.png");
    for ( int i = 0; i < ASSET_ARRAY_LENGTH; i++ ) {
        assetManager->asset_array[i] = debugTexture;
    }
    assetManager->debug_font = NULL;
}

void ASSETS_Load(AssetManager *assetManager, SDL_Renderer *renderer) {
    assetManager->debug_font = TTF_OpenFont("assets/WebPlusIBMMDA/web_ibm_mda.ttf", 16);

    char *filenames[] = {
        "assets/debug.png", 
        "assets/ArrowRight.png", 
        "assets/ArrowUp.png", 
        "assets/ArrowLeft.png", 
        "assets/ArrowDown.png", 
        "assets/link0.bmp", 
        "assets/link1.bmp", 
        "assets/link2.bmp", 
        "assets/link3.bmp", 
        "assets/link4.bmp", 
        "assets/link5.bmp", 
        "assets/link6.bmp", 
        "assets/link7.bmp", 
        "assets/linkSwordRight.bmp", 
        "assets/linkSwordUp.bmp", 
        "assets/linkSwordLeft.bmp", 
        "assets/linkSwordDown.bmp",
        "assets/BowAimingRight.png",
        "assets/BowAimingUp.png",
        "assets/BowAimingLeft.png",
        "assets/BowAimingDown.png",
        "assets/enemy0.bmp",
        "assets/enemy1.bmp",
        "assets/enemy2.bmp",
        "assets/enemy3.bmp",
        "assets/enemy4.bmp",
        "assets/enemy5.bmp",
        "assets/enemy6.bmp",
        "assets/enemy7.bmp",
        "assets/octorokProjectile.png",
        "assets/emptyHeart.png",
        "assets/halfHeart.png",
        "assets/fullHeart.png",
        "assets/Death1.png",
        "assets/Death2.png",
        "assets/Death3.png",
        "assets/Death4.png",
    };
    int filenames_length = 37;

    for (int i = 0; i < filenames_length; i++) {
        assetManager->asset_array[i] = IMG_LoadTexture(renderer, filenames[i]);
        printf("Loading %s\n", filenames[i]);
    }

    if ( assetManager->debug_font == NULL ) {
        fprintf(stderr, "Erreur TTF_OpenFont: %s\n", SDL_GetError());
    }

    assetManager->map_assets = load_MAP_Textures("assets/overworldtiles.bmp", renderer);
}

void ASSETS_Destroy(AssetManager *assetManager) {
    for ( int i = 0; i < ASSET_ARRAY_LENGTH; i++) {
        SDL_DestroyTexture(assetManager->asset_array[i]);
    }
    TTF_CloseFont(assetManager->debug_font);
}

void ASSETS_AccessTexture(SDL_Texture **texture, AssetManager *assetManager, TextureLocation textureLocation) {
    assert(textureLocation < ASSET_ARRAY_LENGTH);
    *texture = assetManager->asset_array[textureLocation];
}