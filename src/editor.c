#include <SDL2/SDL.h>

#include "editor.h"
#include "geometry.h"
#include "game.h"
#include "assets.h"

// Translate mouse position to world position
SDL_FPoint EDITOR_MouseToWorld(SDL_FPoint mouse, SDL_FPoint camera) {
    SDL_FPoint res;
    res.x = mouse.x + camera.x;
    res.y = mouse.y + camera.y;
    return res;
}

void EDITOR_PlaceEntity(Game *game) {
    SDL_FPoint position = EDITOR_MouseToWorld(game->inState->mouse, game->camera_pos);

    // TODO: get entity bundle from player selection
    EDITOR_EntityFromArchetype(game, ARCHETYPE_DEBUG, position);
}


void EDITOR_EntityFromArchetype(Game *game, EntityArchetype archetype, SDL_FPoint pos) {
    EntityPool *pool = game->pool;
    EntityID id = POOL_NewEntity(game->pool);
    int i = id.location;

    pool->position[i] = pos;
    POOL_AddComponentFlags(pool, COMPONENT_POSITION, i);

    switch (archetype) {
        case ARCHETYPE_PLAYER:
            pool->tex_location[i] = TEX_DEBUG;
            pool->display_rect[i] = (SDL_Rect) {-50, -50, 100, 100};
            pool->collision_box[i] = (SDL_FRect) {-50., -50., 100., 100.};
            pool->velocity[i] = (SDL_FPoint) {0., 0.};

            POOL_AddComponentFlags(pool, COMPONENT_TEXTURE | COMPONENT_DISPLAYRECT | COMPONENT_COLLISIONBOX | COMPONENT_VELOCITY, i);
            break;   
        default:
        case ARCHETYPE_DEBUG:
            pool->tex_location[i] = TEX_DEBUG;
            pool->display_rect[i] = (SDL_Rect) {-50, -50, 100, 100};
            pool->collision_box[i] = (SDL_FRect) {-50., -50., 100., 100.};

            POOL_AddComponentFlags(pool, COMPONENT_TEXTURE | COMPONENT_DISPLAYRECT | COMPONENT_COLLISIONBOX, i);
            break;
    }
} 

void EDITOR_DisplayInfo(Game *game) {
    // Compiling things to print
    if (!game->inState->ToggledF4) { return; }

    char str[500];
    sprintf(
        str,
        "SELECTED ARCHETYPE: %s\n"
        "MOUSE POSITION: (%f, %f)"
        ,
        EDITOR_ArchetypeToString(game->inState->selected_archetype),
        EDITOR_MouseToWorld(game->inState->mouse, game->camera_pos).x,
        EDITOR_MouseToWorld(game->inState->mouse, game->camera_pos).y
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

    // Place the text in the bottom right corner of the window
    int window_h = 100;
    int window_w = 100;
    SDL_GetWindowSize(game->window, &window_w, &window_h);
    dst.x = window_w - dst.w;
    dst.y = window_h - dst.h;

    SDL_RenderCopy(game->renderer, messageTexture, NULL, &dst);

    SDL_FreeSurface(messageSurface);
    SDL_DestroyTexture(messageTexture);
}

char* EDITOR_ArchetypeToString(EntityArchetype archetype) {
    char *strs[] = {"DEBUG", "PLAYER"};
    return strs[archetype];
}