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
#define HALF_GRIDSIZE 50

#define GRID_RECT (SDL_Rect) {-HALF_GRIDSIZE, -HALF_GRIDSIZE, GRID_SIZE, GRID_SIZE}
#define GRID_FRECT (SDL_FRect) {-HALF_GRIDSIZE, -HALF_GRIDSIZE, GRID_SIZE, GRID_SIZE}

void EDITOR_PlaceEntity(Game *game);
void EDITOR_EntityFromArchetype(Game *game, EntityArchetype archetype, SDL_FPoint pos);

void EDITOR_DisplayInfo(Game *game);
void EDITOR_DrawGrid(Game *game);
char* EDITOR_ArchetypeToString(EntityArchetype archetype);

#endif