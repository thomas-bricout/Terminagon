#ifndef ENTITY_POOL_H
#define ENTITY_POOL_H

typedef struct EntityPool EntityPool;

typedef struct {
    int location;
    int unique_id;
} EntityID;

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "assets.h"
#include "player.h"
#include "game.h"
#include "input.h"
#include "enemy.h"

#define MAX_ENTITY_COUNT 500

typedef enum ComponentFlags {
    COMPONENT_NONE          = 0b0000000000000000,
    COMPONENT_TEXTURE       = 0b0000000000000001,
    COMPONENT_DISPLAYRECT   = 0b0000000000000010,
    COMPONENT_COLLISIONBOX  = 0b0000000000000100,
    COMPONENT_DAMAGEBOX     = 0b0000000000001000,
    COMPONENT_HITBOX        = 0b0000000000010000,
    COMPONENT_POSITION      = 0b0000000000100000,
    COMPONENT_VELOCITY      = 0b0000000001000000,
    COMPONENT_PROJECTILE    = 0b0000000010000000,
    COMPONENT_VELOCITY_FLEXIBLE    = 0b0000000100000000,
    COMPONENT_AI            = 0b0000001000000000,
    COMPONENT_TARGET        = 0b0000010000000000,
    COMPONENT_DEATHANIM     = 0b0000100000000000,
} ComponentFlags;

struct EntityPool {
    // Keeping track of which spots are occupied by valid entities
    int currentCount;
    int lastEntitylocation; // Available spot

    int emptyLocations[MAX_ENTITY_COUNT];
    int emptyLocationsAmount;

    // Keep track of specific entities or entity lists ( for example, all entities which might damage the player )
    EntityID player_id[4];
    PlayerComponent player_component[4];
    int player_amount;

    // Each list contains a specific component for all entities in the game
    EntityID id[MAX_ENTITY_COUNT];
    TextureLocation tex_location[MAX_ENTITY_COUNT];
    
    SDL_Rect display_rect[MAX_ENTITY_COUNT]; // Where to display the entity relative to its position
    SDL_FRect collision_box[MAX_ENTITY_COUNT];
    SDL_FRect damage_box[MAX_ENTITY_COUNT];

    SDL_FRect hit_box[MAX_ENTITY_COUNT];
    int health_point[MAX_ENTITY_COUNT];
    double last_hit[MAX_ENTITY_COUNT];

    SDL_FPoint position[MAX_ENTITY_COUNT];
    SDL_FPoint velocity[MAX_ENTITY_COUNT];

    EnemyComponent enemy[MAX_ENTITY_COUNT];

    double deathAnim_timeStamp[MAX_ENTITY_COUNT];

    ComponentFlags component_flags[MAX_ENTITY_COUNT]; 
};

void POOL_Init(EntityPool *pool);
void POOL_Load(EntityPool *pool, InState *inState);
EntityID POOL_NewEntity(EntityPool *pool);
EntityID POOL_NewEntityClassic(EntityPool *pool, TextureLocation tex_location, SDL_Rect display_rect, SDL_FPoint position);
void POOL_DestroyEntity(EntityPool *pool, EntityID id);
void POOL_AddComponentFlags(EntityPool *pool, ComponentFlags component_flags, int location);
bool POOL_LacksComponentFlags(EntityPool *pool, ComponentFlags component_flags, int location);
void POOL_RemoveComponentFlags(EntityPool *pool, ComponentFlags component_flags, int location);
void POOL_DestroyEntityFromIndex(EntityPool *pool, int i);

#endif