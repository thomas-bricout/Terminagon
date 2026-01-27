#ifndef TILEMAP_H
#define TILEMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define HAUTEUR 100
#define LARGEUR 256


typedef struct tile Tile;

struct tile{
    Uint8 type_c;
    Uint8 type_l;
    Uint8 blocking;
};

SDL_Texture** load_MAP_Textures(const char* tilefilename, SDL_Renderer *ren);
void loadMap(Tile tilemap[HAUTEUR][LARGEUR]);
#endif