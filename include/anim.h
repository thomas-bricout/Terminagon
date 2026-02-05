#ifndef ANIM_H
#define ANIM_H

typedef enum {
    ANIM_DEATH,
} Animation;

typedef struct {
    Animation anim;
    double TimeStamp;
} AnimComponent;

#include "entity_pool.h"

void ANIM_System(EntityPool *pool, double current_time);

#endif