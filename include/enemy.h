#ifndef ENEMY_H
#define ENEMY_H

typedef enum {
    OCTOROK,
} EnemyType;

typedef enum {
    ENEMY_STILL,
    ENEMY_WALK,
} EnemyAction;

typedef struct EnemyComponent EnemyComponent;
struct EnemyComponent {
    EnemyType type;
    EnemyAction action;
    double timeStamp;
};

#include "entity_pool.h"

void ENEMY_System(EntityPool *pool);
EntityID ENEMY_SpawnOctorok(EntityPool *pool, SDL_FPoint position);

#endif