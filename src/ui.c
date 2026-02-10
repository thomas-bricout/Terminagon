#include "ui.h"

void draw_hearts(AssetManager *assetManager, SDL_Renderer *renderer, int hp, int player);

void draw_ui(Game *game) {
    int player_hp = game->pool->health_point[game->pool->player_id[0].location];
    int arrow_count = game->pool->player_component[0].number_arrows;

    draw_hearts(game->asset_manager, game->renderer, player_hp, 0);
    draw_arrow_count(game->asset_manager, game->renderer, arrow_count, 0);

    player_hp = game->pool->health_point[game->pool->player_id[1].location];
    arrow_count = game->pool->player_component[1].number_arrows;

    draw_hearts(game->asset_manager, game->renderer, player_hp, 1);
    draw_arrow_count(game->asset_manager, game->renderer, arrow_count, 1);
}

void draw_hearts(AssetManager *assetManager, SDL_Renderer *renderer, int hp, int player) {
    for ( int i = 0; i < 10; i++ ) {
        SDL_Texture *tex = NULL;
        TextureIndex tex_l = TEX_HEART_FULL;

        if (hp - i*2 == 1) {
            tex_l = TEX_HEART_HALF;
        } else if (hp - i*2 <= 0) {
            tex_l = TEX_HEART_EMPTY;
        }

        ASSETS_AccessTexture(&tex, assetManager, tex_l);

        SDL_Rect dst = (SDL_Rect) {i * 55 + 5, 5 + player * 55, 50, 50};
        SDL_RenderCopy(renderer, tex, NULL, &dst);
    }
}

void draw_arrow_count(AssetManager *assetManager, SDL_Renderer *renderer, int arrow_count, int player) {
    SDL_Texture *tex = NULL;
    ASSETS_AccessTexture(&tex, assetManager, TEX_ARROW_UP);

    for ( int i = 0; i < arrow_count; i++ ) {
        SDL_RenderCopy(renderer, tex, NULL, &(SDL_Rect) { (i + 10) * 55, 5 + player * 55, 50, 50} );
    }
}