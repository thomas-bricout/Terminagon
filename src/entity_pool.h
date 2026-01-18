#ifndef ENTITY_POOL_H
#define ENTITY_POOL_H

#include <SDL2/SDL.h>
#include "assets.h"

#define MAX_ENTITY_COUNT 500

typedef struct {
    int location;
    int unique_id;
} EntityID;

typedef struct {
    // Keeping track of which spots are occupied by valid entities
    int currentCount;
    int lastEntitylocation; // Available spot

    int emptyLocations[MAX_ENTITY_COUNT];
    int emptyLocationsAmount;

    // Keep track of specific entities or entity lists ( for example, all entities which might damage the player )
    EntityID player;

    // Each list contains a specific component for all entities in the game
    EntityID id[MAX_ENTITY_COUNT];
    TextureLocation tex_location[MAX_ENTITY_COUNT];
    SDL_Rect display_rect[MAX_ENTITY_COUNT]; // Where to display the entity relative to its position

    SDL_Point position[MAX_ENTITY_COUNT];
    SDL_Point velocity[MAX_ENTITY_COUNT];

    // Bitmaps ( whether a specific entity has a component )
    SDL_bool tex_location_map[MAX_ENTITY_COUNT];
    SDL_bool display_rect_map[MAX_ENTITY_COUNT];

    SDL_bool position_map[MAX_ENTITY_COUNT];
    SDL_bool velocity_map[MAX_ENTITY_COUNT];

} EntityPool ;

void POOL_Init(EntityPool *pool);
void POOL_Load(EntityPool *pool);
EntityID POOL_NewEntity(EntityPool *pool);
EntityID POOL_NewEntityClassic(EntityPool *pool, TextureLocation tex_location, SDL_Rect display_rect, SDL_Point point);
void POOL_DestroyEntity(EntityPool *pool, EntityID id);
void POOL_DisplayAll(AssetManager *assetManager, EntityPool *pool, SDL_Renderer *renderer);
void POOL_ApplyVelocity(EntityPool *pool, double deltaTime);

#endif