#ifndef DEBUG_H
#define DEBUG_H

#include "game.h"

void DEBUG_DisplayDebug(Game *game, double deltaTime, double FPS, double elapsed, double current_time);
void DEBUG_DisplayDebugInfo(Game *game, double deltaTime, double FPS, double elapsed, double current_time);

void DEBUG_DisplayCollisionRects(Game *game);
void DEBUG_DisplayDamageRects(Game *game);
void DEBUG_DisplayHitboxRects(Game *game);

#endif