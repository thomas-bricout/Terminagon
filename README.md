# PROJETC

## Compilation
- Compiler et lancer le jeu avec la touche F5 ( dans l'éditeur ).
- Ne pas utiliser le bouton Lancer et Débugguer en haut à droite.
- `.vscode/tasks.json` décrit la compilation
- `.vscode/launch.json` décrit le lancement
- rajouter chaque nouveau .c dans `tasks.json`

## Structure du projet
### main.c
- Initialise SDL ( création de la fenêtre, du renderer, etc.. )
- Appelle AssetsManager ( du module assets )
- Appelle Game et lance la boucle principale GAME_run ( du module game )
- Gère les erreurs

### assets.c
- Charge les assets du jeu

### game.c
- Définit la boucle principale du jeu
- Gère les Events

## Gameplay et idées