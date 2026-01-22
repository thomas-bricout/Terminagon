#ifndef EDITOR_H
#define EDITOR_H

typedef enum EntityArchetype EntityArchetype;
enum EntityArchetype {
    ARCHETYPE_DEBUG,
    ARCHETYPE_PLAYER,
};

#include "game.h"
#include "assets.h"

#define ARCHETYPE_NUMBER 2
#define GRID_SIZE 100

void EDITOR_PlaceEntity(Game *game);
void EDITOR_EntityFromArchetype(Game *game, EntityArchetype archetype, SDL_FPoint pos);

void EDITOR_DisplayInfo(Game *game);
void EDITOR_DrawGrid(Game *game);
char* EDITOR_ArchetypeToString(EntityArchetype archetype);

#endif