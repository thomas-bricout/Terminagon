#include "enemy.h"
#include "geometry.h"

const double OCTOROK_WALK_TIME = 500.;
const double OCTOROK_SHOOTING_TIME = 1000.;
const double OCTOROK_PROJECTILE_SPEED = 0.3;

const double MOBLIN_WALK_TIME = 500.;
const double MOBLIN_SHOOTING_TIME = 1000.;
const double MOBLIN_PROJECTILE_SPEED = 0.3;

void ENEMY_System(EntityPool *pool, double current_time) {
    for (int index_enemy = 0; index_enemy < pool->lastEntitylocation; index_enemy++) {
        if (POOL_LacksComponentFlags(pool, COMPONENT_AI | COMPONENT_POSITION, index_enemy)) { continue; }
        SDL_FPoint *enemy_position = &pool->position[index_enemy];
        SDL_FPoint *enemy_velocity = &pool->velocity[index_enemy];
        TextureIndex *tex = &pool->tex[index_enemy];
        EnemyComponent *enemy = &pool->enemy[index_enemy];

        int j_closest = ENEMY_LocateNearestTarget(pool, *enemy_position, index_enemy);

        // Calculate and gather information about target
        SDL_FPoint target_position = pool->position[j_closest];
        SDL_FPoint relative_position = FPOINT_RelativePoint(target_position, *enemy_position);

        // Calculate direction
        double angle = atan2((double) relative_position.y,(double) relative_position.x);
        int direction = AngleToDirection(angle);

        switch (enemy->type) {
            case MOBLIN:
                *tex = TEX_MOBLIN_RIGHT + direction;
                switch (pool->enemy[index_enemy].action) {
                    case ENEMY_STILL:
                        *enemy_velocity = (SDL_FPoint) {0, 0};
                        enemy->action = ENEMY_WALK;
                        enemy->timeStamp = current_time;
                        break;
                    case ENEMY_WALK:
                        *enemy_velocity = FPOINT_Mul(FPOINT_Normalize(relative_position), .1);
                        if (current_time - enemy->timeStamp > MOBLIN_WALK_TIME) {
                            enemy->timeStamp = current_time;
                            enemy->action = ENEMY_SHOOTING;
                        }
                        break;
                    case ENEMY_SHOOTING:
                        *enemy_velocity = (SDL_FPoint) {0, 0};
                        if (current_time - enemy->timeStamp > OCTOROK_SHOOTING_TIME) {
                            enemy->timeStamp = current_time;
                            enemy->action = ENEMY_WALK;
                            ENEMY_SpawnEnemyProjectile(pool, *enemy_position, direction, MOBLIN);
                        }
                        break;
                }
                break;
            case OCTOROK:
                *tex = TEX_OCTOROK_RIGHT + 2*direction;
                switch (pool->enemy[index_enemy].action) {
                    case ENEMY_STILL:
                        *enemy_velocity = (SDL_FPoint) {0, 0};
                        enemy->action = ENEMY_WALK;
                        enemy->timeStamp = current_time;
                        break;
                    case ENEMY_WALK:
                        *enemy_velocity = FPOINT_Mul(FPOINT_Normalize(relative_position), .1);
                        if (current_time - enemy->timeStamp > OCTOROK_WALK_TIME) {
                            enemy->timeStamp = current_time;
                            enemy->action = ENEMY_SHOOTING;
                        }
                        break;
                    case ENEMY_SHOOTING:
                        *enemy_velocity = (SDL_FPoint) {0, 0};
                        *tex = *tex + 1;
                        if (current_time - enemy->timeStamp > OCTOROK_SHOOTING_TIME) {
                            enemy->timeStamp = current_time;
                            enemy->action = ENEMY_WALK;
                            ENEMY_SpawnEnemyProjectile(pool, *enemy_position, direction, OCTOROK);
                        }
                        break;
            }
            break;
        }
    }
}

EntityID ENEMY_SpawnOctorok(EntityPool *pool, SDL_FPoint position) {
    EntityID id = POOL_NewEntityClassic(pool, TEX_OCTOROK_DOWN, (SDL_Rect) {-40, -40, 80, 80}, position);

    pool->enemy[id.location].action = ENEMY_STILL;
    pool->enemy[id.location].type = OCTOROK;
    pool->collision_box[id.location] = (SDL_FRect) {-40., -40., 80., 80.};
    pool->hit_box[id.location] = (SDL_FRect) {-40., -40., 80., 80.};
    pool->damage_box[id.location] = (SDL_FRect) {-45., -45., 90., 90.};
    pool->health_point[id.location] = 2;
    pool->velocity[id.location] = (SDL_FPoint) {0., 0.};

    POOL_AddComponentFlags(pool, COMPONENT_AI | COMPONENT_COLLISIONBOX | COMPONENT_HITBOX | COMPONENT_VELOCITY | COMPONENT_VELOCITY_FLEXIBLE, id.location);

    return id;
}

