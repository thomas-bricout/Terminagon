#include <SDL2/SDL.h>
#include "entity_pool.h"

void POOL_Init(EntityPool *pool) {
    pool->currentCount = 0;
    SDL_Rect default_rect = {0, 0, 0, 0};
    EntityID default_id = {0, 0};

    for (int i = 0; i < MAX_ENTITY_COUNT; i++) {
        pool->id[i] = default_id;
        pool->rectangle[i] = default_rect;
        pool->texture[i] = TEX_DEBUG;
    }
}