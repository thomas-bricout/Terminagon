#include "tilemap.h"




void loadMap(Tile tilemap[HAUTEUR][LARGEUR]){
    FILE *fp = fopen("./blocking_map", "r");

    if (fp == NULL)
    {
        printf("Le fichier texte.txt n'a pas pu être ouvert\n");
        return;
    }

    printf("Le fichier texte.txt existe\n");


    int ch;
    int h=0;
    int w=0;
    while ((ch = fgetc(fp)) != EOF)
    {
        if(ch=='\n'){
            h++;
            w=0;
            continue;
        }
        switch (ch){
            case 'X':
                tilemap[h][w].type=1;
                tilemap[h][w].blocking=1;
                break;
            
            default:
                tilemap[h][w].type=0;
                tilemap[h][w].blocking=0;
                break;
        }
        w++;
    }
    

    if (fclose(fp) == EOF){
        printf("Erreur lors de la fermeture du flux\n");
        return;        
    }
}