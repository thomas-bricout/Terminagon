#ifndef TILEMAP_H
#define TILEMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define HAUTEUR 100
#define LARGEUR 256


typedef struct tile Tile;

struct tile{
    Uint8 type:7;
    Uint8 blocking:1;
};

void loadMap(Tile tilemap[HAUTEUR][LARGEUR]);
#endif