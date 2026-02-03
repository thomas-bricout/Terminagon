#include "input.h"
#include "entity_pool.h"
#include "assets.h"
#include "player.h"
#include "geometry.h"

#include <math.h>

const float DEFAULT_PLAYER_SPEED = 0.2;
const float SPRINT_PLAYER_SPEED = 0.4;
const float DASHING_PLAYER_SPEED = 2.0;
const float AIMING_PLAYER_SPEED = 0.1;

const double BOW_AIMING_TIME = 500;
const double DASHING_TIME = 200;
const double RECOVERY_TIME = 100;
const double ATTACK_TIME = 100;

const float ARROW_SPEED = 0.5;

PlayerComponent PLAYER_NewComponent(InState *inState) {
    PlayerComponent p_c;
    p_c.action = ACTION_NONE;
    p_c.actionTimeStamp = 0.;
    p_c.angle = 0;
    p_c.walking = false;
    p_c.inState = inState;

    return p_c;
}

int AngleToDirection(double angle) {
    angle = angle * 180. / 3.1415;
    int orientation = 0;
    if (angle <= 45 && angle >= -45) { // Right
        orientation = 0;
    } else if (angle >= 135 || angle <= -135) { // Left
        orientation = 2;
    } else if (angle >= -135 && angle <= 45) { // Up
        orientation = 1;
    } else if (angle >= 45 && angle <= 135) { // Down
        orientation = 3;
    }
    return orientation;
}

EntityID POOL_SpawnArrow(EntityPool *pool, SDL_FPoint position, double angle) {
    SDL_Rect display_rect = {-50, -50, 100, 100};
    SDL_FPoint vect;
    switch (AngleToDirection(angle))
    {
    default:
    case 0:
        vect.x=100.;
        vect.y=0;
        break;
    case 1:
        vect.x=0.;
        vect.y=-100.;
        break;
    case 2:
        vect.x=-100.;
        vect.y=0.;
        break;
    case 3:
        vect.x=0.;
        vect.y=100.;
        break;
    }
    position = FPOINT_Offset(position,vect);  //(SDL_FPoint) {cos(angle)*100,sin(angle)*100}
    int orientation = AngleToDirection(angle);
    TextureLocation tex = TEX_ARROW_RIGHT + orientation;

    EntityID id = POOL_NewEntityClassic(pool, tex, display_rect, position);

    SDL_FRect damage_rect = {-20, -20, 40, 40};
    pool->velocity[id.location] = (SDL_FPoint) { ARROW_SPEED * (vect.x/100) , ARROW_SPEED * (vect.y/100) }; //(SDL_FPoint) { ARROW_SPEED * cos(angle) , ARROW_SPEED * sin(angle) }
    POOL_AddComponentFlags(pool, COMPONENT_VELOCITY | COMPONENT_DAMAGEBOX | COMPONENT_PROJECTILE, id.location);
    pool->damage_box[id.location]=damage_rect;

    return id;
}

