#include <SDL2/SDL.h>

#include "debug.h"
#include "game.h"
#include "geometry.h"

#define size 100

void DEBUG_DisplayCollisionRectsTileMap(Game *game){
    int window_h = SCREEN_H;
    int window_w = SCREEN_W;
    SDL_GetWindowSize(game->window, &window_w, &window_h);

    for(int i=0;i<HAUTEUR;i++){
        for(int j=0;j<LARGEUR;j++){
            SDL_FRect rect = {0, 0, size, size};
            rect.x = size*j - game->camera_pos.x;
            rect.y = size*i - game->camera_pos.y;

            if(rect.x>window_w || rect.y>window_h) continue;
            if(rect.x+size<0 || rect.y+size<0) continue;
            if(game->map[i][j].blocking){
                SDL_RenderFillRectF(game->renderer, &rect);
            }
        }
    }
}


void DEBUG_DisplayCollisionRects(Game *game) {
    SDL_Renderer *renderer = game->renderer;
    EntityPool *pool = game->pool;

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    for (int i = 0; i < pool->lastEntitylocation; i++) {
        
        if (POOL_LacksComponentFlags(pool, COMPONENT_POSITION | COMPONENT_COLLISIONBOX, i)) { continue; }

        SDL_FRect rect = FRECT_Offset(pool->collision_box[i], FPOINT_RelativePoint(pool->position[i], game->camera_pos));
        SDL_RenderFillRectF(renderer, &rect);
    }

    DEBUG_DisplayCollisionRectsTileMap(game);
}

void DEBUG_DisplayDamageRects(Game *game) {
    SDL_Renderer *renderer = game->renderer;
    EntityPool *pool = game->pool;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    for (int i = 0; i < pool->lastEntitylocation; i++) {
        
        if (POOL_LacksComponentFlags(pool, COMPONENT_POSITION | COMPONENT_DAMAGEBOX, i)) { continue; }

        SDL_FRect rect = FRECT_Offset(pool->damage_box[i], pool->position[i]);
        SDL_RenderFillRectF(renderer, &rect);
    }
}

void DEBUG_DisplayHitboxRects(Game *game) {
    SDL_Renderer *renderer = game->renderer;
    EntityPool *pool = game->pool;

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 100);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    for (int i = 0; i < pool->lastEntitylocation; i++) {
        if (POOL_LacksComponentFlags(pool, COMPONENT_POSITION | COMPONENT_HITBOX, i)) { continue; }

        SDL_FRect rect = FRECT_Offset(pool->hit_box[i], pool->position[i]);
        SDL_RenderFillRectF(renderer, &rect);
    }
}

void DEBUG_DisplayDebug(Game *game, double deltaTime, double FPS, double elapsed, double current_time) {
    // Quit if F3 is not toggled
    InState *in = game->inState;
    if (!in->ToggledF3) { return; }

    DEBUG_DisplayCollisionRects(game);
    DEBUG_DisplayDamageRects(game);
    DEBUG_DisplayHitboxRects(game);

    DEBUG_DisplayDebugInfo(game, deltaTime, FPS, elapsed, current_time);
}

void DEBUG_DisplayDebugInfo(Game *game, double deltaTime, double FPS, double elapsed, double current_time) {
    InState *in = game->inState;
    int player_loc = game->pool->player_id[0].location;
    PlayerComponent *pc = &game->pool->player_component[0];

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
        "CURRENT ANGLE %3.0f\n"
        , 
        game->pool->currentCount, 
        deltaTime, FPS,
        elapsed,
        in->LEFT, in->RIGHT, in->UP, in->DOWN,
        in->W, in->X, in->C,
        game->pool->position[player_loc].x,
        game->pool->position[player_loc].y,
        pc->action,
        current_time - pc->actionTimeStamp,
        pc->angle * 180. / 3.1415
    );

    // Display the string on the screen
    SDL_Color white = {255, 255, 255};
    SDL_Surface *messageSurface = TTF_RenderUTF8_Solid_Wrapped(game->asset_manager->debug_font, str, white, 0);
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
