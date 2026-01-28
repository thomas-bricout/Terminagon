#include <SDL2/SDL.h>

#include "entity_pool.h"
#include "geometry.h"
#include "physics.h"

#define size 100

void PHYSICS_MoveAll(Tile map[HAUTEUR][LARGEUR], EntityPool *pool, double deltaTime) {
    for (int i = 0; i < pool->lastEntitylocation; i++) {
        // if (!pool->position_map[i] || !pool->velocity_map[i]) { continue; }
        if (POOL_LacksComponentFlags(pool, COMPONENT_TEXTURE | COMPONENT_VELOCITY, i)) { continue; }

        if (!POOL_LacksComponentFlags(pool, COMPONENT_COLLISIONBOX, i)) { // If entity has a collision map, check possible collisions with every other entities
            bool collided = false;

            SDL_FPoint collider_pos = pool->position[i];
            SDL_FRect collider_box = pool->collision_box[i];
            collider_box = FRECT_Offset(collider_box, FPOINT_ApplyVelocity(collider_pos, pool->velocity[i], deltaTime));

            for (int j = 0; j < pool->lastEntitylocation; j++) { 
                if ( i == j ) { continue; }
                if (POOL_LacksComponentFlags(pool, COMPONENT_COLLISIONBOX | COMPONENT_POSITION, j)) { continue; }

                SDL_FPoint obstacle_pos = pool->position[j];
                SDL_FRect obstacle_box = pool->collision_box[j];

                // Shift collision boxes according to their entity position and velocity
                obstacle_box = FRECT_Offset(obstacle_box, obstacle_pos);

                // Continue there if there is no collision
                if (!SDL_HasIntersectionF(&collider_box, &obstacle_box)) { continue; }

                SDL_FRect intersect_box;
                SDL_IntersectFRect(&collider_box, &obstacle_box, &intersect_box);

                // TODO: Compute proper position
                collided = true;
            }

            int i_map = (int) (collider_box.y/size);
            int i_map_max = (int) ((collider_box.y+collider_box.h)/size)+1;
            if(i_map<0) i_map=0;
            if(i_map_max>HAUTEUR) i_map_max=HAUTEUR;

            int j_map_min = (int) (collider_box.x/size);
            int j_map_max = (int) ((collider_box.x+collider_box.w)/size)+1;
            if(j_map_min<0) j_map_min=0;
            if(j_map_max>LARGEUR) j_map_max=LARGEUR;


            for(;i_map<i_map_max;i_map++){
                for(int j_map=j_map_min;j_map<j_map_max;j_map++){
                    if(map[i_map][j_map].blocking){
                        SDL_FRect obstacle_box = {j_map*size,i_map*size,size,size};
                        if (!SDL_HasIntersectionF(&collider_box, &obstacle_box)) { continue; }
                        collided = true;
                    }
                    
                }
            }

            // Proceed as normal if it does not result in a collision
            if (!collided) {
                pool->position[i] = FPOINT_ApplyVelocity(pool->position[i], pool->velocity[i], deltaTime); 
            }
            // Do not check other possibilities
            continue;
        }

        // ELSE: proceed without dealing with collisions
        pool->position[i] = FPOINT_ApplyVelocity(pool->position[i], pool->velocity[i], deltaTime); 
    }
}

void PHYSICS_DamageAll(Tile map[HAUTEUR][LARGEUR],EntityPool *pool, double deltaTime) {
    for (int i = 0; i < pool->lastEntitylocation; i++) {
        // if (!pool->position_map[i] || !pool->velocity_map[i]) { continue; }
        if (POOL_LacksComponentFlags(pool, COMPONENT_TEXTURE, i)) { continue; }

        if (!POOL_LacksComponentFlags(pool, COMPONENT_DAMAGEBOX, i)) { // If entity has a collision map, check possible collisions with every other entities
            bool collided = false;

            SDL_FPoint collider_pos = pool->position[i];
            SDL_FRect collider_box = pool->damage_box[i];
            collider_box = FRECT_Offset(collider_box, collider_pos);

            for (int j = 0; j < pool->lastEntitylocation; j++) { 
                if ( i == j ) { continue; }
                if (POOL_LacksComponentFlags(pool, COMPONENT_COLLISIONBOX | COMPONENT_POSITION, j)) { continue; }

                SDL_FPoint obstacle_pos = pool->position[j];
                SDL_FRect obstacle_box = pool->collision_box[j];

                // Shift collision boxes according to their entity position and velocity
                obstacle_box = FRECT_Offset(obstacle_box, obstacle_pos);

                // Continue there if there is no collision
                if (!SDL_HasIntersectionF(&collider_box, &obstacle_box)) { continue; }

                SDL_FRect intersect_box;
                SDL_IntersectFRect(&collider_box, &obstacle_box, &intersect_box);

                // TODO: Compute proper position
                collided = true;
            }

            int i_map = (int) (collider_box.y/size);
            int i_map_max = (int) ((collider_box.y+collider_box.h)/size)+1;
            if(i_map<0) i_map=0;
            if(i_map_max>HAUTEUR) i_map_max=HAUTEUR;

            int j_map_min = (int) (collider_box.x/size);
            int j_map_max = (int) ((collider_box.x+collider_box.w)/size)+1;
            if(j_map_min<0) j_map_min=0;
            if(j_map_max>LARGEUR) j_map_max=LARGEUR;


            for(;i_map<i_map_max;i_map++){
                for(int j_map=j_map_min;j_map<j_map_max;j_map++){
                    if(map[i_map][j_map].blocking){
                        SDL_FRect obstacle_box = {j_map*size,i_map*size,size,size};
                        if (!SDL_HasIntersectionF(&collider_box, &obstacle_box)) { continue; }
                        collided = true;
                    }
                    
                }
            }

            // Proceed as normal if it does not result in a collision
            if (collided) {
                if(!POOL_LacksComponentFlags(pool, COMPONENT_PROJECTILE, i)) POOL_DestroyEntity(pool,pool->id[i]);

                
            }
            // Do not check other possibilities
            continue;
        }
    }
}

const double invincibility_ms = 1000. ;
void PHYSICS_UpdateHitPoints(EntityPool *pool, double current_time) {
    for (int i = 0; i < pool->lastEntitylocation; i++) {
        if (POOL_LacksComponentFlags(pool, COMPONENT_HITBOX | COMPONENT_POSITION, i)) { continue; }
        if (current_time - pool->last_hit[i] <= invincibility_ms) { continue; }

        SDL_FRect hitbox = pool->hit_box[i];
        hitbox = FRECT_Offset(hitbox, pool->position[i]);

        for (int j = 0; j < pool->lastEntitylocation; j++) {
            if (i == j) { continue; }
            if (POOL_LacksComponentFlags(pool, COMPONENT_DAMAGEBOX | COMPONENT_POSITION, j)) { continue; }
            SDL_FRect damagebox = pool->damage_box[j];
            damagebox = FRECT_Offset(damagebox, pool->position[j]);
            
            if (!SDL_HasIntersectionF(&hitbox, &damagebox)) { continue; }
            pool->health_point[i] --;
            pool->last_hit[i] = current_time;
            if (pool->health_point[i] <= 0) {
                printf("death\n");
            }
        }
    }
}