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