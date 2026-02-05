#include "anim.h"

const double DEATH_ANIM_DISAPPEAR_TIME = 1000.;
void ANIM_System(EntityPool *pool, double current_time) {
    for (int i = 0; i < pool->lastEntitylocation; i++) {
        if (POOL_LacksComponentFlags(pool, COMPONENT_TEXTURE | COMPONENT_ANIM, i)) {continue;}
        double delay = current_time - pool->anim[i].TimeStamp;

        switch (pool->anim[i].anim) {
            case ANIM_DEATH:
            if ((int) (delay / 100.) % 2) {
                pool->tex_location[i] = TEX_DEATH_ANIM3;
            } else {
                pool->tex_location[i] = TEX_DEATH_ANIM1;
            }

            if (delay >= DEATH_ANIM_DISAPPEAR_TIME ) {
                POOL_DestroyEntityFromIndex(pool, i);
            }
            break;
        }
    }
}