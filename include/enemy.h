#ifndef ENEMY_H
#define ENEMY_H

typedef enum {
    OCTOROK,
    MOBLIN,
    LEEVER_BLUE,
    LEEVER_RED
} EnemyType;

typedef enum {
    ENEMY_STILL,
    ENEMY_WALK,
    ENEMY_SHOOTING,
    ENEMY_SPAWN,
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
EntityID ENEMY_SpawnMoblin(EntityPool *pool, SDL_FPoint position);
EntityID ENEMY_SpawnLeever(EntityPool *pool, SDL_FPoint position, double current_time, int color);
EntityID ENEMY_SpawnDeathAnim(EntityPool *pool, SDL_FPoint position, double current_time);
int ENEMY_LocateNearestTarget(EntityPool *pool, SDL_FPoint position, int index_enemy);
EntityID ENEMY_SpawnEnemyProjectile(EntityPool *pool, SDL_FPoint position, int direction, EnemyType type);

#endif