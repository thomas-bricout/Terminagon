#ifndef PLAYER_H
#define PLAYER_H

#include "input.h"
#include "entity_pool.h"
#include <SDL2/SDL.h>

typedef enum {
    ACTION_NONE,
    ACTION_DASHING,
    ACTION_BOW_AIMING,
    ACTION_SHIELDING,
} PlayerAction;

typedef struct {
    float angle;
    bool walking;
    PlayerAction action;
    double actionTimeStamp;
} PlayerComponent;

void PLAYER_System(EntityPool *pool, InState *inState, double current_time);
PlayerComponent PLAYER_NewComponent();

#endif