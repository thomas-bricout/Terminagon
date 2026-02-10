#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <SDL2/SDL.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wno-unused-function"

extern inline SDL_FRect FRECT_Offset(SDL_FRect rect, SDL_FPoint position);
extern inline SDL_Rect FRECT_ToRect(SDL_FRect frect);

extern inline SDL_Rect RECT_Offset(SDL_Rect rect, SDL_Point position);

extern inline SDL_FPoint FPOINT_Offset(SDL_FPoint point, SDL_FPoint offset);
extern inline SDL_FPoint FPOINT_ApplyVelocity(SDL_FPoint point, SDL_FPoint velocity, float deltaTime);
extern inline SDL_FPoint FPOINT_VelocityFromAngle(double angle, float speed);
extern inline SDL_FPoint FPOINT_RelativePoint(SDL_FPoint point, SDL_FPoint camera);
extern inline SDL_Point FPOINT_ToPoint(SDL_FPoint point);

#pragma GCC diagnostic pop

SDL_FPoint FPOINT_NearestSquare(SDL_FPoint point);
float FPOINT_DistanceSquared(SDL_FPoint point1, SDL_FPoint point2);
SDL_FPoint FPOINT_Normalize(SDL_FPoint vec);
extern inline SDL_FPoint FPOINT_Mul(SDL_FPoint vec, float mul);

int AngleToDirection(double angle);

#endif