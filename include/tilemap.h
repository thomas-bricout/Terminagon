#ifndef TILEMAP_H
#define TILEMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define HAUTEUR 88
#define LARGEUR 256


#define TILE_SIZE_SRC 16
#define TILES_WIDTH 20
#define TILES_HEIGHT 8
#define TILE_SIZE 100


typedef struct tile Tile;

struct tile{
    Uint8 id;
    Uint8 blocking;
};

SDL_Texture** load_MAP_Textures(const char* tilefilename, SDL_Renderer *ren);
void loadMap(Tile tilemap[HAUTEUR][LARGEUR]);
#endif