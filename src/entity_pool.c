#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "entity_pool.h"
#include "player.h"
#include "geometry.h"

void POOL_Init(EntityPool *pool) {
    pool->currentCount = 0;
    pool->lastEntitylocation = 0;
    pool->emptyLocationsAmount = 0;

    EntityID default_id = {0, 0};

    for (int i = 0; i < MAX_ENTITY_COUNT; i++) {
        pool->id[i] = default_id;
        pool->tex_location[i] = TEX_DEBUG;

        pool->tex_location[i] =     false;

        pool->display_rect_map[i] = false;
        pool->collision_box_map[i] =    false;
        pool->damage_box_map[i] = false;
        pool->hit_box_map[i] = false;

        pool->position_map[i] =     false;
        pool->velocity_map[i] =     false;
    }
}

void POOL_Load(EntityPool *pool) {
    // Adds the main player
    SDL_Rect player_display_rect = {-50, -50, 100, 100};

    pool->player_c = PLAYER_NewComponent();
    pool->player = POOL_NewEntityClassic(pool, TEX_DEBUG, player_display_rect, (SDL_FPoint) {0, 0});

    int player_loc = pool->player.location;
    pool->velocity[player_loc] = (SDL_FPoint) {10., 10.};
    pool->velocity_map[player_loc] = true;

    pool->collision_box[player_loc] = (SDL_FRect) {-50., -50., 100., 100.};
    pool->collision_box_map[player_loc] = true;

    EntityID tree = POOL_NewEntityClassic(pool, TEX_DEBUG, player_display_rect, (SDL_FPoint) {300., 300.});
    pool->collision_box[tree.location] = (SDL_FRect) {-50., -50., 100., 100.};
    pool->collision_box_map[tree.location] = true;
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

    pool->tex_location_map[new_id.location] =   true;
    pool->display_rect_map[new_id.location] =   true;
    pool->position_map[new_id.location] =       true;

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
    pool->tex_location_map[id.location] =   false;
    pool->display_rect_map[id.location] =   false;
    pool->collision_box_map[id.location] =  false;
    pool->damage_box_map[id.location] =     false;
    pool->hit_box_map[id.location]  =       false;
    pool->velocity_map[id.location] =       false;
    pool->position_map[id.location] =       false;
}

void POOL_DisplayAll(AssetManager *assetManager, EntityPool *pool, SDL_Renderer *renderer) {
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Starting displaying entities");

    for (int i = 0; i < pool->lastEntitylocation; i++) {
        // Skip entities that don't have texture / display rect / position
        if (!pool->position_map[i] || !pool->display_rect_map[i] || !pool->tex_location_map[i]) { continue; }

        SDL_Texture *tex;
        ASSETS_AccessTexture(&tex, assetManager, pool->tex_location[i]);

        SDL_Rect dst = pool->display_rect[i];
        dst = RECT_Offset(dst, FPOINT_ToPoint(pool->position[i]));

        SDL_RenderCopy(renderer, tex, NULL, &dst);
    }
}

void POOL_ApplyVelocity(EntityPool *pool, double deltaTime) {
    for (int i = 0; i < pool->lastEntitylocation; i++) {
        if (!pool->position_map[i] || !pool->velocity_map[i]) { continue; }

        if (pool->collision_box_map[i]) { // If entity has a collision map, check possible collisions with every other entities
            bool collided = false;
            for (int j = 0; j < pool->lastEntitylocation; j++) { 
                if ( i == j ) { continue; }
                if ( !pool->collision_box_map[j] || !pool->position_map[j] ) { continue; }

                SDL_FPoint obstacle_pos = pool->position[j];
                SDL_FRect obstacle_box = pool->collision_box[j];

                SDL_FPoint collider_pos = pool->position[i];
                SDL_FRect collider_box = pool->collision_box[i];

                // Shift collision boxes according to their entity position and velocity
                obstacle_box = FRECT_Offset(obstacle_box, obstacle_pos);
                collider_box = FRECT_Offset(collider_box, FPOINT_ApplyVelocity(collider_pos, pool->velocity[i], deltaTime));

                // Continue there if there is no collision
                if (!SDL_HasIntersectionF(&collider_box, &obstacle_box)) { continue; }

                SDL_FRect intersect_box;
                SDL_IntersectFRect(&collider_box, &obstacle_box, &intersect_box);

                // TODO: Compute proper position
                collided = true;
            }
            // Proceed as normal if it does not result in a collision
            if (!collided) {
                pool->position[i] = FPOINT_ApplyVelocity(pool->position[i], pool->velocity[i], deltaTime); 
            }
            // Do not check other possibilities
            continue;
        }

        // ELSE: proceed without dealing with collisions
        pool->position[i] = FPOINT_ApplyVelocity(pool->position[i], pool->velocity[i], deltaTime); 
    }
}