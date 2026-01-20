#include "input.h"
#include "entity_pool.h"
#include "assets.h"

const double MAX_PLAYER_SPEED = 0.2;

EntityID POOL_SpawnArrow(EntityPool *pool, SDL_FPoint position) {
    SDL_Rect display_rect = {-20, -20, 40, 40};
    EntityID id = POOL_NewEntityClassic(pool, TEX_ARROW, display_rect, position);

    pool->velocity[id.location] = (SDL_FPoint) {2., 0.};
    pool->velocity_map[id.location] = true;

    return id;
}

void PlayerSystem(EntityPool *pool, InputSituation *inputSituation) {
    int playerLocation = pool->player.location;
    SDL_FPoint *playerVelocity = &pool->velocity[playerLocation];
    SDL_FPoint *playerPosition = &pool->position[playerLocation];

    if ( pool->id[playerLocation].unique_id == pool->player.unique_id ) {
        playerVelocity->x = MAX_PLAYER_SPEED * ( (int) inputSituation->RIGHT - (int) inputSituation->LEFT );
        playerVelocity->y = MAX_PLAYER_SPEED * ( (int) inputSituation->DOWN - (int) inputSituation->UP );
    }

    if (inputSituation->C) {
        POOL_SpawnArrow(pool, *playerPosition);
    }
}
