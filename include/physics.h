#ifndef PHYSICS_H
#define PHYSICS_H

#include "entity_pool.h"

void PHYSICS_MoveAll(Tile map[HAUTEUR][LARGEUR],EntityPool *pool, double deltaTime);
void PHYSICS_DamageAll(Tile map[HAUTEUR][LARGEUR],EntityPool *pool, double deltaTime);
void PHYSICS_UpdateHitPoints(EntityPool *pool, double now);

#endif