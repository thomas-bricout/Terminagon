# PROJETC

## Compilation
- Compiler et lancer le jeu avec la touche F5 ( dans l'éditeur ).
- Ne pas utiliser le bouton Lancer et Débugguer en haut à droite.
- `.vscode/tasks.json` décrit la compilation
- `.vscode/launch.json` décrit le lancement
- rajouter chaque nouveau .c dans `tasks.json` ( évite l'erreur undefined function ... )

## Structure du projet
### main.c
- Initialise SDL ( création de la fenêtre, du renderer, etc.. )
- Appelle AssetsManager ( du module assets )
- Appelle Game et lance la boucle principale GAME_run ( du module game )
- Gère les erreurs
- Désalloue la mémoire à la fin du programme.

### assets.c
- Charge les assets du jeu
- Désalloue les assets du jeu
- Permet d'accéder aux assets du jeu

### game.c
- Définit la boucle principale du jeu et limite les FPS
- Gère les Events ( Fermeture fenêtre, appuis claviers)

## Gameplay et idées