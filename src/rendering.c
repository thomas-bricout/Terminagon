#include "game.h"
#include "rendering.h"
#include "geometry.h"
#define size 100

void RENDER_RenderMap(Game *game, SDL_FPoint camera_pos) {
    //game->map;
    SDL_Texture **assets = game->asset_manager->map_assets;

    // Get number of horizontal and vertical squares to display
    /*int window_h = 1000;
    int window_w = 1000;
    SDL_GetWindowSize(game->window, &window_w, &window_h);
    int j_end = ((int) camera_pos.x + window_w) / size + 1;
    int i_end = ((int) camera_pos.y + window_h) / size + 1;

    SDL_Rect dst = {0, 0, size, size};
    for(int i = ((int) (camera_pos.y) / size);i<i_end; i++){
        for(int j = ((int) (camera_pos.x) / size);j<j_end; j++){
            dst.x = size*j - camera_pos.x;
            dst.y = size*i - camera_pos.y;

            SDL_RenderCopy(game->renderer, assets[game->map[i][j].id], NULL, &dst);
        }
    }*/

    // Get number of horizontal and vertical squares to display
    int window_h = SCREEN_H;
    int window_w = SCREEN_W;
    SDL_GetWindowSize(game->window, &window_w, &window_h);

    SDL_Rect dst = {0, 0, size, size};
    for(int i=0;i<HAUTEUR;i++){
        for(int j=0;j<LARGEUR;j++){
            dst.x = size*j - camera_pos.x;
            dst.y = size*i - camera_pos.y;
            if(dst.x>window_w || dst.y>window_h) continue;
            if(dst.x+size<0 || dst.y+size<0) continue;
            
            SDL_RenderCopy(game->renderer, assets[game->map[i][j].id], NULL, &dst);
        }
    }

    //SDL_Rect rect = {10,10,32,32};
    //SDL_RenderCopy(game->renderer, assets[0], NULL,&rect);
}

void RENDER_RenderAll(Game *game) {

    EntityPool *pool = game->pool;
    AssetManager *asset_manager = game->asset_manager;
    SDL_Renderer *renderer = game->renderer;
    SDL_FPoint camera_pos = game->camera_pos;


    RENDER_RenderMap(game,camera_pos);

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