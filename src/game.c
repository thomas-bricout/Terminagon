#include <SDL2/SDL.h>

#include "game.h"
#include "assets.h"

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

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    double FPS = 0;

    while (!quit) {
        // Calculating deltaTime
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();

        deltaTime = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency() );
        FPS = 1000 / deltaTime;
        SDL_Log("deltaTime: %fms        FPS: %f", deltaTime, FPS);

        // Read game events
        quit = readEvents(game);
    
        // Bidoouillage pour avoir un sprite à l'écran
        SDL_Rect src = {0, 0, 1000, 1000};
        SDL_Rect dst = {0, 0, 1000, 1000};
        SDL_RenderCopy(game->renderer, game->asset_manager->asset_table, &src, &dst);

        // Affichage à l'écran
        SDL_RenderPresent(game->renderer);
        SDL_RenderClear(game->renderer);

        // Limitation des FPS
    }
}