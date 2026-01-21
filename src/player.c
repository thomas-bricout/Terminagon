#include "input.h"
#include "entity_pool.h"
#include "assets.h"
#include "player.h"
#include "geometry.h"

#include <math.h>

const float DEFAULT_PLAYER_SPEED = 0.2;
const float DASHING_PLAYER_SPEED = 0.6;
const float AIMING_PLAYER_SPEED = 0.1;

const double BOW_AIMING_TIME = 1000;
const double DASHING_TIME = 200;
const double RECOVERY_TIME = 100;

const float ARROW_SPEED = 0.5;

PlayerComponent PLAYER_NewComponent() {
    PlayerComponent p_c;
    p_c.action = ACTION_NONE;
    p_c.actionTimeStamp = 0.;
    p_c.angle = 0;
    p_c.walking = false;

    return p_c;
}

EntityID POOL_SpawnArrow(EntityPool *pool, SDL_FPoint position, double angle) {
    SDL_Rect display_rect = {-20, -20, 40, 40};
    EntityID id = POOL_NewEntityClassic(pool, TEX_ARROW, display_rect, position);

    pool->velocity[id.location] = (SDL_FPoint) { ARROW_SPEED * cos(angle) , ARROW_SPEED * sin(angle) };
    POOL_AddComponentFlags(pool, COMPONENT_VELOCITY, id.location);

    return id;
}

void PLAYER_System(EntityPool *pool, InState *inState, double current_time) {
    int playerLocation = pool->player.location;

    SDL_FPoint *playerVelocity = &pool->velocity[playerLocation];
    SDL_FPoint *playerPosition = &pool->position[playerLocation];
    PlayerComponent *pc = &pool->player_c;
    double elapsed_time = current_time - pc->actionTimeStamp;
    
    // Whether player is ready to start a new action
    bool available = pc->action == 0 && elapsed_time >= RECOVERY_TIME;
    if (available) { // Treat new actions
        if (inState->X) {
            pc->action = ACTION_DASHING;
            pc->actionTimeStamp = current_time;
        } else if (inState->W) {
            pc->action = ACTION_SHIELDING;
            pc->actionTimeStamp = current_time;
        } else if (inState->C) {
            pc->action = ACTION_BOW_AIMING;
            pc->actionTimeStamp = current_time;
        }
    } else {        // Treat current action
        switch(pc->action) {
            case ACTION_DASHING:
                if (elapsed_time >= DASHING_TIME) {
                    pc->action = ACTION_NONE;
                    pc->actionTimeStamp = current_time;
                }
                break;
            case ACTION_BOW_AIMING:
                if (!inState->C) {
                    if (elapsed_time >= BOW_AIMING_TIME) {
                        POOL_SpawnArrow(pool, *playerPosition, pc->angle);
                    }
                    pc->action = ACTION_NONE;
                    pc->actionTimeStamp = current_time;
                }
                break;
            case ACTION_SHIELDING:
                if (!inState->W) {
                    pc->action = ACTION_NONE;
                    pc->actionTimeStamp = current_time;
                }
                break;
            case ACTION_NONE:
                break;
        }
    }

    // Set playerVelocity
    double player_speed = 1.;
    switch (pc->action) {
        case ACTION_BOW_AIMING:
            player_speed = AIMING_PLAYER_SPEED;
            break;
        case ACTION_DASHING:
            player_speed = DASHING_PLAYER_SPEED;
            break;
        case ACTION_NONE:
        default:
            player_speed = DEFAULT_PLAYER_SPEED;
            break;
    }

    // Determine the angle
    if ( pc->action == ACTION_DASHING ){
        // Prevent change in direction of the player
        pc->walking = true; // Even if player lets go of buttons
    } else if ((int) inState->RIGHT - (int) inState->LEFT != 0 || (int) inState->DOWN - (int) inState->UP != 0) {
        pc->angle = atan2((double) inState->DOWN - (double) inState->UP, (double) inState->RIGHT - (double) inState->LEFT);
        pc->walking = true;
    } else {
        // Keep current angle
        pc->walking = false;
    }

    // Move player according to walking and angle
    if ( pc->walking ) {
        *playerVelocity = FPOINT_VelocityFromAngle(pc->angle, player_speed);
    } else {
        *playerVelocity = (SDL_FPoint) {0., 0.};
    }
}
