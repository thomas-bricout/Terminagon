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
    SDL_bool emptyLocations[MAX_ENTITY_COUNT];

    // Keep track of specific entities or entity lists ( for example, all entities which might damage the player )
    EntityID player;

    // Each list contains a specific component for all entities in the game
    EntityID id[MAX_ENTITY_COUNT];
    TextureLocation tex_location[MAX_ENTITY_COUNT];
    SDL_Rect display_rect[MAX_ENTITY_COUNT];

    // Bitmaps ( whether a specific entity has a component )
    SDL_bool tex_location_map[MAX_ENTITY_COUNT];
    SDL_bool display_rect_map[MAX_ENTITY_COUNT];
} EntityPool ;

void POOL_Init(EntityPool *pool);
void POOL_Load(EntityPool *pool);
EntityID POOL_New_entity_classic(EntityPool *pool, TextureLocation tex_location, SDL_Rect display_rect);
void POOL_Destroy_entity(EntityPool *pool, EntityID id);
void POOL_Display_All(AssetManager *assetManager, EntityPool *pool, SDL_Renderer *renderer);

#endif