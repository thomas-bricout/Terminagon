#ifndef ENTITY_POOL_H
#define ENTITY_POOL_H

typedef struct EntityPool EntityPool;

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "assets.h"
#include "player.h"
#include "game.h"

#define MAX_ENTITY_COUNT 500

typedef struct {
    int location;
    int unique_id;
} EntityID;

struct EntityPool {
    // Keeping track of which spots are occupied by valid entities
    int currentCount;
    int lastEntitylocation; // Available spot

    int emptyLocations[MAX_ENTITY_COUNT];
    int emptyLocationsAmount;

    // Keep track of specific entities or entity lists ( for example, all entities which might damage the player )
    EntityID player;
    PlayerComponent player_c;

    // Each list contains a specific component for all entities in the game
    EntityID id[MAX_ENTITY_COUNT];
    TextureLocation tex_location[MAX_ENTITY_COUNT];
    
    SDL_Rect display_rect[MAX_ENTITY_COUNT]; // Where to display the entity relative to its position
    SDL_FRect collision_box[MAX_ENTITY_COUNT];
    SDL_FRect damage_box[MAX_ENTITY_COUNT];
    SDL_FRect hit_box[MAX_ENTITY_COUNT];

    SDL_FPoint position[MAX_ENTITY_COUNT];
    SDL_FPoint velocity[MAX_ENTITY_COUNT];

    // Bitmaps ( whether a specific entity has a component )
    bool tex_location_map[MAX_ENTITY_COUNT];

    bool display_rect_map[MAX_ENTITY_COUNT];
    bool collision_box_map[MAX_ENTITY_COUNT];
    bool damage_box_map[MAX_ENTITY_COUNT];
    bool hit_box_map[MAX_ENTITY_COUNT];

    bool position_map[MAX_ENTITY_COUNT];
    bool velocity_map[MAX_ENTITY_COUNT];

};

void POOL_Init(EntityPool *pool);
void POOL_Load(EntityPool *pool);
EntityID POOL_NewEntity(EntityPool *pool);
EntityID POOL_NewEntityClassic(EntityPool *pool, TextureLocation tex_location, SDL_Rect display_rect, SDL_FPoint position);
void POOL_DestroyEntity(EntityPool *pool, EntityID id);
void POOL_DisplayAll(Game *game);
void POOL_ApplyVelocity(EntityPool *pool, double deltaTime);

#endif