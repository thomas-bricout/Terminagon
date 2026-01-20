#include <SDL2/SDL.h>

#include "game.h"
#include "assets.h"
#include "entity_pool.h"
#include "input.h"

const double TARGET_FPS = 60.0;
const double TARGET_FRAME_TIME =  1000.0 / TARGET_FPS; // in ms

const double MAX_PLAYER_SPEED = 0.1;

void GAME_DisplayF3(Game *game, double deltaTime, double FPS, double elapsed);

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
        InputSituation_Update(game->inputSituation, event.type, event.key.keysym.scancode);
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
    int playerLocation = game->pool->player.location;
    if ( game->pool->id[playerLocation].unique_id == game->pool->player.unique_id ) {
        SDL_FPoint *playerVelocity = &game->pool->velocity[playerLocation];
        playerVelocity->x = MAX_PLAYER_SPEED * ( (int) inputSituation->RIGHT - (int) inputSituation->LEFT );
        playerVelocity->y = MAX_PLAYER_SPEED * ( (int) inputSituation->DOWN - (int) inputSituation->UP );
    }
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

        deltaTime = (double)((NOW - LAST)*1000) / (double)perf_freq;
        FPS = 1000 / deltaTime;

        // Read game events
        readEvents(game);
        
        // Physique
        POOL_ApplyVelocity(game->pool, deltaTime);
        
        // Création du rendu
        POOL_DisplayAll(game->asset_manager, game->pool, game->renderer);
        
        // Logging
        GAME_DisplayF3(game, deltaTime, FPS, elapsed);

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

void GAME_DisplayF3(Game *game, double deltaTime, double FPS, double elapsed) {
    // Quit if F3 is not toggled
    InputSituation *in = game->inputSituation;
    if (!in->ToggledF3) { return; }

    // Display the collision rects
    POOL_DisplayDebugRects(game->pool, game->renderer);

    // Compiling things to print
    char str[500];
    sprintf(
        str,
        "ENTITY COUNT: %d\n"
        "deltaTime: %4.2fms FPS: %4.1f\n"
        "COMPUTE TIME PER FRAME: %4.2fms\n"
        "LEFT %d RIGHT %d UP %d DOWN %d\n"
        , 
        game->pool->currentCount, 
        deltaTime, FPS,
        elapsed,
        in->LEFT, in->RIGHT, in->UP, in->DOWN
    );

    // Display the string on the screen
    SDL_Color white = {255, 255, 255};
    SDL_Surface *messageSurface = TTF_RenderUTF8_Solid_Wrapped(game->asset_manager->debug_font, str, white, 5000);
    if (messageSurface == NULL) {
        fprintf(stderr, "Erreur TTF_RenderUTF8_Solid_Wrapped: %s\n", SDL_GetError());
     }

    SDL_Texture *messageTexture = SDL_CreateTextureFromSurface(game->renderer, messageSurface);
    if (messageSurface == NULL) {
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
    }

    SDL_Rect dst = {0, 0, 0, 0};
    if (0 != SDL_QueryTexture(messageTexture, NULL, NULL, &dst.w, &dst.h)) {
        fprintf(stderr, "Erreur SDL_QueryTexture : %s\n", SDL_GetError());
    }

    SDL_RenderCopy(game->renderer, messageTexture, NULL, &dst);

    SDL_FreeSurface(messageSurface);
    SDL_DestroyTexture(messageTexture);
}