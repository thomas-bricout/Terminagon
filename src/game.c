#include <SDL2/SDL.h>

#include "game.h"
#include "assets.h"
#include "entity_pool.h"

const double TARGET_FPS = 60.0;
const double TARGET_FRAME_TIME =  1000.0 / TARGET_FPS; // in ms

void GAME_init(Game *game, SDL_Renderer *renderer, SDL_Window *window, AssetManager *asset_manager, EntityPool *pool) {
    game->renderer = renderer;
    game->window = window;
    game->asset_manager = asset_manager;
    game->pool = pool;
}

SDL_bool readEvents(Game *game) {
    SDL_Event event;
    SDL_bool quit = SDL_FALSE;

    while (SDL_PollEvent(&event) && !quit) {
        SDL_Log("TREATING EVENT: TYPE: %d, TIMESTAMP: %dms", event.type, event.common.timestamp);

        switch (event.type) {
            // Meta events
            case SDL_QUIT:
                quit = SDL_TRUE;
                break;
            case SDL_WINDOWEVENT:
                break;

            // Game play related event
            case SDL_KEYDOWN:
                break;
            case SDL_KEYUP:
                break;
            case SDL_MOUSEMOTION:
                break;
            case SDL_MOUSEBUTTONDOWN:
                break;
        }
    }

    return quit;
}

// Gestion de la boucle principale, et de la limitation des fps
void GAME_run(Game *game) {
    SDL_bool quit = SDL_FALSE;

    Uint64 perf_freq = SDL_GetPerformanceFrequency();
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;

    double deltaTime = 0;
    double FPS = 0;

    while (!quit) {
        // Calculating deltaTime, FPS and logging them
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();

        deltaTime = (double)((NOW - LAST)*1000) / (double)perf_freq;
        FPS = 1000 / deltaTime;
        SDL_Log("deltaTime: %7.2fms FPS: %4.1f", deltaTime, FPS);

        // Read game events
        quit = readEvents(game);
    
        
        // Création du rendu
        POOL_Display_All(game->asset_manager, game->pool, game->renderer);

        // Affichage à l'écran
        SDL_RenderPresent(game->renderer);
        SDL_RenderClear(game->renderer);

        // Logging
        SDL_Log("Current Entity Count: %d", game->pool->currentCount);

        // Limitation des FPS
        Uint64 frame_end = SDL_GetPerformanceCounter();
        double elapsed = (double)(frame_end - NOW) / (double)perf_freq;
        double delay = TARGET_FRAME_TIME - elapsed;
        if (delay > 0.0) {
            SDL_Delay((Uint32) delay);
        }
    }
}