#include "anim.h"

const double DEATH_ANIM_DISAPPEAR_TIME = 1000.;
void ANIM_System(EntityPool *pool, double current_time) {
    for (int i = 0; i < pool->lastEntitylocation; i++) {
        if (POOL_LacksComponentFlags(pool, COMPONENT_TEXTURE | COMPONENT_ANIM, i)) {continue;}
        double delay = current_time - pool->anim[i].TimeStamp;

        switch (pool->anim[i].anim) {
            case ANIM_DEATH:
                if ((int) (delay / 100.) % 2) {
                    pool->tex[i] = TEX_DEATH_ANIM3;
                } else {
                    pool->tex[i] = TEX_DEATH_ANIM1;
                }

                if (delay >= DEATH_ANIM_DISAPPEAR_TIME ) {
                    POOL_DestroyEntityFromIndex(pool, i);
                }
                break;
        }
    }
}

const double HIT_ANIMATION_DURATION = 1000.;
const double HIT_ANIMATION_PERIOD = 100.;

void ANIM_Hurt(EntityPool *pool, double current_time) {
    for (int i = 0; i < pool->lastEntitylocation; i++) {
        if (POOL_LacksComponentFlags(pool, COMPONENT_TEXTURE | COMPONENT_HITBOX, i)) continue;
        if ( current_time - pool->last_hit[i] >= HIT_ANIMATION_DURATION ) continue;

        if ( ((int) (current_time / HIT_ANIMATION_PERIOD)) % 2 ) {
            pool->tex[i] = TEX_VOID;
        }
    }
}