#include "input.h"
#include "entity_pool.h"
#include "assets.h"
#include "player.h"

const double DEFAULT_PLAYER_SPEED = 0.2;
const double DASHING_PLAYER_SPEED = 0.4;
const double BOW_AIMING_TIME = 1000;
const double DASHING_TIME = 500;

PlayerComponent PLAYER_NewComponent() {
    PlayerComponent p_c;
    p_c.action = ACTION_NONE;
    p_c.actionTimeStamp = 0.;
    p_c.angle = 0;
    p_c.walking = false;

    return p_c;
}

EntityID POOL_SpawnArrow(EntityPool *pool, SDL_FPoint position) {
    SDL_Rect display_rect = {-20, -20, 40, 40};
    EntityID id = POOL_NewEntityClassic(pool, TEX_ARROW, display_rect, position);

    pool->velocity[id.location] = (SDL_FPoint) {2., 0.};
    pool->velocity_map[id.location] = true;

    return id;
}

void PlayerSystem(EntityPool *pool, InputSituation *inputSituation, double current_time) {
    int playerLocation = pool->player.location;

    SDL_FPoint *playerVelocity = &pool->velocity[playerLocation];
    SDL_FPoint *playerPosition = &pool->position[playerLocation];
    PlayerComponent *pc = &pool->player_c;
    
    // Whether player is ready to start a new action
    bool available = pc->action == 0;
    if (available) { // Treat new actions
        if (inputSituation->X) {
            pc->action = ACTION_DASHING;
            pc->actionTimeStamp = current_time;
        } else if (inputSituation->C) {
            pc->action = ACTION_BOW_AIMING;
            pc->actionTimeStamp = current_time;
        }
    } else {        // Treat current action
        switch(pc->action) {
            case ACTION_DASHING:
                if (current_time - pc->actionTimeStamp >= DASHING_TIME) {
                    pc->action = ACTION_NONE;
                    pc->actionTimeStamp = current_time;
                }
                break;
            case ACTION_BOW_AIMING:
                if (!inputSituation->C) {
                    if (current_time - pc->actionTimeStamp >= BOW_AIMING_TIME) {
                        POOL_SpawnArrow(pool, *playerPosition);
                    }
                    pc->action = ACTION_NONE;
                    pc->actionTimeStamp = current_time;
                }
                break;
            case ACTION_SHIELDING:
                break;
            case ACTION_NONE:
                break;
        }
    }

    // Set playerVelocity
    double player_speed = pc->action == ACTION_DASHING ? DASHING_PLAYER_SPEED : DEFAULT_PLAYER_SPEED;
    if ( pool->id[playerLocation].unique_id == pool->player.unique_id ) {
        playerVelocity->x = player_speed * ( (int) inputSituation->RIGHT - (int) inputSituation->LEFT );
        playerVelocity->y = player_speed * ( (int) inputSituation->DOWN - (int) inputSituation->UP );
    }

}
