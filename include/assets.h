#ifndef ASSETS_H
#define ASSETS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define ASSET_ARRAY_LENGTH 100

typedef enum {
    TEX_DEBUG,
    TEX_ARROW,
    TEX_PLAYER_RIGHT,
    TEX_PLAYER_RIGHT_W,
    TEX_PLAYER_UP,
    TEX_PLAYER_UP_W,
    TEX_PLAYER_LEFT,
    TEX_PLAYER_LEFT_W,
    TEX_PLAYER_DOWN,
    TEX_PLAYER_DOWN_W,
    TEX_PLAYER_SWORD_RIGHT,
    TEX_PLAYER_SWORD_UP,
    TEX_PLAYER_SWORD_LEFT,
    TEX_PLAYER_SWORD_DOWN,
} TextureLocation ;

typedef struct {
    TTF_Font *debug_font;
    SDL_Texture *asset_array[ASSET_ARRAY_LENGTH];
    SDL_Texture **map_assets;
} AssetManager;

void ASSETS_Init(AssetManager *assetManager, SDL_Renderer *renderer);
void ASSETS_Load(AssetManager *assetManager, SDL_Renderer *renderer);
void ASSETS_Destroy(AssetManager *assetManager);
void ASSETS_AccessTexture(SDL_Texture **texture, AssetManager *assetManager, TextureLocation textureLocation);

#endif