void PLAYER_System(Game *game, double current_time) {
    EntityPool *pool = game->pool;

    for (int i = 0; i < pool->player_amount; i++) {
        int playerLocation = pool->player_id[i].location;

        SDL_FPoint *playerVelocity = &pool->velocity[playerLocation];
        SDL_FPoint *playerPosition = &pool->position[playerLocation];
        PlayerComponent *pc = &pool->player_component[i];
        InState *inState = pc->inState;
        double elapsed_time = current_time - pc->actionTimeStamp;
    
        // Whether player is ready to start a new action
        bool available = pc->action == 0 && elapsed_time >= RECOVERY_TIME;
        if (available) { // Treat new actions
            if (inState->W) {
                pc->action = ACTION_SHIELDING;
                pc->actionTimeStamp = current_time;
            } else if (inState->C) {
                pc->action = ACTION_BOW_AIMING;
                pc->actionTimeStamp = current_time;
            } else if (inState->V) {
                pc->action = ACTION_SWORD;
                pc->actionTimeStamp = current_time;

                if (game->sound[0])
                    Mix_PlayChannel(-1, game->sound[0], 0);

                // TODO: Placre la damage box devant le joueur
                POOL_AddComponentFlags(pool, COMPONENT_DAMAGEBOX, playerLocation);
                double angle = pc->angle * 180. / 3.1415;
                if (angle <= 45 && angle >= -45) { // Right
                    pool->damage_box[playerLocation] = (SDL_FRect) {100, -50, 100, 100};
                } else if (angle >= 135 || angle <= -135) { // Left
                    pool->damage_box[playerLocation] = (SDL_FRect) {-200, -50, 100, 100};
                } else if (angle >= -135 && angle <= 45) { // Up
                    pool->damage_box[playerLocation] = (SDL_FRect) {-50, -200, 100, 100};
                } else if (angle >= 45 && angle <= 135) { // Down
                    pool->damage_box[playerLocation] = (SDL_FRect) {-50, 100, 100, 100};
                }
            }else if(inState->X) {
                pc->action = ACTION_SPRINT;
                pc->actionTimeStamp = current_time;
            }
        } else {        // Treat current action
            switch(pc->action) {
                case ACTION_SPRINT:
                    if (!inState->X) {
                        pc->action = ACTION_NONE;
                        pc->actionTimeStamp = current_time;
                    }
                    break;
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
                case ACTION_SWORD:
                    if (elapsed_time >= ATTACK_TIME) {
                        pc->action = ACTION_NONE;
                        pc->actionTimeStamp = current_time;

                        POOL_RemoveComponentFlags(pool, COMPONENT_DAMAGEBOX, playerLocation);
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
            case ACTION_SPRINT:
                player_speed = SPRINT_PLAYER_SPEED;
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
        } else if (inState->joystick_left.x!=0 || inState->joystick_left.y!=0) {
            pc->angle = atan2((double) inState->joystick_left.y, (double) inState->joystick_left.x);
            pc->walking = true;
        } else if((int) inState->RIGHT - (int) inState->LEFT != 0 || (int) inState->DOWN - (int) inState->UP != 0){
            pc->angle = atan2((double) inState->DOWN - (double) inState->UP, (double) inState->RIGHT - (double) inState->LEFT);
            pc->walking = true;
        }else {
            // Keep current angle
            pc->walking = false;
        }

        // Move player according to walking and angle
        if ( pc->walking ) {
            *playerVelocity = FPOINT_VelocityFromAngle(pc->angle, player_speed);
        } else {
            *playerVelocity = (SDL_FPoint) {0., 0.};
        }

        // Animate the player
        PLAYER_Animate(pool, i, current_time);


        if(i==0){
            int window_h = SCREEN_H;
            int window_w = SCREEN_W;
            SDL_GetWindowSize(game->window, &window_w, &window_h);
            game->camera_pos.x = playerPosition->x - window_w/2;
            game->camera_pos.y = playerPosition->y - window_h/2;
        }
    }

    if(pool->player_amount==2){
        int player1Location = pool->player_id[0].location;
        SDL_FPoint *player1Position = &pool->position[player1Location];
        int player2Location = pool->player_id[1].location;
        SDL_FPoint *player2Position = &pool->position[player2Location];
        int window_h = SCREEN_H;
        int window_w = SCREEN_W;
        SDL_GetWindowSize(game->window, &window_w, &window_h);
        double window_w_lim=(window_w*3)/5;
        double window_h_lim=(window_h*3)/5;

        int p1px=player1Position->x;
        int p2px=player2Position->x;
        int p1py=player1Position->y;
        int p2py=player2Position->y;
        int dist_x= SDL_abs(player1Position->x-player2Position->x);
        int dist_y= SDL_abs(player1Position->y-player2Position->y);
        int signe_x=1;
        int signe_y=1;
        if(player1Position->x-player2Position->x>0){
            signe_x=-1;
        }
        if(player1Position->y-player2Position->y>0){
            signe_y=-1;
        }
        if(dist_x>window_w_lim){
            p2px=p1px+window_w_lim*signe_x;
        }
        if(dist_y>window_h_lim){
            p2py=p1py+window_h_lim*signe_y;
        }
        game->camera_pos.x =(p1px+p2px)/2 - window_w/2;
        game->camera_pos.y =(p1py+p2py)/2 - window_h/2;
        printf("%f %f\n",game->camera_pos.x,game->camera_pos.y);
    }
}

void PLAYER_Animate(EntityPool *pool, int playerIndex, double current_time) {
    int playerLocation = pool->player_id[playerIndex].location;
    TextureLocation *tex = &pool->tex_location[playerLocation];
    PlayerComponent *pc = &pool->player_component[playerIndex];
    SDL_Rect *rect = &pool->display_rect[playerLocation];
    double delay = current_time - pc->actionTimeStamp;
    
    int orientation = AngleToDirection(pc->angle);

    switch (pc->action) {
        case ACTION_NONE:
            *tex = orientation * 2 + TEX_PLAYER_RIGHT;
            if (pc->walking && (int) (delay / 400.) % 2) {
                *tex += 1;
            }
            break;
        case ACTION_SPRINT:
            *tex = orientation * 2 + TEX_PLAYER_RIGHT;
            if (pc->walking && (int) (delay / 200.) % 2) {
                *tex += 1;
            }
            break;
        case ACTION_BOW_AIMING:
            *tex = orientation + TEX_PLAYER_AIMING_RIGHT;
            break;
        case ACTION_DASHING:
            break;
        case ACTION_SHIELDING:
            break;
        case ACTION_SWORD:
            *tex = orientation + TEX_PLAYER_SWORD_RIGHT;
            break;
    }
    
    // Adapt Display_Rect to the size of the image
    switch (*tex) {
        default:
        case TEX_PLAYER_RIGHT   :
        case TEX_PLAYER_RIGHT_W :
        case TEX_PLAYER_UP      :
        case TEX_PLAYER_UP_W    :
        case TEX_PLAYER_LEFT    :
        case TEX_PLAYER_LEFT_W  :
        case TEX_PLAYER_DOWN    :
        case TEX_PLAYER_DOWN_W  :
            *rect = (SDL_Rect) {-50, -50, 100, 100};
            break;
        case TEX_PLAYER_SWORD_RIGHT :
            *rect = (SDL_Rect) {-50, -50, 200, 100};
            break;
        case TEX_PLAYER_SWORD_UP    :
            *rect = (SDL_Rect) {-50, -150, 100, 200};
            break;
        case TEX_PLAYER_SWORD_LEFT  :
            *rect = (SDL_Rect) {-150, -50, 200, 100};
            break;
        case TEX_PLAYER_SWORD_DOWN  :
            *rect = (SDL_Rect) {-50, -50, 100, 200};
            break;
    }
}

