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

    EntityID default_id = {0, 0};

    for (int i = 0; i < MAX_ENTITY_COUNT; i++) {
        pool->id[i] = default_id;
        pool->tex_location[i] = TEX_DEBUG;
        pool->component_flags[i] = COMPONENT_NONE;
    }
}

void POOL_Load(EntityPool *pool, InState *inState) {
    // Adds the main player
    pool->player_component[0] = PLAYER_NewComponent(inState);
    pool->player_amount = 1;

    SDL_Rect player_display_rect = {-45., -45., 95., 95.};//{-50, -50, 100, 100};
    pool->player_id[0] = POOL_NewEntityClassic(pool, TEX_PLAYER_RIGHT, player_display_rect, (SDL_FPoint) {0, 0});

    int player_loc = pool->player_id[0].location;
    pool->velocity[player_loc] = (SDL_FPoint) {0., 0.};
    POOL_AddComponentFlags(pool, COMPONENT_VELOCITY, player_loc);

    pool->collision_box[player_loc] = (SDL_FRect) {-45., -45., 95., 95.};//{-50., -50., 100., 100.};
    pool->hit_box[player_loc] = (SDL_FRect) {-45., -45., 95., 95.};//{-50., -50., 100., 100.};
    pool->health_point[player_loc] = 2;
    POOL_AddComponentFlags(pool, COMPONENT_COLLISIONBOX | COMPONENT_HITBOX, player_loc);

    // Adds the second player
    pool->player_component[1] = PLAYER_NewComponent(inState + 1);
    pool->player_amount = 2;

    player_display_rect = (SDL_Rect) {-45., -45., 95., 95.};//{-50, -50, 100, 100};
    pool->player_id[1] = POOL_NewEntityClassic(pool, TEX_PLAYER_RIGHT, player_display_rect, (SDL_FPoint) {500, 500});

    player_loc = pool->player_id[1].location;
    pool->velocity[player_loc] = (SDL_FPoint) {0., 0.};
    pool->hit_box[player_loc] = (SDL_FRect) {-45., -45., 95., 95.};//{-50., -50., 100., 100.};
    pool->health_point[player_loc] = 2;
    POOL_AddComponentFlags(pool, COMPONENT_VELOCITY, player_loc);

    pool->collision_box[player_loc] = (SDL_FRect) {-50., -50., 100., 100.};
    pool->hit_box[player_loc] = (SDL_FRect) {-50., -50., 100., 100.};
    POOL_AddComponentFlags(pool, COMPONENT_COLLISIONBOX | COMPONENT_HITBOX, player_loc);

    // Adds a tree
    EntityID tree = POOL_NewEntityClassic(pool, TEX_DEBUG, player_display_rect, (SDL_FPoint) {300., 300.});

    pool->collision_box[tree.location] = (SDL_FRect) {-50., -50., 100., 100.};
    POOL_AddComponentFlags(pool, COMPONENT_COLLISIONBOX, tree.location);
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

EntityID POOL_NewEntityClassic(EntityPool *pool, TextureLocation tex_location, SDL_Rect display_rect, SDL_FPoint position) {
    // Create new entity with new a new unique id and a designated position in the pool
    EntityID new_id = POOL_NewEntity(pool);

    pool->tex_location[new_id.location] = tex_location;
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

    // Inform everywhere the entity is destroyed and a new spot is available
    if (id.location == pool->lastEntitylocation) {
        pool->lastEntitylocation --;
    } else {
        pool->emptyLocationsAmount ++;
        pool->emptyLocations[pool->emptyLocationsAmount - 1] = id.location;
    }
    pool->currentCount --;
    pool->id[id.location] = (EntityID) {0, 0};

    // Reset all bitmaps
    pool->component_flags[id.location] = COMPONENT_NONE;
}
