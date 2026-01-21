#ifndef EDITOR_H
#define EDITOR_H

void EDITOR_PlaceEntity(Game *game);

typedef struct {
    TextureLocation texture;
    SDL_Rect texture_box;
    SDL_FRect collision_box;
} EntityBundle;

#endif