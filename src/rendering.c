#include "game.h"
#include "rendering.h"
#include "geometry.h"
#define size 100
void RENDER_RenderMap(Game *game,SDL_FPoint camera_pos) {
    //game->map;
    SDL_Texture **assets = load_MAP_Textures("assets/overworldtiles.bmp",game->renderer);

    SDL_Rect dst;
    int type;
    for(int i=0;i<HAUTEUR;i++){
        for(int j=0;j<LARGEUR;j++){
            dst.x = size*j - camera_pos.x;
            dst.y = size*i - camera_pos.y;
            dst.w = size;
            dst.h = size;
            if(game->map[i][j].blocking){
                type=1;
            }else{
                type=2;
            }
            
            SDL_RenderCopy(game->renderer, assets[type], NULL, &dst);
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