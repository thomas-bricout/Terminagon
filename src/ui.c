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

void draw_pause(Game *game) {
    char str[] = "PAUSED";

    // Display the string on the screen
    SDL_Color white = {255, 255, 255};
    SDL_Surface *messageSurface = TTF_RenderUTF8_Solid_Wrapped(game->asset_manager->debug_font, str, white, 1000);
    if (messageSurface == NULL) {
        fprintf(stderr, "Erreur TTF_RenderUTF8_Solid_Wrapped: %s\n", SDL_GetError());
     }

    SDL_Texture *messageTexture = SDL_CreateTextureFromSurface(game->renderer, messageSurface);
    if (messageSurface == NULL) {
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
    }

    // Place the text in the center of the window
    SDL_Rect dst = {0, 0, 100, 100};
    if (0 != SDL_QueryTexture(messageTexture, NULL, NULL, &dst.w, &dst.h)) {
        fprintf(stderr, "Erreur SDL_QueryTexture : %s\n", SDL_GetError());
    }

    int window_h = 100;
    int window_w = 100;
    SDL_GetWindowSize(game->window, &window_w, &window_h);
    dst.x = (window_w - dst.w) / 2.;
    dst.y = (window_h - dst.h) / 2.;

    if (SDL_RenderCopy(game->renderer, messageTexture, NULL, &dst) != 0) {
        fprintf(stderr, "STD_RenderCopy Error: %s", SDL_GetError());
    }

    SDL_FreeSurface(messageSurface);
    SDL_DestroyTexture(messageTexture);
}

void draw_welcome(Game *game) {
    char str[] = "PRESS INSERT TO SWITCH TO KEYBOARD MODE\n"
                 "OR ANY BUTTON ON YOUR CONTROLLER TO GET RID OF THIS MESSAGE";

    // Display the string on the screen
    SDL_Color white = {255, 255, 255};
    SDL_Surface *messageSurface = TTF_RenderUTF8_Solid_Wrapped(game->asset_manager->debug_font, str, white, 1000);
    if (messageSurface == NULL) {
        fprintf(stderr, "Erreur TTF_RenderUTF8_Solid_Wrapped: %s\n", SDL_GetError());
     }

    SDL_Texture *messageTexture = SDL_CreateTextureFromSurface(game->renderer, messageSurface);
    if (messageSurface == NULL) {
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
    }

    // Place the text in the center of the window
    SDL_Rect dst = {0, 0, 100, 100};
    if (0 != SDL_QueryTexture(messageTexture, NULL, NULL, &dst.w, &dst.h)) {
        fprintf(stderr, "Erreur SDL_QueryTexture : %s\n", SDL_GetError());
    }

    int window_h = 100;
    int window_w = 100;
    SDL_GetWindowSize(game->window, &window_w, &window_h);
    dst.x = (window_w - dst.w) / 2.;
    dst.y = (window_h - dst.h) / 2.;

    if (SDL_RenderCopy(game->renderer, messageTexture, NULL, &dst) != 0) {
        fprintf(stderr, "STD_RenderCopy Error: %s", SDL_GetError());
    }

    SDL_FreeSurface(messageSurface);
    SDL_DestroyTexture(messageTexture);

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