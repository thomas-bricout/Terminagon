#ifndef GAME_H
#define GAME_H

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
} Game;

void initGame(Game *game, SDL_Renderer *renderer, SDL_Window *window);

#endif