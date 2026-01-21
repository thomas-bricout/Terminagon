#ifndef EDITOR_H
#define EDITOR_H

#include "game.h"
#include "assets.h"

void EDITOR_PlaceEntity(Game *game);
void EDITOR_EntityFromBundle(Game *game, EntityBundle bundle, SDL_FPoint pos);

typedef struct {
    TextureLocation texture;
    SDL_Rect texture_box;
    SDL_FRect collision_box;
} EntityBundle;

#endif