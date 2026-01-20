#ifndef PLAYER_H
#define PLAYER_H

#include "input.h"
#include "entity_pool.h"
#include <SDL2/SDL.h>

typedef struct {

} PlayerComponent;

void PlayerSystem(EntityPool *pool, InputSituation *inputSituation);

#endif