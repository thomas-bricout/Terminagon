#include <SDL2/SDL.h>

#include "entity_pool.h"
#include "geometry.h"
#include "physics.h"

void PHYSICS_MoveAll(EntityPool *pool, double deltaTime) {
    for (int i = 0; i < pool->lastEntitylocation; i++) {
        // if (!pool->position_map[i] || !pool->velocity_map[i]) { continue; }
        if (POOL_LacksComponentFlags(pool, COMPONENT_TEXTURE | COMPONENT_VELOCITY, i)) { continue; }

        if (!POOL_LacksComponentFlags(pool, COMPONENT_COLLISIONBOX, i)) { // If entity has a collision map, check possible collisions with every other entities
            bool collided = false;
            for (int j = 0; j < pool->lastEntitylocation; j++) { 
                if ( i == j ) { continue; }
                if (POOL_LacksComponentFlags(pool, COMPONENT_COLLISIONBOX | COMPONENT_POSITION, i)) { continue; }

                SDL_FPoint obstacle_pos = pool->position[j];
                SDL_FRect obstacle_box = pool->collision_box[j];

                SDL_FPoint collider_pos = pool->position[i];
                SDL_FRect collider_box = pool->collision_box[i];

                // Shift collision boxes according to their entity position and velocity
                obstacle_box = FRECT_Offset(obstacle_box, obstacle_pos);
                collider_box = FRECT_Offset(collider_box, FPOINT_ApplyVelocity(collider_pos, pool->velocity[i], deltaTime));

                // Continue there if there is no collision
                if (!SDL_HasIntersectionF(&collider_box, &obstacle_box)) { continue; }

                SDL_FRect intersect_box;
                SDL_IntersectFRect(&collider_box, &obstacle_box, &intersect_box);

                // TODO: Compute proper position
                collided = true;
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