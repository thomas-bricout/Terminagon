#ifndef ENEMY_H
#define ENEMY_H

typedef enum {
    OCTOROK,
} EnemyType;

typedef enum {
    ENEMY_STILL,
    ENEMY_WALK,
    ENEMY_SHOOTING,
} EnemyAction;

typedef struct EnemyComponent EnemyComponent;
struct EnemyComponent {
    EnemyType type;
    EnemyAction action;
    double timeStamp;
};

#include "entity_pool.h"

void ENEMY_System(EntityPool *pool, double current_time);
EntityID ENEMY_SpawnOctorok(EntityPool *pool, SDL_FPoint position);
EntityID ENEMY_SpawnDeathAnim(EntityPool *pool, SDL_FPoint position, double current_time);

#endif