#include <SDL2/SDL.h>
#include <math.h>

SDL_FRect FRECT_Offset(SDL_FRect rect, SDL_FPoint position) {
    SDL_FRect res;
    res.x = rect.x + position.x;
    res.y = rect.y + position.y;
    res.w = rect.w;
    res.h = rect.h;
    return res;
}

SDL_Rect FRECT_ToRect(SDL_FRect frect) {
    SDL_Rect rect;
    rect.x = (int) frect.x;
    rect.y = (int) frect.y;
    rect.h = (int) frect.h;
    rect.w = (int) frect.w;
    return rect;
}

SDL_Rect RECT_Offset(SDL_Rect rect, SDL_Point position) {
    SDL_Rect res;
    res.x = rect.x + position.x;
    res.y = rect.y + position.y;
    res.w = rect.w;
    res.h = rect.h;
    return res;
}

SDL_FPoint FPOINT_Offset(SDL_FPoint point, SDL_FPoint offset) {
    SDL_FPoint res;
    res.x = point.x + offset.x;
    res.y = point.y + offset.y;
    return res;
}

SDL_FPoint FPOINT_ApplyVelocity(SDL_FPoint point, SDL_FPoint velocity, float deltaTime) {
    SDL_FPoint res;
    res.x = point.x + deltaTime * velocity.x;
    res.y = point.y + deltaTime * velocity.y;
    return res;
}

SDL_FPoint FPOINT_VelocityFromAngle(double angle, float speed) {
    SDL_FPoint res;
    res.x = speed * cos(angle);
    res.y = speed * sin(angle);
    return res;
}

SDL_FPoint FPOINT_RelativePoint(SDL_FPoint point, SDL_FPoint camera) {
    SDL_FPoint res;
    res.x = point.x - camera.x;
    res.y = point.y - camera.y;
    return res;
}

SDL_Point FPOINT_ToPoint(SDL_FPoint point) {
    SDL_Point res;
    res.x = (int) point.x;
    res.y = (int) point.y;
    return res;
}