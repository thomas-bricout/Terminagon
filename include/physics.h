#ifndef PHYSICS_H
#define PHYSICS_H

#include "entity_pool.h"

void spawn_mobs(Tile map[HAUTEUR][LARGEUR], EntityPool *pool, double deltaTime);
void PHYSICS_MoveAll(Tile map[HAUTEUR][LARGEUR],EntityPool *pool, double deltaTime);
void PHYSICS_item(EntityPool *pool);
void PHYSICS_DamageAll(Tile map[HAUTEUR][LARGEUR],EntityPool *pool, double deltaTime);
void PHYSICS_UpdateHitPoints(EntityPool *pool, double now);
void Item_drop(EntityPool *pool, int index_enemy);

#endif