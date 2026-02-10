#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>

#include "game.h"

void draw_ui(Game *game);
void draw_arrow_count(AssetManager *assetManager, SDL_Renderer *renderer, int arrow_count, int player);

#endif