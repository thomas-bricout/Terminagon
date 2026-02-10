#include <SDL2/SDL.h>

#include "entity_pool.h"
#include "geometry.h"
#include "physics.h"


void PHYSICS_MoveAll(Tile map[HAUTEUR][LARGEUR], EntityPool *pool, double deltaTime) {
    for (int i = 0; i < pool->lastEntitylocation; i++) {
        // if (!pool->position_map[i] || !pool->velocity_map[i]) { continue; }
        if (POOL_LacksComponentFlags(pool, COMPONENT_TEXTURE | COMPONENT_VELOCITY, i)) { continue; }

        if (!POOL_LacksComponentFlags(pool, COMPONENT_COLLISIONBOX, i)) { // If entity has a collision map, check possible collisions with every other entities
            bool collided = false;
            int axis = 0;



            SDL_FPoint collider_pos = pool->position[i];
            SDL_FRect collider_box = FRECT_Offset(pool->collision_box[i], FPOINT_ApplyVelocity(collider_pos, pool->velocity[i], deltaTime));

            SDL_FRect collider_box_actuel = FRECT_Offset(pool->collision_box[i], collider_pos);

            SDL_FPoint velocity_x = {pool->velocity[i].x,0.};
            SDL_FPoint velocity_y = {0.,pool->velocity[i].y};
            SDL_FRect collider_box_X = FRECT_Offset(pool->collision_box[i], FPOINT_ApplyVelocity(collider_pos, velocity_x, deltaTime));
            SDL_FRect collider_box_Y = FRECT_Offset(pool->collision_box[i], FPOINT_ApplyVelocity(collider_pos, velocity_y, deltaTime));

            for (int j = 0; j < pool->lastEntitylocation; j++) { 
                if ( i == j ) { continue; }
                if (POOL_LacksComponentFlags(pool, COMPONENT_COLLISIONBOX | COMPONENT_POSITION, j)) { continue; }

                SDL_FPoint obstacle_pos = pool->position[j];
                SDL_FRect obstacle_box = pool->collision_box[j];

                // Shift collision boxes according to their entity position and velocity
                obstacle_box = FRECT_Offset(obstacle_box, obstacle_pos);

                // Continue there if there is no collision
                if (!SDL_HasIntersectionF(&collider_box, &obstacle_box)) { continue; }

                //SDL_FRect intersect_box;
                //SDL_IntersectFRect(&collider_box, &obstacle_box, &intersect_box);

                // TODO: Compute proper position
                

                if(axis == 0){//!POOL_LacksComponentFlags(pool, COMPONENT_VELOCITY_FLEXIBLE, i) &&
                    if (!SDL_HasIntersectionF(&collider_box_X, &obstacle_box)) {
                        axis = 1;
                        collider_box=collider_box_X;
                        continue;
                    }
                    if (!SDL_HasIntersectionF(&collider_box_Y, &obstacle_box)) {
                        axis = 2;
                        collider_box=collider_box_Y;
                        continue;
                    }
                }
                if(SDL_HasIntersectionF(&collider_box_actuel, &obstacle_box)) continue;
                collided = true;
                goto resultat;
            }

            int i_map = (int) (collider_box.y/TILE_SIZE);
            int i_map_max = (int) ((collider_box.y+collider_box.h)/TILE_SIZE)+1;
            if(i_map<0) i_map=0;
            if(i_map_max>HAUTEUR) i_map_max=HAUTEUR;

            int j_map_min = (int) (collider_box.x/TILE_SIZE);
            int j_map_max = (int) ((collider_box.x+collider_box.w)/TILE_SIZE)+1;
            if(j_map_min<0) j_map_min=0;
            if(j_map_max>LARGEUR) j_map_max=LARGEUR;


            for(;i_map<i_map_max;i_map++){
                for(int j_map=j_map_min;j_map<j_map_max;j_map++){
                    if(map[i_map][j_map].blocking){
                        SDL_FRect obstacle_box = {j_map*TILE_SIZE,i_map*TILE_SIZE,TILE_SIZE,TILE_SIZE};
                        if (!SDL_HasIntersectionF(&collider_box, &obstacle_box)) continue;

                        if(axis == 0 || axis == 3){//!POOL_LacksComponentFlags(pool, COMPONENT_VELOCITY_FLEXIBLE, i) &&
                            bool test_x = !SDL_HasIntersectionF(&collider_box_X, &obstacle_box);
                            bool test_y = !SDL_HasIntersectionF(&collider_box_Y, &obstacle_box);
                            if (test_x && test_y){
                                axis = 3;
                                continue;
                            }
                            
                            if (test_x) {
                                axis = 1;
                                collider_box=collider_box_X;
                                continue;
                            }
                            if (test_y) {
                                axis = 2;
                                collider_box=collider_box_Y;
                                continue;
                            }
                        }


                        if(SDL_HasIntersectionF(&collider_box_actuel, &obstacle_box)) continue;
                        
                        collided = true;
                        goto resultat;
                    }
                }
            }

resultat:
            // Proceed as normal if it does not result in a collision
            if (!collided) {
                switch (axis){
                    case 1:
                    case 3:
                        pool->position[i] = FPOINT_ApplyVelocity(pool->position[i], velocity_x, deltaTime); 
                        break;
                    
                    case 2:
                        pool->position[i] = FPOINT_ApplyVelocity(pool->position[i], velocity_y, deltaTime); 
                        break;
                
                    default:
                        pool->position[i] = FPOINT_ApplyVelocity(pool->position[i], pool->velocity[i], deltaTime); 
                }
            }
            // Do not check other possibilities
        }else{
            // ELSE: proceed without dealing with collisions
            pool->position[i] = FPOINT_ApplyVelocity(pool->position[i], pool->velocity[i], deltaTime); 
        }
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

            int i_map = (int) (collider_box.y/TILE_SIZE);
            int i_map_max = (int) ((collider_box.y+collider_box.h)/TILE_SIZE)+1;
            if(i_map<0) i_map=0;
            if(i_map_max>HAUTEUR) i_map_max=HAUTEUR;

            int j_map_min = (int) (collider_box.x/TILE_SIZE);
            int j_map_max = (int) ((collider_box.x+collider_box.w)/TILE_SIZE)+1;
            if(j_map_min<0) j_map_min=0;
            if(j_map_max>LARGEUR) j_map_max=LARGEUR;


            for(;i_map<i_map_max;i_map++){
                for(int j_map=j_map_min;j_map<j_map_max;j_map++){
                    if(map[i_map][j_map].blocking){
                        SDL_FRect obstacle_box = {j_map*TILE_SIZE,i_map*TILE_SIZE,TILE_SIZE,TILE_SIZE};
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
                if (POOL_LacksComponentFlags(pool, COMPONENT_PLAYER, i)) {
                    POOL_DestroyEntityFromIndex(pool, i);
                    ENEMY_SpawnDeathAnim(pool, pool->position[i], current_time);
                } else {
                    //POOL_RemoveComponentFlags(pool, COMPONENT_POSITION, i);
                    int j;
                    if(pool->player_id[0].location!=i){
                        j=pool->player_id[0].location;
                    }else{
                        j=pool->player_id[1].location;
                    }
                    
                    pool->position[i] = pool->position[j];
                    pool->velocity[i] = (SDL_FPoint) {0.,0.};
                    pool->health_point[i] = 5;
                }
            }
        }
    }
}