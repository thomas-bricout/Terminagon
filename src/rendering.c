#include "game.h"
#include "rendering.h"
#include "geometry.h"

void RENDER_RenderAll(Game *game) {
    EntityPool *pool = game->pool;
    AssetManager *asset_manager = game->asset_manager;
    SDL_Renderer *renderer = game->renderer;
    SDL_FPoint camera_pos = game->camera_pos;

    SDL_LogDebug(SDL_LOG_CATEGORY_RENDER, "Starting displaying entities");

    for (int i = 0; i < pool->lastEntitylocation; i++) {
        // Skip entities that don't have texture / display rect / position
        // if (!pool->position_map[i] || !pool->display_rect_map[i] || !pool->tex_location_map[i]) { continue; }
        if (POOL_LacksComponentFlags(pool, COMPONENT_POSITION | COMPONENT_DISPLAYRECT | COMPONENT_TEXTURE, i)) { continue; }

        SDL_Texture *tex;
        ASSETS_AccessTexture(&tex, asset_manager, pool->tex_location[i]);
        
        SDL_Rect dst = RECT_Offset(pool->display_rect[i], FPOINT_ToPoint(FPOINT_RelativePoint(pool->position[i], camera_pos)));

        SDL_RenderCopy(renderer, tex, NULL, &dst);
    }
}