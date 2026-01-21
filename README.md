# PROJETC
## Gameplay et idées

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
- Game permet d'accéder à l'entièreté du jeu

### input.c
- Enregistre les Inputs claviers dans InputSituation pour être utilisé par les autres systèmes

### entity_pool.c
- Définit la structure de donnée stockant toutes les unités ( EntityPool )
- Création, Initialisation, Destruction, Lecture de l'ensemble des entités

### player.c
- Définit les actions et déplacements possible par le joueur

### debug.c
- Permet d'afficher des informations de débugage lors de l'appui de la touche F3

### geometry.c
- Permet de manipuler plus rapidement les Rect, FRect et FPoint
