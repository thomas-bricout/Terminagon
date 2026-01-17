#include <SDL2/SDL.h>
#include "entity_pool.h"

void POOL_Init(EntityPool *pool) {
    pool->currentCount = 0;
    SDL_Rect default_rect = {0, 0, 0, 0};
    EntityID default_id = {0, 0};

    for (int i = 0; i < MAX_ENTITY_COUNT; i++) {
        pool->id[i] = default_id;
        pool->display_rect[i] = default_rect;
        pool->tex_location[i] = TEX_DEBUG;
    }
}

void POOL_Load(EntityPool *pool) {
    // Adds the main player
    SDL_Rect player_display_rect = {100, 100, 100, 100};
    POOL_New_entity(pool, TEX_DEBUG, player_display_rect);
}

EntityID POOL_New_entity(EntityPool *pool, TextureLocation tex_location, SDL_Rect display_rect) {
    // Create new entity with new a new unique id and a designated position in the pool
    static int currentID = 1;
    EntityID new_id;

    new_id.location = pool->currentCount; // TODO allow reusage of freed up places
    new_id.id = currentID;
    currentID ++;

    SDL_Log("Creating new Entity with location %d and id %d", new_id.location, new_id.id);

    pool->id[new_id.location] = new_id;
    pool->tex_location[new_id.location] = tex_location;
    pool->display_rect[new_id.location] = display_rect;

    pool->currentCount ++;

    return new_id;
}

void POOL_Display_All(AssetManager *assetManager, EntityPool *pool, SDL_Renderer *renderer) {
    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Starting displaying entities");

    for (int i = 0; i < pool->currentCount; i++) {
        SDL_Texture *tex = assetManager->asset_array[pool->tex_location[i]];
        SDL_RenderCopy(renderer, tex, NULL, &pool->display_rect[i]);

        SDL_LogDebug(
            SDL_LOG_CATEGORY_RENDER,
            "Rendering Entity with location %d, id %d, texture %d, coordinates (%d, %d)",
             i, pool->id[i].id, pool->tex_location[i], pool->display_rect[i].x, pool->display_rect[i].y
        );
    }
}
