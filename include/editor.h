#ifndef EDITOR_H
#define EDITOR_H

#define ARCHETYPE_COUNT 5
typedef enum EntityArchetype EntityArchetype;
enum EntityArchetype {
    ARCHETYPE_DEBUG,
    ARCHETYPE_PLAYER,
    ARCHETYPE_OCTOROK,
    ARCHETYPE_MOBLIN,
    ARCHETYPE_LEEVER,
};

#include "game.h"
#include "assets.h"
#include "entity_pool.h"

#define GRID_SIZE 100
#define HALF_GRIDSIZE 50

#define GRID_RECT (SDL_Rect) {-HALF_GRIDSIZE, -HALF_GRIDSIZE, GRID_SIZE, GRID_SIZE}
#define GRID_FRECT (SDL_FRect) {-HALF_GRIDSIZE, -HALF_GRIDSIZE, GRID_SIZE, GRID_SIZE}

void EDITOR_PlaceEntity(Game *game, double current_time);
void EDITOR_EntityFromArchetype(Game *game, EntityArchetype archetype, SDL_FPoint pos, double current_time);

void EDITOR_DisplayInfo(Game *game);
void EDITOR_DrawGrid(Game *game);
char* EDITOR_ArchetypeToString(EntityArchetype archetype);

bool EDITOR_EntityAtPoint(EntityPool *pool, SDL_Point point, EntityID *id);
void EDITOR_DeleteUnderMouse(Game *game);

#endif