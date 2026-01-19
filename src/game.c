#include <SDL2/SDL.h>

#include "game.h"
#include "assets.h"
#include "entity_pool.h"

const double TARGET_FPS = 60.0;
const double TARGET_FRAME_TIME =  1000.0 / TARGET_FPS; // in ms

const double MAX_PLAYER_SPEED = 0.1;

void GAME_Init(Game *game, SDL_Renderer *renderer, SDL_Window *window, AssetManager *asset_manager, EntityPool *pool) {
    game->renderer = renderer;
    game->window = window;
    game->asset_manager = asset_manager;
    game->pool = pool;
}

SDL_bool readEvents(Game *game) {
    SDL_Event event;
    SDL_bool quit = false;

    static bool HoldingUP;
    static bool HoldingDOWN;
    static bool HoldingLEFT;
    static bool HoldingRIGHT;

    while (SDL_PollEvent(&event) && !quit) {
        SDL_Log("TREATING EVENT: TYPE: %d, TIMESTAMP: %dms", event.type, event.common.timestamp);

        switch (event.type) {
            // Meta events
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_WINDOWEVENT:
                break;

            // Game play related event
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_UP:
                        HoldingUP = true;
                        break;
                    case SDL_SCANCODE_DOWN:
                        HoldingDOWN = true;
                        break;
                    case SDL_SCANCODE_LEFT:
                        HoldingLEFT = true;
                        break;
                    case SDL_SCANCODE_RIGHT:
                        HoldingRIGHT = true;
                        break;
                    case SDL_SCANCODE_F11:
                        Uint32 windowFlags = SDL_GetWindowFlags(game->window);
                        if (windowFlags & SDL_WINDOW_FULLSCREEN_DESKTOP) {
                            SDL_SetWindowFullscreen(game->window, 0);
                        } else if (windowFlags & SDL_WINDOW_FULLSCREEN) {
                            SDL_SetWindowFullscreen(game->window, 0);
                        } else {
                            SDL_SetWindowFullscreen(game->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                        }
                        break;
                    default:
                        break;
                }

                break;
            case SDL_KEYUP:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_UP:
                        HoldingUP = false;
                        break;
                    case SDL_SCANCODE_DOWN:
                        HoldingDOWN = false;
                        break;
                    case SDL_SCANCODE_LEFT:
                        HoldingLEFT = false;
                        break;
                    case SDL_SCANCODE_RIGHT:
                        HoldingRIGHT = false;
                        break;
                    default:
                        break;
                }

                break;
            case SDL_MOUSEMOTION:
                break;
            case SDL_MOUSEBUTTONDOWN:
                break;
        }
    }

    // Treat holden keys
    int playerLocation = game->pool->player.location;

    if ( game->pool->id[playerLocation].unique_id == game->pool->player.unique_id ) {
        SDL_FPoint *playerVelocity = &game->pool->velocity[playerLocation];
        playerVelocity->x = MAX_PLAYER_SPEED * ( (int) HoldingRIGHT - (int) HoldingLEFT );
        playerVelocity->y = MAX_PLAYER_SPEED * ( (int) HoldingDOWN - (int) HoldingUP );
    }

    SDL_LogInfo(
        SDL_LOG_CATEGORY_INPUT,
        "Holding: UP %d DOWN %d LEFT %d RIGHT %d",
        HoldingUP, HoldingDOWN, HoldingLEFT, HoldingRIGHT
    );

    return quit;
}

// Gestion de la boucle principale, et de la limitation des fps
void GAME_Run(Game *game) {
    bool quit = false;

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
        
        // Physique
        POOL_ApplyVelocity(game->pool, deltaTime);
        
        // Création du rendu
        POOL_DisplayAll(game->asset_manager, game->pool, game->renderer);

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