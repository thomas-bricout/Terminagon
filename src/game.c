#include <SDL2/SDL.h>

#include "game.h"
#include "assets.h"
#include "entity_pool.h"
#include "input.h"
#include "debug.h"
#include "rendering.h"
#include "physics.h"
#include "serialize.h"
#include "editor.h"

const double TARGET_FPS = 60.0;
const double TARGET_FRAME_TIME =  1000.0 / TARGET_FPS; // in ms

void GAME_Init(Game *game, SDL_Renderer *renderer, SDL_Window *window, AssetManager *asset_manager, EntityPool *pool, InState *inState ,SDL_GameController *controller0,SDL_GameController *controller1) {
    game->renderer = renderer;
    game->window = window;
    game->asset_manager = asset_manager;
    game->pool = pool;
    game->inState = inState;
    game->camera_pos = (SDL_FPoint) {100*113., 100*77.};
    game->controller0 = controller0;
    game->controller1 = controller1;
    loadMap(game->map);



    for (int i = 0; i < 28; i++) {
        char path[64];
        snprintf(path, sizeof(path), "assets/sounds/Sound Effect (%d).wav", i+1);

        game->sound[i] = Mix_LoadWAV(path);
        if (!game->sound[i]) {
            fprintf(stderr, "Failed to load %s: %s\n", path, Mix_GetError());
        }
    }
}

void Game_ReadEvents(Game *game) {
    SDL_Event event;

    InState *inState1 = game->inState;
    InState *inState2 = game->inState + 1;

    InState_Update_gamecontroller(inState2, event,game->controller0); 
    while (SDL_PollEvent(&event)) {
        InState_Update(inState1, event);
        //InState_Update_gamecontroller(inState1, event,game->controller0); 

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

        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_S) {
            //JSON_Save(game, "saves/save.json");
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_D) {
            //JSON_Load(game, "saves/save.json", game->inState);
        }
        if (event.type == SDL_MOUSEBUTTONDOWN && inState1->ToggledF4) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                EDITOR_PlaceEntity(game);
            } else if (event.button.button == SDL_BUTTON_RIGHT) {
                EDITOR_DeleteUnderMouse(game);
            }
        }
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

    while (!game->inState->QUIT) {
        // Calculating deltaTime, FPS and logging them
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        double current_time = (double) NOW*1000/ (double) perf_freq;

        deltaTime = (double)((NOW - LAST)*1000) / (double)perf_freq;
        FPS = 1000 / deltaTime;

        // Read game events
        Game_ReadEvents(game);
        
        // Physiques et Mécaniques
        PLAYER_System(game, current_time);
        ENEMY_System(game->pool, current_time);

        PHYSICS_MoveAll(game->map,game->pool, deltaTime);
        PHYSICS_UpdateHitPoints(game->pool, current_time);
        PHYSICS_DamageAll(game->map,game->pool, deltaTime);
        MoveCameraIJKL(game, deltaTime);

        // Création du rendu
        RENDER_RenderAll(game);
        
        // Logging
        DEBUG_DisplayDebug(game, deltaTime, FPS, elapsed, current_time);
        EDITOR_DisplayInfo(game);

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

    // JSON_EntityToFileAll(game->pool, "saves/save.json");
}

const float cameraSpeed = 0.8;
void MoveCameraIJKL(Game *game, float deltaTime) {
    if (game->inState->I) { game->camera_pos.y -= cameraSpeed * deltaTime; }
    if (game->inState->K) { game->camera_pos.y += cameraSpeed * deltaTime; }
    if (game->inState->J) { game->camera_pos.x -= cameraSpeed * deltaTime; }
    if (game->inState->L) { game->camera_pos.x += cameraSpeed * deltaTime; }
}