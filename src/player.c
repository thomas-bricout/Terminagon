#include "input.h"
#include "entity_pool.h"

const double MAX_PLAYER_SPEED = 0.2;

void PlayerSystem(EntityPool *pool, InputSituation *inputSituation) {
    int playerLocation = pool->player.location;
    if ( pool->id[playerLocation].unique_id == pool->player.unique_id ) {
        SDL_FPoint *playerVelocity = &pool->velocity[playerLocation];
        playerVelocity->x = MAX_PLAYER_SPEED * ( (int) inputSituation->RIGHT - (int) inputSituation->LEFT );
        playerVelocity->y = MAX_PLAYER_SPEED * ( (int) inputSituation->DOWN - (int) inputSituation->UP );
    }
}