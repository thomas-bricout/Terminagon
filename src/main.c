#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "assets.h"

#define GAMENAME "Mon jeu"
#define VERSION "0.0.0"

// Lancer le code à l'aide de F5 ( pour lire launch.json et tasks.json )
// Ne pas utiliser le bouton run en haut à droite !!! 

int main(int argc, char *argv[])
{
    // Initialise SDL et quitte immédiatement si l'initialisation foire
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }

    // Création de la fenêtre
    SDL_Window *window = NULL;
    window = SDL_CreateWindow(GAMENAME, 0, 0, 900, 600, SDL_WINDOW_SHOWN);
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
    AssetManager asset_manager;
    ASSETS_Load(&asset_manager, renderer);

    // Dit au renderer de dessiner directement sur la fenêtre
    SDL_SetRenderTarget(renderer, NULL);

    // Bidoouillage pour avoir un sprite à l'écran
    SDL_Rect src = {0, 0, 1000, 1000};
    SDL_Rect dst = {0, 0, 1000, 1000};
    SDL_RenderCopy(renderer, asset_manager.asset_table, &src, &dst);
    SDL_RenderPresent(renderer); // Actualise la fenêtre

    // Création du jeu
    Game game;
    GAME_init(&game, renderer, window, &asset_manager);

    // Appelle de la boucle principale de jeu
    GAME_run(&game);

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
    return EXIT_FAILURE;
}
