#include "enemy.h"
#include "geometry.h"

void ENEMY_System(EntityPool *pool) {
    for (int index_enemy = 0; index_enemy < pool->lastEntitylocation; index_enemy++) {
        if (POOL_LacksComponentFlags(pool, COMPONENT_AI | COMPONENT_POSITION, index_enemy)) { continue; }
        SDL_FPoint *enemy_position = &pool->position[index_enemy];
        SDL_FPoint *enemy_velocity = &pool->velocity[index_enemy];
        EnemyComponent *enemy = &pool->enemy[index_enemy];

        // Search for target
        float ds_min = 10000000.;
        int j_closest = 0;
        for (int index_target = 0; index_target < pool->lastEntitylocation; index_target++) {
            if (index_enemy == index_target) { continue; }
            if (POOL_LacksComponentFlags(pool, COMPONENT_POSITION | COMPONENT_TARGET, index_target)) { continue; }
            // Calculate euclidian distance squared
            float ds = FPOINT_DistanceSquared(pool->position[index_target], pool->position[index_enemy]);
            if (ds < ds_min) { ds_min = ds; j_closest = index_target; }
        }

        // Calculate and gather information about target
        SDL_FPoint target_position = pool->position[j_closest];
        SDL_FPoint relative_position = FPOINT_RelativePoint(target_position, *enemy_position);

        switch (pool->enemy[index_enemy].action) {
            case ENEMY_STILL:
                enemy->action = ENEMY_WALK;
                break;
            case ENEMY_WALK:
                *enemy_velocity = FPOINT_Mul(FPOINT_Normalize(relative_position), .1);
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
    pool->velocity[id.location] = (SDL_FPoint) {0., 0.};

    POOL_AddComponentFlags(pool, COMPONENT_AI | COMPONENT_COLLISIONBOX | COMPONENT_HITBOX | COMPONENT_VELOCITY, id.location);
}