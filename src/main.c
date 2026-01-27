#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "assets.h"
#include "entity_pool.h"
#include "input.h"

//#include <switch.h> //switch

#define GAMENAME "Mon jeu"
#define VERSION "0.0.0"

// Lancer le code à l'aide de F5 ( pour lire launch.json et tasks.json )
// Ne pas utiliser le bouton run en haut à droite !!! 

int main(int argc, char *argv[])
{
    // Initialisation Switch romfs
    //romfsInit();//switch
    //chdir("romfs:/");//switch

    // Initialise SDL et quitte immédiatement si l'initialisation foire
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }

    // Création de la fenêtre
    SDL_Window *window = NULL;
    window = SDL_CreateWindow(GAMENAME, 0, 0, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
    if (NULL == window){
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        goto Quit;
    }

    // Création du renderer
    SDL_Renderer *renderer = NULL;
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (NULL == renderer) {
        fprintf(stderr, "Erreur SDL_Renderer: %s", SDL_GetError());
        goto Quit;
    }

    // Chargement des assets
    TTF_Init();
    AssetManager asset_manager;
    ASSETS_Init(&asset_manager, renderer);
    ASSETS_Load(&asset_manager, renderer);

    // Création de InState
    InState inState[2];
    InState_Init(&inState[0]);
    InState_Init(&inState[1]);

    // Dit au renderer de dessiner directement sur la fenêtre
    SDL_SetRenderTarget(renderer, NULL);

    // Init les manettes
    SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);
    SDL_GameController *controller0 = SDL_GameControllerOpen(0);
    SDL_GameController *controller1 = SDL_GameControllerOpen(1);

    // Création de l'entity pool
    EntityPool pool;
    POOL_Init(&pool);
    POOL_Load(&pool, inState);

    // Création du jeu
    Game game;
    GAME_Init(&game, renderer, window, &asset_manager, &pool, inState, controller0,controller1);

    // Appelle de la boucle principale de jeu
    GAME_Run(&game);

    // Clean up sortie normal 
    ASSETS_Destroy(&asset_manager); // Détruit l'asset_manager
    SDL_DestroyRenderer(renderer);  // Détruit le renderer
    SDL_DestroyWindow(window);      // Détruit la fenêtre
    SDL_Quit();                     // Désalloue la mémoire lié à SDL
    return EXIT_SUCCESS;
Quit:
    // Clean up sortie en échec
    // On doit rajouter des check parce qu'on est pas sûr que tout à bien été initialisé
    ASSETS_Destroy(&asset_manager);
    if (NULL != renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (NULL != window) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
    
    //romfsExit();//switch
    return EXIT_FAILURE;
}