EntityID ENEMY_SpawnMoblin(EntityPool *pool, SDL_FPoint position) {
    EntityID id = POOL_NewEntityClassic(pool, TEX_MOBLIN_DOWN, (SDL_Rect) {-40, -40, 80, 80}, position);

    pool->enemy[id.location].action = ENEMY_STILL;
    pool->enemy[id.location].type = MOBLIN;
    pool->collision_box[id.location] = (SDL_FRect) {-40., -40., 80., 80.};
    pool->hit_box[id.location] = (SDL_FRect) {-40., -40., 80., 80.};
    pool->damage_box[id.location] = (SDL_FRect) {-45., -45., 90., 90.};
    pool->health_point[id.location] = 2;
    pool->velocity[id.location] = (SDL_FPoint) {0., 0.};

    POOL_AddComponentFlags(pool, COMPONENT_AI | COMPONENT_COLLISIONBOX | COMPONENT_HITBOX | COMPONENT_VELOCITY | COMPONENT_VELOCITY_FLEXIBLE, id.location);

    return id;
}

EntityID ENEMY_SpawnEnemyProjectile(EntityPool *pool, SDL_FPoint position, int direction, EnemyType type) {
    SDL_FPoint vect;
    switch (direction)
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
    EntityID id;

    switch (type) {
        case OCTOROK:
            id = POOL_NewEntityClassic(pool, TEX_OCTOROK_PROJECTILE, (SDL_Rect) {-25, -25, 50, 50}, position);

            pool->damage_box[id.location] = (SDL_FRect) {-5., -5., 10., 10.};
            pool->collision_box[id.location] = (SDL_FRect) {-4., -4., 9., 9.};
            pool->velocity[id.location] = (SDL_FPoint) {vect.x / 100. * OCTOROK_PROJECTILE_SPEED, vect.y / 100. * OCTOROK_PROJECTILE_SPEED};
            pool->hit_box[id.location] = (SDL_FRect) {-25., -25., 50., 50.};
            pool->health_point[id.location] = 1;
    
            POOL_AddComponentFlags(pool, COMPONENT_DAMAGEBOX | COMPONENT_VELOCITY | COMPONENT_PROJECTILE | COMPONENT_HITBOX, id.location);
            break;
        case MOBLIN:
            TextureIndex tex = TEX_ARROW_RIGHT + direction;
            id = POOL_NewEntityClassic(pool, tex, (SDL_Rect) {-50, -50, 100, 100}, position);

            pool->damage_box[id.location] = (SDL_FRect) {-5., -5., 10., 10.};
            pool->collision_box[id.location] = (SDL_FRect) {-4., -4., 9., 9.};
            pool->velocity[id.location] = (SDL_FPoint) {vect.x / 100. * MOBLIN_PROJECTILE_SPEED, vect.y / 100. * MOBLIN_PROJECTILE_SPEED};
            pool->hit_box[id.location] = (SDL_FRect) {-50., -50., 100., 100.};
            pool->health_point[id.location] = 1;
    
            POOL_AddComponentFlags(pool, COMPONENT_DAMAGEBOX | COMPONENT_VELOCITY | COMPONENT_PROJECTILE | COMPONENT_HITBOX, id.location);
            break;
    }

    return id;
}

EntityID ENEMY_SpawnDeathAnim(EntityPool *pool, SDL_FPoint position, double current_time) {
    EntityID id = POOL_NewEntityClassic(pool, TEX_DEATH_ANIM1, (SDL_Rect) {-50, -50, 100, 100}, position);
    POOL_AddComponentFlags(pool, COMPONENT_ANIM, id.location);
    pool->anim[id.location].TimeStamp = current_time;
    pool->anim[id.location].anim = ANIM_DEATH;
    return id;
} 

int ENEMY_LocateNearestTarget(EntityPool *pool, SDL_FPoint position, int index_enemy) {
    float ds_min = 10000000.;
    int target_closest = 0;
    for (int index_target = 0; index_target < pool->lastEntitylocation; index_target++) {
        if (index_enemy == index_target) { continue; }
        if (POOL_LacksComponentFlags(pool, COMPONENT_POSITION | COMPONENT_TARGET, index_target)) { continue; }
        // Calculate euclidian distance squared
        float ds = FPOINT_DistanceSquared(pool->position[index_target], pool->position[index_enemy]);
        if (ds < ds_min) { ds_min = ds; target_closest = index_target; }
    }

    return target_closest;
}

void Item_drop(EntityPool *pool, int index_enemy){
    int r = rand()%100;
    if(r<50){

    }else if(r<55){
        //resurection
        EntityID id = POOL_NewEntityClassic(pool, TEX_DEATH_ANIM4, (SDL_Rect) {-25, -25, 50, 50}, pool->position[index_enemy]);
        pool->collision_box[id.location] = (SDL_FRect) {-25., -25., 50., 50.};
        POOL_AddComponentFlags(pool, COMPONENT_ITEM | COMPONENT_POSITION, id.location);
    }else if(r<80){
        //heart
        EntityID id = POOL_NewEntityClassic(pool, TEX_HEART_FULL, (SDL_Rect) {-25, -25, 50, 50}, pool->position[index_enemy]);
        pool->collision_box[id.location] = (SDL_FRect) {-25., -25., 50., 50.};
        POOL_AddComponentFlags(pool, COMPONENT_ITEM | COMPONENT_POSITION, id.location);
    }else if(r<100){
        //arrow
        EntityID id = POOL_NewEntityClassic(pool, TEX_ARROW_UP, (SDL_Rect) {-25, -25, 50, 50}, pool->position[index_enemy]);
        pool->collision_box[id.location] = (SDL_FRect) {-25., -25., 50., 50.};
        POOL_AddComponentFlags(pool, COMPONENT_ITEM | COMPONENT_POSITION, id.location);
    }

}