#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "entity_pool.h"
#include "player.h"
#include "geometry.h"
#include "game.h"
#include "serialize.h"

void POOL_AddComponentFlags(EntityPool *pool, ComponentFlags component_flags, int location) {
    pool->component_flags[location] = pool->component_flags[location] | component_flags;
}

bool POOL_LacksComponentFlags(EntityPool *pool, ComponentFlags component_flags, int location) {
    return (pool->component_flags[location] & component_flags) != component_flags;
}

void POOL_RemoveComponentFlags(EntityPool *pool, ComponentFlags component_flags, int location) {
    pool->component_flags[location] = (pool->component_flags[location] & ~(component_flags));
}

void POOL_Init(EntityPool *pool) {
    pool->currentCount = 0;
    pool->lastEntitylocation = 0;
    pool->emptyLocationsAmount = 0;
    pool->player_amount = 0;

    EntityID default_id = {0, 0};

    for (int i = 0; i < MAX_ENTITY_COUNT; i++) {
        pool->id[i] = default_id;
        pool->tex[i] = TEX_DEBUG;
        pool->component_flags[i] = COMPONENT_NONE;
    }
}

void POOL_Load(EntityPool *pool, InState *inState) {
    PLAYER_Spawn(pool, inState, (SDL_FPoint) {100*116., 100*82.});
    PLAYER_Spawn(pool, inState + 1, (SDL_FPoint) {100*120., 100*82.});

    // Adds a few enemies for trying them out
    // ENEMY_SpawnOctorok(pool, (SDL_FPoint) {100*117., 100*84.});
    // ENEMY_SpawnMoblin(pool, (SDL_FPoint) {100*118., 100*85.});
    ENEMY_SpawnLeever(pool, (SDL_FPoint) {100*118., 100*85.}, 0., 0);
}

EntityID POOL_NewEntity(EntityPool *pool) {
    static int currentID = 1;
    EntityID new_id;

    if (pool->emptyLocationsAmount == 0) {  // Append to the end of the list
        new_id.location = pool->lastEntitylocation;
        pool->lastEntitylocation ++;
        if ( pool->lastEntitylocation == MAX_ENTITY_COUNT) { abort(); }
    } else {                                // Fill one of the empty spots
        new_id.location = pool->emptyLocations[pool->emptyLocationsAmount - 1];
        pool->emptyLocationsAmount --;
    }

    new_id.unique_id = currentID;
    currentID ++;
    
    pool->currentCount ++;

    pool->id[new_id.location] = new_id;

    SDL_Log("Creating new Entity with location %d and unique_id %d", new_id.location, new_id.unique_id);
    return new_id;
}

EntityID POOL_NewEntityClassic(EntityPool *pool, TextureIndex tex, SDL_Rect display_rect, SDL_FPoint position) {
    // Create new entity with new a new unique id and a designated position in the pool
    EntityID new_id = POOL_NewEntity(pool);

    pool->tex[new_id.location] = tex;
    pool->display_rect[new_id.location] = display_rect;
    pool->position[new_id.location] = position;

    POOL_AddComponentFlags(pool, COMPONENT_TEXTURE | COMPONENT_DISPLAYRECT | COMPONENT_POSITION, new_id.location);

    return new_id;
}

void POOL_DestroyEntity(EntityPool *pool, EntityID id) {
    // Check if entity has not yet been destroyed
    if (pool->id[id.location].unique_id != id.unique_id) {
        return;
    }
    POOL_DestroyEntityFromIndex(pool, id.location);
}

void POOL_DestroyEntityFromIndex(EntityPool *pool, int i) {
    // Inform everywhere the entity is destroyed and a new spot is available
    if (i == pool->lastEntitylocation) {
        pool->lastEntitylocation --;
    } else {
        pool->emptyLocationsAmount ++;
        pool->emptyLocations[pool->emptyLocationsAmount - 1] = i;
    }
    pool->currentCount --;
    pool->id[i] = (EntityID) {0, 0};

    // Reset all bitmaps
    pool->component_flags[i] = COMPONENT_NONE;

    SDL_Log("Deleting entity at index %d\n", i);
}