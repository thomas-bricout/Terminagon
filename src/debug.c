#include <SDL2/SDL.h>

#include "debug.h"
#include "game.h"
#include "geometry.h"

void DEBUG_DisplayDebug(Game *game, double deltaTime, double FPS, double elapsed, double current_time) {
    // Quit if F3 is not toggled
    InState *in = game->inState;
    if (!in->ToggledF3) { return; }

    DEBUG_DisplayCollisionRects(game->pool, game->renderer);
    DEBUG_DisplayDamageRects(game->pool, game->renderer);
    DEBUG_DisplayHitboxRects(game->pool, game->renderer);

    DEBUG_DisplayDebugInfo(game, deltaTime, FPS, elapsed, current_time);
}

void DEBUG_DisplayDebugInfo(Game *game, double deltaTime, double FPS, double elapsed, double current_time) {
    InState *in = game->inState;

    // Compiling things to print
    char str[500];
    sprintf(
        str,
        "ENTITY COUNT: %d\n"
        "deltaTime: %4.2fms FPS: %4.1f\n"
        "COMPUTE TIME PER FRAME: %4.2fms\n"
        "LEFT %d RIGHT %d UP %d DOWN %d\n"
        "W %d X %d C %d \n"
        "PLAYER POSITION (%4.2f, %4.2f)\n"
        "CURRENT ACTION %d SINCE %5.0f\n"
        "CURRENT ANGLE %f\n"
        , 
        game->pool->currentCount, 
        deltaTime, FPS,
        elapsed,
        in->LEFT, in->RIGHT, in->UP, in->DOWN,
        in->W, in->X, in->C,
        game->pool->position[game->pool->player.location].x,
        game->pool->position[game->pool->player.location].y,
        game->pool->player_c.action,
        current_time - game->pool->player_c.actionTimeStamp,
        game->pool->player_c.angle * 360. / 3.1415
    );

    // Display the string on the screen
    SDL_Color white = {255, 255, 255};
    SDL_Surface *messageSurface = TTF_RenderUTF8_Solid_Wrapped(game->asset_manager->debug_font, str, white, 5000);
    if (messageSurface == NULL) {
        fprintf(stderr, "Erreur TTF_RenderUTF8_Solid_Wrapped: %s\n", SDL_GetError());
     }

    SDL_Texture *messageTexture = SDL_CreateTextureFromSurface(game->renderer, messageSurface);
    if (messageSurface == NULL) {
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
    }

    SDL_Rect dst = {0, 0, 0, 0};
    if (0 != SDL_QueryTexture(messageTexture, NULL, NULL, &dst.w, &dst.h)) {
        fprintf(stderr, "Erreur SDL_QueryTexture : %s\n", SDL_GetError());
    }

    SDL_RenderCopy(game->renderer, messageTexture, NULL, &dst);

    SDL_FreeSurface(messageSurface);
    SDL_DestroyTexture(messageTexture);
}

void DEBUG_DisplayCollisionRects(EntityPool *pool, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    for (int i = 0; i < pool->lastEntitylocation; i++) {
        
        if (!pool->position_map[i] || !pool->collision_box_map[i]) { continue; }

        SDL_FRect rect = OffsetFRect(pool->collision_box[i], pool->position[i]);
        SDL_RenderFillRectF(renderer, &rect);
    }
}

void DEBUG_DisplayDamageRects(EntityPool *pool, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    for (int i = 0; i < pool->lastEntitylocation; i++) {
        
        if (!pool->position_map[i] || !pool->damage_box_map[i]) { continue; }

        SDL_FRect rect = OffsetFRect(pool->damage_box[i], pool->position[i]);
        SDL_RenderFillRectF(renderer, &rect);
    }
}

void DEBUG_DisplayHitboxRects(EntityPool *pool, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 100);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    for (int i = 0; i < pool->lastEntitylocation; i++) {
        
        if (!pool->position_map[i] || !pool->hit_box_map[i]) { continue; }

        SDL_FRect rect = OffsetFRect(pool->hit_box[i], pool->position[i]);
        SDL_RenderFillRectF(renderer, &rect);
    }
}