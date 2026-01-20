#include <SDL2/SDL.h>

SDL_FRect OffsetFRect(SDL_FRect rect, SDL_FPoint position) {
    SDL_FRect res;
    res.x = rect.x + position.x;
    res.y = rect.y + position.y;
    res.w = rect.w;
    res.h = rect.h;
    return res;
}

SDL_Rect FRectToRect(SDL_FRect frect) {
    SDL_Rect rect;
    rect.x = (int) frect.x;
    rect.y = (int) frect.y;
    rect.h = (int) frect.h;
    rect.w = (int) frect.w;
    return rect;
}