#include <SDL2/SDL.h>

#include "game.h"
#include "assets.h"
#include "entity_pool.h"
#include "input.h"

const double TARGET_FPS = 60.0;
const double TARGET_FRAME_TIME =  1000.0 / TARGET_FPS; // in ms

void DEBUG_DisplayDebugInfo(Game *game, double deltaTime, double FPS, double elapsed, double current_time);

void GAME_Init(Game *game, SDL_Renderer *renderer, SDL_Window *window, AssetManager *asset_manager, EntityPool *pool, InputSituation *inputSituation) {
    game->renderer = renderer;
    game->window = window;
    game->asset_manager = asset_manager;
    game->pool = pool;
    game->inputSituation = inputSituation;
}

void readEvents(Game *game) {
    SDL_Event event;
    InputSituation *inputSituation = game->inputSituation;

    while (SDL_PollEvent(&event)) {
        SDL_Log("TREATING EVENT: TYPE: %d, TIMESTAMP: %dms", event.type, event.common.timestamp);
        InputSituation_Update(inputSituation, event.type, event.key.keysym.scancode);
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_F11) {
            Uint32 windowFlags = SDL_GetWindowFlags(game->window);
            if (windowFlags & SDL_WINDOW_FULLSCREEN_DESKTOP) {
                SDL_SetWindowFullscreen(game->window, 0);
            } else if (windowFlags & SDL_WINDOW_FULLSCREEN) {
                SDL_SetWindowFullscreen(game->window, 0);
            } else {
                SDL_SetWindowFullscreen(game->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            }
        }
    }

    // Treat holden keys
}

// Gestion de la boucle principale, et de la limitation des fps
void GAME_Run(Game *game) {
    Uint64 perf_freq = SDL_GetPerformanceFrequency();
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;

    double deltaTime = 0;
    double FPS = 0;
    double elapsed = 0;

    while (!game->inputSituation->QUIT) {
        // Calculating deltaTime, FPS and logging them
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        double current_time = (double) NOW*1000/ (double) perf_freq;

        deltaTime = (double)((NOW - LAST)*1000) / (double)perf_freq;
        FPS = 1000 / deltaTime;

        // Read game events
        readEvents(game);
        
        // Physiques
        PlayerSystem(game->pool, game->inputSituation, current_time);
        POOL_ApplyVelocity(game->pool, deltaTime);
        
        // Création du rendu
        POOL_DisplayAll(game->asset_manager, game->pool, game->renderer);
        
        // Logging
        DEBUG_DisplayDebug(game, deltaTime, FPS, elapsed, current_time);

        // Affichage à l'écran
        SDL_RenderPresent(game->renderer);
        SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
        SDL_RenderClear(game->renderer);

        // Limitation des FPS
        Uint64 frame_end = SDL_GetPerformanceCounter();
        elapsed = (double)(frame_end - NOW) / (double)perf_freq;
        double delay = TARGET_FRAME_TIME - elapsed;
        if (delay > 0.0) {
            SDL_Delay((Uint32) delay);
        }
    }
}