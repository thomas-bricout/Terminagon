#include "tilemap.h"


#define TILE_SIZE 16
#define TILES_WIDTH 20
#define TILES_HEIGHT 8
SDL_Texture** load_MAP_Textures(const char* tilefilename, SDL_Renderer *ren){
    SDL_Surface *loadedImage = SDL_LoadBMP(tilefilename);
    SDL_Texture **tabMAPTextures = (SDL_Texture **)malloc((TILES_WIDTH*TILES_HEIGHT)*sizeof(SDL_Texture *)); 
    int i=0;
    for(int top=0;top<TILES_HEIGHT;top+=1){
        for(int left=0;left<TILES_WIDTH;left+=1){
            SDL_Surface* tileSurf=SDL_CreateRGBSurface(0,TILE_SIZE,TILE_SIZE, 32, 0, 0, 0, 0);
            SDL_Rect tileRect;
	        tileRect.x = (left*(TILE_SIZE+1)) + 1;
	        tileRect.y = (top*(TILE_SIZE+1)) + 1;
	        tileRect.w = TILE_SIZE;
	        tileRect.h = TILE_SIZE;
            SDL_BlitSurface(loadedImage,&tileRect,tileSurf,NULL);
            SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, tileSurf);
            tabMAPTextures[i] = tex;
	    SDL_FreeSurface(tileSurf);
	    i++;
	  }
    }
    SDL_FreeSurface(loadedImage);
    return tabMAPTextures;
}


int convert_num(char ch){
    switch (ch)
    {
    case '0':
        return 0;
    case '1':
        return 1;
    case '2':
        return 2;
    case '3':
        return 3;
    case '4':
        return 4;
    case '5':
        return 5;
    case '6':
        return 6;
    case '7':
        return 7;
    case '8':
        return 8;
    case '9':
        return 9;
    case 'a':
        return 10;
    case 'b':
        return 11;
    case 'c':
        return 12;
    case 'd':
        return 13;
    case 'e':
        return 14;
    case 'f':
        return 15;    
    default:
        return 0;
    }
}


void loadMap(Tile tilemap[HAUTEUR][LARGEUR]){
    FILE *fp = fopen("./maps/blocking_map.txt", "r");

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
                tilemap[h][w].blocking=1;
                break;
            
            default:
                tilemap[h][w].blocking=0;
                break;
        }
        w++;
    }
    

    if (fclose(fp) == EOF){
        printf("Erreur lors de la fermeture du flux\n");
        return;        
    }

    fp = fopen("./maps/tile_map.txt", "r");

    if (fp == NULL)
    {
        printf("Le fichier texte.txt n'a pas pu être ouvert\n");
        return;
    }

    printf("Le fichier texte.txt existe\n");


    h=0;
    w=0;
    int compteur=0;
    while ((ch = fgetc(fp)) != EOF)
    {
        if(ch=='\n'){
            h++;
            w=0;
            compteur=0;
            continue;
        }
        if(ch==' '){
            compteur=0;
            w++;
            continue;
        }

        compteur++;
        if (compteur==1){
            tilemap[h][w].id=convert_num(ch);
        }
        if (compteur==2){
            tilemap[h][w].id=convert_num(ch)+(tilemap[h][w].id*16);
            /*if(tilemap[h][w].id>70){
                tilemap[h][w].id--;
            }*/
        }
    }
    

    if (fclose(fp) == EOF){
        printf("Erreur lors de la fermeture du flux\n");
        return;        
    }
}