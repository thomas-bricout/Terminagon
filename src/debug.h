#ifndef DEBUG_H
#define DEBUG_H

#include "game.h"

void DEBUG_DisplayDebug(Game *game, double deltaTime, double FPS, double elapsed, double current_time);

void DEBUG_DisplayDebugInfo(Game *game, double deltaTime, double FPS, double elapsed, double current_time);
void DEBUG_DisplayCollisionRects(EntityPool *pool, SDL_Renderer *renderer);
void DEBUG_DisplayDamageRects(EntityPool *pool, SDL_Renderer *renderer);
void DEBUG_DisplayHitboxRects(EntityPool *pool, SDL_Renderer *renderer);

#endif