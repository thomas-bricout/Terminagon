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
    EntityBundle bundle;
    bundle.texture = TEX_DEBUG;
    bundle.texture_box = (SDL_Rect) {0, 0, 100, 100};
    bundle.collision_box = (SDL_FRect) {0., 0., 100., 100.};

    EDITOR_EntityFromBundle(game, bundle, position);
}


void EDITOR_EntityFromBundle(Game *game, EntityBundle bundle, SDL_FPoint pos) {
    EntityPool *pool = game->pool;
    EntityID id = POOL_NewEntity(game->pool);
    int i = id.location;

    pool->tex_location[i] = bundle.texture;
    pool->display_rect[i] = bundle.texture_box;
    pool->collision_box[i] = bundle.collision_box;
    pool->position[i] = pos;

    POOL_AddComponentFlags(pool, COMPONENT_TEXTURE | COMPONENT_DISPLAYRECT | COMPONENT_COLLISIONBOX | COMPONENT_POSITION, i);
} 