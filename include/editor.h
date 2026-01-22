#ifndef EDITOR_H
#define EDITOR_H

#include "game.h"
#include "assets.h"

typedef enum {
    ARCHETYPE_DEBUG,
    ARCHETYPE_PLAYER,
} EntityArchetype;

void EDITOR_PlaceEntity(Game *game);
void EDITOR_EntityFromArchetype(Game *game, EntityArchetype archetype, SDL_FPoint pos);

#endif