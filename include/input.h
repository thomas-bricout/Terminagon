#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct {
    bool QUIT;

    bool UP;
    bool DOWN;
    bool LEFT;
    bool RIGHT;

    bool W;
    bool X;
    bool C;

    bool I;
    bool J;
    bool K;
    bool L;

    bool ToggledF3;
    bool ToggledF4;

    SDL_FPoint mouse;
} InState;

void InState_Init(InState *inState);
void InState_Update(InState *inState, SDL_Event event);

#endif