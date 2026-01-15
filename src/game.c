#include <SDL2/SDL.h>

#include "game.h"
#include "assets.h"

#define MAX_FPS 60
static double FPS = MAX_FPS;
static double FrameTime = 1.0 / MAX_FPS;

void GAME_init(Game *game, SDL_Renderer *renderer, SDL_Window *window, AssetManager *asset_manager) {
    game->renderer = renderer;
    game->window = window;
    game->asset_manager = asset_manager;
}

SDL_bool readEvents(Game *game) {
    SDL_Event event;
    SDL_bool quit = SDL_FALSE;

    while (SDL_PollEvent(&event) && !quit) {
        // Deal with events here
        switch (event.type) {
            case SDL_QUIT:
                quit = SDL_TRUE;
                break;
        }
    }

    return quit;
}

// Gestion de la boucle principale, et de la limitation des fps
void GAME_run(Game *game) {
    SDL_bool quit = SDL_FALSE;

    while (!quit) {
        quit = readEvents(game);

        // Limitation des FPS
    }
}