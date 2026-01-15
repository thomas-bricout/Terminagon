#include <SDL2/SDL.h>

#include "game.h"

void initGame(Game *game, SDL_Renderer *renderer, SDL_Window *window) {
    game->renderer = renderer;
    game->window = window;
}