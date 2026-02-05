#ifndef PLAYER_H
#define PLAYER_H

#include "input.h"
#include "entity_pool.h"
#include <SDL2/SDL.h>

typedef enum {
    ACTION_NONE,
    ACTION_SPRINT,
    ACTION_DASHING,
    ACTION_BOW_AIMING,
    ACTION_SHIELDING,
    ACTION_SWORD
} PlayerAction;

typedef struct {
    InState *inState;
    float angle;
    bool walking;
    PlayerAction action;
    double actionTimeStamp;
} PlayerComponent;

void PLAYER_System(Game *game, double current_time);
PlayerComponent PLAYER_NewComponent(InState *inState);
void PLAYER_Animate(EntityPool *pool, int playerIndex, double current_time);
EntityID PLAYER_Spawn(EntityPool *pool, InState *inState, SDL_FPoint position);

#endif