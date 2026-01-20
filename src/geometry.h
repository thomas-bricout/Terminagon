#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <SDL2/SDL.h>

SDL_FRect OffsetFRect(SDL_FRect rect, SDL_FPoint position);
SDL_Rect FRectToRect(SDL_FRect frect);

#endif