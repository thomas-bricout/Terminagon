#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <SDL2/SDL.h>

SDL_FRect FRECT_Offset(SDL_FRect rect, SDL_FPoint position);
SDL_Rect FRECT_ToRect(SDL_FRect frect);

SDL_Rect RECT_Offset(SDL_Rect rect, SDL_Point position);

SDL_FPoint FPOINT_Offset(SDL_FPoint point, SDL_FPoint offset);
SDL_FPoint FPOINT_ApplyVelocity(SDL_FPoint point, SDL_FPoint velocity, float deltaTime);
SDL_FPoint FPOINT_VelocityFromAngle(double angle, float speed);
SDL_FPoint FPOINT_RelativePoint(SDL_FPoint point, SDL_FPoint camera);
SDL_Point FPOINT_ToPoint(SDL_FPoint point);

SDL_FPoint FPOINT_NearestSquare(SDL_FPoint point);

#endif